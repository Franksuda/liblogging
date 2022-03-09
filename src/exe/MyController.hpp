#ifndef MyController_hpp
#define MyController_hpp

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)


class MyController : virtual public oatpp::web::server::api::ApiController {
protected:
  MyController(const std::shared_ptr<ObjectMapper> &objectMapper)
      : oatpp::web::server::api::ApiController(objectMapper) {}

public:
  static std::shared_ptr<MyController>
  createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::shared_ptr<MyController>(new MyController(objectMapper));
  }


  //===============================start===============================C++_DIAG_API_*10===================================
  ENDPOINT_ASYNC("GET", "/Frank", get_frank) {
    ENDPOINT_ASYNC_INIT(get_frank)
    Action act() override {
      auto requestId = request->getHeader("RequestID");
      auto requestIDValue = requestId->c_str();

      auto response =
          controller->createResponse(Status::CODE_200, requestIDValue);
      response->putHeader("content-type", "text/plain; charset=UTF-8");
      return _return(response);
    };
  };

  ENDPOINT_ASYNC("POST", "/Lucy", get_lucy) {
    ENDPOINT_ASYNC_INIT(get_lucy)
    Action act() override {
      return request->readBodyToStringAsync().callbackTo(
          &get_lucy::returnResponse);
    };
    Action returnResponse(const oatpp::String &body) {

      auto requestId = request->getHeader("RequestID");
      auto requestIDValue = requestId->c_str();

      auto response = controller->createResponse(
          Status::CODE_200, requestIDValue);
      response->putHeader("content-type", "text/plain; charset=UTF-8");
      return _return(response);
    };
  };

#include OATPP_CODEGEN_END(ApiController)
};
#endif /* MyController_hpp */