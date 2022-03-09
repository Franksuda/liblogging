#include <exception>
#include <regex>
#include <stdexcept>
#include <string>

#include <fmt/format.h>
#include <uriparser/Uri.h>

#include <synyi/utils/communication/http/http_exception.h>
#include <synyi/utils/communication/http/http_request.h>

namespace synyi {
namespace utils {
namespace communication {
namespace http {

http_request::http_request(const httplib::Headers &headers) {
  this->headers = headers;
}

http_request::http_request(http_methods method, const httplib::Headers &headers,
                           const std::string &host, const std::string &target)
    : http_request::http_request(headers) {
  this->method = method;
  this->host = host;
  this->target = target;
}

http_request::http_request(http_methods method, const httplib::Headers &headers,
                           const std::string &host, const std::string &target,
                           const std::string &content_type,
                           const std::string &content)
    : http_request::http_request(method, headers, host, target) {
  this->content_type = content_type;
  this->content = content;
}

std::string http_request::fullUrl() const {
  if (!this->validate()) {
    // throw std::invalid_argument("The given url parameters are not valid.");
    throw synyi::utils::communication::http::http_exception(
        404, "The given url parameters are not valid.");
  }
  return this->make_full_url(this->host, this->target);
}

bool http_request::validate() const {
  if (this->check_if_absolute_url(this->host) &&
      (!this->check_if_absolute_url(this->target))) {
    return true;
  }
  return false;
}

httplib::Result http_request::send() const {
  httplib::Client client(this->host.c_str());

  switch (this->method) {
  case http_methods::GET:
    return client.Get(this->target.c_str(), this->headers);
  case http_methods::POST:
    return client.Post(this->target.c_str(), this->headers,
                       this->content.c_str(), this->content.size(),
                       this->content_type.c_str());
  case http_methods::PUT:
    return client.Put(this->target.c_str(), this->headers,
                      this->content.c_str(), this->content.size(),
                      this->content_type.c_str());
  case http_methods::DELETEE:
    return client.Delete(this->target.c_str(), this->headers,
                         this->content.c_str(), this->content.size(),
                         this->content_type.c_str());
  default:
    throw std::invalid_argument("Invalid http method given.");
  }
}

bool http_request::check_if_absolute_url(
    const std::string &relativeOrAbsoluteUrl) const {
  std::regex self_regex("^(?:[a-z]+:)?//", std::regex_constants::ECMAScript |
                                               std::regex_constants::icase);
  return std::regex_search(relativeOrAbsoluteUrl, self_regex);
}

std::string inline to_string(const UriUriA &uri) {
  int charsRequired;
  if (uriToStringCharsRequiredA(&uri, &charsRequired) != URI_SUCCESS) {
    // throw std::runtime_error(
    //     "Failed to calculate the size needed to hold the uri string.");
    throw synyi::utils::communication::http::http_exception(
        404, "Failed to calculate the size needed to hold the uri string.");
  }
  charsRequired++;
  std::string error_msg;
  char *uriString = NULL;
  uriString = (char *)malloc(charsRequired * sizeof(char));
  if (uriString == NULL) {
    error_msg = "Failed to allocate memory for result buffer.";
    goto to_string_end;
  }
  if (uriToStringA(uriString, &uri, charsRequired, NULL) != URI_SUCCESS) {
    error_msg = "Failed to convert the UriUriA object to string buffer.";
    goto to_string_end;
  }
to_string_end:
  std::string result = uriString;
  free(uriString);
  if (error_msg != "") {
    // throw std::runtime_error(error_msg.c_str());
    throw synyi::utils::communication::http::http_exception(404, error_msg);
  }
  result.resize(result.length());
  return std::move(result);
}

std::string
http_request::make_full_url(const std::string &baseUrl,
                            const std::string &relativeOrAbsoluteUrl) const {
  bool success = false;
  std::string result;

  UriUriA absoluteDest;
  UriUriA relativeSource;
  UriUriA absoluteBase;

  const char *errorPos;

  if (uriParseSingleUriA(&absoluteBase, baseUrl.c_str(), &errorPos) !=
      URI_SUCCESS) {
    // throw std::invalid_argument(
    //     fmt::format("The given 'baseUrl' is invalid: {}.", errorPos));
    throw synyi::utils::communication::http::http_exception(
        404, fmt::format("The given 'baseUrl' is invalid: {}.", errorPos));
  }

  if (uriParseSingleUriA(&relativeSource, relativeOrAbsoluteUrl.c_str(),
                         &errorPos) != URI_SUCCESS) {
    // throw std::invalid_argument(fmt::format(
    //     "The given 'relativeOrAbsoluteUrl' is invalid: {}.", errorPos));
    throw synyi::utils::communication::http::http_exception(
        404, fmt::format("The given 'relativeOrAbsoluteUrl' is invalid: {}.",
                         errorPos));
  }

  if (uriAddBaseUriA(&absoluteDest, &relativeSource, &absoluteBase) ==
      URI_SUCCESS) {
    result = to_string(absoluteDest);
  }

  uriFreeUriMembersA(&absoluteDest);
  uriFreeUriMembersA(&relativeSource);
  uriFreeUriMembersA(&absoluteBase);

  return result;
}


} // namespace http
} // namespace communication
} // namespace utils
} // namespace synyi