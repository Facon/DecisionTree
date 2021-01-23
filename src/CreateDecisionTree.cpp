#include "CreateDecisionTree.h"

#include <optional>
#include <future>

std::atomic_uint16_t CreateDecisionTree::threadCounter = 0;

CreateDecisionTree::CreateDecisionTree(const TrainingData& trainingData) 
	: trainingData(std::make_shared<const TrainingData>(trainingData)), featureIndexToPredict()
{
}

VariantNode CreateDecisionTree::operator()(size_t featureIndexToPredict)
{
    checkCorrectTrainingDataBoundaries();
    clearCache();
    this->featureIndexToPredict = featureIndexToPredict;

    DataSubset dataSubset = createDataSubset();

    return buildTree(dataSubset);
}

void CreateDecisionTree::checkCorrectTrainingDataBoundaries()
{
    if (trainingData->empty())
    {
        throw std::invalid_argument("Empty training data!");
    }

    if (trainingData->at(0).empty())
    {
        throw std::invalid_argument("No examples!");
    }

    const auto numberOfFeatures = trainingData->at(0).size();

    for (const auto& example : *trainingData)
    {
        if (example.size() != numberOfFeatures)
        {
            throw std::invalid_argument("Example with different number of features!");
        }
    }
}

void CreateDecisionTree::clearCache()
{
    uniqueFeaturesByColumn.clear();
}

DataSubset CreateDecisionTree::createDataSubset() const
{
    DataSubset dataSubset;

    for (auto& example : *trainingData)
    {
        dataSubset.emplace_back(example);
    }

    return dataSubset;
}

VariantNode CreateDecisionTree::buildTree(const DataSubset& dataSubset) const
{

    const auto bestResult = findBestSplit(dataSubset);

    if (bestResult.gain == 0)
    {
        return VariantNode(DecisionLeaf(dataSubset, featureIndexToPredict));
    }

    DataSubset trueDataSubset;
    DataSubset falseDataSubset;

    std::tie(trueDataSubset, falseDataSubset) = partitionBetweenTrueAndFalse(dataSubset, 
        bestResult.question);

    auto counter = threadCounter.load();

    if (counter <= std::thread::hardware_concurrency())
    {
        VariantNode trueBranch;
        VariantNode falseBranch;

        threadCounter.fetch_add(1);

        auto f1 = std::async([*this, &trueDataSubset, &trueBranch]()
            { trueBranch = buildTree(trueDataSubset); });
        auto f2 = std::async([*this, &falseDataSubset, &falseBranch]()
            { falseBranch = buildTree(falseDataSubset); });

        f1.wait();
        f2.wait();

        threadCounter.fetch_sub(1);

        return VariantNode(std::make_unique<DecisionNode>(bestResult.question,
            std::move(trueBranch), std::move(falseBranch)));
    }
    else
    {
        auto trueBranch = buildTree(trueDataSubset);
        auto falseBranch = buildTree(falseDataSubset);

        return VariantNode(std::move(std::make_unique<DecisionNode>(bestResult.question,
            std::move(trueBranch), std::move(falseBranch))));
    }
}

CreateDecisionTree::BestResult CreateDecisionTree::findBestSplit(const DataSubset& dataSubset) const
{
    std::optional<BestResult> bestResult(createEmptyBestResult());
    auto currentUncertainty = calculateGiniImpurity(dataSubset);
    auto numberOfFeatures = dataSubset.at(0).get().size() - 1;

    for (auto featureIndex = 0; featureIndex < numberOfFeatures; ++featureIndex)
    {
        auto& uniqueFeatures = getUniqueFeaturesByColumn(dataSubset, featureIndex);

        for (auto& feature : uniqueFeatures)
        {
            Question question(featureIndex, feature);

            auto [trueDataSubset, falseDataSubset] = partitionBetweenTrueAndFalse(dataSubset,
                question);

            if (trueDataSubset.empty() || falseDataSubset.empty())
            {
                continue;
            }

            auto gain = calculateInformationGain(trueDataSubset, falseDataSubset,
                currentUncertainty);

            if (gain >= bestResult.value().gain)
            {
                bestResult.emplace(BestResult{ gain, question });
            }
        }
    }

    return bestResult.value_or(createEmptyBestResult());
}

CreateDecisionTree::BestResult CreateDecisionTree::createEmptyBestResult() const
{
    return BestResult{ 0.f, Question(0, Feature()) };
}

std::tuple<DataSubset, DataSubset> CreateDecisionTree::partitionBetweenTrueAndFalse(
    const DataSubset& dataSubset, const Question& question) const
{
    DataSubset trueData;
    DataSubset falseData;

    for (const auto& example : dataSubset)
    {
        if (question.match(example))
        {
            trueData.emplace_back(example);
        }
        else
        {
            falseData.emplace_back(example);
        }
    }

    return std::make_tuple(trueData, falseData);
}

float CreateDecisionTree::calculateGiniImpurity(const DataSubset& dataSubset) const
{
    const auto counts = getFeatureCounts(dataSubset, featureIndexToPredict);
    const float nodeSize = static_cast<float>(dataSubset.size());
    auto impurity = 1.f;

    for (const auto& featureIterator : counts)
    {
        auto featureProbability = featureIterator.second / nodeSize;
        impurity -= featureProbability * featureProbability;
    }

    return impurity;
}

CreateDecisionTree::FeatureCounts CreateDecisionTree::getFeatureCounts(
    const DataSubset& dataSubset, size_t featureIndex)
{
    std::map<std::reference_wrapper<const Feature>, uint32_t> featureCounts;

    for (const auto& example : dataSubset)
    {
        const auto& feature = example.get().at(featureIndex);
        auto iterator = featureCounts.find(feature);

        if (iterator == featureCounts.end())
        {
            auto result = featureCounts.emplace(feature, 0);
            iterator = result.first;
        }

        ++(iterator->second);
    }

    return featureCounts;
}

std::set<std::reference_wrapper<const Feature>>& CreateDecisionTree::getUniqueFeaturesByColumn(
    const DataSubset& dataSubset, size_t featureIndex) const
{
    auto iterator = uniqueFeaturesByColumn.find(featureIndex);

    if (iterator == uniqueFeaturesByColumn.end())
    {
        std::set<std::reference_wrapper<const Feature>> uniqueFeatures;

        for (const auto& example : dataSubset)
        {
            uniqueFeatures.emplace(example.get().at(featureIndex));
        };

        auto results = uniqueFeaturesByColumn.emplace(featureIndex, uniqueFeatures);

        iterator = results.first;
    }

    return iterator->second;
}

float CreateDecisionTree::calculateInformationGain(const DataSubset& leftDataSubset,
    const DataSubset& rightDataSubset, float currentUncertainty) const
{
    auto probability = static_cast<float>(leftDataSubset.size()) / 
        (leftDataSubset.size() + rightDataSubset.size());

    auto leftProbability = probability * calculateGiniImpurity(leftDataSubset);
    auto rightProbability = (1.f - probability) * calculateGiniImpurity(rightDataSubset);

    return currentUncertainty - leftProbability - rightProbability;
}
