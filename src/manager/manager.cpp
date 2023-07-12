#include "manager.h"

#include <filesystem>
#include <fstream>

#define LINE_BREAK std::endl << std::endl

namespace Helpy {
    void Manager::writeHelpyfileTemplate(const std::string &path) {
        std::ofstream file(path + "Helpyfile");

        // name
        file << "/*\n"
             << "This will be the name of your class. It will also be used to name the generated code files.\n"
             << "Only letters, digits and '_' are allowed. The name must NOT start with a number.\n"
             << "\n"
             << "Note: If this field is omitted, the class will be named Helpy.\n"
             << "*/\n";

        file << "NAME: Helpy" << LINE_BREAK;

        // arguments
        file << "/*\n"
             << "This specifies the number of arguments each Helpy command will have.\n"
             << "*/\n";

        file << "ARGUMENTS: 3" << LINE_BREAK;

        // commands
        file << "/*\n"
             << "The following lines detail the Helpy commands.\n"
             << "To add a command, simply enter a new line, type '-' and write the keywords\n"
             << "(eg: - RUN SORTING ALGORITHM).\n"
             << "\n"
             << "Note: Commands are case insensitive.\n"
             << "*/\n";

        file << "COMMANDS:\n"
             << "- // write your command here\n";
    }

    void Manager::init(std::string path, const std::string &dirname) {
        std::string path_(std::move(path));

        if (!path_.empty() && path_.back() != '/')
            path_.push_back('/');

        path_ += dirname;

        // create directory
        if (std::filesystem::is_directory(path_))
            throw std::exception();

        std::filesystem::create_directory(path_);

        path_ += '/';
        writeHelpyfileTemplate(path_);
    }
}

