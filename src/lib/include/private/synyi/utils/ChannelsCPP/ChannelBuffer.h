#ifndef EC8F7534_9CEA_4584_AFF1_1BD326997382
#define EC8F7534_9CEA_4584_AFF1_1BD326997382

#include <memory>
#include <queue>


#include <atomic>
#include <condition_variable>
#include <mutex>
#include <synyi/utils/ChannelsCPP/Circular_buffer.h>


namespace synyi {
namespace utils {
namespace ChannelsCPP {

template <typename T, std::size_t Buffer_Size = 0> 
class ChannelBuffer {
private:
  synyi::utils::ChannelsCPP::Circular_buffer<T, Buffer_Size> buffer;
  std::mutex bufferLock;
  std::condition_variable inputWait;
  std::condition_variable outputWait;
  std::atomic_bool is_closed;

public:
  ChannelBuffer() : is_closed(false) {}
  ~ChannelBuffer() = default;

  int getbuffersize() {
    return buffer.size();
  }
  
  // Wait for the next value
  T getNextValue() {
    std::unique_lock<std::mutex> ulock(bufferLock);
    if (buffer.empty()) {
      if (is_closed) // if closed we always return the default initialisation of
                     // T
        return {};
      inputWait.wait(ulock, [&]() { return !buffer.empty() || is_closed; });
      if (buffer.empty() && is_closed) // when we close the channel and there
                                       // was more waiting then available value
        return {};
    }

    T temp;
    std::swap(temp, buffer.front());
    buffer.pop();
    outputWait.notify_one();
    return temp;
  }

  // Should use std::optional but MSVC and Clang doesn't support it yet :(
  // #C++17
  std::unique_ptr<T> tryGetNextValue() {
    if (is_closed) // if closed we always return the default initialisation of T
      return std::make_unique<T>(T{});
    std::unique_lock<std::mutex> ulock(bufferLock);
    if (buffer.empty()) {
      return nullptr;
    }
    std::unique_ptr<T> temp = std::make_unique<T>(buffer.front());
    buffer.pop();
    outputWait.notify_one();
    return std::move(temp);
  }

  void insertValue(T in) {
    if (!is_closed) {
      {
        std::unique_lock<std::mutex> lock(bufferLock);
        if (buffer.full()) {
          outputWait.wait(lock, [&]() { return !buffer.full() || is_closed; });
          if (is_closed) // if channel was closed end all awaiting inputs
                         // (cannot send to a closed channel)
          {
            return;
          }
        }
        buffer.push(in);
      }
      inputWait.notify_one();
    }
  }

  void close() {
    is_closed = true;
    inputWait.notify_one();
    outputWait.notify_all();
  }

  bool isClosed() { return is_closed; }
};

} // namespace ChannelsCPP
} // namespace utils
} // namespace synyi

#endif /* EC8F7534_9CEA_4584_AFF1_1BD326997382 */
