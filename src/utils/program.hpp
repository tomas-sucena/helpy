#ifndef HELPY_PROGRAM_HPP
#define HELPY_PROGRAM_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace Helpy {
    /**
     * @brief A struct that models the program and its internal state.
     */
    struct Program {
        std::vector<std::string> code;
        bool error;
        uint16_t warnings;

        /* CONSTRUCTOR */
        /**
         * @brief Creates a program.
         */
        Program() : error(false), warnings(0) {
            code.emplace_back("");
        }

        /* METHODS */
        /**
         * @brief Adds a character of the source code to the program.
         * @param c character to be added
         */
        void addChar(char c) {
            if (c == '\n')
                code.emplace_back("");
            else
                code.back().push_back(c);
        }

        /**
         * @brief Outputs a program to a stream.
         *
         * Used exclusively for testing purposes.
         * @param os output stream
         * @param program program to be output
         * @return output stream, after outputting the program
         */
        friend std::ostream& operator<<(std::ostream &os, const Program &program) {
            for (const std::string &line : program.code)
                os << line << '\n';

            return os;
        }
    };
}

#endif //HELPY_PROGRAM_HPP
