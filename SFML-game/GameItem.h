#pragma once
class GameItem
{
public:
	GameItem(int gid, const std::string &name);
	~GameItem();

	const std::string& Name() const { return name; }
private:
	std::string gid;
	std::string name;
};

