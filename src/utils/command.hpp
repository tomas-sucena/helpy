#ifndef HELPY_COMMAND_HPP
#define HELPY_COMMAND_HPP

#include <string>
#include <vector>

namespace Helpy {
    class Command {
        std::vector<std::string> arguments;
        std::string methodName;

    /* METHODS */
    public:
        void push(std::string argument) {
            // update arguments
            for (char &c : argument)
                c = (char) tolower(c);

            arguments.emplace_back(argument);

            // update method name
            if (!methodName.empty())
                argument.front() = (char) toupper(argument.front());

            methodName += argument;
        }

        const std::string& operator[](int index) const {
            return arguments[index];
        }

        [[nodiscard]] const std::string& getMethodName() const {
            return methodName;
        }
    };
}

#endif //HELPY_COMMAND_HPP
