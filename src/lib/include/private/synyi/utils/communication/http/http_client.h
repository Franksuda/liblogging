#pragma once

#ifndef B6325315_7752_4500_AF28_8F7090B42F66
#define B6325315_7752_4500_AF28_8F7090B42F66

#include <string>

#include <synyi/utils/communication/http/http_methods.h>
#include <synyi/utils/communication/http/http_request.h>
#include <synyi/utils/communication/http/http_response.h>

namespace synyi {
namespace utils {
namespace communication {
namespace http {

class http_client {
public:
  http_client();
  http_client(const std::string &baseUrl);

  ~http_client();

public:
  virtual http_response send_request(const http_request &request) const;

protected:
  std::string _baseUrl;
};

} // namespace http
} // namespace communication
} // namespace utils
} // namespace synyi

#endif /* B6325315_7752_4500_AF28_8F7090B42F66 */
