#include <synyi/utils/ChannelsCPP/Channel.h>

template<typename T, std::size_t Buffer_Size>
T& operator<<(synyi::utils::ChannelsCPP::Chan<T, Buffer_Size>& ch, const T& obj)
{
	ch.insertValue(obj);
	return ch;
}


template<typename T, std::size_t Buffer_Size>
synyi::utils::ChannelsCPP::Chan<T, Buffer_Size>& operator >> (synyi::utils::ChannelsCPP::Chan<T, Buffer_Size>& ch, T& obj)
{
	obj = ch.getNextValue();
	return obj;

}