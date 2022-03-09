#pragma once

#ifndef D18EBD1F_1454_4BDE_9389_72CE1F834C97
#define D18EBD1F_1454_4BDE_9389_72CE1F834C97

#include <cstdint>
#include <exception>
#include <mutex>
#include <string>

#include <fmt/core.h>

#include <spdlog/common.h>

#include <synyi/utils/ArgsHelper.h>
#include <synyi/utils/ChannelsCPP/Channel.h>
#include <synyi/utils/communication/http/http_methods.h>
#include <synyi/utils/communication/http/http_response.h>

namespace synyi {
namespace logging {

template <int64_t bufferSize = 100> class ChannelsCPPHelper {
public:
  ChannelsCPPHelper() {}
  synyi::utils::ChannelsCPP::Chan<std::string, bufferSize> m_multi;
};

class client {
public:
  client(std::string loggingUrl, std::string streamName,
         std::string environment, std::string application,
         std::string machineName, std::string logger, std::string appVersion,
         int64_t intervallogcount, int64_t watchdogintervaltime,
         int64_t sendintervaltime)
      : m_loggingUrl(loggingUrl), m_streamName(streamName),
        m_environment(environment), m_application(application),
        m_appVersion(appVersion), m_machineName(machineName), m_logger(logger),
        m_intervallogcount(intervallogcount),
        m_watchdogintervaltime(watchdogintervaltime),
        m_sendintervaltime(sendintervaltime), m_bstartSend(false) {

    m_defaultMap.insert(
        std::pair<std::string, std::string>("Environment", m_environment));
    m_defaultMap.insert(
        std::pair<std::string, std::string>("Application", m_application));
    m_defaultMap.insert(
        std::pair<std::string, std::string>("AppVersion", m_appVersion));
    m_defaultMap.insert(std::pair<std::string, std::string>(
        "SdkVersion", this->getSdkVersion()));
  }

  ~client() {}

  std::string getSdkVersion() const;

  void initialization(std::function<int(int, const char *)> errorHandler);

  bool createLoggingGroupName(const std::string &groupName);

  void sendStructLog(const std::map<std::string, std::string> &log);

private:
  synyi::utils::communication::http::http_response send_request(
      const std::string &url, const std::string &target,
      const std::string &content,
      const httplib::Headers &headers = {{"Content-Type", "application/json"},
                                         {"Authorization",
                                          "Basic YWRtaW46YWRtaW4="}},
      synyi::utils::communication::http::http_methods method =
          synyi::utils::communication::http::http_methods::POST);

  std::string generateContent();

  std::string m_loggingUrl;
  int64_t m_intervallogcount;
  int64_t m_watchdogintervaltime;
  int64_t m_sendintervaltime;
  std::string m_groupName;
  std::string m_streamName;
  int64_t m_buffersize;
  synyi::logging::ChannelsCPPHelper<> m_channels;
  bool m_bstartSend;
  std::map<std::string, std::string> m_defaultMap;
  std::string m_environment;
  std::string m_application;
  std::string m_appVersion;
  std::string m_machineName;
  std::string m_logger;
};

} // namespace logging
} // namespace synyi

#endif /* D18EBD1F_1454_4BDE_9389_72CE1F834C97 */
