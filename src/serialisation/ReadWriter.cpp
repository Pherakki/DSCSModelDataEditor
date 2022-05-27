#include "ReadWriter.hpp"

namespace serialisation
{
    // ####################### //
    // # Filepointer Helpers # //
    // ####################### //
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


    // assertAtEOF
    template<>
    const void ReadWriter<readmode>::assertAtEOF()
    {
        if (bytestream.peek() != EOF)
            throw NotAtEOFException(this->Tell());
    }
    template<>
    const void ReadWriter<writemode>::assertAtEOF() {}


    // ####################### //
    // # ReadWrite Functions # //
    // ####################### //

    // readWriteBytes
    template<>
    const void ReadWriter<readmode>::readWriteBytes(ByteVector& bytes_slot)
    {
        bytestream.read(reinterpret_cast<char*>(bytes_slot.data()), bytes_slot.size());
    }

    template<>
    const void ReadWriter<writemode>::readWriteBytes(ByteVector& bytes_slot)
    {
        bytestream.write(reinterpret_cast<char*>(bytes_slot.data()), bytes_slot.size());
    }

    // readWriteAscii
    template<>
    const void ReadWriter<readmode>::readWriteAscii(std::string& string_slot, size_t string_length)
    {
        ByteVector read_value(string_length);
        ReadWriter<readmode>::readWriteBytes(read_value);
        std::string string_value(read_value.begin(), read_value.end());
        string_slot = string_value;
    }
    template<>
    const void ReadWriter<writemode>::readWriteAscii(std::string& string_slot, size_t string_length)
    {
        ByteVector write_value(string_length);
        write_value = ByteVector(string_slot.begin(), string_slot.end());
        ReadWriter<writemode>::readWriteBytes(write_value);
    }

    // readWriteAsciiToEOF
    template<>
    const void ReadWriter<readmode>::readWriteAsciiToEOF(std::string& string_slot)
    {
        ByteVector read_value;
        ByteVector loop_var(1);
        while (bytestream.peek() != EOF)
        {
            ReadWriter::readWriteBytes(loop_var);
            read_value.push_back(loop_var[0]);
        }
        std::string string_value(read_value.begin(), read_value.end());
        string_slot = string_value;
    }
    template<>
    const void ReadWriter<writemode>::readWriteAsciiToEOF(std::string& string_slot)
    {
        size_t string_length = string_slot.size();
        ReadWriter<writemode>::readWriteAscii(string_slot, string_length);
    }

    // readWriteConstAscii
    template<>
    const void ReadWriter<readmode>::readWriteConstAscii(const std::string& string_slot, const std::string& string_slot_name)
    {
        std::string temp_string_slot;
        ReadWriter<readmode>::readWriteAscii(temp_string_slot, string_slot.size());
        if (temp_string_slot != string_slot)
        {
            throw UnexpectedValueException<std::string>(string_slot_name, string_slot, temp_string_slot);
        }
    }
    template<>
    const void ReadWriter<writemode>::readWriteConstAscii(const std::string& string_slot, const std::string& string_slot_name)
    {
        std::string temp_string_slot = string_slot;
        ReadWriter<writemode>::readWriteAscii(temp_string_slot, string_slot.size());
    }
}
