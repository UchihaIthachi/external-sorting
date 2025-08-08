#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./generate_input <output_file> <size_in_MB>\n";
        return 1;
    }

    std::string fileName = argv[1];
    int fileSizeMB = std::stoi(argv[2]);
    size_t bytesToWrite = fileSizeMB * 1024 * 1024;
    size_t intsToWrite = bytesToWrite / sizeof(int);

    std::ofstream outFile(fileName, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open output file.\n";
        return 1;
    }

    std::srand(std::time(nullptr));
    for (size_t i = 0; i < intsToWrite; ++i) {
        int num = 1 + std::rand() % 1000000;
        outFile.write(reinterpret_cast<char*>(&num), sizeof(int));
    }

    outFile.close();
    std::cout << "File '" << fileName << "' generated (" << fileSizeMB << " MB, " 
          << intsToWrite << " numbers).\n";
    return 0;
}
