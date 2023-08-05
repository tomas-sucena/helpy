#include "manager.h"

#include <filesystem>
#include <iostream>

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../writer/writer.h"

namespace Helpy {
    void Manager::formatPath(std::string &path) {
        if (!path.empty() && path.back() != '/')
            path.push_back('/');
    }

    void Manager::writeHelpyfileTemplate(const std::string &path) {
        std::ofstream file(path + "Helpyfile");

        // name
        file << "/*\n"
             << "This will be the name of your class. It will also be used to name the generated code files.\n"
             << "Only letters, digits and '_' are allowed. The name must NOT start with a number.\n"
             << '\n'
             << "Note: If this field is omitted, the class will be named Helpy.\n"
             << "*/\n"
             << "NAME: Helpy\n";

        // color
        file << '\n'
             << "/*\n"
             << "This will be the main color used to style the command line.\n"
             << "It can be one of the following: red, green, yellow, blue, purple, cyan or white.\n"
             << '\n'
             << "Note: If this field is omitted, the selected color will be yellow.\n"
             << "*/\n"
             << "COLOR: yellow\n";

        // commands
        file << '\n'
             << "/*\n"
             << "The following lines detail the Helpy commands.\n"
             << "To add a command, simply enter a new line, type '-' and write the keywords\n"
             << "(eg: - RUN SORTING ALGORITHM).\n"
             << "\n"
             << "Note: Commands are case insensitive.\n"
             << "*/\n"
             << "COMMANDS:\n"
             << "- // write your command here\n";
    }

    void Manager::init(std::string path, const std::string &dirname) {
        formatPath(path);
        path += dirname;

        // create directory
        if (std::filesystem::is_directory(path))
            throw std::exception();

        std::filesystem::create_directory(path);

        path += '/';
        writeHelpyfileTemplate(path);
    }

    void Manager::run(std::string path, std::string filename) {
        formatPath(path);
        filename = path + filename;

        // read Helpyfile
        if (!std::filesystem::is_regular_file(filename))
            throw std::runtime_error("Error: Could not find file '" + filename + "'!");

        Lexer lexer(filename);
        Parser parser(lexer.execute());

        Writer writer(path, parser.execute());
        writer.execute();
    }
}
