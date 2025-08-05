#include "io_utils.hpp"
#include <iostream>

// Buffer implementation
Buffer::Buffer(size_t size) : capacity(size), data() {
    data.reserve(capacity);
}

bool Buffer::isFull() const {
    return data.size() >= capacity;
}

void Buffer::add(int value) {
    if (!isFull()) {
        data.push_back(value);
    }
}

void Buffer::clear() {
    data.clear();
}

size_t Buffer::size() const {
    return data.size();
}

const std::vector<int>& Buffer::getData() const {
    return data;
}

// FileReader implementation
FileReader::FileReader(const std::string& filename, Buffer& buffer) : buffer(buffer), current_pos(0) {
    file.open(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
    }
    fillBuffer();
}

FileReader::~FileReader() {
    close();
}

bool FileReader::hasNext() {
    return current_pos < buffer.size() || !file.eof();
}

int FileReader::next() {
    if (current_pos >= buffer.size()) {
        fillBuffer();
    }
    if (current_pos < buffer.size()) {
        return buffer.getData()[current_pos++];
    }
    return -1; // Should not happen if hasNext() is checked
}

void FileReader::fillBuffer() {
    buffer.clear();
    current_pos = 0;
    // Read integers from the file into the buffer
    int val;
    while (buffer.size() < buffer.getData().capacity() && file.read(reinterpret_cast<char*>(&val), sizeof(int))) {
        buffer.add(val);
    }
}

void FileReader::close() {
    if (file.is_open()) {
        file.close();
    }
}

// FileWriter implementation
FileWriter::FileWriter(const std::string& filename, Buffer& buffer) : buffer(buffer), current_filename(filename) {
    file.open(filename, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
    }
}

FileWriter::~FileWriter() {
    close();
}

void FileWriter::write(int value) {
    if (buffer.isFull()) {
        flush();
    }
    buffer.add(value);
}

void FileWriter::flush() {
    const std::vector<int>& data = buffer.getData();
    if (!data.empty()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(int));
    }
    buffer.clear();
}

void FileWriter::close() {
    if (file.is_open()) {
        flush();
        file.close();
    }
}

bool FileWriter::isOpen() const {
    return file.is_open();
}

std::string FileWriter::fileName() const {
    return current_filename;
}

bool FileWriter::bufferFull() const {
    return buffer.isFull();
}
