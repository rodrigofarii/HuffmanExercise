#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <list>

// Min - Heap Implementation
// Min-Heap is a complete binary tree in which the value in each internal node is smaller
// than or equal to the values in the children of that node.

struct MinHeapNode {
	char data; // Character stored
	unsigned freq; // Frequency of the character
	MinHeapNode *left; //Left Child node
	MinHeapNode *right; //Right Child node

	//Constructor
	MinHeapNode(char data, unsigned freq) {
		left = right = NULL; // Initialize empty Min-Heap
		this->data = data;
		this->freq = freq;
	}
};

struct compare { // returns true if the frequency of the Left child is greater, else returns false
	bool operator()(MinHeapNode *left, MinHeapNode *right) {
		return (left->freq > right->freq);
	}
};

//The printCodesFromHuffman() function will print the code-words using the HuffmanTree
//Pre: MinHeapNode containing the root of the HuffmanTree.
//Post: Output message of the code words in the screen.
void printCodesFromHuffman( // Printing the code-words using the HuffmanTree, receives a HuffmanTree and prints each character and its codes
	struct MinHeapNode *root, std::string str, std::list<char> &characters, std::list<std::string> &codes) {
	if (!root) {
		return;
	}
	if (root->data != '$') {
		//std::cout << root->data << ": " << str << "\n";
		characters.push_back(root->data);
		codes.push_back(str);
	}
	printCodesFromHuffman(root->left, str + "0", characters, codes); // Check for left child and prints 0.
	printCodesFromHuffman(root->right, str + "1", characters, codes); // Check for right child and prints 1.
}

//The saveHuffmanTreeToFile function() will write and save a file containing the HuffmanTree.
//Pre: empty file as input and valid HuffmanTree.
//Post: File as output containing the HuffmanTree to encoded message.
void saveHuffmanTreeToFile(struct MinHeapNode *root, std::ofstream *myTreeFile) { // Writes a file with the Huffman Tree
	//First depth - PostOrder recurse
	if (root == NULL) {
		return;
	}
	saveHuffmanTreeToFile(root->left, myTreeFile); // First recurse on the left subTree
	saveHuffmanTreeToFile(root->right, myTreeFile); // Then recurse on the right subTree
	*myTreeFile << root->data << std::endl << root->freq << std::endl;
}

//The HuffmanDecodeFromTree() function will decode the message from the HuffmanTree file.
//Pre: file which contain the encodedFile and file which contain the HuffmanTree.
//Post: Message output with the decoded message in the screen.
void HuffmanDecodeFromTree(std::ifstream *myTreeFile, std::ifstream *encodedFile) { // Decode the message from the Huffman Tree File
	MinHeapNode *root = NULL, *leftChild, *rightChild, *temp = NULL;
	std::string dataLine;
	std::string freqLine;
	std::string encodedMessage;
	int freqInt;
	std::queue<MinHeapNode*> heap;
	std::stack<MinHeapNode*> treeStack;

	getline(*encodedFile, encodedMessage);
	std::string::iterator it = encodedMessage.begin();

	while (getline(*myTreeFile, dataLine)) {
		getline(*myTreeFile, freqLine);
		freqInt = stoi(freqLine);
		heap.push(new MinHeapNode(dataLine[0], freqInt));
	}

	while (!heap.empty()) {
		root = heap.front();
		if (root->data != '$') {
			treeStack.push(root);
		}
		else if (root->data == '$') {
			rightChild = treeStack.top();
			treeStack.pop();
			leftChild = treeStack.top();
			treeStack.pop();

			root->right = rightChild;
			root->left = leftChild;

			treeStack.push(root);
		}
		heap.pop();
	}
	temp = root;
	for (std::string::iterator it = encodedMessage.begin(); it != encodedMessage.end(); it++) {
		if (*it == '0') {
			temp = temp->left;
			if (temp->data != '$') {
				std::cout << temp->data;
				temp = root;
			}
		}
		if (*it == '1') {
			temp = temp->right;
			if (temp->data != '$') {
				std::cout << temp->data;
				temp = root;
			}
		}
	}
}

//The HuffmanCoding() function will encode the input message.
//Pre: None.
//Post: Message encode properly using Huffman compression.
void HuffmanCoding(char data[], int freq[], int size, std::list<char> &characters, std::list<std::string> &codes, std::ofstream *myTreeFile) {
	struct MinHeapNode *left, *right, *top;
	std::priority_queue<MinHeapNode*, std::vector<MinHeapNode*>, compare> minHeap; // MinHeap containing all characters of data[]
	for (int i = 0; i < size; ++i) {
		minHeap.push(new MinHeapNode(data[i], freq[i])); // Pushes into the minHeap every Node with its data and frequency.
	}
	while (minHeap.size() != 1) { // Iterates through the minHeap
		left = minHeap.top();
		minHeap.pop();

		right = minHeap.top();
		minHeap.pop();

		top = new MinHeapNode('$', left->freq + right->freq);// Declares new internal nodes with frequency equals to the sum of the two nodes frequencies.
		top->left = left;
		top->right = right;

		minHeap.push(top);
	}

	printCodesFromHuffman(minHeap.top(), "", characters, codes);
	saveHuffmanTreeToFile(minHeap.top(), myTreeFile);
}
