#pragma once

#ifndef ECEBB265_F50F_499D_90C2_3CB4B977976C
#define ECEBB265_F50F_499D_90C2_3CB4B977976C

#include <nlohmann/json.hpp>

template <class Dummy> struct JsonBridgeGlobals_ {
  static nlohmann::json const default_json;
};

template <class Dummy>
nlohmann::json const JsonBridgeGlobals_<Dummy>::default_json = {
    {"default", ""},
};

using JsonBridgeGlobals = JsonBridgeGlobals_<void>;

inline const nlohmann::json &safe_get_json_value(const nlohmann::json &j,
                                                 const std::string &key) {
  if (j.contains(key)) {
    return j.at(key);
  }
  return JsonBridgeGlobals::default_json.at("default");
}

#define SAFE_JSON_FROM(v1)                                                     \
  safe_get_json_value(nlohmann_json_j, #v1).get_to(nlohmann_json_t.v1);

#define SAFE_DEFINE_TYPE_NON_INTRUSIVE(Type, ...)                              \
  inline void to_json(nlohmann::json &nlohmann_json_j,                         \
                      const Type &nlohmann_json_t) {                           \
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__))   \
  }                                                                            \
  inline void from_json(const nlohmann::json &nlohmann_json_j,                 \
                        Type &nlohmann_json_t) {                               \
    NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(SAFE_JSON_FROM, __VA_ARGS__))     \
  }

#endif /* ECEBB265_F50F_499D_90C2_3CB4B977976C */
