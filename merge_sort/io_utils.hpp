#pragma once

#include <string>
#include <vector>
#include <fstream>

class Buffer {
public:
    explicit Buffer(size_t size_in_bytes);
    bool isFull() const;
    void add(int value);
    void clear();
    size_t size() const;
    const std::vector<int>& getData() const;

private:
    std::vector<int> data;
    size_t capacity;
};

class FileReader {
public:
    FileReader(const std::string& filename, Buffer& buffer);
    ~FileReader();
    bool hasNext();
    int next();
    void close();

private:
    void fillBuffer();
    std::ifstream file;
    Buffer& buffer;
    size_t current_pos;
};

class FileWriter {
public:
    FileWriter(const std::string& filename, Buffer& buffer);
    ~FileWriter();
    void write(int value);
    void flush();
    void close();
    bool isOpen() const;
    std::string fileName() const;
    bool bufferFull() const;

private:
    std::ofstream file;
    Buffer& buffer;
    std::string current_filename;
};
