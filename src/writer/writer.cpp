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
               << "#include <unordered_map>\n";
    }

    void Writer::writeSourceIncludes() {
        source << "#include \"" << info.filename << ".h\"\n"
               << '\n'
               << "#include <iostream>\n";
    }

    void Writer::writeCommandMethods() {
        for (const Command &command : info.commands) {
            source << '\n'
                   << "void " << info.classname << "::" << command.getMethodName() << "() {\n"
                   << "\tstd::cout << \"Under development!\";\n"
                   << "}\n";
        }
    }

    void Writer::writeHeader() {
        writeHeaderIncludes();

        // close the header guard
        header << '\n'
               << "#endif\n";
    }

    void Writer::writeSource() {
        writeSourceIncludes();
        writeCommandMethods();
    }

    void Writer::execute() {
        std::thread t1(&Writer::writeSource, this);
        writeHeader();

        t1.join();
    }
}
