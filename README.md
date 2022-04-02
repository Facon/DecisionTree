# DecisionTree
Sample implementation of a multithreading decision tree in C++17. I have based it on https://www.youtube.com/watch?v=LDRbO9a6XPU, but I have added threading using a fork-join strategy.

It's not the fastest implementation but is fast enough and is a good starting point.

## How to compile

1. You need to have CMake and Conan installed.
2. I have provided Conan profiles that I have used.
3. Open cmd in folder:
	1. cd ..
	2. mkdir build-DecisionTree-Release
	3. conan install ../DecisionTree -pr=visual_studio -b=missing
	4. cmake ../DecisionTree -DCMAKE_BUILD_TYPE=Release (May need extra parameters for selecting the compiler.)
4. Compile and execute.
