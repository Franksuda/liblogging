#ifndef DA1B31DB_341C_4757_86ED_5B6B33AD15F5
#define DA1B31DB_341C_4757_86ED_5B6B33AD15F5

#include <synyi/utils/ChannelsCPP/ChannelBuffer.h>

namespace synyi {
namespace utils {
namespace ChannelsCPP {


	template<typename T, std::size_t Buffer_Size> class IChan;

	template<typename T, std::size_t Buffer_Size = 0>
	class OChan //: private Chan<T>
	{
	protected:
		std::shared_ptr<synyi::utils::ChannelsCPP::ChannelBuffer<T, Buffer_Size>> m_buffer;
		OChan(std::shared_ptr<synyi::utils::ChannelsCPP::ChannelBuffer<T, Buffer_Size>> buffer) : m_buffer(buffer) {}
	public:
		OChan() = default;
		OChan(const OChan<T, Buffer_Size>& ch) = default;// :m_buffer(ch.m_buffer) {}
		OChan(OChan<T, Buffer_Size>&& ch) { swap(m_buffer, ch.m_buffer); }
											//Insert in channel
		friend 	OChan<T, Buffer_Size>& operator<<(OChan<T, Buffer_Size>& ch, const T& obj)
		{
			ch.m_buffer->insertValue(obj);
			return ch;
		}
		friend 	OChan<T, Buffer_Size>& operator >> (const T& obj, OChan<T, Buffer_Size>& ch)
		{
			ch.m_buffer->insertValue(obj);
			return  ch;

		}

		template<std::size_t IBuffer_Size>
		friend 	OChan<T, Buffer_Size>& operator<<(OChan<T, Buffer_Size>& ch, const IChan<T, IBuffer_Size>& obj)
		{
			T temp;
			temp << obj;
			ch << temp;
			return ch;
		}

		template<std::size_t IBuffer_Size>
		friend 	OChan<T, Buffer_Size>& operator >> (const IChan<T, IBuffer_Size>& obj, OChan<T, Buffer_Size>& ch)
		{
			T temp;
			temp << obj;
			ch << temp;
			return  ch;

		}

		//Stream
		friend std::ostream& operator<<(std::ostream& os, OChan<T, Buffer_Size>& ch)
		{
			os << ch.m_buffer->getNextValue();
			return os;
		}

		void close()
		{
			m_buffer->close();
		}
	};


}}}

#endif /* DA1B31DB_341C_4757_86ED_5B6B33AD15F5 */
