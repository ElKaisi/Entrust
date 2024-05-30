#include <iostream>
#include <string.h>
#include <stdio.h>
#include <chrono>
using namespace std;


const int MAX_CHARS_PER_LINE = 80;
const int MAX_LINES_PER_PAGE = 25;
const char STRING_TERMINATION = 0x00;
const unsigned int MAX_UTF8_BYTES = 4;

/*
 * Precondition: None
 * Postcondition: Returns true if byte represents
 * the start of a new Unicode character, false
 * otherwise
 * 
 * This function takes advatage of the format of UTF-8
 * having '10xxxxxx' bits at the start of every continuation 
 * byte. We use AND to extract the first two bits, and check if it
 * matches the format described above, if it does it's a continuation
 * byte, if it does not then it is a new character.
 */
bool isNewUTF8Character(char &byte) {
    return (byte & 0b11000000) != 0b10000000;
}


int main(int argc, char** argv) {
    //Check for the correct number of arguments
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <path_to_file>" << endl;
        return 1;
    }
    //Get the path to the file from the command line
    const char* PATH_TO_FILE = "china.txt";PATH_TO_FILE = argv[1];

    FILE* inputFile = fopen(PATH_TO_FILE,"r");      //File descriptor of the opened file, path can exist, otherwise it creates it
   
    FILE* outputFile = fopen("NewFile.txt","w");     //Creates new file for output, or erases existing file (O_TRUNC), with all permissions
    
    if (inputFile == NULL or outputFile == NULL) {  //Error checking for both open calls
        perror("Open failed\n");
        exit(1);
    }
    
    //String that will store the one-line text
    char* oneLineText = nullptr;
    //Number of bytes dynamically allocated for the text by getline()
    size_t bytesAllocated = 0;
    //Use getine() to read and allocate the one-line text file
    int bytes = getline(&oneLineText,&bytesAllocated,inputFile);

    //A single word can be at most 80 characters given that the line limit is 80
    char currentWordInBytes[MAX_CHARS_PER_LINE*MAX_UTF8_BYTES];    
    char currentByte;               //Used to store 1 byte in the while loop
    int bytePositionInWord = 0;     //Index to the current word, in bytes
    int charsInLine = 0;            //Keep count of the number of characters currently written on one line
    int currentWordLength = 0;      //Keep track of the length of the current word, in characters
    int numLines = 0;               //Keep track of the number of lines written to the file onone page
    int pageCount = 1;              //Keep track of the number of pages written to the file so far
    for (int i = 0; i <= bytes; ++i) {
    
        currentByte = oneLineText[i];                          //Read one byte from the text
        currentWordInBytes[bytePositionInWord] = currentByte;  //Insert the byte in the current word
        ++bytePositionInWord;                               //Add one to the index
        
        
        if (isNewUTF8Character(currentByte)) {                 //Check if the byte represents a new character in UTF-8
            charsInLine++;                                  //Count one character in the line
            currentWordLength++;                            //Count one character in the current word
        }
        
        if (currentByte == ' ' or currentByte == STRING_TERMINATION) {         //Check whether a word ends
            
            if (charsInLine > MAX_CHARS_PER_LINE) {            //Check whether we're over the character limit per line
                fputc('\n',outputFile);                         //Add a line feed character
                ++numLines;                                     //Increment the number of lines written to the file

                if (numLines == MAX_LINES_PER_PAGE) {           //Check whether we're over the line limit per page
                    fprintf(outputFile,"\nPage %d\n\n",pageCount);  //Add a page number to the file
                    numLines = 0;                               //Reset the number of lines written to the file
                    ++pageCount;                                //Increment the number of pages written to the file
                }
                charsInLine = currentWordLength;                //Add the characters of the current word to the next line's character count
            }
            
            currentWordInBytes[bytePositionInWord] = STRING_TERMINATION;    //Add the null byte to end the string
            fputs(currentWordInBytes,outputFile);                           //Write the current word to file
            bytePositionInWord = 0;                                         //Reset word counters
            currentWordLength = 0;
        }
    }
    //last page print
    
    fprintf(outputFile,"\nPage %d\n\n",pageCount);
    

    //Don't forget to free allocated memory and close file descriptors
    free(oneLineText);
    fclose(outputFile);
    fclose(inputFile);
    return 0;
}
