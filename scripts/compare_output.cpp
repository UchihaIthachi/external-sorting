#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./compare_output <file1> <file2>\n";
        return 1;
    }

    std::ifstream file1(argv[1], std::ios::binary);
    std::ifstream file2(argv[2], std::ios::binary);

    if (!file1 || !file2) {
        std::cerr << "Error opening files.\n";
        return 1;
    }

    int a, b;
    size_t index = 0;
    while (file1.read(reinterpret_cast<char*>(&a), sizeof(int)) &&
           file2.read(reinterpret_cast<char*>(&b), sizeof(int))) {
        if (a != b) {
            std::cout << "Mismatch at index " << index << ": " << a << " != " << b << '\n';
        }
        ++index;
    }

    if (file1.read(reinterpret_cast<char*>(&a), sizeof(int))) {
        std::cout << "Extra data in file1 at index " << index << '\n';
    }
    if (file2.read(reinterpret_cast<char*>(&b), sizeof(int))) {
        std::cout << "Extra data in file2 at index " << index << '\n';
    }

    std::cout << "Comparison done.\n";
    return 0;
}
