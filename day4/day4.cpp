#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// Converts integer to vector of digits
std::vector<int> numberToVector(int number)
{
    std::vector<int> intVector;

    while (number != 0) {
        intVector.emplace_back(number % 10);
        number /= 10;
    }

    std::reverse(begin(intVector), end(intVector));

    return intVector;
}

// Checks if the number has at least one double or more digit
bool hasSameAdjacent(std::vector<int> const& numberVector)
{
    return std::adjacent_find(begin(numberVector), end(numberVector)) != end(numberVector);
}

// Checks if the digits are in rising order
bool isRising(std::vector<int> const& numberVector)
{
    return std::adjacent_find(begin(numberVector),
                              end(numberVector),
                              [](auto const& first, auto const& second) {
                                  return first > second;
                              }) == end(numberVector);
}

// Checks if there is at least one double digit
bool hasTwoAdjacent(std::vector<int> const& numberVector)
{
    int previousNumber = numberVector.front();
    int count = 0;
    std::vector<int> counts;

    for(auto const& number: numberVector) {
        if(number == previousNumber) {
            ++count;
        } else {
            counts.push_back(count);
            count = 1;
            previousNumber = number;
        }
    }
    counts.push_back(count);

    return std::find(begin(counts), end(counts), 2) != end(counts);
}

int main()
{
    int start = 367479;
    int end = 893698;

    std::vector<int> firstPasswords;

    for (int i = start; i <= end; ++i) {
        if (auto numberVector = numberToVector(i);
            hasSameAdjacent(numberVector) && isRising(numberVector)) {
            firstPasswords.push_back(i);
        }
    }

    std::cout << "Solution for the first part: " << firstPasswords.size() << std::endl;

    std::vector<int> secondPasswords;

    std::copy_if(std::begin(firstPasswords), std::end(firstPasswords), std::back_inserter(secondPasswords), [](auto const& password) {
        return hasTwoAdjacent(numberToVector(password));
    });

    std::cout << "Solution for the second part: " << secondPasswords.size() << std::endl;

    return EXIT_SUCCESS;
}