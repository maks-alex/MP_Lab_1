#include <iostream>
#include <fstream>
#include <string>
using namespace std;


int main()
{
// variables
    string fileName = "inputShort.txt";
    int wordPerPage = 100;
    const int rowsPerPage = 45;

    int wordArrayLength = 50;
    string* arrayWords = new string[wordArrayLength];
    int currentRow = 1;
    int i = 0;
    int j = 0;
    int letter = 0;
    int wordsQuantity = 0;
    int** pages = new int* [wordArrayLength];
fillArrayRef:
    if (i < wordArrayLength) 
    {
        pages[i] = new int[102];
        i++;
        goto fillArrayRef;
    }

    const int stopWordsQuantity = 9;
    string stopWords[stopWordsQuantity] = { "a", "an", "and", "at", "for", "in", "of","the", "to" };

    bool isEqual;
    string wordProcessing;


// open/read file
    fstream fsin;
    fsin.open(fileName);
    if (fsin)
    {
    readFile:
        if (fsin.peek() == '\n') {
            currentRow++;
            fsin.get();
            goto readFile;
        }
        wordProcessing = "";
        if (fsin >> wordProcessing)
        {

// normalize the word for low-case

            i = 0;
            j = 0;
            if (wordProcessing[i] != '\0')
            {
                char* wordNormalized = new char[wordProcessing.length() + 1];
            normalizeWord:
                if (i < wordProcessing.length())
                {
                    if (wordProcessing[i] >= 65 && wordProcessing[i] <= 90 ||
                        wordProcessing[i] >= 97 && wordProcessing[i] <= 122 ||
                        wordProcessing[i] == '-' && i != 0)
                    {
                        if (wordProcessing[i] >= 65 && wordProcessing[i] <= 90)
                        {
                            wordNormalized[j] = wordProcessing[i] + 32;
                            i++;
                            j++;
                            goto normalizeWord;
                        }
                        else
                        {
                            wordNormalized[j] = wordProcessing[i];
                            i++;
                            j++;
                            goto normalizeWord;
                        }
                    }
                    else
                    {
                        i++;
                        goto normalizeWord;
                    }
                }
                else
                {
                    wordNormalized[j] = '\0';
                    string normalizedString(wordNormalized);
                    wordProcessing = normalizedString;
                }
            }
            if (wordProcessing == "") {
                goto readFile;
            }

//check stop-word list		
            i = 0;
        checkStopWord:
            if (i < stopWordsQuantity)
            {
                if (wordProcessing == stopWords[i])
                {
                    goto readFile;
                }
                else
                {
                    i++;
                    goto checkStopWord;
                }
            }

// check existing words
            i = 0;
        loopArray:
            if (i < wordsQuantity)
            {
                if (i < wordsQuantity)
                {
                    j = 0;
                    isEqual = true;
                checkCycle:
                    if (arrayWords[i][j] != '\0' && wordProcessing[j] != '\0') {
                        if (arrayWords[i][j] != wordProcessing[j]) {
                            isEqual = false;
                            goto continueCheck;
                        }
                        j++;
                        goto checkCycle;
                    }
                    if (arrayWords[i][j] != '\0' && wordProcessing[j] == '\0' ||
                        arrayWords[i][j] == '\0' && wordProcessing[j] != '\0') {
                        isEqual = false;
                    }
                continueCheck:
                    if (isEqual) {
                        if (pages[i][0] < wordPerPage + 1) {
                            j = ++pages[i][0];
                            pages[i][j] = currentRow / rowsPerPage + 1;
                        }
                        goto readFile;
                    }
                    i++;
                    goto loopArray;
                }
            }

// resize array
            if (wordsQuantity >= wordArrayLength)
            {
                wordArrayLength *= 2;
                string* new_arrayWords = new string[wordArrayLength];
                int** new_pages = new int* [wordArrayLength];

                i = 0;
            fillNewArray:
                if (i < wordsQuantity) {
                    new_arrayWords[i] = arrayWords[i];
                    new_pages[i] = pages[i];

                    i++;
                    goto fillNewArray;
                }
            fillFullArray:
                if (i < wordArrayLength) {
                    new_pages[i] = new int[wordPerPage + 2];
                    i++;
                    goto fillFullArray;
                }
                delete[]arrayWords;
                delete[]pages;
                arrayWords = new_arrayWords;
                pages = new_pages;
            }

// add new word in array
            arrayWords[wordsQuantity] = wordProcessing;
            pages[wordsQuantity][0] = 1;
            pages[wordsQuantity][1] = currentRow / rowsPerPage + 1;
            wordsQuantity++;
            goto readFile;
        }
        fsin.close();
    }
 
//sort array
    i = j = 0;
outerSortingLoop:
    if (i < wordsQuantity - 1)
    {
        j = 0;
    innerSortingLoop:
        if (j < wordsQuantity - i - 1)
        {
            if (arrayWords[j] > arrayWords[j + 1])
            {
                string temp = arrayWords[j];
                arrayWords[j] = arrayWords[j + 1];
                arrayWords[j + 1] = temp;
                int* bufferPages = pages[j];
                pages[j] = pages[j + 1];
                pages[j + 1] = bufferPages;
            }
            j++;
            goto innerSortingLoop;
        }
        i++;
        goto outerSortingLoop;
    }

//printout results in file
    ofstream fsout;
    fsout.open("output.txt");
    if (fsout.is_open())
    {
        i = 0;
    outputCycle:
        if (i < wordsQuantity)
        {
            if (pages[i][0] < wordPerPage + 1) 
            {
                fsout << arrayWords[i] << " : ";
                j = 1;
            outputPages:
                if (j < pages[i][0]) {
                    fsout << pages[i][j] << ", ";
                    j++;
                    goto outputPages;
                }
                fsout << pages[i][j] << '\n';
            }
            i++;
            goto outputCycle;
        }
    }
    fsout.close();
    delete[] arrayWords;

relezeArrayRef:
    if (i < wordArrayLength)
    {
        delete[] pages[i];
        i++;
        goto relezeArrayRef;
    }
}