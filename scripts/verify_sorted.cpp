#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./verify_sorted <file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    int prev_val;
    if (!file.read(reinterpret_cast<char*>(&prev_val), sizeof(int))) {
        std::cout << "File is empty or contains only one element. It is considered sorted." << std::endl;
        return 0;
    }

    int current_val;
    long long count = 1;
    while (file.read(reinterpret_cast<char*>(&current_val), sizeof(int))) {
        if (current_val < prev_val) {
            std::cerr << "Verification failed: File is not sorted." << std::endl;
            std::cerr << "Mismatch at index " << count << ":" << std::endl;
            std::cerr << "Previous value: " << prev_val << std::endl;
            std::cerr << "Current value: " << current_val << std::endl;
            return 1;
        }
        prev_val = current_val;
        count++;
    }

    std::cout << "Verification successful: File is sorted." << std::endl;
    return 0;
}
