#pragma once
#include <filesystem>
#include "Layer.hpp"

#include "App/ResourceManager.hpp"

class TileSet
{
public:
	bool load(const Json::Value &root, const std::filesystem::path &directory, Textures &textures);
	bool save(Json::Value &tilesets) const;
	
	AnimationTileMap animatedTiles;
	void loadAnimatedTiles(int firstGid, const Json::Value &tileset);
	void saveAnimatedTiles(Json::Value &tileset) const;

	TileSize tileSize;
	std::filesystem::path image;
	int firstgid;
	std::string name;
	const sf::Texture *texture;
	int width, height;
};
