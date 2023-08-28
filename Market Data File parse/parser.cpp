#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm> // For std::replace

int main()
{
    std::ifstream inputFile("log.txt");
    std::ofstream outputFile("output.txt");
    if (!inputFile.is_open() || !outputFile.is_open())
    {
        std::cerr << "Error opening files!" << std::endl;
        return 1;
    }

    std::string line;

    while (std::getline(inputFile, line))
    {
        std::istringstream iss(line);
        std::vector<std::string> columns;
        std::string column;
        std::string modifiedLine;

        int colIndex = 0;

        while (iss >> column)
        {
            // Replace unwanted characters with spaces
           
            std::replace(column.begin(), column.end(), '-', ' ');
          

            if (colIndex != 0 && colIndex != 2 && colIndex != 3 && colIndex != 4 && colIndex != 5)
            {
                modifiedLine += column + " ";
            }

            colIndex++;
        }

        // Remove trailing space and write the modified line to the output file
        if (!modifiedLine.empty())
        {
            modifiedLine.pop_back(); // Remove trailing space
            outputFile << modifiedLine << "\n";
        }
    }

    inputFile.close();
    outputFile.close();

    return 0;
}
