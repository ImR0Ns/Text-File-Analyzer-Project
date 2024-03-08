#include <iostream>
#include <fstream> //for handling files
#include <map> // for dictionary
#include <cctype> // character transformations and punctuations
#include <string>

class TextAnalyzer {
private:

	//variables
	std::fstream file;
	std::string fileName;
	std::map<std::string, int> wordCount; //for word counter
	std::map<char, int> charCount; // for char counter

	std::string finalString = "", longestWord = "", shortestWord = ""; //finalString refer to the entire text in the file!
	int lineCounter = 0;//how many lines are

	int totalCharacters = 0; // here we include whitespaces too because I want to use it for Average Word Length.
	int totalNumberOfWords = 0; // same as totalCharacters

	std::string mostCommonWord = "";

	/*
		Check if the current word is longer/shorter in size for longestWord/shortestWord

		\param toCompare -> A string input

	*/
	void checkForLwSw(std::string toCompare) { //check for longest and shortest word to avoid repeating code
		toCompare.size() > longestWord.size() ? longestWord = toCompare : longestWord;
		toCompare.size() < shortestWord.size() ? shortestWord = toCompare : shortestWord == "" ? shortestWord = toCompare : shortestWord;
	}

	/*
		Look for the word that appeared the most times
	*/
	void getMostCommonWord() {
		int countMax = 0;
		for (auto str : wordCount) {
			if (str.second >= countMax) {
				countMax = str.second;
				mostCommonWord = str.first;
			}
		}
	}

public:
	TextAnalyzer(std::string fileName) : fileName(fileName) {
		file.open(fileName, std::ios::in); //file opened in reading mode
		if (!file) {
			std::cout << "Error while opening the file";
		}
		else {
			char x;
			std::string completeString = ""; // for word counter

			while (!file.eof()) {
				// I choose to use tolower() only on append because I want to keep te original finalString intact
				x = file.get();

				if (file.eof()) { break; } //these to remove the eof from map
				charCount[tolower(x)]++; // add to character counter
				finalString += x; // add to final string
				totalCharacters++;

				if (std::ispunct(x)) {
					continue;
				}

				if (x == ' ' || x == '\n') {
					checkForLwSw(completeString);
					wordCount[completeString]++; // add to word counter
					totalNumberOfWords++; // count words
					completeString = ""; //reset temp string
				}
				else {
					completeString += tolower(x); // temp string				
				}
			}

			lineCounter = charCount['\n'] + 1; // because we already count how many \n (new line) we can use it from map
			charCount.erase('\n'); //after getting the lines remove from map

			totalNumberOfWords++;

			//here was a bug with empty string at final and overwriting the shortest
			if (!completeString.empty()) {
				wordCount[completeString]++; //adding last str
				checkForLwSw(completeString); // check again if is shorter or longer than what we have				
			}

			getMostCommonWord();
			finalString += '\n';
		}
	}

	/*
		Check the dictionary (for debugging more)
	*/
	void seeCharCount() const {
		for (auto c : this->charCount) {

			if (c.first == ' ') {
				std::cout << "Char ' '" << ": " << c.second << "\n";
			}
			else {
				std::cout << "Char " << c.first << ": " << c.second << "\n";
			}
		}
		std::cout << "\n";
	}

	/*
		Check the dictionary (for debugging more)
	*/
	void seeWordCount() const {
		for (auto s : this->wordCount) {
			std::cout << "String " << s.first << ": " << s.second << "\n";
		}
		std::cout << "\n";
	}

	/*
		Return longest string
	*/
	std::string getLongestStr() const {
		return this->longestWord;
	}

	/*
		Return shortest string
	*/
	std::string getShortestStr() const {
		return this->shortestWord;
	}


	/*
		Returns all the text from the read file
	*/
	std::string getFinalText() const {
		return this->finalString;
	}

	/*
		Returns the lines from the file
	*/
	int getLineCounter() const {
		return this->lineCounter;
	}

	/*
		Returns total characters + special characters and spaces ' '
	*/
	int getTotalCharacters() const {
		return this->totalCharacters;
	}

	/*
		Returns total words
	*/
	int getTotalWords() const {
		return this->totalNumberOfWords;
	}

	/*
		Returns Average Word Length
	*/
	int avgWrdLen() const {
		return this->totalCharacters / totalNumberOfWords;
	}

	/*
		Look for the word that appeared the most times
	*/
	std::string getMostCommonWrd() const {
		return this->mostCommonWord;
	}

	/*
		Return the entire data
	*/
	std::string getFullData() const {
		return this->finalString;
	}

	void createFile() {

		// Create the HTML content string
		std::string htmlContent =
			"<!DOCTYPE html>\n"
			"<html lang=\"en\">\n"
			"<head>\n"
			"    <meta charset=\"UTF-8\">\n"
			"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
			"    <title>Text Analyzer</title>\n"
			"    <style>\n"
			"        body {\n"
			"            display: flex;\n"
			"            flex-direction: row;\n"
			"            justify-content: space-between;\n"
			"        }\n"
			"    </style>\n"
			"</head>\n"
			"<body>\n"
			"<div>\n"
			"    <h1>Text File Analyzer</h1>\n"
			"    <p>Word count: " + std::to_string(getTotalWords()) + "</p>\n"
			"    <p>Character count: " + std::to_string(getTotalCharacters()) + "</p>\n"
			"    <p>Line count: " + std::to_string(getLineCounter()) + "</p>\n"
			"    <p>Longest Word: " + getLongestStr() + "</p>\n"
			"    <p>Shortest Word: " + getShortestStr() + "</p>\n"
			"    <p>Average Word Length: " + std::to_string(avgWrdLen()) + "</p>\n"
			"    <p>Most Common Word: " + getMostCommonWrd() + "</p>\n"
			"</div>\n"
			"<div>\n"
			"    <h1>Word Frequency:</h1>\n"
			"    <ul>\n";

		// Iterate over the wordCount map and add its contents to the HTML content string
		for (const auto& entry : this->wordCount) {
			htmlContent += "        <li>" + entry.first + ": " + std::to_string(entry.second) + " appears</li>\n";
		}

		htmlContent +=
			"</ul>\n"
			"</div>\n"
			"<div>\n";

		htmlContent +=
			"<h1>Character Frequency:</h1>\n"
			"<ul>\n";

		for (const auto& entry2 : this->charCount) {
			htmlContent += "        <li>" + std::string(1, entry2.first) + " : " + std::to_string(entry2.second) + " appears</li>\n";
		}


		htmlContent +=
			"    </ul>\n"
			"</div>\n"
			"</body>\n"
			"</html>\n";

		std::fstream outputFile("analyzer.html", std::ios::out);

		if (!outputFile.is_open()) {
			std::cout << "Error: Unable to open/create file." << std::endl;
		}

		// Write the HTML content to the file
		outputFile << htmlContent;

		// Close the file
		outputFile.close();

	}

	~TextAnalyzer() {
		file.close(); //close file
		createFile();
	}
};

int main() {

	bool run = true;

	while (run) {
		std::string fName;
		std::cout << "Insert file name | ex: text.txt\n";
		std::cout << "File name: ";
		std::cin >> fName;
		TextAnalyzer text(fName);
		break;
	}
	std::cout << "The analysis was done successfully!";

	//TextAnalyzer text("Text.txt");

	//text.createFile();

	//input tests
	//std::cout<<text.getFullData();
	//text.getLongestShortest();
	//text.seeWordCount();
	//std::cout<<text.getFinalText();
	//text.seeCharCount();
	//std::cout<<text.getLineCounter();
	//std::cout<< text.getMostCommonWrd();
}