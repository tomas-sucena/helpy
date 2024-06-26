#include "writer.h"

#include <thread>

#include "../utils/utils.hpp"

namespace Helpy {
    Writer::Writer(const std::string &path, ParserInfo info) : info(std::move(info)) {
        header = std::ofstream(path + this->info.filename + ".h");
        source = std::ofstream(path + this->info.filename + ".cpp");
        utils = std::ofstream(path + this->info.filename + "_utils.hpp");

        maps.resize(this->info.numArguments);
    }

    void Writer::writeHeaderGuards() {
        std::string uppercaseFilename;

        for (char c : info.filename)
            uppercaseFilename += (char) toupper(c);

        header << "#ifndef " << uppercaseFilename << "_H\n"
               << "#define " << uppercaseFilename << "_H\n";

        utils << "#ifndef " << uppercaseFilename << "_UTILS_H\n"
              << "#define " << uppercaseFilename << "_UTILS_H\n";
    }

    void Writer::writeIncludes() {
        utils << '\n'
              << "#include <filesystem>\n"
              << "#include <iostream>\n"
              << "#include <sstream>\n"
              << "#include <string>\n"
              << "#include <unordered_map>\n"
              << "#include <unordered_set>\n"
              << "#include <vector>\n"
              << '\n'
              << "#include \"../external/libfort/fort.hpp\"\n"
              << '\n'
              << "#define uMap std::unordered_map\n"
              << "#define uSet std::unordered_set\n";

        header << '\n'
               << "#include \"" << info.filename << "_utils.hpp\"\n";

        source << "#include \"" << info.filename << ".h\"\n";
    }

    void Writer::writeMethodsDeclaration() {
        // user-defined methods
        header << "\n"
                  "\t/* METHODS */\n"
                  "\t// commands\n";

        for (const Command &command : info.commands)
            header << "\tvoid " << command.getSignature() << "();\n";

        // Helpy methods
        header << "\n"
                  "\t// DO NOT ALTER THE DECLARATIONS BELOW!\n"
                  "\tstatic std::string readInput(const std::string &instruction, bool caseSensitive = false);\n"
                  "\tstatic std::string readInput(const std::string &instruction, const std::vector<std::string> &options);\n"
                  "\tstatic bool readYesOrNo(const std::string &instruction, bool strict = false);\n"
                  "\tstatic double readNumber(const std::string &instruction);\n"
                  "\tstatic double readNumber(const std::string &instruction, double minimum, double maximum);\n"
                  "\tstatic double readNumber(const std::string &instruction, const std::vector<double> &options);\n"
                  "\tstatic std::string readFilename(const std::string &instruction);\n"
                  "\tstatic std::string readDirname(const std::string &instruction);\n"
                  "\tstatic std::vector<std::string> readCSV(const std::string &instruction, char delimiter = ',');\n"
                  "\n"
                  "\tbool executeCommand(int value);\n"
                  "\tvoid advancedMode();\n"
                  "\tvoid guidedMode();\n";

        header << '\n'
               << "public:\n"
               << "\tvoid run();\n";
    }

    void Writer::writeClass() {
        header << '\n'
               << "class " << info.classname << " {\n"
               << "\tstatic uMap<std::string, int> ";

        for (int i = 1; i <= info.numArguments; ++i)
            header << "map" << i << ((i < info.numArguments) ? ", " : ";\n");

        writeMethodsDeclaration();
        header << "};\n";
    }

    void Writer::writeMacros() {
        source << '\n'
               << "// formatting\n"
                  "#define RESET        \"\\033[0m\"\n"
                  "#define BOLD         \"\\033[1m\"\n"
                  "#define ITALICS      \"\\033[3m\"\n"
                  "#define UNDERLINE    \"\\033[4m\"\n"
               << '\n'
               << "// output colors\n"
                  "#define RED          \"\\033[31m\"\n"
                  "#define GREEN        \"\\033[32m\"\n"
                  "#define YELLOW       \"\\033[33m\"\n"
                  "#define BLUE         \"\\033[34m\"\n"
                  "#define PURPLE       \"\\033[35m\"\n"
                  "#define CYAN         \"\\033[36m\"\n"
                  "#define WHITE        \"\\033[37m\"\n"
               << '\n'
               << "// text\n"
                  "#define DASHED_LINE  \"- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\"\n"
                  "#define BREAK        '\\n' << " << info.color << " << DASHED_LINE << RESET << '\\n' << std::endl\n"
                  "#define YES_NO       std::string(\" (\") + GREEN + \"Yes\" + RESET + \"/\" + RED + \"No\" + RESET + \")\"\n";
    }

    void Writer::writeKeywordMaps() {
        source << '\n';
        int prime = 2;

        for (int i = 0; i < info.numArguments; ++i) {
            source << "uMap<std::string, int> " << info.classname << "::map" << i + 1 << " = {";

            int value = prime;
            for (Command &command : info.commands) {
                const std::string &argument = command[i];

                // new argument
                if (maps[i].insert({argument, value}).second) {
                    source << "{\"" << command[i] << "\", " << value << "},";
                    value *= prime;
                }

                command += maps[i][argument];
            }

            source << "};\n";
            Utils::nextPrime(prime);
        }
    }

    void Writer::writeUserMethods() {
        for (const Command &command : info.commands) {
            source << '\n'
                   << "/**\n"
                   << " * @brief ";

            // write the method description
            for (const char &c : command.getDescription()) {
                source << c;
                if (c == '\n') source << " * ";
            }

            source << '\n'
                   << " */\n"
                   << "void " << info.classname << "::" << command.getSignature() << "() {\n"
                   << "\tstd::cout << BREAK;\n"
                   << "\tstd::cout << \"Under development!\" << std::endl;\n"
                   << "}\n";
        }
    }

    void Writer::writeHelpyMethods() {
        source << "\n"
                  "/********************************************************\n"
                  "\tDO NOT ALTER THE METHODS BELOW!\n"
                  " ********************************************************/\n";

        // readInput()
        source << "\n"
                  "/**\n"
                  " * @brief Reads a line of user input.\n"
                  " * @param instruction the instruction that will be displayed before prompting the user to input\n"
                  " * @param caseSensitive boolean indicating whether the input should be treated as case-sensitive"
                  " * @return read input\n"
                  " */\n"
               << "std::string " << info.classname << "::readInput(const std::string &instruction, bool caseSensitive) {\n"
                  "\t// display the instruction\n"
                  "\tstd::cout << BREAK;\n"
                  "\tstd::cout << instruction << '\\n' << std::endl;\n"
                  "\n"
                  "\t// read the user input\n"
                  "\tstd::string input; getline(std::cin >> std::ws, input);\n"
                  "\n"
                  "\t// if the input is NOT case-sensitive, convert it to lowercase\n"
                  "\tif (!caseSensitive) {\n"
                  "\t\tUtils::toLowercase(input);\n"
                  "\t}\n"
                  "\n"
                  "\treturn input;\n"
                  "}\n";

        // readInput(options)
        source << "\n"
                  "/**\n"
                  " * @brief Reads user input.\n"
                  " * @param instruction the instruction that will be displayed before prompting the user to input\n"
                  " * @param options the options that will be displayed to the user\n"
                  " * @return read input\n"
                  " */\n"
               << "std::string " << info.classname << "::readInput(const std::string &instruction, const std::vector<std::string> &options) {\n"
                  "\tstd::string input;\n"
                  "\tbool valid = false;\n"
                  "\n"
                  "\t// hash the options to achieve better search performance\n"
                  "\tuSet<std::string> options_(options.begin(), options.end());\n"
                  "\n"
                  "\tfor (;;) {\n"
                  "\t\tstd::string line = " << info.classname << "::readInput(instruction);\n"
                  "\t\tstd::istringstream line_(line);\n"
                  "\n"
                  "\t\twhile (line_ >> input) {\n"
                  "\t\t\tif (options_.find(input) == options_.end())\n"
                  "\t\t\t\tcontinue;\n"
                  "\n"
                  "\t\t\tvalid = true;\n"
                  "\t\t\tbreak;\n"
                  "\t\t}\n"
                  "\n"
                  "\t\tif (valid) break;\n"
                  "\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << RED << \"Invalid command! Please, try again.\" << RESET << std::endl;\n"
                  "\t}\n"
                  "\n"
                  "\treturn input;\n"
                  "}\n";

        // readYesOrNo()
        source << '\n'
               << "/**\n"
                  " * @brief Reads the user's answer to a Yes/No question.\n"
                  " * @param instruction the instruction that will be displayed before prompting the user to input\n"
                  " * @param strict boolean indicating if the user must explicitly type Yes or No\n"
                  " * @return 'true' if the user answered Yes, 'false' otherwise\n"
                  " */\n"
               << "bool " << info.classname << "::readYesOrNo(const std::string &instruction, bool strict) {\n"
                  "\tstd::string input = strict\n"
                  "\t\t? " << info.classname << "::readInput(instruction + YES_NO, {\"yes\", \"no\", \"y\", \"n\"})\n"
                  "\t\t: " << info.classname << "::readInput(instruction + YES_NO);\n"
                  "\n"
                  "\treturn input == \"yes\" || input == \"y\";\n"
                  "}\n";

        // readNumber()
        source << '\n'
               << "/**\n"
                  " * @brief Reads a number from the console.\n"
                  " * @param instruction the instruction that will be displayed before prompting the user to input the number\n"
                  " * @return the number input by the user\n"
                  " */\n"
               << "double " << info.classname << "::readNumber(const std::string &instruction){\n"
                  "\tdouble number;\n"
                  "\tbool valid = false;\n"
                  "\n"
                  "\tfor (;;) {\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << instruction << '\\n' << std::endl;\n"
                  "\n"
                  "\t\tstd::string line; getline(std::cin >> std::ws, line);\n"
                  "\t\tUtils::toLowercase(line);\n"
                  "\n"
                  "\t\tstd::istringstream line_(line);\n"
                  "\n"
                  "\t\tstd::string temp;\n"
                  "\t\twhile (line_ >> temp){\n"
                  "\t\t\ttry {\n"
                  "\t\t\t\tnumber = stod(temp);\n"
                  "\t\t\t}\n"
                  "\t\t\tcatch(...) {\n"
                  "\t\t\t\tcontinue;\n"
                  "\t\t\t}\n"
                  "\n"
                  "\t\t\tvalid = true;\n"
                  "\t\t\tbreak;\n"
                  "\t\t}\n"
                  "\n"
                  "\t\tif (valid) break;\n"
                  "\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << RED << \"Invalid input! Please, try again.\" << RESET << std::endl;\n"
                  "\t}\n"
                  "\n"
                  "\treturn number;\n"
                  "}\n";

        // readNumber(minimum, maximum)
        source << '\n'
               << "/**\n"
                  "* @brief Reads a number from the console.\n"
                  "* @param instruction the instruction that will be displayed before prompting the user to input the number\n"
                  "* @param minimum the minimum accepted number\n"
                  "* @param maximum the maximum accepted number\n"
                  "* @return the number input by the user\n"
                  "*/\n"
               << "double " << info.classname << "::readNumber(const std::string &instruction, double minimum, double maximum) {\n"
                  "\tdouble number;\n"
                  "\n"
                  "\tfor(;;) {\n"
                  "\t\tnumber = " << info.classname << "::readNumber(instruction);\n"
                  "\n"
                  "\t\t// verify if the number is within the specified range\n"
                  "\t\tif (number >= minimum && number <= maximum) {\n"
                  "\t\t\tbreak;\n"
                  "\t\t}\n"
                  "\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << RED << \"Invalid number! Please, try again.\" << RESET << std::endl;\n"
                  "\t}\n"
                  "\n"
                  "\treturn number;\n"
                  "}\n";

        // readNumber(options)
        source << '\n'
               << "/**\n"
                  " * @brief Reads a number from the console.\n"
                  " * @param instruction the instruction that will be displayed before prompting the user to input the number\n"
                  " * @param options the options (numbers) that will be displayed to the user\n"
                  " * @return the number input by the user\n"
                  " */\n"
               << "double " << info.classname << "::readNumber(const std::string &instruction, const std::vector<double> &options) {\n"
                  "\tdouble number;\n"
                  "\n"
                  "\t// hash the options to achieve better search performance\n"
                  "\tuSet<double> options_(options.begin(), options.end());\n"
                  "\n"
                  "\tfor (;;) {\n"
                  "\t\tnumber = " << info.classname << "::readNumber(instruction);\n"
                  "\n"
                  "\t\t// verify if the number is one of the options\n"
                  "\t\tif (options_.find(number) != options_.end())\n"
                  "\t\t\tbreak;\n"
                  "\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << RED << \"Invalid number! Please, try again.\" << RESET << std::endl;\n"
                  "\t}\n"
                  "\n"
                  "\treturn number;\n"
                  "}\n";

        // readFilename()
        source << '\n'
               << "/**\n"
                  " * @brief Reads a path from the console and verifies if it corresponds to a file.\n"
                  " * @param instruction the instruction that will be displayed before prompting the user to input the number\n"
                  " * @return the path input by the user\n"
                  " */\n"
               << "std::string " << info.classname << "::readFilename(const std::string &instruction) {\n"
                  "\tstd::string filename;\n"
                  "\tbool valid = false;\n"
                  "\n"
                  "\tfor (;;) {\n"
                  "\t\tstd::string line = " << info.classname << "::readInput(instruction, true);\n"
                  "\t\tstd::istringstream line_(line);\n"
                  "\n"
                  "\t\twhile (!line_.eof()) {\n"
                  "\t\t\t// fetch the filename\n"
                  "\t\t\tif (line_.peek() == '\"') {\n"
                  "\t\t\t\tline_.get(); // consume the leading quotation mark\n"
                  "\t\t\t\tstd::getline(line_, filename, '\"');\n"
                  "\t\t\t}\n"
                  "\t\t\telse {\n"
                  "\t\t\t\tline_ >> filename;\n"
                  "\t\t\t}\n"
                  "\n"
                  "\t\t\t// verify if the file exists\n"
                  "\t\t\tif (!std::filesystem::is_regular_file(filename)) {\n"
                  "\t\t\t\tcontinue;\n"
                  "\t\t\t}\n"
                  "\n"
                  "\t\t\tvalid = true;\n"
                  "\t\t\tbreak;\n"
                  "\t\t}\n"
                  "\n"
                  "\t\tif (valid) break;\n"
                  "\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << RED << \"Invalid filename! Please, try again.\" << RESET << std::endl;\n"
                  "\t}\n"
                  "\n"
                  "\treturn filename;\n"
                  "}\n";

        // readDirname()
        source << '\n'
               << "/**\n"
                  " * @brief Reads a path from the console and verifies if it corresponds to a directory.\n"
                  " * @param instruction the instruction that will be displayed before prompting the user to input the number\n"
                  " * @return the path input by the user\n"
                  " */\n"
               << "std::string " << info.classname << "::readDirname(const std::string &instruction) {\n"
                  "\tstd::string dirname;\n"
                  "\tbool valid = false;\n"
                  "\n"
                  "\tfor (;;) {\n"
                  "\t\tstd::string line = " << info.classname << "::readInput(instruction, true);\n"
                  "\t\tstd::istringstream line_(line);\n"
                  "\n"
                  "\t\twhile (!line_.eof()) {\n"
                  "\t\t\t// fetch the name of the directory\n"
                  "\t\t\tif (line_.peek() == '\"') {\n"
                  "\t\t\t\tline_.get(); // consume the leading quotation mark\n"
                  "\t\t\t\tstd::getline(line_, dirname, '\"');\n"
                  "\t\t\t}\n"
                  "\t\t\telse {\n"
                  "\t\t\t\tline_ >> dirname;\n"
                  "\t\t\t}\n"
                  "\n"
                  "\t\t\t// verify if the file exists\n"
                  "\t\t\tif (!std::filesystem::is_directory(dirname)) {\n"
                  "\t\t\t\tcontinue;\n"
                  "\t\t\t}\n"
                  "\n"
                  "\t\t\tvalid = true;\n"
                  "\t\t\tbreak;\n"
                  "\t\t}\n"
                  "\n"
                  "\t\tif (valid) break;\n"
                  "\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << RED << \"Invalid directory! Please, try again.\" << RESET << std::endl;\n"
                  "\t}\n"
                  "\n"
                  "\t// format the name of the directory\n"
                  "\tif (dirname.back() != '/') {\n"
                  "\t\tdirname += '/';\n"
                  "\t}\n"
                  "\n"
                  "\treturn dirname;\n"
                  "}\n";

        // readCSV()
        source << '\n'
               << "/**\n"
                  " * @brief Reads and parses a line of user input containing comma-separated values.\n"
                  " * @param instruction the instruction that will be displayed before prompting the user to input\n"
                  " * @param delimiter the character that separates each value\n"
                  " * @return the values input by the user\n"
                  " */\n"
               << "std::vector<std::string> " << info.classname << "::readCSV(const std::string &instruction, char delimiter) {\n"
                  "\tstd::cout << BREAK;\n"
                  "\tstd::cout << instruction << '\\n' << std::endl;\n"
                  "\n"
                  "\t// read the user input\n"
                  "\tstd::string line; getline(std::cin >> std::ws, line);\n"
                  "\tUtils::toLowercase(line);\n"
                  "\n"
                  "\tstd::istringstream line_(line);\n"
                  "\n"
                  "\t// separate the user input into values\n"
                  "\tstd::vector<std::string> values;\n"
                  "\tfor (std::string value; getline(line_, value, delimiter); ) {\n"
                  "\t\tvalues.emplace_back(value);\n"
                  "\t}\n"
                  "\n"
                  "\treturn values;\n"
                  "}\n";

        // executeCommand()
        source << "\n"
                  "/**\n"
                  " * @brief Parses the arguments that were input and executes the corresponding command.\n"
                  " * @param value the value that will be used in the switch case to choose which command to execute\n"
                  " * @return 'true' if the command exists, 'false' otherwise\n"
                  " */\n"
               << "bool " << info.classname << "::executeCommand(int value) {\n"
                  "\tswitch (value) {\n";

        for (int i = 0; i < (int) info.commands.size(); ++i)
            source << "\t\tcase -" << i + 1 << " :\n"
                      "\t\tcase " << info.commands[i].getValue() << " :\n"
                      "\t\t\t" << info.commands[i].getSignature() << "();\n"
                      "\t\t\t" << "break;\n\n";

        source << "\t\tdefault :\n"
               << "\t\t\tstd::cout << BREAK;\n"
               << "\t\t\tstd::cout << RED << \"Invalid command! Please, type another command.\" << RESET << std::endl;\n"
               << '\n'
               << "\t\t\treturn false;\n"
               << "\t}\n"
               << '\n'
               << "\treturn true;\n"
               << "}\n";

        // advancedMode()
        source << '\n'
               << "/**\n"
                  " * @brief Executes the advanced mode of the UI.\n"
                  " */\n"
               << "void " << info.classname << "::advancedMode() {\n"
                  "\tfor (;;) {\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << \"How can I be of assistance?\" << '\\n' << std::endl;\n"
                  "\n"
                  "\t\tstd::string ";

        for (int i = 1; i <= info.numArguments; ++i)
            source << 's' << i << ((i < info.numArguments) ? ", " : ";\n");

        source << "\t\tstd::istringstream s_;\n"
                  "\n"
                  "\t\tstd::cin >> s1; Utils::toLowercase(s1);\n"
                  "\n"
                  "\t\tif (s1 == \"quit\" || s1 == \"no\" || s1 == \"die\")\n"
                  "\t\t\tbreak;\n"
                  "\n";

        for (int i = 2; i <= info.numArguments; ++i)
            source << "\t\tstd::cin >> s" << i << "; Utils::toLowercase(s" << i << ");\n";

        source << "\n"
                  "\t\tif (!executeCommand(";

        for (int i = 1; i <= info.numArguments; ++i)
            source << "map" << i << "[s" << i << ']' << ((i < info.numArguments) ? " + " : "))\n");

        source << "\t\t\tcontinue;\n"
                  "\n"
                  "\t\t// ask the user if they want to execute another command\n"
                  "\t\tif (!" << info.classname << "::readYesOrNo(\"Anything else?\"))\n"
                  "\t\t\tbreak;\n"
                  "\t}\n"
                  "}\n";

        // guidedMode()
        source << '\n'
               << "/**\n"
                  " * @brief Executes the guided mode of the UI.\n"
                  " */\n"
               << "void " << info.classname << "::guidedMode() {\n"
                  "\tstd::string instruction = \"How can I be of assistance?\\n\\n\";\n"
                  "\n";

        int numCommands = (int) info.commands.size();

        for (int i = 1; i <= numCommands; ++i) {
            const Command &command = info.commands[i - 1];

            source << "\t// " << command.getSignature() << "()\n"
                   << "\tinstruction += (std::string) BOLD + " << info.color << " + \"" << i << " - \" + WHITE + \"";

            bool hasDescription = !command.getDescription().empty(); // verify if the command has a description
            bool isLast = i == numCommands; // verify if we are writing the last command

            switch ((isLast << 1) + hasDescription) {
                // write command that neither has a description nor is last
                case 0 :
                    source << command.getName() << "\\n\"\n"
                           << "\t            + RESET + '\\n'";
                    break;

                // write command that has a description but is not last
                case 1 :
                    source << command.getName() << "\\n\"\n"
                           << "\t            + RESET + ITALICS + \"" << command.getDescription() << "\\n\"\n"
                           << "\t            + RESET + '\\n'";
                    break;

                // write the last command (without description)
                case 2 :
                    source << command.getName() << "\"\n"
                           << "\t            + RESET";
                    break;

                // write the last command (with description)
                case 3 :
                    source << command.getName() << "\\n\"\n"
                           << "\t            + RESET + ITALICS + \"" << command.getDescription() << "\"\n"
                           << "\t            + RESET";
                    break;
            }

            source << ";\n"
                   << '\n';
        }

        source << "\tfor (;;) {\n"
                  "\t\tint num = (int) -readNumber(instruction);\n"
                  "\t\tif (!executeCommand(num))\n"
                  "\t\t\tcontinue;\n"
                  "\n"
                  "\t\t// ask the user if they want to execute another command\n"
                  "\t\tif (!" << info.classname << "::readYesOrNo(\"Anything else?\"))\n"
                  "\t\t\tbreak;\n"
                  "\t}\n"
                  "}\n";

        // run()
        source << '\n'
               << "/**\n"
                  " * @brief Runs the command-line menu.\n"
                  " */\n"
               << "void " << info.classname << "::run() {\n"
               << "\tstd::string instruction = \"Which mode would you prefer?\\n\\n\"\n"
                  "                            " << info.color << " \"* \" RESET \"Guided\\n\"\n"
                  "                            " << info.color << " \"* \" RESET \"Advanced\";\n"
                  "\n"
                  "\t(readInput(instruction, {\"guided\", \"advanced\", \"adv\"}) == \"guided\")\n"
                  "\t\t? " << info.classname << "::guidedMode()\n"
                  "\t\t: " << info.classname << "::advancedMode();\n"
                  "\n"
                  "\tstd::cout << BREAK;\n"
                  "\tstd::cout << \"See you next time!\\n\" << std::endl;\n"
                  "}\n";
    }

    void Writer::writeUtils() {
        utils << "\n"
                 "namespace Utils {\n";

        // toLowercase()
        utils << "\t/**\n"
                 "\t * @brief Turns all the characters of a string into lowercase.\n"
                 "\t * @complexity O(n)\n"
                 "\t * @param s string to be modified\n"
                 "\t */\n"
              << "\tstatic void toLowercase(std::string &s) {\n"
                 "\t\tfor (char &c : s)\n"
                 "\t\t\tc = (char) tolower(c);\n"
                 "\t}\n";

        // toUppercase()
        utils << "\n"
                 "\t/**\n"
                 "\t * @brief Turns all the characters of a string into uppercase.\n"
                 "\t * @complexity O(n)\n"
                 "\t * @param s string to be modified\n"
                 "\t */\n"
              << "\tstatic void toUppercase(std::string &s) {\n"
                 "\t\tfor (char &c : s)\n"
                 "\t\t\tc = (char) toupper(c);\n"
                 "\t}\n";

        // createTable()
        utils << "\n"
                 "\t/**\n"
                 "\t* @brief Creates a fort::char_table that will be used to display information in the terminal.\n"
                 "\t* @param columnNames list containing the name of each column of the table\n"
                 "\t* @return fort::char_table object\n"
                 "\t*/\n"
              << "\tstatic fort::char_table createTable(const std::vector<std::string> &columnNames) {\n"
                 "\t\tfort::char_table table;\n"
                 "\n"
                 "\t\t// customize the table\n"
                 "\t\ttable.set_border_style(FT_NICE_STYLE);\n"
                 "\t\ttable.row(0).set_cell_content_text_style(fort::text_style::bold);\n"
                 "\t\ttable.row(0).set_cell_content_fg_color(fort::color::yellow);\n"
                 "\n"
                 "\t\t// set the table header\n"
                 "\t\ttable << fort::header;\n"
                 "\t\tauto it = columnNames.begin();\n"
                 "\n"
                 "\t\tfor (int i = 0; it != columnNames.end(); ++i) {\n"
                 "\t\t\ttable << *it++;\n"
                 "\n"
                 "\t\t\t// set the alignment of each column\n"
                 "\t\t\ttable.column(i).set_cell_text_align(fort::text_align::center);\n"
                 "\t\t}\n"
                 "\n"
                 "\t\ttable << fort::endr;\n"
                 "\t\treturn table;\n"
                 "\t}\n";

        // createUTF8Table()
        utils << "\n"
                 "\t/**\n"
                 "\t* @brief Creates a fort::utf8_table that will be used to display information in the terminal.\n"
                 "\t* @param columnNames list containing the name of each column of the table\n"
                 "\t* @return fort::utf8_table object\n"
                 "\t*/\n"
              << "\tstatic fort::utf8_table createUTF8Table(const std::vector<std::string> &columnNames) {\n"
                 "\t\tfort::utf8_table table;\n"
                 "\n"
                 "\t\t// customize the table\n"
                 "\t\ttable.set_border_style(FT_NICE_STYLE);\n"
                 "\t\ttable.row(0).set_cell_content_text_style(fort::text_style::bold);\n"
                 "\t\ttable.row(0).set_cell_content_fg_color(fort::color::yellow);\n"
                 "\n"
                 "\t\t// set the table header\n"
                 "\t\ttable << fort::header;\n"
                 "\t\tauto it = columnNames.begin();\n"
                 "\n"
                 "\t\tfor (int i = 0; it != columnNames.end(); ++i) {\n"
                 "\t\t\ttable << *it++;\n"
                 "\n"
                 "\t\t\t// set the alignment of each column\n"
                 "\t\t\ttable.column(i).set_cell_text_align(fort::text_align::center);\n"
                 "\t\t}\n"
                 "\n"
                 "\t\ttable << fort::endr;\n"
                 "\t\treturn table;\n"
                 "\t}\n";

        // createMDTable()
        utils << "\n"
                 "\t/**\n"
                 "\t * @brief Creates a table that will be output to a Markdown file.\n"
                 "\t * @param columnNames list containing the name of each column of the table\n"
                 "\t * @return string representing a Markdown table\n"
                 "\t */\n"
              << "\tstatic std::string createMDTable(const std::vector<std::string> &columnNames) {\n"
                 "\t\tsize_t numColumns = columnNames.size();\n"
                 "\t\tif (numColumns == 0) return \"\";\n"
                 "\n"
                 "\t\tstd::string table = \"|\";\n"
                 "\n"
                 "\t\t// set the table header\n"
                 "\t\tfor (const std::string &columnName : columnNames)\n"
                 "\t\t\ttable += ' ' + columnName + \" |\";\n"
                 "\n"
                 "\t\ttable += \"\\n|\";\n"
                 "\n"
                 "\t\t// set the alignment of each column\n"
                 "\t\tfor (size_t i = 0; i < numColumns; ++i)\n"
                 "\t\t\ttable += \":-:|\";\n"
                 "\n"
                 "\t\ttable += '\\n';\n"
                 "\t\treturn table;\n"
                 "\t}\n";

        utils << "}\n"
                 "\n"
                 "#endif\n";
    }

    void Writer::writeHeader() {
        writeClass();

        // close the header guard
        header << '\n'
               << "#endif\n";
    }

    void Writer::writeSource() {
        writeMacros();
        writeKeywordMaps();
        writeUserMethods();
        writeHelpyMethods();
    }

    void Writer::execute() {
        writeHeaderGuards();
        writeIncludes();

        // write the code
        std::thread t1(&Writer::writeSource, this);
        std::thread t2(&Writer::writeUtils, this);
        writeHeader();

        t1.join();
        t2.join();
    }
}
