#pragma once
#include "DialogNode.h"

class DialogNodeParser
{
public:
	DialogNodeParser();
	~DialogNodeParser();

	void Parse(const std::string &fileName);

	//Right now just parses 1 dialog tree, in the future will be bigger

	void ParseMessageNode(const std::string &id, Json::Value &attr);
	std::map<std::string, MessageNode> messageNodes;
	
	void ParsePossibilityNode(const std::string &id, Json::Value &attr);
	std::map<std::string, PossibilityNode> possibilityNodes;
	
	void ParseConditionalNode(const std::string &id, Json::Value &attr);
	std::map<std::string, ConditionalNode> conditionalNodes;
	
};

