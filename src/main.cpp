#include <iostream>
#include <string>

#include "AdultCSVReader.h"
#include "CreateDecisionTree.h"
#include "DecisionTreePrinter.h"
#include "Classify.h"

std::tuple<std::string, uint32_t> handleArguments(int argc, char* argv[]);
TrainingData createTrainingData(std::string_view filePath);
AdultCSVReader::FileContent readAdultCSV(std::string_view filePath);

int main(int argc, char* argv[])
{
	auto arguments = handleArguments(argc, argv);

	auto trainingData = createTrainingData(std::get<0>(arguments));
	CreateDecisionTree createDecisionTree(trainingData);
	auto tree = createDecisionTree(std::get<1>(arguments));

	printDecisionTree(std::cout, tree);

	const std::vector<std::vector<std::string>> testingDataSource
	{
		{ "Middle", "Self-emp-inc", "Bachelors", "Married-civ-spouse", "Exec-managerial", "Husband", "White", "Male", "United-States", ">50K"},
		{ "Middle", "Private", "Bachelors", "Married-civ-spouse", "Prof-specialty", "Husband", "Amer-Indian-Eskimo", "Male", "United-States", ">50K"},
		{ "Aged", "Private", "HS-grad", "Married-civ-spouse", "Handlers-cleaners", "Husband", "White", "Male", "United-States", "<=50K"},
		{ "Middle", "Private", "HS-grad", "Married-civ-spouse", "Machine-op-inspct", "Husband", "Black", "Male", "United-States", ">50K"},
		{ "Middle", "Self-emp-not-inc", "HS-grad", "Married-civ-spouse", "Transport-moving", "Husband", "White", "Male", "United-States", ">50K"}
	};

	TrainingData testingData;

	for (auto& example : testingDataSource)
	{
		testingData.emplace_back(example.cbegin(), example.cend());
	}

	for (auto& data : testingData)
	{
		const auto predictions = getPredictionsByFeatureInString(classify(tree, data));
		printPredictions(std::cout, data.at(std::get<1>(arguments)), predictions);
	}

	return 0;
}

AdultCSVReader::FileContent readAdultCSV(std::string_view filePath)
{
	AdultCSVReader reader(filePath);

	return reader.read();
}

std::tuple<std::string, uint32_t> handleArguments(int argc, char* argv[])
{
	if (argc != 3)
	{
		constexpr const char* MESSAGE = "Wrong number of arguments!";
		std::cerr << MESSAGE << "\n";

		throw std::invalid_argument(MESSAGE);
	}

	if (argv[2][0] < '0' || argv[2][0] > '9')
	{
		constexpr const char* MESSAGE = "Second argument is not an unsigned number!";
		std::cerr << MESSAGE << "\n";

		throw std::invalid_argument(MESSAGE);
	}

	return std::make_tuple(std::string(argv[1]), std::stoul(argv[2]));
}

TrainingData createTrainingData(std::string_view filePath)
{
	TrainingData trainingData;

	auto fileContent = readAdultCSV(filePath);

	Question::setHeaders(fileContent.headers);

	for (auto& example : fileContent.data)
	{
		trainingData.emplace_back(example.cbegin(), example.cend());
	}

	return trainingData;
}