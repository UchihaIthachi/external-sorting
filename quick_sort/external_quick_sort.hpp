#ifndef EXTERNAL_QUICK_SORT_HPP
#define EXTERNAL_QUICK_SORT_HPP

#include "interval_heap.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

void externalQuickSort(std::string inputFile, std::string outputFile, size_t memLimit);

#endif
