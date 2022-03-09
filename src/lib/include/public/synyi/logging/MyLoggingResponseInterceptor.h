#ifndef AFAC2ED2_F109_457B_ADD0_7C22133CBA63
#define AFAC2ED2_F109_457B_ADD0_7C22133CBA63

#include <string>

#include <oatpp/web/server/interceptor/ResponseInterceptor.hpp>

#include <synyi/logging/core.h>

namespace synyi {
namespace logging {
class MyResponseInterceptor
    : public oatpp::web::server::interceptor::ResponseInterceptor {
public:
  MyResponseInterceptor(const SYNYI_LOGGING_CLIENT_HANDLE hClient) {
    m_pclient = (synyi::logging::client *)hClient;
  }

  std::shared_ptr<
      oatpp::web::server::interceptor::ResponseInterceptor::OutgoingResponse>
  intercept(
      const std::shared_ptr<
          oatpp::web::server::interceptor::ResponseInterceptor::IncomingRequest>
          &request,
      const std::shared_ptr<oatpp::web::server::interceptor::
                                ResponseInterceptor::OutgoingResponse>
          &response) override {
    if (request) {
      const auto &requestId = request->getHeader("RequestID");
      auto requestIDValue = requestId->std_str();

      auto requstTime(boost::posix_time::from_iso_extended_string(
          request->getHeader("requestTime")->c_str()));
      auto dur =
          boost::posix_time::microsec_clock::universal_time() - requstTime;
      auto sendMap = synyi::logging::ArgsHelper::format(
          spdlog::level::level_enum::info,
          "Request finished {RequestID} {AuditUserAgent} {StatusCoce} {ElapsedMilliseconds} "
          "{Path}",
          requestIDValue,
          request->getHeader("user-agent") ? request->getHeader("user-agent")->c_str() : "",
          std::to_string(response->getStatus().code),
          std::to_string(dur.total_milliseconds()),
          request->getStartingLine().path.std_str());
      m_pclient->sendStructLog(sendMap);
    }

    return response; // return modified response
  }

private:
  synyi::logging::client *m_pclient;
};
} // namespace logging
} // namespace synyi

#endif /* AFAC2ED2_F109_457B_ADD0_7C22133CBA63 */
