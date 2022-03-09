#include <memory>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include <synyi/logging/Logging_sink.h>
#include <synyi/logging/MyOatppLogger.h>
#include <synyi/logging/client.h>
#include <synyi/logging/core.h>
#include <synyi/utils/ArgsHelper.h>

extern "C" {
extern const char *VERSION;
}

void registerSpdlogSink(const SYNYI_LOGGING_CLIENT_HANDLE hClient) {
  std::vector<spdlog::sink_ptr> sinks;
  sinks.push_back(
      std::make_shared<spdlog::sinks::synyi_logging_sink_mt>(hClient));

  auto synyi_logger = std::make_shared<spdlog::logger>(
      "synyi_logging", sinks.begin(), sinks.end());
  spdlog::register_logger(synyi_logger);
  synyi_logger->set_level(spdlog::level::info);

  spdlog::set_default_logger(synyi_logger);
}

void registerOatppLogger(const SYNYI_LOGGING_CLIENT_HANDLE hClient) {
  /* Init oatpp Environment */
  oatpp::base::Environment::init(
      std::make_shared<synyi::logging::MyOatppLogger>(hClient));
}

int SYNYI_LOGGING_CLIENT_LIB_EXPORT getLoggingSdkVersion(char **version) {
  *version = (char *)VERSION;
  return 0;
}

SYNYI_LOGGING_CLIENT_HANDLE SYNYI_LOGGING_CLIENT_LIB_EXPORT
initialize_logging_client(const char *loggingUrl, const char *streamName,
                          const char *environment, const char *application,
                          const char *machineName, const char *logger,
                          const char *appVersion, int64_t intervallogcount,
                          int64_t watchdogintervaltime,
                          int64_t sendintervaltime) {
  return new synyi::logging::client(
      loggingUrl, streamName, environment, application, machineName, logger,
      appVersion, intervallogcount, watchdogintervaltime, sendintervaltime);
}

int SYNYI_LOGGING_CLIENT_LIB_EXPORT
dispose_logging_client(const SYNYI_LOGGING_CLIENT_HANDLE hClient) {
  delete ((synyi::logging::client *)hClient);
  return 0;
}

bool SYNYI_LOGGING_CLIENT_LIB_EXPORT createLoggingGroupName(
    const SYNYI_LOGGING_CLIENT_HANDLE hClient, const char *groupName) {

  synyi::logging::client *pClient = (synyi::logging::client *)hClient;
  return pClient->createLoggingGroupName(groupName);
}

void SYNYI_LOGGING_CLIENT_LIB_EXPORT
initialization(const SYNYI_LOGGING_CLIENT_HANDLE hClient,
               std::function<int(int, const char *)> errorHandler) {
  synyi::logging::client *pClient = (synyi::logging::client *)hClient;
  registerSpdlogSink(pClient);
  registerOatppLogger(pClient);
  pClient->initialization(errorHandler);
}

void SYNYI_LOGGING_CLIENT_LIB_EXPORT
sendLog(const SYNYI_LOGGING_CLIENT_HANDLE hClient,
        const std::map<std::string, std::string> &sendMap) {
  synyi::logging::client *pClient = (synyi::logging::client *)hClient;
  pClient->sendStructLog(sendMap);
}
