#pragma once

#include <array>

// bleh, no bounds checking...
template<typename T, uint16_t size>
class SubarrayView
{
private:
	T* const ptr;
public:
	SubarrayView(T& first_value) : ptr{ &first_value } { };
	T& operator[](uint16_t idx) { return this->ptr[idx]; }
	const T& operator[](uint16_t idx) const { return this->ptr[idx]; }
	void set(std::array<T, size> val) { for (uint16_t i = 0; i < size; ++i) ptr[i] = val[i]; };
};
