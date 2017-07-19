# Crazy-Cool

**The Project:**  
> Code generation for the Classroom Object-Oriented Language (COOL) implemented in C++.

**Important Note:**
> This is the ***best*** COOL code generator on the market.

**Final Note:**
> This code generator follows the [COOL manual](https://theory.stanford.edu/~aiken/software/cool/cool-manual.pdf) for instructions on how to generate correct COOL code. The code should all compile normally (though it very well may crash at runtime). If you find any examples of incorrect code being produced, please report to "nphirning@gmail.com". You may be rewarded. 

## Setup Instructions

The generator is written in C++ 11. So the instructions follow:

* Make sure C++ 11 is installed on your system.
* Compile the C++ files (go ahead and use the makefiles for this task --- there are four of them, but you should only `make` inside the `src/` directory).
* The resulting executable will work. Read the section on input flags for customization.

## Input Flags

This currently supports the following optional input flags.

* `-c` allows the user to configure the number of classes generated. This must be followed by a number.
* `-w` allows the user to input a corpus from which to draw words. You should supply either the absolute path or the relative path to the corpus from the location where the program is executed (not necessarily the location of the executable). The corpus should have a different word on each line (casing doesn't matter).

## Things to Do

* Add support for let, case.
* Add try-catch blocks for NameGenerator inside ClassTree.cc.
* Consider making things static (?)
* Oh. Switch things to enums.
* Benchmark the different expression types to figure out what takes the most time in generation (my suspicion: dispatch).
* Add support for configuration via input flags.
* Error checking for corpus -- words with nonletter first characters, whitespace in words, etc.
* Expand corpus of my favorite words to include more of my favorite words.
* Generate COOL program with ~1000000 lines and upload it to show dominance over all the other COOL code generators out there.
* Delete this section in the README.
