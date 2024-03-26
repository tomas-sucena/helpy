#ifndef HELPY_COMMAND_HPP
#define HELPY_COMMAND_HPP

#include <string>
#include <vector>

namespace Helpy {
    class Command {
        std::vector<std::string> arguments;
        std::string name;
        std::string signature;
        std::string description;
        unsigned value;

    /* CONSTRUCTOR */
    public:
        Command() : value(0) {}

    /* METHODS */
    public:
        void push(std::string argument) {
            bool empty = arguments.empty();

            // update arguments
            for (char &c : argument)
                c = (char) tolower(c);

            arguments.emplace_back(argument);

            // update name
            if (!empty)
                name += ' ';

            name += argument;

            // update signature
            if (!empty)
                argument.front() = (char) toupper(argument.front());

            signature += argument;
        }

        void setDescription(const std::string &newDescription) {
            description = newDescription;
        }

        const std::string& operator[](int index) const {
            return arguments[index];
        }

        void operator+=(unsigned val) {
            value += val;
        }

        [[nodiscard]] const std::string& getName() const {
            return name;
        }

        [[nodiscard]] const std::string& getSignature() const {
            return signature;
        }

        [[nodiscard]] const std::string& getDescription() const {
            return description;
        }

        [[nodiscard]] unsigned getValue() const {
            return value;
        }
    };
}

#endif //HELPY_COMMAND_HPP
