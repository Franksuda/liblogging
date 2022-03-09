#pragma once

#ifndef C1F65CA7_F922_451E_8606_345F2D982E21
#define C1F65CA7_F922_451E_8606_345F2D982E21

#include <mutex>
#include <string>

#include <nlohmann/json.hpp>

namespace synyi {
namespace utils {
namespace config {

struct configuration_container {
public:
  nlohmann::json configuration;

public:
  configuration_container();
  ~configuration_container();

  std::string get_or_set_default_config_file_path();
  std::string get_or_set_default_config_file_path(const std::string &path);

  void load_from_json();
  void load_from_json(const std::string &path);

  void save_as_json();
  void save_as_json(const std::string &path);

  std::string get(const std::string &key);
  std::string get(const std::string &key, const std::string &default_value);
  void set(const std::string &key, const std::string &value);

  template <typename _TValue, typename _Transformer>
  _TValue get(const std::string &key, const std::string &default_value,
              _Transformer transformer) {
    const auto &str_value = this->get(key, default_value);
    return transformer(str_value);
  }

private:
  std::string default_config_file_path;

  std::mutex configuration_mutex;
};

} // namespace config
} // namespace utils
} // namespace synyi

#endif /* C1F65CA7_F922_451E_8606_345F2D982E21 */
