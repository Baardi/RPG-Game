#include "stdafx.h"
#include "DialogNodeParser.h"
#include <fstream>

DialogNodeParser::DialogNodeParser()
{
}


DialogNodeParser::~DialogNodeParser()
{
}

void DialogNodeParser::Parse(const std::string &fileName)
{

	// Will contain the data we read in
	Json::Value root;

	// Parses the file
	Json::Reader reader;

	// Stream used for reading the data file. The data file has been saved as JSON in Tiled
	std::ifstream file(fileName);

	// Read data from file into root object
	bool parsingSuccessful = reader.parse(file, root);
	if (!parsingSuccessful)
		return;

	Json::Value &nodes = root["nodes"];

	for (Json::ValueIterator it = nodes.begin(); it != nodes.end(); ++it)
	{
		auto &node = root["nodes"][it.index()];
		auto &attr = node["attributes"];
		auto nodeType = attr["label"].asString();
		auto id = attr["id"].asString();

		if (nodeType == "messageNode")
			ParseMessageNode(id, attr);
		else if (nodeType == "conditionalNode")
			ParseConditionalNode(id, attr);
		else if (nodeType == "possibilityNode")
			ParsePossibilityNode(id, attr); // Choices should be savable to variablemap!

		messageNodes.emplace(node["id"].asString(), attr["Message"].asString());
	}
}

void DialogNodeParser::ParseMessageNode(const std::string& id, Json::Value& attr)
{
	MessageNode node;
	node.NextNode = attr["nextNode"].asString();
	node.message = attr["message"].asString();

	messageNodes.emplace(id, node);
}

void DialogNodeParser::ParsePossibilityNode(const std::string& id, Json::Value& attr)
{
	MessageNode node;
	node.NextNode = attr["nextNode"].asString();
	node.message = attr["message"].asString();

	possibilityNodes.emplace(id, node);
}

void DialogNodeParser::ParseConditionalNode(const std::string& id, Json::Value& attr)
{
	MessageNode node;
	node.NextNode = attr["nextNode"].asString();
	node.message = attr["message"].asString();

	conditionalNodes.emplace(id, node);
}
