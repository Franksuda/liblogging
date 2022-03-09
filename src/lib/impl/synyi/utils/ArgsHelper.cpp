
#include <ctime>
#include <nlohmann/json.hpp>
#include <string>
#include <synyi/utils/ArgsHelper.h>

void synyi::logging::ArgsHelper::getstructuredKey(
    std::string &format_str, std::vector<std::string> &outstring) {
  size_t start = 0;
  size_t end = 0;

  int i = 0;

  while ((start = format_str.find_first_of("{", end)) != std::string::npos) {
    end = format_str.find("}", start);
    if (end != std::string::npos) {
      if (end - start - 1 == 0) {
        format_str.insert(end, std::to_string(i));
        outstring.push_back(std::to_string(i));
        i++;
      } else {
        outstring.push_back(format_str.substr(start + 1, end - start - 1));
      }
    }
  }
}

std::map<std::string, std::string>
synyi::logging::ArgsHelper::createNormalLog(spdlog::level::level_enum level,
                                            const std::string &fullog) {

  std::map<std::string, std::string> normalLogMap;
  std::time_t result = std::time(nullptr);
  normalLogMap.insert(
      std::pair<std::string, std::string>("@t", getISOTimeString()));
  normalLogMap.insert(
      std::pair<std::string, std::string>("@l", levelToString(level)));
  normalLogMap.insert(std::pair<std::string, std::string>("@m", fullog));
  normalLogMap.insert(
      std::pair<std::string, std::string>("ThreadId", getCurrentThreadID()));
  normalLogMap.insert(
      std::pair<std::string, std::string>("ProcessId", getCurrentProcessID()));

  return std::move(normalLogMap);
}