#pragma once

#ifndef C44DF743_9797_49FE_9A16_1A6462876080
#define C44DF743_9797_49FE_9A16_1A6462876080

#include <string>

namespace synyi {
namespace utils {
namespace config {

std::string get_default_config_path();

std::string get_default_config_path(const std::string &path_to_append);

} // namespace config
} // namespace utils
} // namespace synyi

#endif /* C44DF743_9797_49FE_9A16_1A6462876080 */
