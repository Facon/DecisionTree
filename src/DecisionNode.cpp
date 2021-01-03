#include "DecisionNode.h"

VariantNode DecisionNode::copyVariantNode(const VariantNode& variantNode)
{
	if (std::holds_alternative<DecisionLeaf>(variantNode))
	{
		return std::get<DecisionLeaf>(variantNode);
	}

	return std::get<std::shared_ptr<DecisionNode>>(variantNode);
}

DecisionNode::DecisionNode(const Question& question, VariantNode&& trueNode,
	VariantNode&& falseNode) : question(question), trueNode(std::move(trueNode)), 
	falseNode(std::move(falseNode))
{}

DecisionNode::DecisionNode(const DecisionNode& decisionNode) : question(decisionNode.question), 
	trueNode(decisionNode.trueNode), 
	falseNode(decisionNode.falseNode)
{
}

DecisionNode::DecisionNode(DecisionNode && decisionNode) noexcept :
	question(decisionNode.question), trueNode(std::move(decisionNode.trueNode)),
	falseNode(std::move(decisionNode.falseNode))
{}

DecisionNode& DecisionNode::operator=(const DecisionNode& decisionNode)
{
	return *this = DecisionNode(decisionNode);
}

DecisionNode& DecisionNode::operator=(DecisionNode&& decisionNode) noexcept
{
	new (this) DecisionNode(std::move(decisionNode));

	return *this;
}
