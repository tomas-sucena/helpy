#ifndef HELPY_UTILS_HPP
#define HELPY_UTILS_HPP

#include <string>

namespace Helpy {
    class Utils {
    /* METHODS */
    public:
        static std::string toSnakeCase(const std::string &string) {
            std::string string_;

            for (char c : string) {
                if (std::isupper(c) && !string_.empty())
                    string_ += '_';

                string_ += (char) tolower(c);
            }

            return string_;
        }

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
