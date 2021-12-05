#pragma once

#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Exceptions.hpp"
#include "Serialisation.hpp"

namespace serialisation
{
    const std::ios_base::openmode readmode = std::ios_base::in;
    const std::ios_base::openmode writemode = std::ios_base::out;

    template<std::ios_base::openmode openmode_flag>
    class ReadWriter
    {
    public:
        ReadWriter<openmode_flag>(const std::string& filepath);
        ~ReadWriter<openmode_flag>();
        void Seek(std::streampos pos);
        std::streampos Tell();
        template<typename T, std::endian endianness> const void readWriteData(T& data_slot);
        template<typename T, std::endian endianness> const void readWriteConstData(const T& data_slot, const std::string& data_slot_name);
        template<typename T, std::endian endianness> const void readWriteDataVector(std::vector<T>& data_slot);
        const void readWriteAsciiToEOF(std::string& string_slot);
        const void readWriteAscii(std::string& string_slot, size_t string_length);
        const void readWriteConstAscii(const std::string& string_slot, const std::string& string_slot_name);
        const void readWriteBytes(ByteVector& bytes);
        const void readWritePadBytesLeftInIncompleteChunk(std::streampos position, size_t chunksize);
        template<typename T, std::endian endianness> const void readWriteBytesLeftInIncompleteChunk(std::streampos position, size_t chunksize, T pad_value);
        const void assertFilePointerIsAt(int pos);
        const void assertAtEOF();
        const size_t bytesLeftInChunk(size_t position, size_t chunksize);
    private:
        std::fstream bytestream;
    };

    // ################ //
    // # Constructors # //
    // ################ //
    template<std::ios_base::openmode openmode_flag>
    ReadWriter<openmode_flag>::ReadWriter(const std::string& filepath)
    {
        bytestream.open(filepath, openmode_flag | std::fstream::binary);
    }

    // ############### //
    // # Destructors # //
    // ############### //
    template<std::ios_base::openmode openmode_flag>
    ReadWriter<openmode_flag>::~ReadWriter()
    {

    }

    // ####################### //
    // # Filepointer Helpers # //
    // ####################### //
    template<std::ios_base::openmode openmode_flag>
    const void ReadWriter<openmode_flag>::assertFilePointerIsAt(int pos)
    {
        size_t actual_pos = this->Tell();
        if (actual_pos != pos)
            throw BadFilePointerException(pos, actual_pos);
    }


    // ####################### //
    // # ReadWrite Functions # //
    // ####################### //
    // readWriteData
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<readmode>::readWriteData(T& data_slot)
    {
        const int type_size = sizeof(T);
        ByteVector read_value(type_size);
        ReadWriter<readmode>::readWriteBytes(read_value);
        data_slot = serialisation::unpackBytes<T, endianness>(read_value);
    }
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<writemode>::readWriteData(T& data_slot)
    {
        const int type_size = sizeof(T);
        ByteVector write_value(type_size);
        write_value = serialisation::packBytes<T, endianness>(data_slot);
        ReadWriter<writemode>::readWriteBytes(write_value);
    }


    // readWriteConstData
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<readmode>::readWriteConstData(const T& data_slot, const std::string& data_slot_name)
    {
        T temp_data_slot;
        ReadWriter<readmode>::readWriteData<T, endianness>(temp_data_slot);
        if (temp_data_slot != data_slot)
        {
            throw UnexpectedValueException<T>(data_slot_name, data_slot, temp_data_slot);
        }
    }
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<writemode>::readWriteConstData(const T& data_slot, const std::string& data_slot_name)
    {
        T temp_data_slot = data_slot;
        ReadWriter<writemode>::readWriteData<T, endianness>(temp_data_slot);
    }


    // readWriteDataVector
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<readmode>::readWriteDataVector(std::vector<T>& data_slot)
    {
        const int type_size = sizeof(T);
        size_t size = data_slot.size();
        ByteVector read_value(size * type_size);
        ReadWriter::readWriteBytes(read_value);
        data_slot = serialisation::unpackVectorOfBytes<T, endianness>(read_value);
    }
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<writemode>::readWriteDataVector(std::vector<T>& data_slot)
    {
        const int type_size = sizeof(T);
        size_t size = data_slot.size();
        ByteVector write_value(size * type_size);
        write_value = serialisation::packVectorOfBytes<T, endianness>(data_slot);
        ReadWriter<writemode>::readWriteBytes(write_value);
    }

    // bytesLeftInIncompleteChunk
    template<std::ios_base::openmode openmode_flag>
    const size_t ReadWriter<openmode_flag>::bytesLeftInChunk(size_t position, size_t chunksize)
    {
        return (chunksize - (position % chunksize)) % chunksize;
    }

    // readWritePadBytesLeftInIncompleteChunk
    template<std::ios_base::openmode openmode_flag>
    const void ReadWriter<openmode_flag>::readWritePadBytesLeftInIncompleteChunk(std::streampos position, size_t chunksize)
    {
        this->readWriteBytesLeftInIncompleteChunk<char, LE>(position, chunksize, 0x00);
    }

    // readWriteBytesLeftInIncompleteChunk
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<readmode>::readWriteBytesLeftInIncompleteChunk(std::streampos position, size_t chunksize, T pad_value)
    {
        const size_t bytes_to_read = ReadWriter<readmode>::bytesLeftInChunk((size_t)position, chunksize);
        T out;
        for (unsigned int i = 0; i < bytes_to_read; i += sizeof(T))
        {
            this->readWriteData<T, endianness>(out);
            assert(out == pad_value);
        }
    }
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<writemode>::readWriteBytesLeftInIncompleteChunk(std::streampos position, size_t chunksize, T pad_value)
    {
        const size_t bytes_to_write = ReadWriter<writemode>::bytesLeftInChunk((size_t)position, chunksize);
        for (unsigned int i = 0; i < bytes_to_write; i += sizeof(T))
            this->readWriteData<T, endianness>(pad_value);
    }

}
