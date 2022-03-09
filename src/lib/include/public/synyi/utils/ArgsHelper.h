#ifndef C85CC4AC_7BF9_4A03_AAFF_7E2EBD725741
#define C85CC4AC_7BF9_4A03_AAFF_7E2EBD725741

#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

#include <nlohmann/json.hpp>
#include <spdlog/common.h>

#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

namespace synyi {
namespace logging {
class ArgsHelper {
public:
  static std::map<std::string, std::string>
  createNormalLog(spdlog::level::level_enum level, const std::string &fullog);

  template <typename... Args> static void getArgsList(Args... args) {
    // auto a = std::forward_as_tuple(args...);
  }

  template <class T, class... Args>
  static void getArgsList(std::vector<std::string> &msg, std::string &structStr,
                          T head, Args... rest) {
    structStr += fmt::format(" '{}' ", head);
    msg.push_back(fmt::format("{}", head));

    getArgsList(msg, structStr, rest...);
  }

  static std::string levelToString(spdlog::level::level_enum level) {
    switch (level) {
    case 0:
      return "TRACE";
    case 1:
      return "DEBUG";
    case 2:
      return "INFO";
    case 3:
      return "WARN";
    case 4:
      return "ERROR";
    case 5:
      return "CRITICAL";
    case 6:
      return "OFF";
    default:
      return "NOLEVEL";
    }
  }

  static std::string getISOTimeString() {
    auto t = boost::posix_time::microsec_clock::universal_time();
    return to_iso_extended_string(t) + "+08:00";
  }

  static std::string getCurrentThreadID() {
    const auto &threadId =
        boost::lexical_cast<std::string>(boost::this_thread::get_id());
    return std::move(threadId);
  }

  static std::string getCurrentProcessID() {
    const auto &pid = boost::lexical_cast<std::string>(
        boost::interprocess::ipcdetail::get_current_process_id());
    return std::move(pid);
  }

  static void getstructuredKey(std::string &format_str,
                               std::vector<std::string> &outstring);

  // Pass char_t as a default template parameter instead of using
  // std::basic_string<char_t<S>> to reduce the symbol size.
  template <typename S, typename... Args>
  static std::map<std::string, std::string>
  format(spdlog::level::level_enum level, const S &format_str, Args &&...args) {

    std::map<std::string, std::string> propertiesmap;
    std::vector<std::string> structuredKeyVect;

    auto structString = std::string(format_str);
    getstructuredKey(structString, structuredKeyVect);

    std::vector<std::string> structuredValueVect;
    std::string structStr;
    getArgsList(structuredValueVect, structStr, args...);

    if (structuredKeyVect.size() != structuredValueVect.size()) {
      return createNormalLog(spdlog::level::level_enum::err,
                             structString + structStr);
    }

    std::time_t result = std::time(nullptr);
    propertiesmap.insert(
        std::pair<std::string, std::string>("@t", getISOTimeString()));
    propertiesmap.insert(
        std::pair<std::string, std::string>("@l", levelToString(level)));
    propertiesmap.insert(
        std::pair<std::string, std::string>("@mt", structString));
    propertiesmap.insert(
        std::pair<std::string, std::string>("ThreadId", getCurrentThreadID()));
    propertiesmap.insert(std::pair<std::string, std::string>(
        "ProcessId", getCurrentProcessID()));

    for (size_t i = 0; i < structuredKeyVect.size(); i++) {
      propertiesmap.insert(std::pair<std::string, std::string>(
          structuredKeyVect[i], structuredValueVect[i]));
    }
    return std::move(propertiesmap);
  }
};
} // namespace logging
} // namespace synyi

#endif /* C85CC4AC_7BF9_4A03_AAFF_7E2EBD725741 */
