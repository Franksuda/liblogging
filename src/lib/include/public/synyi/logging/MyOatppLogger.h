#ifndef CA335D60_A993_4842_B521_8E36EE501997
#define CA335D60_A993_4842_B521_8E36EE501997

#include <iostream>
#include <oatpp/core/base/Environment.hpp>
#include <string>
#include <synyi/logging/core.h>

namespace synyi {
namespace logging {
class MyOatppLogger : public oatpp::base::Logger {
public:
  MyOatppLogger(const SYNYI_LOGGING_CLIENT_HANDLE hClient) {
    m_pclient = (synyi::logging::client *)hClient;
  }

  void log(v_uint32 priority, const std::string &tag,
           const std::string &message) {
    auto sendMap = synyi::logging::ArgsHelper::createNormalLog(
        spdlog::level::level_enum(priority), tag + message);
    m_pclient->sendStructLog(sendMap);
  }

private:
  synyi::logging::client *m_pclient;
};

} // namespace logging
} // namespace synyi
#endif /* CA335D60_A993_4842_B521_8E36EE501997 */
