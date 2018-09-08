#pragma once

#include "DialogNodeParser.h" // As of now only parses 1 tree, which means it will itself contain a tree

// Over time it would be nice with pointers to each other, instead of id-strings

class DialogNode
{
	friend class DialogNodeParser;
public:
	DialogNode() = default;
	virtual ~DialogNode() = default;

	std::string id;		  // It's own id 
	std::string NextNode; // Id for next node
	std::string message;  // Always a message or condition or choice or whatever
};

class MessageNode : public DialogNode
{
public:
	MessageNode() = default;
	~MessageNode() = default;
};

class ConditionalNode : public DialogNode
{
public:
	ConditionalNode() = default;
	~ConditionalNode() = default;

	//std::string NextNode; // Unused here
};

class PossibilityNode : public DialogNode
{
public:
	PossibilityNode() = default;
	~PossibilityNode() = default;

	std::string nodeOwner; // The ConditionalNode that owns this node
	//std::string message;   // The option, i.e Red (where the condition, from the conditionalNode is the question)
};