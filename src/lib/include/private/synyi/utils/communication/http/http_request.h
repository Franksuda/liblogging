#pragma once

#ifndef B75D45AE_DBBA_49ED_B6AE_CA49451A711E
#define B75D45AE_DBBA_49ED_B6AE_CA49451A711E

#include <string>
#include <vector>
#include <map>

#include <httplib.h>

#include <synyi/utils/communication/http/http_methods.h>
#include <synyi/utils/communication/http/http_response.h>


namespace synyi {
namespace utils {
namespace communication {
namespace http {


struct http_request {
  http_methods method;
  std::string host;
  std::string target;
  httplib::Headers headers;
  std::string content;
  std::string content_type;

  http_request(const httplib::Headers &headers);

  http_request(http_methods method, const httplib::Headers &headers,
               const std::string &host, const std::string &target);

  http_request(http_methods method, const httplib::Headers &headers,
               const std::string &host, const std::string &target,
               const std::string &content_type, const std::string &content);

  std::string fullUrl() const;

  bool validate() const;

  httplib::Result send() const;

private:
  bool check_if_absolute_url(const std::string &relativeOrAbsoluteUrl) const;

  std::string make_full_url(const std::string &baseUrl,
                            const std::string &relativeOrAbsoluteUrl) const;
};
} // namespace http
} // namespace communication
} // namespace utils
} // namespace synyi

#endif /* B75D45AE_DBBA_49ED_B6AE_CA49451A711E */
