#pragma once

#include <memory>
#include <variant>
#include "DecisionLeaf.h"
#include "Question.h"

struct DecisionNode;

using VariantNode = std::variant<std::unique_ptr<DecisionNode>, DecisionLeaf>;

struct DecisionNode
{
	DecisionNode(const Question& question, VariantNode&& trueNode,
		VariantNode&& falseNode);
	~DecisionNode() = default;
	DecisionNode(const DecisionNode& decisionNode) = delete;
	DecisionNode(DecisionNode&& decisionNode) noexcept;
	
	DecisionNode& operator=(const DecisionNode& decisionNode) = delete;
	DecisionNode& operator=(DecisionNode&& decisionNode);

	const Question question;
	VariantNode trueNode;
	VariantNode falseNode;
};
