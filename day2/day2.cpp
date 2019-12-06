#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

// Reads comma separated integers from a file into a vector
std::vector<int> readFile(char const* filename)
{
    std::vector<int> contents;
    std::ifstream file(filename);

    if (file.is_open()) {
        try {
            for (std::string line; getline(file, line, ',');) {
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

class Computer
{
public:
    Computer() : intcode_{}, instructionPointer_{0}
    {
    }

    void setIntcode(std::vector<int> intcode)
    {
        instructionPointer_ = 0;
        intcode_ = std::move(intcode);
    }

    void setProgramInputs(int noun, int verb)
    {
        intcode_.at(1) = noun;
        intcode_.at(2) = verb;
    }

    // Runs the program
    void run()
    {
        do {
            auto instruction = getInstruction(instructionPointer_);

            if (instruction.opcode == 1) {
                intcode_.at(instruction.destinationPos) = intcode_.at(instruction.inputPos1) +
                                                          intcode_.at(instruction.inputPos2);
            } else if (instruction.opcode == 2) {
                intcode_.at(instruction.destinationPos) = intcode_.at(instruction.inputPos1) *
                                                          intcode_.at(instruction.inputPos2);
            } else if (instruction.opcode == 99) {
                break;
            }
        } while (stepForward());
    }

    // Returns value at given position of intcode
    int getValue(int position = 0)
    {
        return intcode_.at(position);
    }

private:
    // Single instruction
    struct Instruction
    {
        int opcode;
        int inputPos1;
        int inputPos2;
        int destinationPos;
    };

    // Returns instruction at position /index/
    Instruction getInstruction(int index) const
    {
        return {
            intcode_.at(index),
            intcode_.at(index + 1),
            intcode_.at(index + 2),
            intcode_.at(index + 3),
        };
    }

    // Steps to next instruction. Returns false if end of the intcode is reached.
    bool stepForward()
    {
        instructionPointer_ += 4;

        if (instructionPointer_ + 1 > intcode_.size()) {
            return false;
        }

        return true;
    }

    std::vector<int> intcode_;
    int instructionPointer_;
};

// Tries to find a noun and verb that produce the given solution
std::optional<std::pair<int, int>> findInputs(int solution,
                                              std::vector<int> const& intcode,
                                              Computer& computer)
{
    for (int noun = 0; noun <= 99; ++noun) {
        for (int verb = 0; verb <= 99; ++verb) {
            computer.setIntcode(intcode);
            computer.setProgramInputs(noun, verb);
            computer.run();
            if (computer.getValue() == solution) {
                return std::pair{noun, verb};
            }
        }
    }

    return std::nullopt;
}

int main()
{
    auto intcode = readFile("input");

    Computer shipComputer;

    // First part
    shipComputer.setIntcode(intcode);
    shipComputer.setProgramInputs(12, 2);
    shipComputer.run();
    std::cout << "Solution for the first part: " << shipComputer.getValue() << std::endl;

    // Second part
    auto inputs = findInputs(19690720, intcode, shipComputer);
    if (inputs.has_value()) {
        auto& [noun, verb] = inputs.value();
        auto solution = 100 * noun + verb;
        std::cout << "Solution for the second part: 100 * " << noun << " + " << verb << " = "
                  << solution << std::endl;
    }

    return EXIT_SUCCESS;
}