#include <cstdlib>
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <uriparser/Uri.h>

#include <fmt/format.h>

#include <synyi/utils/communication/http/http_client.h>

using namespace std;

namespace synyi {
namespace utils {
namespace communication {
namespace http {

http_client::http_client() {}

http_client::http_client(const std::string &baseUrl) {
  this->_baseUrl = baseUrl;
}

http_client::~http_client() {}

http_response http_client::send_request(const http_request &request) const {
  if (!request.validate()) {
    throw std::invalid_argument("The given request is not valid.");
  }

  const auto &result = request.send();
  http_response response(result);

  return std::move(response);
}

} // namespace http
} // namespace communication
} // namespace utils
} // namespace synyi