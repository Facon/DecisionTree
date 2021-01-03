#include "DecisionLeaf.h"

#include "CreateDecisionTree.h"

DecisionLeaf::DecisionLeaf(const DataSubset& dataSubset, size_t featureIndex) : 
	predictions(CreateDecisionTree::getFeatureCounts(dataSubset, featureIndex))
{
}

DecisionLeaf::DecisionLeaf(const DecisionLeaf& decisionLeaf) : 
	predictions(decisionLeaf.predictions)
{
}

DecisionLeaf::DecisionLeaf(DecisionLeaf&& decisionLeaf) noexcept : 
	predictions(decisionLeaf.predictions)
{
}

DecisionLeaf& DecisionLeaf::operator=(const DecisionLeaf& decisionLeaf)
{
	return *this = DecisionLeaf(decisionLeaf);
}

DecisionLeaf& DecisionLeaf::operator=(DecisionLeaf&& decisionLeaf) noexcept
{
	new (this) DecisionLeaf(std::move(decisionLeaf));

	return *this;
}

uint32_t DecisionLeaf::getCounts(const Feature& feature)
{
	return predictions.at(feature);
}

const DecisionLeaf::Predictions& DecisionLeaf::getPredictions()
{
	return predictions;
}

std::ostream& operator<<(std::ostream& out, const DecisionLeaf& leaf)
{
	if (leaf.predictions.empty())
	{
		return out;
	}

	out << "{ ";

	auto predictionIterator = leaf.predictions.cbegin();
	const auto endIterator = leaf.predictions.cend();

	out << predictionIterator->first.get() << ": " << predictionIterator->second;
	++predictionIterator;

	while (predictionIterator != endIterator)
	{
		out << ", " << predictionIterator->first.get() << ": " << predictionIterator->second;
		++predictionIterator;
	}

	out << " }";

	return out;
}