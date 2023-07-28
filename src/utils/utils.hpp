#ifndef HELPY_UTILS_HPP
#define HELPY_UTILS_HPP

#include <iostream>
#include <string>

// output formatting
#define RESET      "\033[0m"
#define BOLD       "\033[1m"
#define R_BOLD     "\033[22m"
#define ITALICS    "\033[3m"
#define UNDERLINE  "\033[4m"

// output colors
#define RED        "\033[31m"

namespace Helpy {
    /**
     * @brief A class that encompasses helpful functions.
     */
    class Utils {
    /* METHODS */
    public:
        static void printError(const std::basic_string<char> &errorMsg, unsigned line = 0) {
            std::cout << RED << BOLD << "Error";
            if (line) std::cout << " in line " << line;

            std::cout << ": " << R_BOLD << errorMsg;
            exit(1);
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

                for (int i = 3; i < n; ++i) {
                    if (n % i) continue;

                    isPrime = false;
                    break;
                }

                if (isPrime) break;
                n += 2;
            }
        }
    };
}

#endif //HELPY_UTILS_HPP
