#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "BinarySearchTree.h"

//The characterFrequency() function will calculate and return the frequency of a character in the given string.
//Pre: none.
//Post: Frequency of a character is returned.
int characterFrequency(std::string str, char a) {
	std::string::iterator it;
	int freq = 0;
	for (it = str.begin(); it != str.end(); it++) {
		if (a == *it) {
			freq++;
		}
	}
	return freq;
}

// The printCharactersAndFrequencies() function will print all characters present in the list and the frequencies.
//Pre: chars list must match its frequency.
//Post: Output each character with each frequency on the screen.
void printCharactersAndFrequencies(std::list<char> &chars, std::list<int> &frequencies) {
	std::list<int>::iterator itFreq = frequencies.begin();

	std::cout << "Characters:           Frequency: " << std::endl;
	for (std::list<char>::iterator it = chars.begin(); it != chars.end(); it++) {
		std::cout << "     " << *it << "                " << *itFreq << std::endl;
		itFreq++;
	}
}

template<typename T>
T * listToArray(std::list<T> &list) {
	T *a = NULL;
	a = new T[list.size()]; // Array to return
	typename std::list<T>::iterator it = list.begin();
	for (unsigned i = 0; i < list.size(); i++) {
		a[i] = *it;
		it++;
	}
	return a;
}

//The printDataInfo() will print the total number of characters of the string and how much data long is the data.
//Pre: None.
//Post: Message output of data long of the data in the screen.
void printDataInfo(std::string input) {
	int numOfChars = input.length();
	for (unsigned int i = 0; i < input.length(); i++) {
		if (input.at(i) == ' ') {
			numOfChars--;
		}
	}
	std::cout << "Number of characters: " << numOfChars << std::endl;
	std::cout << "Your data is: " << numOfChars * 7 << " bits long." << std::endl;
}

// The main function will compress with Huffman Coding the input message.
//Pre: None.
//Post: Message output of the message encoded to the screen and file containing the HuffmanTree.
int main() {
	// Variables declaration
	std::string input;
	std::ifstream myFile;
	std::ofstream encodedFile;
	std::ifstream encodedFileToOpen;
	std::ofstream myTreeFile;
	std::list<char> chars; // List of each character present in the input string.
	std::list<char>::iterator flag;
	std::list<int> frequencies;
	std::list <char> characters;
	std::list<std::string> codes;
	char *dataArray = NULL;
	int *frequencyArray = NULL;
	int freq = 0;
	int size = 0;
	MinHeapNode *codesTree = NULL;

	//Input
	std::cout << "Type the message you want to encode: " << std::endl;
	getline(std::cin, input);

	myTreeFile.open("HuffmanCodes.txt");
	encodedFile.open("EncodedFile.txt");

	std::transform(input.begin(), input.end(), input.begin(), ::tolower); // Makes the users input array to lowercase

	for (std::string::iterator it = input.begin(); it != input.end(); it++) { // Loop through the input and look out each character
		if (it == input.begin()) chars.push_back(*it);
		flag = std::find(chars.begin(), chars.end(), *it); // Flag whether the char of the input its already in the chars list.
		if (flag != chars.end()) {
			// Char of the input its already in the list of characters chars.
		}
		else {
			if (*it != ' ') { // Avoid push in the chars list the space character.
				chars.push_back(*it);
			}
		}
	}

	for (std::list<char>::iterator it = chars.begin(); it != chars.end(); it++) { // Checks the character frequency of each character in the string and puts it on the frequencies list.
		freq = characterFrequency(input, *it);
		frequencies.push_back(freq);
		//std::cout << *it;
	}

	std::cout << std::endl;

	//Data Information print.
	printCharactersAndFrequencies(chars, frequencies);
	printDataInfo(input);

	//List to Array conversion, Huffman Coding function is implemented with arrays as input parameters.
	dataArray = listToArray(chars);
	frequencyArray = listToArray(frequencies);

	//Sorts array in ascending order.
	for (unsigned i = 0; i < frequencies.size() - 1; i++) {
		for (unsigned j = i + 1; j < frequencies.size(); j++) {
			int tempFreq;
			char tempData;
			if (frequencyArray[i] > frequencyArray[j]) {
				tempFreq = frequencyArray[i];
				tempData = dataArray[i];
				frequencyArray[i] = frequencyArray[j];
				dataArray[i] = dataArray[j];
				frequencyArray[j] = tempFreq;
				dataArray[j] = tempData;
			}
		}
	}

	std::cout << std::endl;

	//Huffman Codes acquisition.
	size = frequencies.size();

	HuffmanCoding(dataArray, frequencyArray, size, characters, codes, &myTreeFile);

	myTreeFile.close();
	myFile.open("HuffmanCodes.txt"); //File that contains HuffmanTree

	std::cout << "Characters:             Code: " << std::endl;
	std::list<std::string>::iterator iter = codes.begin();
	for (std::list<char>::iterator it = characters.begin(); it != characters.end(); it++) {
		std::cout << "     " << *it << "                    " << *iter << std::endl;
		iter++;
	}

	// Encoded message printing section
	std::cout << "Your encoded message is: ";
	for (std::string::iterator k = input.begin(); k != input.end(); k++) {//Looping through the input and output corresponding code for each byte.
		std::list<std::string>::iterator j = codes.begin();
		for (std::list<char>::iterator it = characters.begin(); it != characters.end(); it++) {
			if (*k == *it) {
				std::cout << *j;
				encodedFile << *j;
				break;
			}
			j++;
		}
	}

	encodedFile.close();
	std::cout << std::endl;

	// Decoded message printing section
	encodedFileToOpen.open("EncodedFile.txt"); // File that contains the message already encoded.

	std::cout << "Your decoded message is: ";
	HuffmanDecodeFromTree(&myFile, &encodedFileToOpen); // Decoded message using the file which contain Huffman codes.

	std::cout << std::endl;

	myFile.close();
	encodedFileToOpen.close();

	system("pause");
	return 0;

}