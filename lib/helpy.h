
/********************************************************
   src/utils/command.hpp
 ********************************************************/

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

/********************************************************
   src/utils/program.hpp
 ********************************************************/

#ifndef HELPY_PROGRAM_HPP
#define HELPY_PROGRAM_HPP

#include <cstdint>
#include <ostream>
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

/********************************************************
   src/utils/token.hpp
 ********************************************************/

/** @file */

#ifndef HELPY_TOKEN_HPP
#define HELPY_TOKEN_HPP

#include <cstdint>
#include <ostream>
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
     * @brief A struct that represents a Helpyfile token.
     */
    struct Token {
        TokenType type;
        uint16_t line, start, end;
        std::string value;

        /* CONSTRUCTOR */
        /**
         * @brief Creates a token.
         *
         * Creates a token given its type and, in the case of a literal, its value.
         * @param type the type of the token
         * @param line the line where the token was found
         * @param start the position where the token starts
         * @param end the position where the token ends
         * @param value the value (if any) of the token
         */
        Token(TokenType type, uint16_t line, uint16_t start, uint16_t end, std::string value = "")
            : type(type), line(line), start(start), end(end), value(std::move(value)) {}

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

            os << " , line: " << token.line << " , pos: " << token.start;
            if (!token.value.empty()) os << " , value: " << token.value;

            os << " }";
            return os;
        }
    };
}

#endif //HELPY_TOKEN_HPP

/********************************************************
   src/utils/utils.hpp
 ********************************************************/

#ifndef HELPY_UTILS_HPP
#define HELPY_UTILS_HPP

#include <cmath>
#include <iostream>
#include <string>

// output formatting
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define R_BOLD      "\033[22m"
#define ITALICS     "\033[3m"
#define R_ITALICS   "\033[23m"
#define UNDERLINE   "\033[4m"
#define R_UNDERLINE "\033[24m"

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

/********************************************************
   src/lexer/lexer.h
 ********************************************************/

#ifndef HELPY_LEXER_H
#define HELPY_LEXER_H

#include <fstream>
#include <unordered_map>
#include <vector>

#define uMap std::unordered_map

namespace Helpy {
    class Lexer {
        std::ifstream file;
        char curr;
        uint16_t line, pos;
        Program &program;

        static uMap<std::string, TokenType> keywords;

    /* CONSTRUCTOR */
    public:
        explicit Lexer(const std::string &path, Program &program);

    /* METHODS */
    private:
        void read();
        std::string readWord();
        std::string readString(char delimiter);
        void ignoreComment(bool multiline);

    public:
        std::vector<Token> execute();
    };
}

#endif //HELPY_LEXER_H

/********************************************************
   src/manager/manager.h
 ********************************************************/

#ifndef HELPY_MANAGER_H
#define HELPY_MANAGER_H

#include <string>

namespace Helpy {
    class Manager {
    /* METHODS */
    private:
        static std::string readInput(const std::string &instr);
        static void formatDirname(std::string &path);
        static bool createDirectory(std::string &path);
        static void writeHelpyfileTemplate(const std::string &path);

    public:
        static void init(std::string outputDir);
        static void run(std::string path, std::string outputDir);
    };
}

#endif //HELPY_MANAGER_H

/********************************************************
   src/parser/parser.h
 ********************************************************/

#ifndef HELPY_PARSER_H
#define HELPY_PARSER_H

namespace Helpy {
    struct ParserInfo {
        unsigned numArguments;
        std::string color;
        std::string classname;
        std::string filename;
        std::vector<Command> commands;
    };

    class Parser {
        std::vector<Token> tokens;
        std::vector<Token>::iterator it;
        Program &program;

    /* CONSTRUCTOR */
    public:
        Parser(std::vector<Token> tokens, Program &program);

    /* METHODS */
    private:
        std::string parseColor();
        std::vector<Command> parseCommands(unsigned &numArguments);
        void parseDescriptions(std::vector<Command> &commands);
        std::string parseName();

    public:
        ParserInfo execute();
    };
}

#endif //HELPY_PARSER_H

/********************************************************
   src/writer/writer.h
 ********************************************************/

#ifndef HELPY_WRITER_H
#define HELPY_WRITER_H

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#define uMap std::unordered_map

namespace Helpy {
    class Writer {
        ParserInfo info;
        std::ofstream header, source, utils;
        std::vector<uMap<std::string, int>> maps;

    /* CONSTRUCTOR */
    public:
        Writer(const std::string &path, ParserInfo info);

    /* METHODS */
    private:
        void writeHeaderGuards();
        void writeIncludes();

        // header
        void writeMethodsDeclaration();
        void writeClass();

        // source
        void writeMacros();
        void writeKeywordMaps();
        void writeUserMethods();
        void writeHelpyMethods();

        void writeUtils();
        void writeHeader();
        void writeSource();

    public:
        void execute();
    };
}

#endif //HELPY_WRITER_H
