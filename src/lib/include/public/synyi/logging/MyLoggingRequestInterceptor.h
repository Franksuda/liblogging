#ifndef BCF8E227_B2D9_47C0_BB0A_E89DE8F4385C
#define BCF8E227_B2D9_47C0_BB0A_E89DE8F4385C

#include <oatpp/core/Types.hpp>
#include <oatpp/web/server/interceptor/RequestInterceptor.hpp>

#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <synyi/logging/core.h>

namespace synyi {
namespace logging {
class MyRequestInterceptor
    : public oatpp::web::server::interceptor::RequestInterceptor {
public:
  MyRequestInterceptor(const SYNYI_LOGGING_CLIENT_HANDLE hClient) {
    m_pclient = (synyi::logging::client *)hClient;
  }

  std::shared_ptr<
      oatpp::web::server::interceptor::RequestInterceptor::OutgoingResponse>
  intercept(
      const std::shared_ptr<
          oatpp::web::server::interceptor::RequestInterceptor::IncomingRequest>
          &request) override {
    oatpp::String requestID = "RequestID";
    oatpp::String requestTime = "requestTime";

    const auto &guid = generate_guid();
    oatpp::String requestIDValue(guid.c_str());


    auto sendMap = synyi::logging::ArgsHelper::format(
        spdlog::level::level_enum::info,
        "Request starting {RequestID}  {AuditUserAgent} {Method} {Protocol} {Path}", guid,
        request->getHeader("user-agent") ? request->getHeader("user-agent")->c_str() : "",
        request->getStartingLine().method.std_str(),
        request->getStartingLine().protocol.std_str(),
        request->getStartingLine().path.std_str());

    request->putHeader(requestID, requestIDValue);
    auto requestStringTime = sendMap["@t"];
    oatpp::String requestTimeValue(requestStringTime.c_str());
    request->putHeader(requestTime, requestTimeValue);
    m_pclient->sendStructLog(sendMap);

    return nullptr;
  }

private:
  std::string generate_guid() {
    const auto &guid = boost::uuids::random_generator()();
    std::stringstream ss;
    ss << guid;
    return ss.str();
  }

private:
  synyi::logging::client *m_pclient;
};
} // namespace logging
} // namespace synyi

#endif /* BCF8E227_B2D9_47C0_BB0A_E89DE8F4385C */
