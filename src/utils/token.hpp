/** @file */

#ifndef HELPY_TOKEN_HPP
#define HELPY_TOKEN_HPP

#include <cstdint>
#include <string>

namespace Helpy {
    /**
     * @brief An enum that details all the types of tokens.
     */
    enum class TokenType {
        Hyphen, /**< a hyphen */
        Word, /**< a single word */
        String, /**< a string, which can be comprised of many words */

        // keywords
        ColorKeyword, /**< the string 'COLOR' */
        CommandsKeyword, /**< the string 'COMMANDS' */
        DescriptionsKeyword, /**< the string 'DESCRIPTIONS' */
        NameKeyword, /**< the string 'NAME' */
    };

    /**
     * @brief A class that represents a Helpyfile token.
     */
    struct Token {
        TokenType type;
        uint16_t line;
        std::string value;

        /* CONSTRUCTOR */
        /**
         * @brief Creates a token.
         *
         * Creates a token given its type and, in the case of a literal, its value
         * @param type the type of the token
         * @param line the line where the token was found
         * @param value the value (if any) of the token
         */
        explicit Token(TokenType type, unsigned line, std::string value = "")
            : type(type), line(line), value(std::move(value)) {}

        /* METHODS */
        /**
         * @brief Outputs a token to a stream.
         *
         * Used exclusively for testing.
         * @param os output stream
         * @param token token to be output
         * @return output stream, after outputting the token
         */
        friend std::ostream& operator<<(std::ostream &os, const Token &token) {
            os << "{ type: ";

            switch (token.type) {
                case TokenType::Hyphen:
                    os << "Hyphen";
                    break;
                case TokenType::Word:
                    os << "Literal";
                    break;
                case TokenType::String:
                    os << "String";
                    break;
                case TokenType::ColorKeyword:
                    os << "COLOR";
                    break;
                case TokenType::CommandsKeyword:
                    os << "COMMANDS";
                    break;
                case TokenType::DescriptionsKeyword:
                    os << "DESCRIPTIONS";
                    break;
                case TokenType::NameKeyword:
                    os << "NAME";
                    break;
            }

            if (!token.value.empty()) os << ", value: " << token.value;

            os << " }";
            return os;
        }
    };
}

#endif //HELPY_TOKEN_HPP
