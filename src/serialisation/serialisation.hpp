#pragma once

#include <algorithm>
#include <array>
#include <vector>
#include <bit>


namespace serialisation
{
    // Typedefs
    const std::endian LE = std::endian::little;
    const std::endian BE = std::endian::big;
    template <size_t size>
    using ByteArray = std::array<unsigned char, size>;
    using ByteVector = std::vector<unsigned char>;


    // Functions
    template<size_t size>
    ByteVector bytearrayToBytevector(const ByteArray<size>& in)
    {
        return ByteVector(in.begin(), in.end());
    }

    template<size_t size>
    ByteArray<size> bytearrayToBytevector(const ByteVector& in)
    {
        ByteArray<size> out;
        std::copy_n(in.begin(), size, out.begin());
        return out;
    }

    template<typename T, std::endian endianness>
    T unpackBytes(const ByteVector& bytes)
    {

        ByteVector machine_independent_bytes(sizeof(T));
        machine_independent_bytes = flipEndiannessIfRequired<T, endianness>(bytes);
        T res;
        memcpy(&res, reinterpret_cast<char*>(machine_independent_bytes.data()), sizeof(T));
        return res;
    }

    template<typename T, std::endian endianness>
    std::vector<T> unpackVectorOfBytes(const ByteVector& bytes)
    {
        size_t size = (size_t)(bytes.size() / sizeof(T));
        std::vector<T> out(size);
        ByteVector bytes_to_send(sizeof(T));
        for (size_t i = 0; i < size; i++)
        {
            for (size_t j = 0; j < sizeof(T); j++)
                bytes_to_send[j] = bytes[i * sizeof(T) + j];
            out[i] = unpackBytes<T, endianness>(bytes_to_send);
        }
        return out;
    }

    template<typename T, std::endian endianness>
    ByteVector packBytes(T data)
    {
        ByteVector machine_specific_bytes(sizeof(T));
        memcpy(reinterpret_cast<char*>(machine_specific_bytes.data()), &data, sizeof(T));
        ByteVector machine_independent_bytes(sizeof(T));
        machine_independent_bytes = flipEndiannessIfRequired<T, endianness>(machine_specific_bytes);
        return machine_independent_bytes;
    }

    template<typename T, std::endian endianness>
    ByteVector packVectorOfBytes(const std::vector<T>& data)
    {
        size_t size = data.size();
        ByteVector out(sizeof(T) * size);
        for (size_t i = 0; i < size; i++)
        {
            ByteVector bytes_to_send(sizeof(T));
            bytes_to_send = packBytes<T, endianness>(data[i]);
            for (int j = 0; j < sizeof(T); j++)
                out[i * sizeof(T) + j] = bytes_to_send[j];
        }
        return out;
    }

    template<typename T, std::endian endianness>
    ByteVector flipEndiannessIfRequired(const ByteVector& bytes)
    {
        ByteVector out(sizeof(T));
        out = bytes;
        // This is supposed to make sure the result of memcpy is the right way around
        if constexpr (endianness != std::endian::native)
            std::reverse(out.begin(), out.end());
        return out;
    }
}
