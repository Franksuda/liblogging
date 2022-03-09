#ifndef AA5A6F7E_DF41_4241_A0E9_B0112D40F33B
#define AA5A6F7E_DF41_4241_A0E9_B0112D40F33B

#include <exception>
#include <stdexcept>
#include <string>

namespace synyi {
namespace utils {
namespace communication {
namespace http {
// CLASS invalid_argument
class http_exception
    : public std::runtime_error { // base of all invalid-argument exceptions
public:
  using _Mybase = std::runtime_error;

  explicit http_exception(int errorcode, const std::string &_Message)
      : m_errorcode(errorcode), _Mybase(_Message.c_str()) {}

  explicit http_exception(int errorcode, const char *_Message)
      : m_errorcode(errorcode), _Mybase(_Message) {}

  int error_code() const { return m_errorcode; }

private:
  int m_errorcode;
};

} // namespace http
} // namespace communication
} // namespace utils
} // namespace synyi

#endif /* AA5A6F7E_DF41_4241_A0E9_B0112D40F33B */
