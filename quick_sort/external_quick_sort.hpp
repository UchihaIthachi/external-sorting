#ifndef EXTERNAL_QUICK_SORT_HPP
#define EXTERNAL_QUICK_SORT_HPP

#include "interval_heap.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

void externalQuickSort(std::string inputFile, std::string outputFile, size_t memLimit, int recursion_level = 0);

#endif
