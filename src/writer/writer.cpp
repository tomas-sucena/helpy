#include "writer.h"

#include <thread>

#include "../utils/utils.hpp"

namespace Helpy {
    Writer::Writer(const std::string &path, ParserInfo info) : info(std::move(info)) {
        header = std::ofstream(path + this->info.filename + ".h");
        source = std::ofstream(path + this->info.filename + ".cpp");

        maps.resize(this->info.numArguments);
    }

    void Writer::writeHeaderIncludes() {
        std::string uppercaseFilename;

        for (char c : info.filename)
            uppercaseFilename += (char) toupper(c);

        header << "#ifndef " << uppercaseFilename << "_H\n"
               << "#define " << uppercaseFilename << "_H\n"
               << '\n'
               << "#include <string>\n"
               << "#include <unordered_map>\n"
               << "#include <unordered_set>\n"
               << '\n'
               << "#define uMap std::unordered_map\n"
               << "#define uSet std::unordered_set\n"
               << '\n'
               << "using std::string;\n";
    }

    void Writer::writeMethodsDeclaration() {
        header << '\n'
               << "\t/* METHODS */\n"
               << "\tstatic string readInput(const string &instruction, uSet<string> &options);\n"
               << "\tstatic double readNumber(const string &instruction);\n"
               << "\tvoid advancedMode();\n"
               << "\tvoid guidedMode();\n"
               << "\tbool processCommand(";

        for (int i = 1; i <= info.numArguments; ++i)
            header << "const string &s" << i << ((i < info.numArguments) ? ", " : ");\n");

        // user-defined methods
        header << "\n\t// commands\n";

        for (const Command &command : info.commands)
            header << "\tvoid " << command.getMethodName() << "();\n";

        header << '\n'
               << "public:\n"
               << "\tvoid run();\n";
    }

    void Writer::writeClass() {
        header << '\n'
               << "class " << info.classname << " {\n"
               << "\tstatic uMap<string, int> ";

        for (int i = 1; i <= info.numArguments; ++i)
            header << "map" << i << ((i < info.numArguments) ? ", " : ";\n");

        writeMethodsDeclaration();
        header << "};\n";
    }

    void Writer::writeSourceIncludes() {
        source << "#include \"" << info.filename << ".h\"\n"
               << '\n'
               << "#include <iostream>\n"
               << "#include <sstream>\n"
               << '\n'
               << "using std::cout;\n"
               << "using std::endl;\n"
               << '\n'
               << "// output colors\n"
                  "#define RESET   \"\\033[0;m\"\n"
                  "#define RED     \"\\033[1;31m\"\n"
                  "#define GREEN   \"\\033[32m\"\n"
                  "#define BLUE    \"\\033[34m\"\n"
                  "#define YELLOW  \"\\033[33m\"\n"
                  "#define BOLD    \"\\033[1m\"\n"
               << '\n'
               << "// text\n"
                  "#define DASHED_LINE \"- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\"\n"
                  "#define BREAK       endl << YELLOW << DASHED_LINE << RESET << endl << endl\n"
                  "#define YES_NO      \" (\" << GREEN << \"Yes\" << RESET << '/' << RED << \"No\" << RESET << ')'\n";
    }

    void Writer::writeKeywordMaps() {
        source << '\n';
        int prime = 2;

        for (int i = 0; i < info.numArguments; ++i) {
            source << "uMap<string, int> " << info.classname << "::map" << i + 1 << " = {";

            int value = prime;
            for (auto &command : info.commands) {
                if (!maps[i].insert({command[i], value}).second)
                    continue;

                source << "{\"" << command[i] << "\", " << value << "},";
                value *= prime;
            }

            source << "};\n";
            Utils::nextPrime(prime);
        }
    }

    void Writer::writeMethodsDefinition() {
        for (const Command &command : info.commands) {
            source << '\n'
                   << "void " << info.classname << "::" << command.getMethodName() << "() {\n"
                   << "\tstd::cout << \"Under development!\" << std::endl;\n"
                   << "}\n";
        }
    }

    void Writer::writeHelpyMethods() {
        // readInput()
        source << "\n"
                  "/**\n"
                  " * @brief reads a line of user input\n"
                  " * @param instruction the instruction that will be displayed before prompting the user to type\n"
                  " * @param options the options that will be displayed to the user\n"
                  " * @return read input\n"
                  " */\n"
               << "string " << info.classname << "::readInput(const string &instruction, uSet<string> &options) {\n"
                  "\tstring res;\n"
                  "\tbool valid = false;\n"
                  "\n"
                  "\twhile (true) {\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << instruction << endl << endl;\n"
                  "\n"
                  "\t\tstring line; getline(std::cin >> std::ws, line);\n"
                  "\t\tUtils::lowercase(line);\n"
                  "\n"
                  "\t\tstd::istringstream line_(line);\n"
                  "\n"
                  "\t\twhile (line_ >> res) {\n"
                  "\t\t\tif (options.find(res) == options.end())\n"
                  "\t\t\t\tcontinue;\n"
                  "\n"
                  "\t\t\tvalid = true;\n"
                  "\t\t\tbreak;\n"
                  "\t\t}\n"
                  "\n"
                  "\t\tif (valid) break;\n"
                  "\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << RED << \"Invalid command! Please, try again.\" << RESET << endl;\n"
                  "\t}\n"
                  "\n"
                  "\treturn res;\n"
                  "}\n";

        // readNumber()
        source << '\n'
               << "/**\n"
                  " * @brief reads a number from the console\n"
                  " * @param instruction the instruction that will be displayed before prompting the user to input the number\n"
                  " * @return the number inputted by the user\n"
                  " */\n"
               << "double " << info.classname << "::readNumber(const string &instruction){\n"
                  "\tdouble res;\n"
                  "\tbool valid = false;\n"
                  "\n"
                  "\twhile (true){\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << instruction << std::endl << std::endl;\n"
                  "\n"
                  "\t\tstring line; getline(std::cin >> std::ws, line);\n"
                  "\t\tUtils::lowercase(line);\n"
                  "\n"
                  "\t\tstd::istringstream line_(line);\n"
                  "\n"
                  "\t\tstring temp;\n"
                  "\t\twhile (line_ >> temp){\n"
                  "\t\t\ttry {\n"
                  "\t\t\t\tres = stod(temp);\n"
                  "\t\t\t}\n"
                  "\t\t\tcatch(...){\n"
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
                  "\treturn res;\n"
                  "}\n";

        // processCommand()
        source << '\n'
               << "bool " << info.classname << "::processCommand(";

        for (int i = 1; i <= info.numArguments; ++i)
            source << "const string &s" << i << ((i < info.numArguments) ? ", " : ") {\n");

        source << "\tswitch (";

        for (int i = 1; i <= info.numArguments; ++i)
            source << "map" << i << "[s" << i << ']'
                   << ((i < info.numArguments) ? " + " : ") {\n");

        for (const Command &command : info.commands) {
            int sum = 0;
            for (int i = 0; i < info.numArguments; ++i)
                sum += maps[i][command[i]];

            source << "\t\tcase (" << sum << ") :\n"
                   << "\t\t\t" << command.getMethodName() << "();\n"
                   << "\t\t\t" << "break;\n";
        }

        source << "\t\tdefault :\n"
               << "\t\t\tcout << BREAK;\n"
               << "\t\t\tcout << RED << \"Invalid command! Please, type another command.\" << RESET << endl;\n"
               << '\n'
               << "\t\t\treturn false;\n"
               << "\t}\n"
               << '\n'
               << "\treturn true;\n"
               << "}\n";
    }

    void Writer::writeHeader() {
        writeHeaderIncludes();
        writeClass();

        // close the header guard
        header << '\n'
               << "#endif\n";
    }

    void Writer::writeSource() {
        writeSourceIncludes();
        writeKeywordMaps();
        writeMethodsDefinition();
        writeHelpyMethods();
    }

    void Writer::execute() {
        std::thread t1(&Writer::writeSource, this);
        writeHeader();

        t1.join();
    }
}
