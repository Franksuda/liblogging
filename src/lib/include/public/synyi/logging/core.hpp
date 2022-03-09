#pragma once

#include <mutex>
#ifndef DD3C7186_1B21_43E8_A599_4EB2493D4433
#define DD3C7186_1B21_43E8_A599_4EB2493D4433

#if __cplusplus

#include <spdlog/common.h>

#include <synyi/logging/core.h>
#include <synyi/logging/exports.h>

namespace synyi {
namespace logging {

struct LoggingOptions {
  std::string groupName;
  std::string loggingUrl = "http://synyi-platform-logging-frontend-906-dev.sy";
  std::string streamName = "default";
  std::string environment = "Development";
  std::string application = "Synyi.Logging.Client.WebTest";
  std::string machineName = "LINGBO";
  std::string logger = "Microsoft.Hosting.Lifetime";
  std::string appVersion = "1.0.0.0";
  int64_t intervallogcount = 25;
  int64_t watchdogintervaltime = 50;
  int64_t sendintervaltime = 1000;
};

class logging_client {
public:
  inline logging_client() : m_is_configured(false) {}
  inline logging_client(const LoggingOptions &opts) {
    set_logging_options(opts);
  }

  inline logging_client(
      const std::string &groupName,
      std::string loggingUrl =
          "http://synyi-platform-logging-frontend-906-dev.sy",
      std::string streamName = "default",
      std::string environment = "Development",
      std::string application = "Synyi.Logging.Client.WebTest",
      std::string machineName = "LINGBO",
      std::string logger = "Microsoft.Hosting.Lifetime",
      std::string appVersion = "1.0.0.0", int64_t intervallogcount = 25,
      int64_t watchdogintervaltime = 50, int64_t sendintervaltime = 1000) {
    LoggingOptions opts;
    opts.groupName = groupName;
    opts.loggingUrl = loggingUrl;
    opts.streamName = streamName;
    opts.environment = environment;
    opts.application = application;
    opts.machineName = machineName;
    opts.logger = logger;
    opts.application = appVersion;
    opts.intervallogcount = intervallogcount;
    opts.watchdogintervaltime = watchdogintervaltime;
    opts.sendintervaltime = sendintervaltime;
    set_logging_options(opts);
  }

  void set_logging_options(const LoggingOptions &opts) {
    m_groupName = opts.groupName;
    m_loggingUrl = opts.loggingUrl;
    m_streamName = opts.streamName;
    m_environment = opts.environment;
    m_application = opts.application;
    m_machineName = opts.machineName;
    m_logger = opts.logger;
    m_appVersion = opts.appVersion;
    m_intervallogcount = opts.intervallogcount;
    m_watchdogintervaltime = opts.watchdogintervaltime;
    m_sendintervaltime = opts.sendintervaltime;
    m_is_configured = true;
  }

  bool initialize(std::function<int(int, const char *)> errorHandler) {
    if (!m_is_configured) {
      errorHandler(1, "No configuration");
      return false;
    }
    this->_client = ::initialize_logging_client(
        m_loggingUrl.c_str(), m_streamName.c_str(), m_environment.c_str(),
        m_application.c_str(), m_machineName.c_str(), m_logger.c_str(),
        m_appVersion.c_str(), m_intervallogcount, m_watchdogintervaltime,
        m_sendintervaltime);
    bool bcreateGroup =
        ::createLoggingGroupName(this->_client, m_groupName.c_str());
    if (!bcreateGroup) {
      errorHandler(1, "failed to create group name");
      return false;
    }
    ::initialization(this->_client, errorHandler
                                        ? errorHandler
                                        : &logging_client::error_handler);
    return true;
  }

  SYNYI_LOGGING_CLIENT_HANDLE getClient() { return _client; }

  inline ~logging_client() { ::dispose_logging_client(this->_client); }

public:
  inline void sendLog(const std::map<std::string, std::string> &sendMsg) {
    ::sendLog(this->_client, sendMsg);
  }

  inline int static error_handler(int errorType, const char *message) {
    return 0;
  }

private:
  SYNYI_LOGGING_CLIENT_HANDLE _client;
  std::string m_groupName;
  std::string m_loggingUrl;
  std::string m_streamName;
  std::string m_environment;
  std::string m_application;
  std::string m_machineName;
  std::string m_logger;
  std::string m_appVersion;
  int64_t m_intervallogcount;
  int64_t m_watchdogintervaltime;
  int64_t m_sendintervaltime;
  bool m_is_configured;
};

} // namespace logging
} // namespace synyi

#endif

#endif /* DD3C7186_1B21_43E8_A599_4EB2493D4433 */
