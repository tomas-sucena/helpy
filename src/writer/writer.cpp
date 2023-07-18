#include "writer.h"

#include <thread>

namespace Helpy {
    Writer::Writer(const std::string &path, ParserInfo info) : info(std::move(info)) {
        header = std::ofstream(path + this->info.filename + ".h");
        source = std::ofstream(path + this->info.filename + ".cpp");
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
               << "#include <iostream>\n";
    }

    void Writer::writeKeywordMaps() {
        source << '\n';
        int prime = 2;

        for (int i = 1; i <= info.numArguments; ++i) {
            source << "uMap<string, int> " << info.classname << "::map" << i << " = {";

            int value = prime;
            for (auto it = info.commands.begin(); it != info.commands.end(); ) {
                source << "{\"" << (*it)[i - 1] << "\", " << value << '}'
                       << ((++it == info.commands.end()) ? "};\n" : ", ");

                value *= prime;
            }

            prime += 1;
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
    }

    void Writer::execute() {
        std::thread t1(&Writer::writeSource, this);
        writeHeader();

        t1.join();
    }
}
