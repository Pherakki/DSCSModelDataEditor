#include "BitManip.hpp"
#include <bit>

bool UtilBitScanReverse(unsigned long* output_bit_index, unsigned long input_bytes)
{
	*output_bit_index = std::bit_width(input_bytes) - 1;
	return input_bytes > 0;
}
