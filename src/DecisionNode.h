#pragma once

#include <memory>
#include <variant>
#include "DecisionLeaf.h"
#include "Question.h"

struct DecisionNode;

using VariantNode = std::variant<std::shared_ptr<DecisionNode>, DecisionLeaf>;

struct DecisionNode
{
	DecisionNode(const Question& question, VariantNode&& trueNode,
		VariantNode&& falseNode);
	~DecisionNode() = default;
	DecisionNode(const DecisionNode& decisionNode);
	DecisionNode(DecisionNode&& decisionNode) noexcept;
	
	DecisionNode& operator=(const DecisionNode& decisionNode);
	DecisionNode& operator=(DecisionNode&& decisionNode) noexcept;

	const Question question;
	VariantNode trueNode;
	VariantNode falseNode;
};
