#include "DecisionNode.h"

DecisionNode::DecisionNode(const Question& question, VariantNode&& trueNode, 
	VariantNode&& falseNode) : question(question), trueNode(std::move(trueNode)), 
	falseNode(std::move(falseNode))
{}

DecisionNode::DecisionNode(DecisionNode && decisionNode) noexcept :
	question(decisionNode.question), trueNode(std::move(decisionNode.trueNode)),
	falseNode(std::move(decisionNode.falseNode))
{}

DecisionNode& DecisionNode::operator=(DecisionNode&& decisionNode)
{
	new (this) DecisionNode(std::move(decisionNode));

	return *this;
}
