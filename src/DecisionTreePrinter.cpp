#include "DecisionTreePrinter.h"

void printDecisionTree(std::ostream& out, const VariantNode& node, std::string_view spacing)
{
    constexpr const char* SPACING = "  ";

    if (std::holds_alternative<DecisionLeaf>(node))
    {
        auto& decisionLeaf = std::get<DecisionLeaf>(node);

        out << spacing << "Predict " << decisionLeaf << std::endl;

        return;
    }

    auto& decisionNode = std::get<std::unique_ptr<DecisionNode>>(node);
    
    out << spacing << decisionNode->question << "\n";
    std::string newSpacing(spacing);
    newSpacing.append(SPACING);

    out << spacing << "--> True: " << "\n";
    printDecisionTree(out, decisionNode->trueNode, newSpacing);

    out << spacing << "--> False: " << "\n";
    printDecisionTree(out, decisionNode->falseNode, newSpacing);
}

PredictionsByFeature getPredictionsByFeatureInString(const DecisionLeaf::Predictions& predictions)
{
    using namespace std::literals;
    auto total = 0.f;

    for (auto& prediction : predictions)
    {
        total += prediction.second;
    }

    PredictionsByFeature predictionsByFeature;

    for (auto& prediction : predictions)
    {
        predictionsByFeature.emplace(prediction.first,
            std::to_string(static_cast<uint8_t>(prediction.second / total * 100)) + "%"s);
    }

    return predictionsByFeature;
}

void printPredictions(std::ostream& out, const Feature& feature, const PredictionsByFeature& predictions)
{
    std::ostringstream predictionsBuffer;
    predictionsBuffer << "{ ";

    auto predictionIterator = predictions.cbegin();
    auto predictionEndIterator = predictions.cend();

    if (predictionIterator != predictionEndIterator)
    {
        predictionsBuffer << '\'' << predictionIterator->first << "\': "
            << '\'' << predictionIterator->second << '\'';
        ++predictionIterator;
    }

    while (predictionIterator != predictionEndIterator)
    {
        predictionsBuffer << ", \'" << predictionIterator->first << "\': "
            << '\'' << predictionIterator->second << '\'';
        ++predictionIterator;
    }

    predictionsBuffer << " }";

    out << "Actual: " << feature
        << ". Predicted: " << predictionsBuffer.str() << "\n";
}
