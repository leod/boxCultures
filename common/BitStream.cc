#include "BitStream.hh"

#include <algorithm>
#include <iostream>
#include <cassert>

BitStreamWriter::BitStreamWriter() {
}

void BitStreamWriter::writeBytes(uint8_t const* data, size_t size) {
    assert(data != nullptr);
    assert(size > 0);

    auto oldSize = buffer.size();
    buffer.resize(oldSize + size);
    std::copy(data, data + size, &buffer[oldSize]);
}

uint8_t const* BitStreamWriter::ptr() const {
    assert(buffer.size() > 0);

    return &buffer[0];
}

size_t BitStreamWriter::size() const {
    return buffer.size();
}

void BitStreamWriter::reset() {
    buffer.resize(0);
}

BitStreamReader::BitStreamReader(std::vector<uint8_t> const& v)
    : BitStreamReader(&v[0], v.size()) {
}

BitStreamReader::BitStreamReader(uint8_t const* buffer, size_t bufferLength)
    : buffer(buffer), bufferLength(bufferLength), index(0) {
    assert(buffer != nullptr);
    assert(bufferLength > 0);
}

void BitStreamReader::readBytes(uint8_t* out, size_t size) {
    assert(out != nullptr);
    assert(size > 0);
    assert(index + size <= bufferLength);

    std::copy(buffer + index, buffer + index + size, out);
    index += size;
}

bool BitStreamReader::eof() {
    return index == bufferLength;
}

std::vector<uint8_t> BitStreamReader::restVector() const {
    return std::vector<uint8_t>(buffer + index, buffer + bufferLength);
}

void BitStreamReader::skip(size_t offset) {
    assert(index + offset <= bufferLength);
    index += offset;
}

void write(BitStreamWriter& stream, std::string const& str) {
    write(stream, (uint32_t) str.size());
    stream.writeBytes(
            reinterpret_cast<const uint8_t*>(str.c_str()),
            str.size());
}

void read(BitStreamReader& stream, std::string& str) {
    uint32_t size;
    read(stream, size);

    uint8_t* buffer = new uint8_t[size+1];
    
    // This might just explode on you.
    stream.readBytes(
            reinterpret_cast<uint8_t*>(buffer),
            size);

    buffer[size] = 0;

    str = std::string((char *) buffer);

    delete buffer;
}
