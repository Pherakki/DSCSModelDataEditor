#pragma once

#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <serialisation/Exceptions.hpp>
#include <serialisation/Serialisation.hpp>

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
        template<typename T, std::endian endianness> const void ReadWriteData(T& data_slot);
        template<typename T, std::endian endianness> const void ReadWriteConstData(const T& data_slot, const std::string& data_slot_name);
        template<typename T, std::endian endianness> const void ReadWriteDataArray(std::vector<T>& data_slot);
        const void ReadWriteAsciiToEOF(std::string& string_slot);
        const void ReadWriteAscii(std::string& string_slot, size_t string_length);
        const void ReadWriteConstAscii(const std::string& string_slot, const std::string& string_slot_name);
        const void ReadWriteBytes(ByteVector& bytes);
        const void ReadWriteBytesLeftInIncompleteChunk(std::streampos position, size_t chunksize, char pad_value = 0x00);
        const void AssertFilePointerIsAt(int pos);
        const void AssertAtEOF();
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
    const void ReadWriter<openmode_flag>::AssertFilePointerIsAt(int pos)
    {
        size_t actual_pos = this->Tell();
        if (actual_pos != pos)
            throw BadFilePointerException(pos, actual_pos);
    }

    // AssertAtEOF
    template<>
    const void ReadWriter<readmode>::AssertAtEOF()
    {
        if (bytestream.peek() != EOF)
            throw NotAtEOFException(this->Tell());
    }
    template<>
    const void ReadWriter<writemode>::AssertAtEOF() {}

    // Seek
    template<>
    void ReadWriter<readmode>::Seek(std::streampos pos)
    {
        this->bytestream.seekg(pos);
    }
    template<>
    void ReadWriter<writemode>::Seek(std::streampos pos)
    {
        this->bytestream.seekp(pos);
    }

    // Tell
    template<>
    std::streampos ReadWriter<readmode>::Tell()
    {
        return this->bytestream.tellg();
    }
    template<>
    std::streampos ReadWriter<writemode>::Tell()
    {
        return this->bytestream.tellp();
    }

    // ####################### //
    // # ReadWrite Functions # //
    // ####################### //
    // ReadWriteData
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<readmode>::ReadWriteData(T& data_slot)
    {
        const int type_size = sizeof(T);
        ByteVector read_value(type_size);
        ReadWriter<readmode>::ReadWriteBytes(read_value);
        data_slot = serialisation::unpackBytes<T, endianness>(read_value);
    }
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<writemode>::ReadWriteData(T& data_slot)
    {
        const int type_size = sizeof(T);
        ByteVector write_value(type_size);
        write_value = serialisation::packBytes<T, endianness>(data_slot);
        ReadWriter<writemode>::ReadWriteBytes(write_value);
    }


    // ReadWriteConstData
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<readmode>::ReadWriteConstData(const T& data_slot, const std::string& data_slot_name)
    {
        T temp_data_slot;
        ReadWriter<readmode>::ReadWriteData<T, endianness>(temp_data_slot);
        if (temp_data_slot != data_slot)
        {
            throw UnexpectedValueException<T>(data_slot_name, data_slot, temp_data_slot);
        }
    }
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<writemode>::ReadWriteConstData(const T& data_slot, const std::string& data_slot_name)
    {
        T temp_data_slot = data_slot;
        ReadWriter<writemode>::ReadWriteData<T, endianness>(temp_data_slot);
    }


    // ReadWriteDataArray
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<readmode>::ReadWriteDataArray(std::vector<T>& data_slot)
    {
        const int type_size = sizeof(T);
        size_t size = data_slot.size();
        ByteVector read_value(size * type_size);
        ReadWriter::ReadWriteBytes(read_value);
        data_slot = serialisation::unpackVectorOfBytes<T, endianness>(read_value);
    }
    template<>
    template<typename T, std::endian endianness>
    const void ReadWriter<writemode>::ReadWriteDataArray(std::vector<T>& data_slot)
    {
        const int type_size = sizeof(T);
        size_t size = data_slot.size();
        ByteVector write_value(size * type_size);
        write_value = serialisation::packVectorOfBytes<T, endianness>(data_slot);
        ReadWriter<writemode>::ReadWriteBytes(write_value);
    }

    // ReadWriteAsciiToEOF
    template<>
    const void ReadWriter<readmode>::ReadWriteAsciiToEOF(std::string& string_slot)
    {
        ByteVector read_value;
        ByteVector loop_var(1);
        while (bytestream.peek() != EOF)
        {
            ReadWriter::ReadWriteBytes(loop_var);
            read_value.push_back(loop_var[0]);
        }
        std::string string_value(read_value.begin(), read_value.end());
        string_slot = string_value;
    }
    template<>
    const void ReadWriter<writemode>::ReadWriteAsciiToEOF(std::string& string_slot)
    {
        size_t string_length = string_slot.size();
        ReadWriter<writemode>::ReadWriteAscii(string_slot, string_length);
    }

    // ReadWriteAscii
    template<>
    const void ReadWriter<readmode>::ReadWriteAscii(std::string& string_slot, size_t string_length)
    {
        ByteVector read_value(string_length);
        ReadWriter::ReadWriteBytes(read_value);
        std::string string_value(read_value.begin(), read_value.end());
        string_slot = string_value;
    }
    template<>
    const void ReadWriter<writemode>::ReadWriteAscii(std::string& string_slot, size_t string_length)
    {
        ByteVector write_value(string_length);
        write_value = ByteVector(string_slot.begin(), string_slot.end());
        ReadWriter::ReadWriteBytes(write_value);
    }
    // ReadWriteConstAscii
    template<>
    const void ReadWriter<readmode>::ReadWriteConstAscii(const std::string& string_slot, const std::string& string_slot_name)
    {
        std::string temp_string_slot;
        ReadWriter<readmode>::ReadWriteAscii(temp_string_slot, string_slot.size());
        if (temp_string_slot != string_slot)
        {
            throw UnexpectedValueException<std::string>(string_slot_name, string_slot, temp_string_slot);
        }
    }
    template<>
    const void ReadWriter<writemode>::ReadWriteConstAscii(const std::string& string_slot, const std::string& string_slot_name)
    {
        std::string temp_string_slot = string_slot;
        ReadWriter<writemode>::ReadWriteAscii(temp_string_slot, string_slot.size());
    }

    // ReadWriteBytes
    template<>
    const void ReadWriter<readmode>::ReadWriteBytes(ByteVector& bytes_slot)
    {
        bytestream.read(reinterpret_cast<char*>(bytes_slot.data()), bytes_slot.size());
    }
    template<>
    const void ReadWriter<writemode>::ReadWriteBytes(ByteVector& bytes_slot)
    {
        bytestream.write(reinterpret_cast<char*>(bytes_slot.data()), bytes_slot.size());
    }

    // bytesLeftInIncompleteChunk
    template<std::ios_base::openmode openmode_flag>
    const size_t ReadWriter<openmode_flag>::bytesLeftInChunk(size_t position, size_t chunksize)
    {
        return (chunksize - (position % chunksize)) % chunksize;
    }

    // ReadWriteBytesLeftInIncompleteChunk
    const void ReadWriter<readmode>::ReadWriteBytesLeftInIncompleteChunk(std::streampos position, size_t chunksize, char pad_value)
    {
        const size_t bytes_to_read = ReadWriter<readmode>::bytesLeftInChunk((size_t)position, chunksize);
        char out[1];
        for (unsigned int i = 0; i < bytes_to_read; i++)
        {
            bytestream.read(out, 1);
            assert(out[0] == pad_value);
        }

    }
    const void ReadWriter<writemode>::ReadWriteBytesLeftInIncompleteChunk(std::streampos position, size_t chunksize, char pad_value)
    {
        const size_t bytes_to_write = ReadWriter<writemode>::bytesLeftInChunk((size_t)position, chunksize);
        for (unsigned int i = 0; i < bytes_to_write; i++)
            bytestream.write(&pad_value, 1);
    }

}