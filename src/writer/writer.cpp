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
              << "#include <iostream>\n"
              << "#include <list>\n"
              << "#include <sstream>\n"
              << "#include <string>\n"
              << "#include <unordered_map>\n"
              << "#include <unordered_set>\n"
              << '\n'
              << "#include \"../external/libfort/fort.hpp\"\n"
              << '\n'
              << "#define uMap std::unordered_map\n"
              << "#define uSet std::unordered_set\n"
              << '\n'
              << "using std::string;\n";

        header << '\n'
               << "#include \"utils.hpp\"\n";

        source << "#include \"" << info.filename << ".h\"\n";
    }

    void Writer::writeMethodsDeclaration() {
        header << '\n'
               << "\t/* METHODS */\n"
               << "\tstatic string readInput(const string &instruction, uSet<string> &options);\n"
               << "\tstatic double readNumber(const string &instruction);\n"
               << '\n'
               << "\tbool executeCommand(";

        for (int i = 1; i <= info.numArguments; ++i)
            header << "const string &s" << i << ((i < info.numArguments) ? ", " : ");\n");

        header << "\tvoid advancedMode();\n"
               << "\tvoid guidedMode();\n";

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

    void Writer::writeMacros() {
        source << '\n'
               << "// formatting\n"
                  "#define RESET      \"\\033[0m\"\n"
                  "#define BOLD       \"\\033[1m\"\n"
                  "#define ITALICS    \"\\033[3m\"\n"
                  "#define UNDERLINE  \"\\033[4m\"\n"
               << '\n'
               << "// output colors\n"
                  "#define RED        \"\\033[31m\"\n"
                  "#define GREEN      \"\\033[32m\"\n"
                  "#define YELLOW     \"\\033[33m\"\n"
                  "#define BLUE       \"\\033[34m\"\n"
                  "#define PURPLE     \"\\033[35m\"\n"
                  "#define CYAN       \"\\033[36m\"\n"
                  "#define WHITE      \"\\033[37m\"\n"
               << '\n'
               << "// text\n"
                  "#define DASHED_LINE \"- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\"\n"
                  "#define BREAK       '\\n' << " << info.color << " << DASHED_LINE << RESET << '\\n' << std::endl\n"
                  "#define YES_NO      \" (\" << GREEN << \"Yes\" << RESET << '/' << RED << \"No\" << RESET << ')'\n";
    }

    void Writer::writeKeywordMaps() {
        source << '\n';
        int prime = 2;

        for (int i = 0; i < info.numArguments; ++i) {
            source << "uMap<string, int> " << info.classname << "::map" << i + 1 << " = {";

            int value = prime;
            for (auto &command : info.commands) {
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

    void Writer::writeMethodsDefinition() {
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
                   << "void " << info.classname << "::" << command.getMethodName() << "() {\n"
                   << "\tstd::cout << BREAK;\n"
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
                  "\t\tstd::cout << instruction << std::endl << std::endl;\n"
                  "\n"
                  "\t\tstring line; getline(std::cin >> std::ws, line);\n"
                  "\t\tUtils::toLowercase(line);\n"
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
                  "\t\tstd::cout << RED << \"Invalid command! Please, try again.\" << RESET << std::endl;\n"
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
                  "\t\tUtils::toLowercase(line);\n"
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

        // executeCommand()
        source << '\n'
               << "/**\n"
               << " * @brief parses the arguments that were inputted and executes the corresponding command\n";

        std::string numerals[8] = {"first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth"};

        for (int i = 1; i <= info.numArguments; ++i)
            source << " * @param s" << i << ' ' << numerals[i - 1] << " argument of the command\n";

        source << " * @return 'true' if the command exists, 'false' otherwise\n"
               << " */\n"
               << "bool " << info.classname << "::executeCommand(";

        for (int i = 1; i <= info.numArguments; ++i)
            source << "const string &s" << i << ((i < info.numArguments) ? ", " : ") {\n");

        source << "\tswitch (";

        for (int i = 1; i <= info.numArguments; ++i)
            source << "map" << i << "[s" << i << ']'
                   << ((i < info.numArguments) ? " + " : ") {\n");

        for (const Command &command : info.commands)
            source << "\t\tcase (" << command.getValue() << ") :\n"
                   << "\t\t\t" << command.getMethodName() << "();\n"
                   << "\t\t\t" << "break;\n";

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
                  " * @brief executes the advanced mode of the UI\n"
                  " */\n"
               << "void " << info.classname << "::advancedMode() {\n"
                  "\tbool done = false;\n"
                  "\n"
                  "\twhile (!done) {\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << \"How can I be of assistance?\" << std::endl << std::endl;\n"
                  "\n"
                  "\t\tstring ";

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
            source << 's' << i << ((i < info.numArguments) ? ", " : "))\n");

        source << "\t\t\tcontinue;\n"
                  "\n"
                  "\t\tstd::cout << BREAK;\n"
                  "\t\tstd::cout << \"Anything else?\" << YES_NO << std::endl << std::endl;\n"
                  "\n"
                  "\t\ts1.clear(); getline(std::cin >> std::ws, s1);\n"
                  "\t\tUtils::toLowercase(s1);\n"
                  "\n"
                  "\t\ts_.clear(); s_.str(s1);\n"
                  "\t\tdone = true;\n"
                  "\n"
                  "\t\twhile (s_ >> s1) {\n"
                  "\t\t\tif (s1 != \"yes\" && s1 != \"y\")\n"
                  "\t\t\t\tcontinue;\n"
                  "\n"
                  "\t\t\tdone = false;\n"
                  "\t\t\tbreak;\n"
                  "\t\t}\n"
                  "\t}\n"
                  "\n"
                  "\tstd::cout << BREAK;\n"
                  "\tstd::cout << \"See you next time!\" << std::endl << std::endl;\n"
                  "}\n";

        // guidedMode()
        source << '\n'
               << "void " << info.classname << "::guidedMode() {\n"
               << "\tstd::cout << \"Temporarily unavailable!\" << std::endl;\n"
               << "}\n";

        // run()
        source << '\n'
               << "void " << info.classname << "::run() {\n"
               << "\tstring instruction = \"Which mode would you prefer?\\n\\n\"\n"
                  "                         \"* Guided\\n\"\n"
                  "                         \"* Advanced\";\n"
                  "\tuSet<string> options = {\"guided\", \"advanced\", \"adv\"};\n"
                  "\n"
                  "\t(readInput(instruction, options) == \"guided\") ? guidedMode() : advancedMode();\n"
                  "}\n";
    }

    void Writer::writeUtils() {
        utils << '\n'
              << "namespace Utils {\n";

        // toLowercase()
        utils << "\t/**\n"
              << "\t * @brief turns all the characters of a string into lowercase\n"
              << "\t * @complexity O(n)\n"
              << "\t * @param s string to be modified\n"
              << "\t */\n"
              << "\tstatic void toLowercase(string &s) {\n"
              << "\t\tfor (char &c : s)\n"
              << "\t\t\tc = (char) tolower(c);\n"
              << "\t}\n";

        // toUppercase()
        utils << '\n'
              << "\t/**\n"
              << "\t * @brief turns all the characters of a string into uppercase\n"
              << "\t * @complexity O(n)\n"
              << "\t * @param s string to be modified\n"
              << "\t */\n"
              << "\tstatic void toUppercase(string &s) {\n"
              << "\t\tfor (char &c : s)\n"
              << "\t\t\tc = (char) toupper(c);\n"
              << "\t}\n";

        // createTable()
        utils << '\n'
              << "\t/**\n"
              << "\t* @brief creates a fort::utf8_table that will be used to display information in the terminal\n"
              << "\t* @param columnNames list containing the name of each column of the table\n"
              << "\t* @return fort::char_table object\n"
              << "\t*/\n"
              << "\tstatic fort::char_table createTable(const std::list<string> &columnNames) {\n"
              << "\t\tfort::char_table table;\n"
              << "\n"
              << "\t\ttable.set_border_style(FT_NICE_STYLE);\n"
              << "\t\ttable.row(0).set_cell_content_text_style(fort::text_style::bold);\n"
              << "\t\ttable.row(0).set_cell_content_fg_color(fort::color::yellow);\n"
              << "\t\ttable << fort::header;\n"
              << '\n'
              << "\t\tauto it = columnNames.begin();\n"
              << "\t\tfor (int i = 0; it != columnNames.end(); ++i){\n"
              << "\t\t\ttable << *it++;\n"
              << "\t\t\ttable.column(i).set_cell_text_align(fort::text_align::center);\n"
              << "\t\t}\n"
              << '\n'
              << "\t\ttable << fort::endr;\n"
              << "\t\treturn table;\n"
              << "\t}\n";

        utils << "}\n"
              << '\n'
              << "#endif\n";
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
        writeMethodsDefinition();
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
