#ifndef E679031E_83F8_45CB_A0E9_5FC2B4929D5C
#define E679031E_83F8_45CB_A0E9_5FC2B4929D5C

#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <synyi/logging/core.h>
#include "LoggingDto.h"

namespace synyi {
namespace logging {

/**
 * Custom Request Handler
 */
class MyLoggingRequestHandler : public oatpp::web::server::HttpRequestHandler {
private:
  std::shared_ptr<oatpp::data::mapping::ObjectMapper> m_objectMapper;

public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - object mapper used to serialize objects.
   */
  MyLoggingRequestHandler(
      const std::shared_ptr<oatpp::data::mapping::ObjectMapper> &objectMapper,
      const SYNYI_LOGGING_CLIENT_HANDLE hClient)
      : m_objectMapper(objectMapper) {
    m_pclient = (synyi::logging::client *)hClient;
  }

  /**
   * Handle incoming request and return outgoing response.
   */
  std::shared_ptr<OutgoingResponse>
  handle(const std::shared_ptr<IncomingRequest> &request) override {
    {
      auto requestId = request->getHeader("RequestID");
      auto requestIDValue = requestId->c_str();
      
      auto message = LoggingDto::createShared();
      message->statusCode = 1024;
      message->message = " Logging DTO!";
      return ResponseFactory::createResponse(Status::CODE_200, message,
                                             m_objectMapper);
    }
  }

private:
  synyi::logging::client *m_pclient;
};
} // namespace logging
} // namespace synyi
#endif /* E679031E_83F8_45CB_A0E9_5FC2B4929D5C */
