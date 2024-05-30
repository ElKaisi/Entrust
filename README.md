# Pagination application

This is a pagination application for one-line text files. The objective is to divide a one-line text file into 80-character lines and 25-line pages. This application uses UTF8 formatted text and for that reason it can accept and count characters from all languages. This repository contains two code files:

- [`fast.cc`](#fastcc): Low-level version written in C-style code with calls to POSIX functions. It is not portable to windows. This version is speed oriented and is $2\times$ as fast as the other version.

- [`portable.cc`](#portablecc): Higher-level abstraction version to make it more readable and portable. It uses built-in libraries and C++ streams.

## `fast.cc`

This code is written in a lower-level fashion, where manipulation of singular bytes is necessary. This comes with certain pros and cons. Particularly, this code is certainly faster than the portable version, but it's also harder to read and maintain and is not as portable.I used standard C libraries and I/O functions to handle the reading and writing. I used the buffered version of I/O to add to the efficiency of the application. I used getline() (this is a POSIX function, not available in Windows systems) to read the one-line text and allocate memory for it. Then we loop through every byte of the text, identifying every time if it's a new UTF8 character by analyzing the bits and keeping track of the lines and characters written at all times. If we reach the end of a word we decide whether or not that word belongs to the next line by checking the counters that we keep updated.

## `portable.cc`

This code is for the exact same application as the one described above, but I used a higher level of abstraction by using the built-in utf8 conversions, functions and C++ streams to facilitate the process. This abstraction level comes with the cost of being considerably slower, but with the benefit of making very readable, type-safe and memory-safe code.

# Instructions

In order to run any of the two codes mentioned above we first need to compile them using g++.

  - 1) `g++ fast.cc -o fast` OR `g++ portable.cc -o portable` 
  - 2) `./fast INPUT` OR `./portable INPUT`, INPUT can be the path to any one-line text file
  - 3) The resulting formatted text will be stored in a new file called `NewFile.txt`

# Tests and results

You will find some inputs with their respective outputs in this repository.

  - 1) INPUT `document.txt`, OUTPUT `formattedDocument.txt` 
  - 2) INPUT `chinese.txt`, OUTPUT `formattedChinese.txt` 
