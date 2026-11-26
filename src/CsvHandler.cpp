#include "CsvHandler.hpp"

#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>

#include <unistd.h>

namespace {
std::string timeOfDay(std::time_t time) {
    std::tm local{};
    char formatted[6];
    if (!localtime_r(&time, &local) || !std::strftime(formatted, sizeof formatted, "%H:%M", &local)) {
        throw std::runtime_error("Could not format pass time");
    }
    return formatted;
}
} // namespace

void writePasses(const std::filesystem::path& output, const std::vector<Pass>& passes) {
    const auto temporary = output.string() + "." + std::to_string(getpid()) + ".tmp";
    try {
        std::ofstream csv;
        csv.exceptions(std::ios::failbit | std::ios::badbit);
        csv.open(temporary);
        csv.imbue(std::locale::classic());
        csv << "Satellite name,pass_start,pass_end,elevation\n";
        for (const auto& pass : passes) {
            csv << pass.name << ',' << timeOfDay(pass.start) << ','
                << timeOfDay(pass.end) << ',' << pass.elevation << '\n';
        }
        csv.close();
        std::filesystem::rename(temporary, output);
    } catch (...) {
        std::error_code error;
        std::filesystem::remove(temporary, error);
        throw;
    }
}

std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    file.close();
    return data;
}
