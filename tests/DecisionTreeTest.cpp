#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <vector>
#include <set>
#include <functional>
#include "CreateDecisionTree.h"
#include "DecisionTreePrinter.h"
#include "Classify.h"

const std::vector<std::string> headers
{
    "Age", "Workclass", "Education", "Marital - status", "Occupation", "Relationship", "Race", "Sex", "Native - country", "Income"
};

const TrainingData trainingData = []()
{
    static const std::vector<std::vector<std::string>> trainingDataSource
    {
        { "Middle", "Self-emp-inc", "Bachelors", "Married-civ-spouse", "Exec-managerial", "Husband", "White", "Male", "United-States", ">50K"},
        { "Middle", "Private", "Bachelors", "Married-civ-spouse", "Prof-specialty", "Husband", "Amer-Indian-Eskimo", "Male", "United-States", ">50K"},
        { "Aged", "Private", "HS-grad", "Married-civ-spouse", "Handlers-cleaners", "Husband", "White", "Male", "United-States", "<=50K"},
        { "Middle", "Private", "HS-grad", "Married-civ-spouse", "Machine-op-inspct", "Husband", "Black", "Male", "United-States", ">50K"},
        { "Middle", "Self-emp-not-inc", "HS-grad", "Married-civ-spouse", "Transport-moving", "Husband", "White", "Male", "United-States", ">50K"}
    };

    TrainingData trainingData;

    for (auto& example : trainingDataSource)
    {
        trainingData.emplace_back(example.cbegin(), example.cend());
    }

    return trainingData;
}();

const auto LAST_COLUMN = trainingData.at(0).size() - 1;

TEST_CASE("Get unique values for \"Education\" column")
{
    const auto featureIndex = 2;

    DataSubset dataSubset;

    for (auto& example : trainingData)
    {
        dataSubset.emplace_back(example);
    }

    CHECK(CreateDecisionTree(trainingData)
        .getUniqueFeaturesByColumn(dataSubset, LAST_COLUMN).size() == 2);
}

TEST_CASE("Count the number of features in column 0")
{
    DataSubset dataSubset;

    for (auto& example : trainingData)
    {
        dataSubset.emplace_back(example);
    }

    CHECK(CreateDecisionTree(trainingData)
        .getFeatureCounts(dataSubset, LAST_COLUMN).size() == 2);
}

TEST_CASE("Is an example middle age?")
{
    Question::setHeaders(headers);
    Feature middle("Middle");
    Question isMiddleAge(0, middle);

    CHECK(isMiddleAge.match(trainingData.at(0)));
    CHECK(isMiddleAge.match(trainingData.at(2)) == false);
}

TEST_CASE("Separate training data by is middle age or not")
{
    Question::setHeaders(headers);
    Feature middle("Middle");
    Question isMiddleAge(0, middle);

    DataSubset dataSubset;

    for (auto& example : trainingData)
    {
        dataSubset.emplace_back(example);
    }

    auto [trueData, falseData] = CreateDecisionTree(trainingData)
        .partitionBetweenTrueAndFalse(dataSubset, isMiddleAge);

    CHECK(trueData.size() == 4);
    CHECK(falseData.size() == 1);
}

TEST_CASE("Calculate Gini impurity")
{
    DataSubset dataSubset;

    for (auto& example : trainingData)
    {
        dataSubset.emplace_back(example);
    }

    auto impurity = CreateDecisionTree(trainingData)
        .calculateGiniImpurity(dataSubset);

    CHECK(impurity == doctest::Approx(0.32));
}

TEST_CASE("Calculate information gain")
{
    CreateDecisionTree createDecisionTree(trainingData);
    Feature privateClass("Private");
    Question isWorkclassPrivate(1, privateClass);

    DataSubset dataSubset;

    for (auto& example : trainingData)
    {
        dataSubset.emplace_back(example);
    }

    auto currentUncertainty = createDecisionTree.calculateGiniImpurity(dataSubset);
    auto [leftNode, rightNode] = createDecisionTree.partitionBetweenTrueAndFalse(dataSubset,
        isWorkclassPrivate);
    auto informationGain = createDecisionTree.calculateInformationGain(leftNode, rightNode, 
        currentUncertainty);

    CHECK(informationGain == doctest::Approx(0.0533).epsilon(0.01));
}

TEST_CASE("Find best split")
{
    CreateDecisionTree createDecisionTree(trainingData);

    DataSubset dataSubset;

    for (auto& example : trainingData)
    {
        dataSubset.emplace_back(example);
    }

    auto result = createDecisionTree.findBestSplit(dataSubset);

    CHECK(result.gain == doctest::Approx(0.32f));
    CHECK(result.question == Question(4, Feature("Handlers-cleaners")));
}

TEST_CASE("Create DecisionTree and print")
{
    CreateDecisionTree createDecisionTree(trainingData);

    auto tree = createDecisionTree(LAST_COLUMN);

    std::ostringstream buffer;
    printDecisionTree(buffer, tree);

    CHECK(buffer.str() == "Is Occupation == Handlers-cleaners?\n--> True: \n\tPredict { <=50K: 1 }\n--> False: \n\tPredict { >50K: 4 }\n");
}

TEST_CASE("Classify an example with a generated Decision Tree and print")
{
    CreateDecisionTree createDecisionTree(trainingData);
    auto tree = createDecisionTree(LAST_COLUMN);

    const auto predictions = getPredictionsByFeatureInString(classify(tree, trainingData[0]));
    std::ostringstream buffer;
    printPredictions(buffer, trainingData[0].at(LAST_COLUMN), predictions);

    CHECK(buffer.str() == "Actual: >50K. Predicted: { '>50K': '100%' }");
}
