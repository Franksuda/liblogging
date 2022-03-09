#include <string>


#include <synyi/utils/communication/http/http_response.h>

namespace synyi {
namespace utils {
namespace communication {
namespace http {

http_response::http_response() {}

http_response::http_response(const http_response &other) {
  this->success = other.success;
  this->message = other.message;
  this->status_code = other.status_code;
  this->body = other.body;
}

http_response::http_response(http_response &&other) {
  this->success = other.success;
  this->status_code = other.status_code;
  this->body = std::move(other.body);
  this->message = std::move(other.message);
}

http_response::http_response(const httplib::Result &result) {
  this->success = result->status == 200;
  this->status_code = result->status;
  this->message = result->reason;
  this->body = result->body;
}

} // namespace http
} // namespace communication
} // namespace utils
} // namespace synyi