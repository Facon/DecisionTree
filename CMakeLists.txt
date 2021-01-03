cmake_minimum_required(VERSION 3.13)
project(DecisionTree)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

SET(LIBRARY DecisionTreeLibrary)
add_library(${LIBRARY})

target_include_directories(${LIBRARY} PUBLIC ${PROJECT_SOURCE_DIR}/src)

target_sources(${LIBRARY} 
PUBLIC
	src/DataSubset.h
	src/Question.h
	src/Example.h
	src/Feature.h
	src/DecisionNode.h
	src/DecisionLeaf.h
	src/CreateDecisionTree.h
	src/DecisionTreePrinter.h
	src/Classify.h
	src/AdultCSVReader.h
	
PRIVATE
	src/Feature.cpp
	src/Question.cpp
	src/Example.cpp
	src/DecisionNode.cpp
	src/DecisionLeaf.cpp
	src/CreateDecisionTree.cpp
	src/DecisionTreePrinter.cpp
	src/Classify.cpp
	src/AdultCSVReader.cpp
)

target_link_libraries(${LIBRARY} PUBLIC CONAN_PKG::fast-cpp-csv-parser)

set(EXECUTABLE "dectree")
add_executable(${EXECUTABLE})

target_sources(${EXECUTABLE} PRIVATE
	src/main.cpp
)

target_link_libraries(${EXECUTABLE} PRIVATE ${LIBRARY})

set(TEST DecisionTreeTest)
add_executable(${TEST})

target_sources(${TEST} PRIVATE
	tests/DecisionTreeTest.cpp
	tests/AdultCSVReaderTest.cpp
)

target_link_libraries(${TEST} PRIVATE ${LIBRARY} CONAN_PKG::doctest)
