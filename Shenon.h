#pragma once
#include "Header.h"
#include "List.h"
#include <string>
#include <iostream>

using namespace std;

class ShenonPhano
{
	struct Node {
		int freq = 1;
		char symbol;
		string code = "";
		friend bool operator == (const Node& first, const Node& second)
		{
			return first.symbol == second.symbol;
		}
		Node(char symbol2) { symbol = symbol2; }
		Node() {}
	};

	List<Node> sourceList;
	Tree<string, char> sourceTree;
	Tree<char, string> reverseSourceTree;
	void getFreq(string text); // get frequency and sorting
	void getCode(size_t startIndex, size_t endIndex);
	void addCode(char code, size_t startIndex, size_t endIndex);
	void getTree(string text);
	size_t getSum(size_t startIndex, size_t endIndex);
public:
	string encode(string text, bool dev = false); // encode
	string decode(string text, Tree<char, string>&); // decode
	void test(string text);
};

size_t ShenonPhano::getSum(size_t startIndex, size_t endIndex)
{
	if (endIndex < 0)
		return 0;
	else 
	{
		size_t sum = 0;
		if (startIndex == endIndex)
			sum = sourceList.at(startIndex).freq;
		for (size_t i = startIndex; i <= endIndex; i++)
		{
			sum += sourceList.at(i).freq;
		}
		return sum;
	}
}

void ShenonPhano::addCode(char code, size_t startIndex, size_t endIndex)
{
	for (size_t i = startIndex; i <= endIndex; i++)
	{
		sourceList.changeCode(code, i);
	}
}

void ShenonPhano::getCode(size_t startIndex, size_t endIndex)
{
	if (startIndex != endIndex){
			size_t newStartIndex = startIndex;
			size_t averageVal = getSum(startIndex, endIndex) / 2;
			size_t sum = sourceList.at(startIndex).freq;
			while (averageVal > sum)
			{
				newStartIndex++;
				sum = getSum(startIndex, newStartIndex);
			}
			addCode('0', startIndex, newStartIndex);
			getCode(startIndex, newStartIndex);
			newStartIndex++;
			addCode('1', newStartIndex, endIndex);
			getCode(newStartIndex, endIndex);
	}
	else
	{

	}

}

void ShenonPhano::test(string text)
{
	string encodeStr = encode(text);
	string decodeStr = decode(encodeStr, reverseSourceTree);
}

void ShenonPhano::getFreq(string text)
{
	for (size_t i = 0; i < text.length(); i++)
		sourceList.addSymbol(Node(text[i]));
	sourceList.sorting();
}

void ShenonPhano::getTree(string text)
{
	getFreq(text);
	getCode(0, (sourceList.get_size() - 1));
	for (size_t i = 0; i < sourceList.get_size(); i++)
	{
		sourceTree.insert(sourceList.at(i).code, sourceList.at(i).symbol);
		reverseSourceTree.insert(sourceList.at(i).symbol, sourceList.at(i).code);
	}
}

string ShenonPhano::encode(string text, bool dev)
{
	getTree(text);
	string response;
	for (size_t i = 0; i < text.length(); i++)
		response += sourceTree.find(text[i])->data;
	if (dev)
	{
		for (size_t i = 0; i < sourceList.get_size(); i++)
			cout << sourceList.at(i).symbol << ":::" << sourceList.at(i).freq << ":::" << sourceList.at(i).code << endl;
		cout << endl;
		cout << "k = " << (float)response.length() / (float)(text.length() * 8) << endl << endl;
	}
	return response;
}

string ShenonPhano::decode(string text, Tree<char, string>& tree)
{
	string checkStr, response;
	for (size_t i = 0; i < text.length(); i++)
	{
		checkStr.push_back(text[i]);
		if (tree.find(checkStr) != nullptr)
		{
			response.push_back(tree.find(checkStr)->data);
			checkStr.erase();
		}
	}
	return response;
}


