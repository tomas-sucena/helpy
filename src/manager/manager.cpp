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

namespace Helpy {
    /**
     * @brief Reads a line of user input.
     *
     * Reads a line of user input. This function was copied from Helpy.
     * @param instruction the instruction that will be displayed before prompting the user to input
     * @return read input
     */
    std::string Manager::readInput(const std::string &instruction) {
        // display the instruction
        std::cout << BREAK;
        std::cout << instruction << '\n' << std::endl;

        // read the user input
        std::string input; getline(std::cin >> std::ws, input);

        return input;
    }

    void Manager::formatDirname(std::string &path) {
        if (!path.empty() && path.back() != '/')
            path.push_back('/');
    }

    bool Manager::createDirectory(std::string &path) {
        formatDirname(path);

        // check if the directory is usable
        if (std::filesystem::exists(path) && !std::filesystem::is_empty(path)) {
            std::ostringstream instr;
            instr << BOLD << YELLOW << "WARNING: " << RESET
                  << "The selected directory is already in use. Would you still like to continue?" << YES_NO;

            // verify if the user consented to using the directory
            char answer = Manager::readInput(instr.str()).front();
            return answer == 'Y' || answer == 'y';
        }

        // create the directory
        std::filesystem::create_directory(path);
        return true;
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
     * @brief Writes a template for a Helpyfile, which is the file that contains the Helpy specification
     * @param outputDir path to the directory where the Helpyfile will be placed
     */
    void Manager::init(std::string outputDir) {
        if (createDirectory(outputDir))
            writeHelpyfileTemplate(outputDir);
    }

    /**
     * @brief Creates a new Helpy instance according to a Helpyfile.
     * @param path path to either the Helpyfile or the directory where it is stored
     * @param outputDir path where the files pertaining to Helpy will be output
     */
    void Manager::run(std::string path, std::string outputDir) {
        // verify if the user input the path to a directory
        if (std::filesystem::is_directory(path)) {
            formatDirname(path);
            path += "Helpyfile";
        }

        // verify if the Helpyfile exists
        if (!std::filesystem::is_regular_file(path)) {
            Utils::printError((std::string) "Could not find the file '" + BOLD + ITALICS + path + RESET + "'! "
                + "Please verify if the specified path is correct.");
            exit(EXIT_FAILURE);
        }

        // lex the Helpyfile
        Program program;
        std::vector<Token> tokens = Lexer(path, program).execute();

        if (program.error) exit(EXIT_FAILURE);

        if (program.warnings) {
            std::cout << '\n'
                      << RESET << "Lexing terminated with " << BOLD << YELLOW << program.warnings << RESET
                      << " warning";

            if (program.warnings > 1) std::cout << 's';
            std::cout << ". Would you still like to continue?" << YES_NO << '\n';

            char answer;
            std::cin >> answer;

            if (answer != 'Y' && answer != 'y') return;
        }

        // parse the Helpyfile
        program.warnings = 0;
        ParserInfo info = Parser(tokens, program).execute();

        if (program.error) exit(EXIT_FAILURE);

        if (program.warnings) {
            std::cout << '\n'
                      << RESET << "Parsing terminated with " << BOLD << YELLOW << program.warnings << RESET
                      << " warning";

            if (program.warnings > 1) std::cout << 's';
            std::cout << ". Would you still like to continue?" << YES_NO << '\n';

            char answer;
            std::cin >> answer;

            if (answer != 'Y' && answer != 'y') return;
        }

        // write the Helpy instance
        if (createDirectory(outputDir))
            Writer(outputDir, info).execute();
    }
}
