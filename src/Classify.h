#pragma once

#include "Example.h"
#include "DecisionNode.h"

DecisionLeaf::Predictions classify(const VariantNode& node, const Example& example);