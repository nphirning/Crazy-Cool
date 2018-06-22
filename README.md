# Crazy-Cool

**Project Description**  
> Code generation for the Classroom Object-Oriented Language (COOL) implemented in C++.

**Note**
> This code generator follows the [COOL manual](https://theory.stanford.edu/~aiken/software/cool/cool-manual.pdf) for instructions on how to generate correct COOL code. The code should all compile normally (though it probably will crash at runtime). If you find any examples of incorrect code being produced, report them to me at "nphirning@gmail.com" for cash prizes.

## Sample

I've provided a sample of the output in the file `sample.cl`. This contains 500 classes and over 100k lines of COOL code, with words drawn from Google's 10k word corpus. Enjoy!

## Setup Instructions

The generator is written in C++ 11. To setup:

* Make sure C++ 11 is installed on your system.
* Compile the C++ files with the makefiles (`make` inside the `src/` directory only).
* Read the section on input flags for customization.

## Input Flags

This currently supports the following optional input flags.

* `-c` allows the user to configure the number of classes generated. This must be followed by a number.
* `-w` allows the user to input a corpus from which to draw words. You should supply either the absolute path or the relative path to the corpus from the location where the program is executed (not necessarily the location of the executable). The corpus should have a different word on each line (casing doesn't matter).
