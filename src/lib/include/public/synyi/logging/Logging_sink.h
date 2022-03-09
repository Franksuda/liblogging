#ifndef D33EE2BA_5CA1_4590_9448_B7A42F5153CE
#define D33EE2BA_5CA1_4590_9448_B7A42F5153CE

#include <iostream>
#include <mutex>
#include <spdlog/details/null_mutex.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/sinks/base_sink.h>
#include <synyi/logging/core.h>

namespace spdlog {
namespace sinks {

template <typename Mutex>
class synyi_logging_sink : public spdlog::sinks::base_sink<Mutex> {
public:
  explicit synyi_logging_sink(const SYNYI_LOGGING_CLIENT_HANDLE hClient) {
    m_pclient = (synyi::logging::client *)hClient;
  }
  ~synyi_logging_sink() {}

protected:
  void sink_it_(const spdlog::details::log_msg &msg) override {

    // log_msg is a struct containing the log entry info like level, timestamp,
    // thread id etc. msg.raw contains pre formatted log

    // If needed (very likely but not mandatory), the sink formats the message
    // before sending it to its final destination:
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
    auto sendStructMap = synyi::logging::ArgsHelper::createNormalLog(
      msg.level, fmt::to_string(formatted));

    m_pclient->sendStructLog(sendStructMap);
  }

  void flush_() override { std::cout << std::flush; }

private:
  synyi::logging::client *m_pclient;
};

using synyi_logging_sink_mt = synyi_logging_sink<std::mutex>;
using synyi_logging_sink_st = synyi_logging_sink<spdlog::details::null_mutex>;

} // namespace sinks
} // namespace spdlog

#endif /* D33EE2BA_5CA1_4590_9448_B7A42F5153CE */
