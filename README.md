## sbTradutor1

C++ code implementing a Educational Assembly Compiler. 

Developed by:
  - Raphael Arthur Resende
  - Yuri Gonçalves Rocha

October 2015, Brasilia - Brazil

## Required Libraries
   - Boost headers
   - Boost Program Options
   - Boost Regex
To install them use the following commands:
```
$ sudo apt-get install libboost-dev
$ sudo apt-get install libboost-program-options1.55-dev 
$ sudo apt-get install libboost-regex1.55-dev
```
        
Also the C++11 version or above is required.

## Building and Running Instructions

We provided a script to build the code, however the script need running permitions to be executed:
```
$ chmod +x build_release.sh
```
To build the code just run the following command:
```
$ ./build_release.sh
```
The project can be executed running the following command:
```
$ ./Tradutor <arguments>
```
To read all the instructions it is possible to use the argument -h or --help (the instructions are in Portuguese):
```
$ ./Tradutor -h
$ ./Tradutor --help
```
