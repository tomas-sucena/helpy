#ifndef HELPY_COMMAND_HPP
#define HELPY_COMMAND_HPP

#include <string>
#include <vector>

namespace Helpy {
    class Command {
        std::vector<std::string> arguments;

    /* METHODS */
    public:
        void push(std::string argument) {
            for (char &c : argument)
                c = (char) tolower(c);

            arguments.emplace_back(argument);
        }

        [[nodiscard]] std::string getMethodName() const {
            std::string name;
            bool firstWord = true;

            for (std::string argument : arguments) {
                if (!firstWord) argument[0] = (char) toupper(argument[0]);
                firstWord = false;

                name += argument;
            }

            return name;
        }
    };
}

#endif //HELPY_COMMAND_HPP
