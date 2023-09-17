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
         * @brief Adds a character of the source code.
         * @param c character to be added
         */
        void addChar(char c) {
            if (c == '\n')
                code.emplace_back("");
            else
                code.back().push_back(c);
        }
    };
}

#endif //HELPY_PROGRAM_HPP
