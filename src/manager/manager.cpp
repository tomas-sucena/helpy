#include "manager.h"

#include <filesystem>
#include <iostream>

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "../writer/writer.h"
#include "../utils/utils.hpp"

// formatting
#define RESET      "\033[0m"
#define BOLD       "\033[1m"
#define ITALICS    "\033[3m"
#define UNDERLINE  "\033[4m"

// output colors
#define RED        "\033[31m"
#define GREEN      "\033[32m"
#define YELLOW     "\033[33m"
#define BLUE       "\033[34m"
#define PURPLE     "\033[35m"
#define CYAN       "\033[36m"
#define WHITE      "\033[37m"

// text
#define DASHED_LINE "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
#define BREAK       std::endl << YELLOW << DASHED_LINE << RESET << std::endl << std::endl
#define YES_NO      " (" << GREEN << "Yes" << RESET << '/' << RED << "No" << RESET << ')'

namespace fs = std::filesystem;

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

    /**
     * @brief writes a template for a Helpyfile, which is the file that contains the Helpy specification
     * @param path path to the directory where the Helpyfile will be placed
     */
    void Manager::init(std::string path) {
        formatPath(path);

        // check if the directory is usable
        if (fs::exists(path) && !fs::is_empty(path)) {
            std::cout << BOLD << YELLOW << "WARNING: " << RESET
                      << "The selected directory is already in use. Would you like to overwrite it?" << YES_NO << '\n';

            char answer;
            std::cin >> answer;

            if (answer != 'Y' && answer != 'y') return;

            // delete the content of the directory
            fs::remove_all(path);
        }

        // create the directory
        fs::create_directory(path);
        
        writeHelpyfileTemplate(path);
    }

    void Manager::run(std::string path, std::string filename) {
        formatPath(path);
        filename = path + filename;

        // read Helpyfile
        if (!std::filesystem::is_regular_file(filename)) {
            Utils::printError((std::string) "Could not find the file '" + UNDERLINE + filename + RESET + RED
                + "'! Please verify if the specified path is correct.");

            exit(1);
        }

        Lexer lexer(filename);
        Parser parser(lexer.execute());

        Writer writer(path, parser.execute());
        writer.execute();
    }
}
