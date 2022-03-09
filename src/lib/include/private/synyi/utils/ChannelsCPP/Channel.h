#ifndef DB40AAD2_BC30_4C4A_97F3_CC2BFC5FAE9A
#define DB40AAD2_BC30_4C4A_97F3_CC2BFC5FAE9A
#include <queue>
#include <deque>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <limits>

#include <synyi/utils/ChannelsCPP/ChannelBuffer.h>
#include <synyi/utils/ChannelsCPP/IChannel.h>
#include <synyi/utils/ChannelsCPP/OChannel.h>
#include <synyi/utils/ChannelsCPP/ChannelUtility.h>
#include <synyi/utils/ChannelsCPP/Circular_buffer.h>

namespace synyi {
namespace utils {
namespace ChannelsCPP {

// Channel Type references: https://golang.org/ref/spec#Channel_types
template <typename T, std::size_t Buffer_Size = 1>
class Chan : public IChan<T, Buffer_Size>, public OChan<T, Buffer_Size> {
public:
  Chan() {
    Chan::IChan::m_buffer = Chan::OChan::m_buffer =
        std::make_shared<synyi::utils::ChannelsCPP::ChannelBuffer<T, Buffer_Size>>();
    // IChan<T, Buffer_Size>::m_buffer = OChan<T, Buffer_Size>::m_buffer =
    // std::make_shared<internal::ChannelBuffer<T, Buffer_Size>>();
  }
  ~Chan() = default;


  int getIChanSize(){ return Chan::IChan::m_buffer->getbuffersize();}
  int getOChanSize(){ return Chan::OChan::m_buffer->getbuffersize();}

  // Insert in channel
  friend OChan<T, Buffer_Size> &operator<<(Chan<T, Buffer_Size> &ch,
                                           const T &obj) {
    return static_cast<OChan<T, Buffer_Size> &>(ch) << obj;
    /*ch.m_buffer->insertValue(obj);
    return ch;*/
  }
  friend OChan<T, Buffer_Size> &operator>>(const T &obj,
                                           Chan<T, Buffer_Size> &ch) {
    return static_cast<OChan<T, Buffer_Size> &>(ch) << obj;

    /*ch.m_buffer->insertValue(obj);
    return  ch;*/
  }
  // Stream
  friend std::ostream &operator<<(std::ostream &os, Chan<T, Buffer_Size> &ch) {
    return os << static_cast<OChan<T, Buffer_Size> &>(ch);
  }
  friend std::istream &operator>>(std::istream &is, Chan<T, Buffer_Size> &ch) {
    return is >> static_cast<IChan<T, Buffer_Size> &>(ch);
  }
};

} // namespace ChannelsCPP
} // namespace utils
} // namespace synyi

#endif /* DB40AAD2_BC30_4C4A_97F3_CC2BFC5FAE9A */
