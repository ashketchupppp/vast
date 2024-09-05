#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>

#include "framedifference.hpp"
#include "logging.hpp"

struct Options {
    bool validArguments;
    std::string inputFile;
    std::string outputFile;
};

Options parseArgs (int argc, char** argv) {
    // ignore first argument, that's the exe path
    argv++;

    Options opts;
    opts.validArguments = true;

    // Put argv into args vector
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++) {
        args.push_back(*argv);
        argv++;
    }

    if (args.size() < 2) {
        Log::error("Not enough arguments!");
        Log::error("Usage: Vast <input-file> <output-file>");
        opts.validArguments = false;
        return opts;
    }

    opts.inputFile = args[0];
    opts.outputFile = args[1];

    return opts;
}

bool validateOptions(Options opts) {
    if (!std::filesystem::exists(opts.inputFile)) {
        Log::error(opts.inputFile + " does not exist");
        return false;
    }
    // Do this check when command line parsing is better and its easier to add a --force option
    // if (std::filesystem::exists(opts.outputFile)) {
    //     Log::error(opts.outputFile + " already exists");
    //     return false;
    // }
    return true;
}

int main(int argc, char** argv)
{
    Options opts = parseArgs(argc, argv);

    // What are you asking for?
    if (!opts.validArguments) {
        return 1;
    }

    // Is what you are asking for valid?
    if (!validateOptions(opts)) {
        return 1;
    }

    // Do frame difference

    return frameDifference(opts.inputFile, opts.outputFile);
}