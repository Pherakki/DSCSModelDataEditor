#pragma once


#include <array>
#include <cmath>
#include <vector>


template<uint8_t size>
float dot(const std::array<float, size>& vec_1, const std::array<float, size>& vec_2)
{
	float out = 0;
	for (uint8_t i = 0; i < size; ++i)
		out += vec_1[i] * vec_2[i];
	return out;
}

template<uint8_t size>
std::array<float, size> normalise(const std::array<float, size>& vec)
{
	float magnitude = std::sqrt(dot<3>(vec, vec));

	std::array<float, size> out;
	for (uint8_t i = 0; i < size; ++i)
		out[i] = vec[i] / magnitude;

	return out;
}

template<typename T, size_t N>
std::array<T, N> sumVects(const std::array<T, N>& vec_1, const std::array<T, N>& vec_2)
{
	std::array<T, N> retval;
	for (size_t i = 0; i < N; ++i)
		retval[i] = vec_1[i] + vec_2[i];
	return retval;
}



std::array<float, 3> cross(const std::array<float, 3>& vec_1, const std::array<float, 3>& vec_2);
std::array<float, 4> mulQuats(const std::array<float, 4>& quat_1, const std::array<float, 4>& quat_2);
void fixQuaternionSigns(std::vector<std::array<float, 4>>& quat_vec);
void fixQuaternionSigns2(std::vector<std::array<float, 4>>& quat_vec);