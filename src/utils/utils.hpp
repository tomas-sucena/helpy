#ifndef HELPY_UTILS_HPP
#define HELPY_UTILS_HPP

#include <string>

namespace Helpy {
    /**
     * A class that encompasses helpful functions.
     */
    class Utils {
    /* METHODS */
    public:
        /**
         * @brief converts a string to snake_case, which is a writing style where each word is in lowercase and spaces
         * are replaced with underscores
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
         * @brief computes the first prime number that comes after the argument number
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
