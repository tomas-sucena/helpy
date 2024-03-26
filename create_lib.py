# The purpose of this script is to join all the header and source code files into a single header
# and source code files, respectively.
#
# This script was heavily inspired by Libfort's amalgamation script, which can be found here:
# https://github.com/seleznevae/libfort/blob/develop/amalgamate.py


import os
import re


def copy_file(filename: str, output_file):
    with open(filename, "r") as file:
        output_file.write("\n/********************************************************\n")
        output_file.write(f"   {filename}\n")
        output_file.write(" ********************************************************/\n\n")

        # copy the contents from the file
        for line in file.readlines():
            # ignore relative includes
            if not re.search('#include ".+"', line.strip()):
                output_file.write(line)


def create_lib(config):
    # create the directory, if it doesn't exist
    os.makedirs(config["lib_dir"], exist_ok=True)

    # create the header file
    with open(f"{config["lib_dir"]}/{config["lib_name"]}.h", "w") as output_file:
        for filename in config["header_files"]:
            copy_file(filename, output_file)

    # create the source file
    with open(f"{config["lib_dir"]}/{config["lib_name"]}.cpp", "w") as output_file:
        # include the header file
        output_file.write(f"#include \"{config["lib_name"] + ".h"}\"\n")

        for filename in config["source_files"]:
            copy_file(filename, output_file)


def main():
    # list the program modules
    modules = [
        "utils",
        "lexer",
        "manager",
        "parser",
        "writer",
    ]

    # create the configuration
    config = {
        "lib_dir": "lib",
        "lib_name": "helpy",
    }

    header_files = []
    source_files = []

    for module in modules:
        dirname = f"src/{module}"

        for filename in os.listdir(dirname):
            # header files
            if ".h" in filename or ".hpp" in filename:
                header_files.append(f"{dirname}/{filename}")
            # source files
            elif ".c" in filename:
                source_files.append(f"{dirname}/{filename}")

    config["header_files"] = header_files
    config["source_files"] = source_files

    create_lib(config)


if __name__ == "__main__":
    main()
