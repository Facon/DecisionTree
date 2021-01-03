#pragma once

#include <map>
#include <set>
#include <tuple>
#include "Question.h"
#include "Example.h"
#include "DecisionNode.h"
#include "DataSubset.h"

using TrainingData = std::vector<Example>;

class CreateDecisionTree
{
public:
	using FeatureCounts = std::map<std::reference_wrapper<const Feature>, uint32_t>;

	struct BestResult
	{
		float gain;
		Question question;
	};

	static FeatureCounts getFeatureCounts(const DataSubset& dataSubset, size_t featureIndex);

	CreateDecisionTree(const TrainingData& trainingData);

	VariantNode operator()(size_t featureIndexToPredict);
	BestResult findBestSplit(const DataSubset& dataSubset) const;
	std::set<std::reference_wrapper<const Feature>>& getUniqueFeaturesByColumn(
		const DataSubset& dataSubset, size_t featureIndex) const;
	std::tuple<DataSubset, DataSubset> partitionBetweenTrueAndFalse(const DataSubset& dataSubset,
		const Question& question) const;
	float calculateGiniImpurity(const DataSubset& dataSubset) const;
	float calculateInformationGain(const DataSubset& leftDataSubset,
		const DataSubset& rightDataSubset, float currentUncertainty) const;

private:
	void checkCorrectTrainingDataBoundaries();
	void clearCache();
	DataSubset createDataSubset() const;
	VariantNode buildTree(const DataSubset& dataSubset);
	BestResult createEmptyBestResult() const;

	TrainingData trainingData;
	size_t featureIndexToPredict;
	mutable std::map<size_t, std::set<std::reference_wrapper<const Feature>>> uniqueFeaturesByColumn;
};