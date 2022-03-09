#include <iterator>
#include <random>
#include <string>
#include <thread>

#include <nlohmann/json.hpp>

#include <synyi/logging/client.h>

#include <synyi/utils/communication/http/http_client.h>
#include <synyi/utils/communication/http/http_exception.h>

extern "C" {
extern const char *VERSION;
}

synyi::utils::communication::http::http_response
synyi::logging::client::send_request(
    const std::string &url, const std::string &target,
    const std::string &content, const httplib::Headers &headers,
    synyi::utils::communication::http::http_methods method) {
  synyi::utils::communication::http::http_client client;
  synyi::utils::communication::http::http_request synyirequest(
      method, headers, url, target, "application/json", content);

  try {
    auto response = client.send_request(synyirequest);
    return std::move(response);

  } catch (const synyi::utils::communication::http::http_exception &e) {
    synyi::utils::communication::http::http_response response;
    response.success = false;
    response.status_code = e.error_code();
    response.message = e.what();
    return std::move(response);
  }
}

std::string synyi::logging::client::getSdkVersion() const { return VERSION; }

bool synyi::logging::client::createLoggingGroupName(
    const std::string &groupName) {
  std::string targetName = "/api/group";

  std::map<std::string, std::string> bodyGroupMap = {{"name", groupName}};
  std::string jsonPara = nlohmann::json(bodyGroupMap).dump();
  const auto &response = send_request(m_loggingUrl, targetName, jsonPara);

  if (response.success) {
    m_groupName = groupName;
  } else if (!response.success &&
             response.body.compare(
                 "ERROR: duplicate key value violates unique constraint") !=
                 0) {
    m_groupName = groupName;
  } else {
    return false;
  }
  return m_groupName == "" ? false : true;
}

void synyi::logging::client::sendStructLog(
    const std::map<std::string, std::string> &sendMap) {
  try {
    std::map<std::string, std::string> sendTempMap;
    for (auto item : sendMap) {
      sendTempMap.insert(
          std::pair<std::string, std::string>(item.first, item.second));
    }
    for (auto item : m_defaultMap) {
      sendTempMap.insert(
          std::pair<std::string, std::string>(item.first, item.second));
    }

    auto sendMsg = nlohmann::json(sendTempMap).dump();
    m_channels.m_multi << sendMsg;
  } catch (const std::exception &e) {
    // m_channels.m_multi << "failed to sendStructLog";
  }

  if (m_channels.m_multi.getIChanSize() > m_intervallogcount) {
    m_bstartSend = true;
  }
}

void synyi::logging::client::initialization(
    std::function<int(int, const char *)> errorHandler) {
  std::thread([&]() {
    while (1) {
      try {
        m_channels.m_multi << "";
      } catch (const std::exception &e) {
      }

      if (m_channels.m_multi.getIChanSize() > m_intervallogcount) {
        m_bstartSend = true;
      }

      std::this_thread::sleep_for(
          std::chrono::milliseconds(m_watchdogintervaltime));
    };
  }).detach();

  std::thread([&]() {
    std::string targetName = fmt::format("/api/seq/{}/stream/{}/api/events/raw",
                                         m_groupName, m_streamName);
    synyi::utils::communication::http::http_response response;
    while (1) {
      auto sendMsg = generateContent();
      if (sendMsg != "") {
        sendMsg += "\n";

        auto response = send_request(m_loggingUrl, targetName, sendMsg, {{"Content-Type", "application/vnd.serilog.clef"},
                                         {"X-Synyi-ApiKey",
                                          "write-log-event"}});
        if (!response.success) {
          int errorType = 0;
          if (response.status_code >= 400 && response.status_code < 500)
            errorType = 1; // Client error
          else if (response.status_code >= 500 && response.status_code < 600)
            errorType = 2; // Server error
          else
            errorType = 3;
          errorHandler(errorType, response.message.c_str());
        }
      }
      std::this_thread::sleep_for(
          std::chrono::milliseconds(m_sendintervaltime));
    };
  }).detach();

  std::thread([&]() {
    std::string targetName = fmt::format("/api/seq/{}/stream/{}/api/events/raw",
                                         m_groupName, m_streamName);
    synyi::utils::communication::http::http_response response;
    while (1) {
      if (m_bstartSend) {
        auto sendMsg = generateContent();
        if (sendMsg != "") {
          sendMsg += "\n";

          auto response = send_request(m_loggingUrl, targetName, sendMsg,  {{"Content-Type", "application/vnd.serilog.clef"},
                                         {"X-Synyi-ApiKey",
                                          "write-log-event"}});
          if (!response.success) {
            int errorType = 0;
            if (response.status_code >= 400 && response.status_code < 500)
              errorType = 1; // Client error
            else if (response.status_code >= 500 && response.status_code < 600)
              errorType = 2; // Server error
            else
              errorType = 3;
            errorHandler(errorType, response.message.c_str());
          }
        }
        m_bstartSend = false;
      }

      std::this_thread::sleep_for(
          std::chrono::milliseconds(m_watchdogintervaltime));
    };
  }).detach();
}

std::string synyi::logging::client::generateContent() {
  std::string sendMsg;
  for (int i = 0; i < m_intervallogcount; i++) {
    std::string temp;
    try {
      m_channels.m_multi >> temp;
    } catch (const std::exception &) {
    }

    if (temp == "")
      continue;
    sendMsg += temp + "\n";
  }

  return std::move(sendMsg);
}