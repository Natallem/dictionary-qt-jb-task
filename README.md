# Dictionary task

## About realization
* Used: Qt
* Used two threads : one for checking loop message and printing result, second for searching suitable words in dicitonary
* Program loads rom input dictionary file into memory map from char into position of words containing this char, for each user's input picks all positions of words that contain necessary chars and check then using KMP algo
* When all words found user can see message ```Total occurrences number = n```


## Optional tasks
* dictionary isn't loaded in memory, use position changing in dictionary file
* sequential substring searching (with checkbox)

## How to run
* __qmake__ with `dictionary.pro`
  

* __cmake__ with Qt6 and `CMakeLists.txt`
    > To run with __cmake__ in __CLion__ do not forget to set _-DCMAKE_PREFIX_PATH=<path_to_your_Qt6_cmake_dir>_ in ```File | Settings | Build, Execution, Deployment | CMake | CMake options``` (ex. ```-DCMAKE_PREFIX_PATH=C:\ProgramFiles\Qt\6.0.1\mingw81_64\lib\cmake```)
