#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

struct Line {
    std::string time;
    std::string content;
};

bool compareLines(const Line& a, const Line& b) {
    return a.time < b.time;
}

bool containsKey(const std::string& line, const std::string& key) {
    size_t found = line.find(key);
    if (found != std::string::npos) {
        // Check if key is not part of a larger number (e.g., "841=")
        if (found == 0 || line[found - 1] == ' ') {
            return true;
        }
    }
    return false;
}

int main() {
    std::ifstream inputFile("output.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open input.txt" << std::endl;
        return 1;
    }

    std::vector<Line> lines;

    std::string line;
    while (std::getline(inputFile, line)) {
        Line parsedLine;
        parsedLine.time = line.substr(0, 8); // Assuming the time is always at the beginning
        parsedLine.content = line;
        lines.push_back(parsedLine);
    }

    inputFile.close();

    std::sort(lines.begin(), lines.end(), compareLines);

    std::ofstream outputFile("final.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Unable to open output.txt" << std::endl;
        return 1;
    }

    const std::vector<std::string> keysToFind = {"41=", "49=", "84=", "140","295="};
  //49 , 84 , 140 , 295 , 1537
    for (const Line& line : lines) {
        for (const std::string& key : keysToFind) {
            if (containsKey(line.content, key)) {
                outputFile << line.content << std::endl;
                break; // No need to check other keys for this line
            }
        }
    }

    outputFile.close();

    std::cout << "Filtered lines have been written to output.txt" << std::endl;

    return 0;
}
