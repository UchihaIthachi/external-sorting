#pragma once
#include "tournament_tree.hpp"
#include "io_utils.hpp" 
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
#include <iostream>

void externalMergeSort(const std::string& inputFile, const std::string& outputFile, size_t memLimit);
