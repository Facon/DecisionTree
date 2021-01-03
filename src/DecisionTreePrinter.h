#pragma once

#include "DecisionNode.h"
#include <string_view>
#include <sstream>

using PredictionsByFeature = std::map<std::reference_wrapper<const Feature>, std::string>;

void printDecisionTree(std::ostream& out, const VariantNode& tree, std::string_view spacing = "");
PredictionsByFeature getPredictionsByFeatureInString(const DecisionLeaf::Predictions& predictions);
void printPredictions(std::ostream& out, const Feature& feature,
	const PredictionsByFeature& predictions);