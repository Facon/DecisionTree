#pragma once

#include <map>
#include "DataSubset.h"

class DecisionLeaf
{
public:
	using Predictions = std::map<std::reference_wrapper<const Feature>, uint32_t>;
	DecisionLeaf(const DataSubset& dataSubset, size_t featureIndex);

	uint32_t getCounts(const Feature& feature);
	const Predictions& getPredictions();

private:
	friend std::ostream& operator<<(std::ostream& out, const DecisionLeaf& leaf);

	const Predictions predictions;
};

std::ostream& operator<<(std::ostream& out, const DecisionLeaf& leaf);