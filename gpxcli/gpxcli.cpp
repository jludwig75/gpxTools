#include <iostream>
#include <string>
#include <vector>

#include "gpx/parser.h"


int parseGpxFile(const std::string& gpxFileName)
{
    auto activity = gpx::parseFile(gpxFileName);
    return 0;
}

int usage(const std::string& message, int ec = 0)
{
    if (!message.empty())
    {
        std::cout << message << std::endl << std::endl;
    }

    std::cout << "GPX utility\nUsage:\n\tgpx <command> <args ...>\n";
    std::cout << "Comamnd can be:\n";
    std::cout << "\tparse <gpx_file_name>\n";
    return ec;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty())
    {
        return usage("You must specify a command", -1);
    }

    auto command = args[0];

    if (command == "parse")
    {
        if (args.size() < 2)
        {
            return usage("\"parse\" command expects a file name");
        }

        return parseGpxFile(args[1]);
    }
    else
    {
        return usage("Invalid command \"" + command + "\"");
    }
    

    return 0;
}
