#include "Classify.h"

DecisionLeaf::Predictions classify(const VariantNode& node, const Example& example)
{
    if (std::holds_alternative<DecisionLeaf>(node))
    {
        DecisionLeaf decisionLeaf = std::get<DecisionLeaf>(node);

        return decisionLeaf.getPredictions();
    }

    auto& decisionNode = std::get<std::unique_ptr<DecisionNode>>(node);

    if (decisionNode->question.match(example))
    {
        return classify(decisionNode->trueNode, example);
    }

	return classify(decisionNode->falseNode, example);
}
