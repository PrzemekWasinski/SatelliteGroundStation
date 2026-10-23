#include <iostream>
#include <vector>
#include "CsvHandler.hpp"

int main() {
	auto data = readCSV("satellite_passes.csv");
	for (const auto& row : data) {
		for (const auto& cell : row) {
			std::cout << cell << "\t";
		}
		std::cout << std::endl;
	}

	return 0;
}
