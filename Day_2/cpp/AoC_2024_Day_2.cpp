/* AoC_2024_Day_2
 * Joey Sachtleben
 * Licenced under the MIT licence. See LICENSE.TXT for details.
 *
 */

#include <filesystem>
#include <string>
#include <sstream>
#include <exception>
#include <iostream>
#include <fstream>
#include <vector>

#include "AoC_2024_Day_2.h"

using std::cout;
using std::cerr;
using std::vector;

using ss = std::stringstream;
using Report = std::vector<int>;

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

bool IsPositive(int i)
{
	return abs(i) == i;
}

/* CheckReportSafety
 *	Determines if a report meets the requirements to be considered safe.
 *
 * Parameters:
 *	report - The report to be checked.
 *
 * Returns:
 *	A boolean representing if the report was safe.
 *
 * Throws:
 *	May throw exceptions from the Report.
 */
bool CheckReportSafety(const Report& report)
{

	int delta1 = report.at(1) - report.at(0);
	if (abs(delta1) > 3 || delta1 == 0)
	{
		return false;
	}

	for (size_t i = 2; i < report.size(); ++i)
	{
		int delta2 = report.at(i) - report.at(i-1);
		if (abs(delta2) > 3 || delta2 == 0)
		{
			return false;
		}

		if (IsPositive(delta1) != IsPositive(delta2))
		{
			return false;
		}

		delta1 = delta2;

	}

	return true;
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
		fs::path inputPath = ValidatePath(inputFilePath);
		std::ifstream input(inputPath);

		if (!input.is_open())
		{
			ss msg;
			msg << "Unable to open file " << inputFilePath << " .\n";
			throw std::runtime_error(msg.str());
		}

		// Read in all reports
		vector<Report> reports;
		int lineCounter = 0;
		while (input.good())
		{
			Report report;
			std::string rawLine;
			lineCounter++;
			std::getline(input, rawLine);
			if (input.bad())
			{
				ss msg;
				msg << "Error reading line " << lineCounter << " from input file.\n";
				throw std::runtime_error(msg.str());
			}
			

			ss line;
			line << rawLine;
			while (!line.eof())
			{
				int n = -1;
				line >> n;
				if (n == -1)
				{
					ss msg;
					msg << "Error reading next number from the line \"" << line.str() << "\" on line " << lineCounter << ".\n";
					throw std::runtime_error(msg.str());
				}
				report.push_back(n);
			}
			reports.emplace_back(report);
		}


		// Find all safe reports
		vector<bool> isReportSafe;
		vector<bool> isReportSafeWithDampener;
		for (const Report& report : reports)
		{
			bool isSafe = CheckReportSafety(report);
			isReportSafe.push_back(isSafe);

			// Do we need to try The Problem Dampener?
			if (!isSafe)
			{
				bool becameSafe = false;
				for (size_t i = 0; i < report.size(); ++i)
				{
					Report modReport = report;
					modReport.erase(modReport.begin() + i);

					// Is the report now safe?
					bool isNowSafe = CheckReportSafety(modReport);
					if (isNowSafe)
					{
						becameSafe = true;
						break;
					}
				}
				isReportSafeWithDampener.push_back(becameSafe);
			}
			else
			{
				isReportSafeWithDampener.push_back(isSafe);
			}
		}

		// Count how many reports were safe
		int totalSafe = 0;
		for (bool reportSafe : isReportSafe)
		{
			if (reportSafe == true)
			{
				totalSafe++;
			}
		}
		int totalSafeWithDampener = 0;
		for (bool reportSafe : isReportSafeWithDampener)
		{
			if (reportSafe == true)
			{
				totalSafeWithDampener++;
			}
		}

		cout << "There are " << totalSafe << " safe reports out of " << isReportSafe.size() << " reports.\n";
		cout << "With the problem dampener, there are a total of " << totalSafeWithDampener << " safe reports.\n";

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
