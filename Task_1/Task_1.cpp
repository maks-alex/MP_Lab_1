#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

int main()
{
	int wordsToDisplay;
	string fileName;

	int arrayCapacity = 5;
	string* arrayWords = new string[arrayCapacity];
	int* arrayQuantity = new int[arrayCapacity];
	int wordsQuantity = 0;
	string wordProcessing;

	int index = 0, i = 0, j = 0;
	string stopWords[] = { "a", "an", "and", "at", "for", "in", "of","the", "to" };
	int stopWordsLength = 9;

	cout << "How many words to display: ";
	cin >> wordsToDisplay;

enterFileName:
	//cout << "Enter file name: "; cin >> wordQuantity;
	//hardcorded value for lab purpose
	fileName = "input.txt";

	fstream fsin;
startOpenFile:
	fsin.open(fileName);

// 3 attempts to open the file
	if (!(fsin.is_open()) && i < 3)
	{
		i++;
		goto startOpenFile;
	}
	else if (i == 3)
	{
		cout << "File name is incorrect.";
		goto enterFileName;
	}
	else
	{
loopTextProcessing:
		if (!fsin.eof())
		{
			fsin >> wordProcessing;
// check array size and double size if it is low
			if (wordsQuantity == arrayCapacity)
			{
				arrayCapacity *= 2;
				string* newArrayWords = new string[arrayCapacity];
				int* newArrayQuantity = new int[arrayCapacity];

				i = 0;
			copyArrays:
				if (i < arrayCapacity / 2)
				{
					newArrayWords[i] = arrayWords[i];
					newArrayQuantity[i] = arrayQuantity[i];
					i++;
					goto copyArrays;
				}

				delete[] arrayWords;
				arrayWords = newArrayWords;
				delete[] arrayQuantity;
				arrayQuantity = newArrayQuantity;
			} 
// normalize the word for low-case
			i = 0;
			if (wordProcessing[i] != '\0')
			{
				char* wordNormalized = new char[wordProcessing.length() + 1];
			loopNormalizeWord:
				if (i < wordProcessing.length())
				{
					if (wordProcessing[i] >= 65 && wordProcessing[i] <= 90)
					{
						wordNormalized[i] = wordProcessing[i] + 32;
						i++;
						goto loopNormalizeWord;
					}
					else
					{
						wordNormalized[i] = wordProcessing[i];
						i++;
						goto loopNormalizeWord;
					}
				}
				else
				{
					wordNormalized[i] = '\0';
					string normalizedString(wordNormalized);
					wordProcessing = normalizedString;
				}
			}
//check stop-word list		
			i = 0;
		checkStopWord:
			
			if (i < stopWordsLength)
			{
				string curStopWord = stopWords[i];
				if (wordProcessing == stopWords[i])
				{
					goto loopTextProcessing;
				}
				else
				{
					i++;
					goto checkStopWord;
				}
			}

//add word to array
			i = 0;
			bool notFound = true;

			if (wordsQuantity == 0)
			{
				arrayWords[i] = wordProcessing;
				arrayQuantity[i] = 1;
				wordsQuantity++;
				notFound = false;
			}
			else
			{
loopArrayWordCheckMultipleWords:
				if (i < wordsQuantity && wordsQuantity != 0)
				{
					if (wordProcessing == arrayWords[i])
					{
						arrayQuantity[i] += 1;
						notFound = false;
						goto exitloopArrayWordCheckMultipleWords;
					}
					else
					{
						i++;
						goto loopArrayWordCheckMultipleWords;
					}
				}
			}
		exitloopArrayWordCheckMultipleWords:
//add new word in arrayWords
 			if (notFound)
			{
				arrayWords[i] = wordProcessing;
				arrayQuantity[i] = 1;

				wordsQuantity++;
			} 


			
			goto loopTextProcessing;
		}
		
		fsin.close();

//sort array
			i = j = 0;
outerSortingLoop:
		if (i < wordsQuantity - 1)
		{
			j = 0;
		innerSortingLoop:
			if (j < wordsQuantity - i - 1)
			{
				if (arrayQuantity[j] < arrayQuantity[j + 1])
				{
					string temp = arrayWords[j];
					arrayWords[j] = arrayWords[j + 1];
					arrayWords[j + 1] = temp;
					int tmp = arrayQuantity[j];
					arrayQuantity[j] = arrayQuantity[j + 1];
					arrayQuantity[j + 1] = tmp;
				}
				j++;
				goto innerSortingLoop;
			}
			i++;
			goto outerSortingLoop;
		}

//printout results in file	
		fstream fsout;
		fsout.open("output.txt");
		if (fsout.is_open())
		{
			i = 0;
			if (wordsToDisplay < wordsQuantity)
			{
				wordsQuantity = wordsToDisplay;
			}
loopPrintOut:
			if (i < wordsQuantity)
			{
				
				fsout << arrayQuantity[i] << " - " << arrayWords[i] << '\n';;

				i++;
				goto loopPrintOut;
			}
		}
		fsout.close();
		delete[] arrayQuantity;
		delete[] arrayWords;
	}
}