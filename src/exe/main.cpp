#include "AppComponent.hpp"
#include "MyController.hpp"

#include <oatpp/network/Server.hpp>

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <synyi/logging/core.h>
#include <synyi/logging/core.hpp>

#include "MyLoggingRequestHandler.h"


SYNYI_LOGGING_CLIENT_HANDLE hClient;

void run() {
  AppComponent components; // Create scope Environment components

  /* create ApiControllers and add endpoints to router */

  auto router = components.httpRouter.getObject();

  router->addController(MyController::createShared());

  /* create server */

  oatpp::network::Server server(components.serverConnectionProvider.getObject(),
                                components.serverConnectionHandler.getObject());

  OATPP_LOGI("efusion Server", "Running on port %s...",
             components.serverConnectionProvider.getObject()
                 ->getProperty("port")
                 .toString()
                 ->c_str());

  server.run();
  oatpp::base::Environment::destroy();
}

// errorType
// 1. client errors
// 2. server errors
// 3. unknow errors
int error_handler(int errorType, const char *message) {
  switch (errorType) {
  case 1:
    return -1;
  case 2:
    return 0;
  case 3:
    return -1;
  default:
    return 0;
  }
}

int main() {

  std::string groupName = "SynyiLoggingHi";
  // synyi::logging::LoggingOptions loggingOptions;
  // loggingOptions.groupName = groupName;
  // synyi::logging::logging_client client(loggingOptions);
  synyi::logging::logging_client client(groupName);
  if (!client.initialize(error_handler))
    return -1;

  std::thread([&]() {
    // send structured log with synyiLog
    auto sendSturctMap = synyi::logging::ArgsHelper::format(
        spdlog::level::level_enum::info, "logging : {address} {} {} {}",
        1234, "symptom", 12.5, "bbb");
    client.sendLog(sendSturctMap);

    auto sendPartSturctMap = synyi::logging::ArgsHelper::format(
        spdlog::level::level_enum::info,
        "partStruct logging: {address} {} {} {}", "http://localhost:5000",
        "symptom", "hhh", "bbb");
    client.sendLog(sendPartSturctMap);

    auto sendBrokenSturctMap = synyi::logging::ArgsHelper::format(
        spdlog::level::level_enum::info,
        "BrokenSturct logging: {request_id},  { key1: key1},  problem }",
        "requestId", "test");
    client.sendLog(sendBrokenSturctMap);

    auto sendNoMatchedSturctMap = synyi::logging::ArgsHelper::format(
        spdlog::level::level_enum::info,
        "NoMatchedSturctMap logging: {request_id},  { key1: key1}",
        "requestId", "test" , "hi");
    client.sendLog(sendNoMatchedSturctMap);


    // send unstructured log  with synyiLog
    auto sendNormalMsg = synyi::logging::ArgsHelper::createNormalLog(
        spdlog::level::level_enum::info, "unstruct log with synyilogging");
    client.sendLog(sendNormalMsg);

    // send unstructured log  with spdlog
    spdlog::info("info spdlog");
    spdlog::error("error spdlog");

    // send unstructured log  with OATPP_LOG
    OATPP_LOGI("synyi", "Server running on port %s", "255");

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }).detach();

  hClient = client.getClient();
  run();
  return 0;
}