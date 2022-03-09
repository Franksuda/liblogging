#pragma once

#ifndef E9ABA186_3C98_4BFF_B517_BDA0F2E0821C
#define E9ABA186_3C98_4BFF_B517_BDA0F2E0821C

#include <string>

#include <httplib.h>

namespace synyi {
namespace utils {
namespace communication {
namespace http {

struct http_response {
  bool success;
  int status_code = 0;
  std::string message;
  std::string body;

  http_response();

  http_response(const http_response &other);

  http_response(http_response &&other);

  http_response(const httplib::Result &result);

};

} // namespace http
} // namespace communication
} // namespace utils
} // namespace synyi

#endif /* E9ABA186_3C98_4BFF_B517_BDA0F2E0821C */
