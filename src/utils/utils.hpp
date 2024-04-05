#ifndef HELPY_UTILS_HPP
#define HELPY_UTILS_HPP

#include <cmath>
#include <iostream>
#include <string>

// output formatting
#define RESET           "\033[0m"
#define BOLD            "\033[1m"
#define R_BOLD          "\033[22m"
#define ITALICS         "\033[3m"
#define R_ITALICS       "\033[23m"
#define UNDERLINE       "\033[4m"
#define R_UNDERLINE     "\033[24m"

// output colors
#define RED         "\033[31m"
#define YELLOW      "\033[33m"

namespace Helpy::Utils {
    /**
     * @brief Prints an error message in the console.
     * @param errorMsg string containing the error message
     * @param line number of the line in which the error occurred
     * @param quit bool indicating if the program should terminate
     */
    static void printError(const std::string &errorMsg, unsigned line = 0, bool quit = true) {
        std::cout << BOLD << RED << "Error";
        if (line) std::cout << R_BOLD << " (line " << line << ')' << BOLD;

        std::cout << ": " << RESET << errorMsg << std::endl;
        if (quit) exit(1);
    }

    /**
     * @brief Prints a warning message in the console.
     * @param warningMsg string containing the warning message
     * @param line number of the line which triggered the warning
     */
    static void printWarning(const std::string &warningMsg, unsigned line = 0) {
        std::cout << BOLD << YELLOW << "WARNING";
        if (line) std::cout << R_BOLD << " (line " << line << ')' << BOLD;

        std::cout << ": " << RESET << warningMsg << std::endl;
    }

    /**
     * @brief Converts a string to snake_case.
     *
     * Converts a string to snake_case, which is a writing style where each word is in lowercase and spaces
     * are replaced with underscores.
     * @param string string to be converted to snake_case
     * @return converted string
     */
    static std::string toSnakeCase(const std::string &string) {
        std::string string_;

        for (char c : string) {
            if (std::isupper(c) && !string_.empty())
                string_ += '_';

            string_ += (char) tolower(c);
        }

        return string_;
    }

    /**
     * @brief Computes the first prime number that comes after the argument number.
     * @param n variable which will store the first prime that comes after its initial value
     */
    static void nextPrime(int &n) {
        if (n < 2) {
            n = 2;
            return;
        }

        n += (n % 2) ? 2 : 1;

        while (true) {
            bool isPrime = true;

            for (int i = 3; i < sqrt(n); ++i) {
                if (n % i) continue;

                isPrime = false;
                break;
            }

            if (isPrime) break;
            n += 2;
        }
    }
}

#endif //HELPY_UTILS_HPP
