#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

std::vector<int> readFile(char const* filename)
{
    std::vector<int> contents;
    std::ifstream file(filename);

    if (file.is_open()) {
        try {
            for (std::string line; getline(file, line);) {
                contents.emplace_back(std::stoi(line));
            }
        } catch (std::exception const& e) {
            std::cerr << "readFile: Exception caught when reading a file: " << filename
                      << std::endl;
            throw(e);
        }
    }

    return contents;
}

int simpleFuel(int mass)
{
    return mass / 3 - 2;
}

int recursiveFuel(int mass)
{
    int fuel = mass / 3 - 2;

    if (fuel <= 0) {
        return 0;
    }

    return fuel + recursiveFuel(fuel);
}

int totalFuel(std::vector<int> const& masses, std::function<int(int)> function)
{
    return std::accumulate(begin(masses), end(masses), 0, [&function](auto& sum, auto& mass) {
        return sum + function(mass);
    });
}

int main()
{
    auto masses = readFile("input");

    std::cout << "Solution for the first part: " << totalFuel(masses, simpleFuel) << std::endl;
    std::cout << "Solution for the second part: " << totalFuel(masses, recursiveFuel) << std::endl;

    return EXIT_SUCCESS;
}