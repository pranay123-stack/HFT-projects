#include <iostream>
#include <fstream>

#include "OrderBook.h"

const auto INPUT_FILE = "input_example.txt";

int main()
{

	// read file
	std::ifstream inf(INPUT_FILE);

	if (!inf)
	{
		std::cout << "Failed to open " << INPUT_FILE << " - exiting." << std::endl;
		return 1;
	}

	std::cout << "Opened " << INPUT_FILE << " successfully." << std::endl;

	OrderBook book;

	// Loop through trade input file
	// Format:
	// DIRECTION SYMBOL LOTS PRICE
	// Example:
	// BUY AAPL 10 14801
	// (Buy 10 AAPL shares for 14801)

	std::string direction;
	Order order;

	while (inf >> direction >> order.symbol >> order.lots >> order.price)
	{
		std::vector<Trade> matches;
		if (order.lots <= 0)
		{
			std::cout << "lots <= 0 detected - exiting.";
			return 1;
		}
		if (direction == "BUY")
		{
			// check sell part of orderbook for matches.
			matches = book.add_buy(order); // returns collection of matches
		}
		else if (direction == "SELL")
		{
			// check buy part of orderbook for matches.
			matches = book.add_sell(order);
		}
		else
		{
			std::cout << "Unrecognised direction: " << direction << " - exiting." << std::endl;
			return 1;
		}

		// Open the output file stream
		std::ofstream outputFile("output.txt");

		// Check if the file is opened successfully
		if (!outputFile.is_open())
		{
			std::cerr << "Error opening output.txt" << std::endl;
			return 1;
		}


		
		// Loop through matches and write to the output file
		for (const auto &match : matches)
		{
			outputFile << "TRADED " << match.lots << " " << match.symbol << " AT " << match.price << std::endl;
		}

		// Close the output file
		outputFile.close();
	}

	return 0;
}