#include "writer.h"

namespace Helpy {
    Writer::Writer(const std::string &path, ParserInfo info) : info(std::move(info)) {
        header = std::ofstream(path + this->info.name + ".h");
        source = std::ofstream(path + this->info.name + ".cpp");
    }

    void Writer::writeIncludes() {
        source << "#include <iostream>\n";
    }

    void Writer::writeCommandMethods() {
        for (const Command &command : info.commands) {
            source << '\n'
                   << "void " << info.name << "::" << command.getMethodName() << "() {\n"
                   << "\tstd::cout << \"Under development!\";\n"
                   << "}\n";
        }
    }

    void Writer::writeSource() {
        writeIncludes();
        writeCommandMethods();
    }

    void Writer::execute() {
        writeSource();
    }
}