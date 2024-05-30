#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <locale>
#include <codecvt>
using namespace std;

const int MAX_CHARS_PER_LINE = 80;
const int MAX_LINES_PER_PAGE = 25;
const char* PATH_TO_FILE = "./china.txt";

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <path_to_file>" << endl;
        return 1;
    }
    const char* PATH_TO_FILE = argv[1];

    // Create locale with codecvt for UTF-8 conversion
    locale utf8_locale(locale(), new codecvt_utf8<wchar_t>);

    // Open and set locale to UTF8 for the input file
    wifstream inputFile(PATH_TO_FILE);
    if (!inputFile.is_open()) {
        cerr << "Failed to open input file." << endl;
        return 1;
    }
    inputFile.imbue(utf8_locale);

    // Open the output file with wide character support
    wofstream outputFile("NewFile.txt");
    if (!outputFile.is_open()) {
        cerr << "Failed to open output file." << endl;
        return 1;
    }
    outputFile.imbue(utf8_locale);

    // Read the input file word by word and write to the output file
    wstring currentWord;
    int charsInLine = 0;
    int numLines = 0;
    int numPages = 1;
    //Fetch the words from the file one at a time
    while (inputFile >> currentWord) {
        int wordSize = currentWord.size();
        charsInLine += wordSize + 1;

        if (charsInLine > MAX_CHARS_PER_LINE) {
            outputFile.put(L'\n');
            if (++numLines >= MAX_LINES_PER_PAGE) {
                //print tow linebreaks and a page number
                outputFile << L"\n" << L"Page " << numPages << L"\n\n";
                numLines = 0;
                ++numPages;
            }
            charsInLine = wordSize + 1; // Start new line with current word length
        }
        outputFile << currentWord << L" ";
    }
    //Print the last page number
    outputFile << L"\n\n" << L"Page " << numPages << L"\n\n";
    inputFile.close();
    outputFile.close();
    
    return 0;
}
