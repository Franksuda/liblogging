#ifndef C317E59A_BD74_4BB4_83F9_724C1045C4AF
#define C317E59A_BD74_4BB4_83F9_724C1045C4AF

#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

namespace synyi {
namespace logging {

/**
 * Message Data-Transfer-Object
 */
class LoggingDto : public oatpp::DTO {

  DTO_INIT(LoggingDto, DTO /* Extends */)

  DTO_FIELD(Int32, statusCode); // Status code field
  DTO_FIELD(String, message);   // Message field
};

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)


} // namespace cdss
} // namespace synyi

#endif /* C317E59A_BD74_4BB4_83F9_724C1045C4AF */
