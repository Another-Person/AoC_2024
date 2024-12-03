/* Advent of Code 2024 -- Day N
 * Joey Sachtleben
 * Licensed under the MIT license. See LICENSE for details.
 *
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using std::cout;
using std::cerr;
using std::vector;

using ss = std::stringstream;

namespace fs = std::filesystem;

/* ValidatePath
 *   Creates a path to a specified file and validates that it is a reachable regular file.
 *
 * Parameters:
 *   pathName - A string containing the path to the desired file.
 *
 * Returns:
 *   A path to the file.
 *
 * Throws:
 *   invalid_argument if the file does not exist or is not a regular file.
 */
fs::path ValidatePath(const std::string_view pathName)
{
	fs::path path = pathName;
	path.make_preferred();

	if (!fs::exists(path))
	{
		ss msg;
		msg << "File \"" << path.string() << "\" does not exist.\n";
		throw std::invalid_argument(msg.str());
	}

	if (!fs::is_regular_file(path))
	{
		ss msg;
		msg << "File \"" << path.string() << "\" is not a valid regular file.\n";
		throw std::invalid_argument(msg.str());
	}

	return path;
}

int main(int argc, char* argv[])
{
    if (argc > 2)
	{
		cerr << "Too many arguments.\n"
			 << "Expected usage: " << argv[0] << " [INPUT FILE]\n";
		return 1;
	}

	int retval = 0;

	std::string inputFilePath = "../input.txt";
	if (argc == 2)
	{
		inputFilePath = std::string(argv[1]);
	}
	cout << "Using file path " << inputFilePath << " ...\n";

    try
    {
        fs::path filePath = ValidatePath(inputFilePath);

		std::ifstream input(filePath);

        // Start here!
    }
    catch (const std::exception& e)
    {
        cerr << "Caught exception: " << e.what() << '\n';
        retval = 1;
    }
    catch (...)
    {
        cerr << "Caught unknown exception!\n";
        retval = 1;
    }

    return retval;
}
