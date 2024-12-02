/* AoC_2024_Day_1
 * Joey Sachtleben
 * Licenced under the MIT licence. See LICENSE.txt for details.
 *
 ***
 *
 */

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <numeric>

#include "AoC_2024_Day_1.h"


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

		if(!input.is_open())
		{
			ss msg;
			msg << "Unable to open " << inputFilePath << ".\n";
			throw std::runtime_error(msg.str());
		}

		vector<int32_t> list1;
		vector<int32_t> list2;

		// Read input into two lists
		int32_t lineCounter = 0;
		while (input.good())
		{
			int32_t n1 = -1;
			int32_t n2 = -1;
			input >> n1 >> n2;
			lineCounter++;

			if (n1 == -1 || n2 == -1)
			{
				ss msg;
				msg << "Error reading numbers from line " << lineCounter << ". Please check the input file.\n";
				throw std::runtime_error(msg.str());
			}

			list1.push_back(n1);
			list2.push_back(n2);
		}

		// Sort the two lists for pairing
		std::ranges::sort(list1);
		std::ranges::sort(list2);


		// Part 1 
		// Find the distance between each pair
		vector<int32_t> distances;
		for (size_t i = 0; i < list1.size(); ++i)
		{
			distances.push_back(abs(list1.at(i) - list2.at(i)));
		}

		// Sum the distances
		auto totalDistance = std::accumulate(distances.begin(), distances.end(), 0);


		cout << "The total distance is: " << totalDistance << "!\n";

		// Part 2
		// Find similarity scores
		vector<int32_t> oneInTwo;
		for (int32_t num : list1)
		{
			int total = 0;
			for (int32_t n2 : list2)
			{
				if (num == n2)
				{
					total++;
				}
			}
			oneInTwo.push_back(total);
		}
		// Multiply & sum score
		int32_t totalScore = 0;
		for (size_t i = 0; i < list1.size(); ++i)
		{
			totalScore += list1.at(i) * oneInTwo.at(i);
		}
		// Sum the scores

		cout << "The total similarity score is: " << totalScore << "!\n";

	}
	catch (std::exception& e)
	{
		cerr << "Caught exception: " << e.what() << "\n";
		retval = 1;
	}
	catch (...)
	{
		cerr << "Caught unknown exception!\n";
		retval = 1;
	}


	return retval;
}
