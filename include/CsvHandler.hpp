#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "Pass.hpp"

std::vector<std::vector<std::string>> readCSV(const std::string& filename);
void writePasses(const std::filesystem::path& output, const std::vector<Pass>& passes);
