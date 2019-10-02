#include "stdafx.h"
#include "TileSet.hpp"

void TileSet::load(const Json::Value &tilesetVal, const std::filesystem::path &directory, std::map<std::string, sf::Texture> &textures)
{
	image = tilesetVal["image"].asString();
	firstgid = tilesetVal["firstgid"].asInt();
	name = tilesetVal["name"].asString();
	width = tilesetVal["imagewidth"].asInt();
	height = tilesetVal["imageheight"].asInt();
	tileSize.x = tilesetVal["tilewidth"].asInt();
	tileSize.y = tilesetVal["tileheight"].asInt();
	tileSize.s = tilesetVal["spacing"].asInt();
	tileSize.m = tilesetVal["margin"].asInt();

	auto imagePath = directory / image;
	auto [it, inserted] = textures.try_emplace(imagePath.string());
	texture = &it->second;
	if (inserted)
		it->second.loadFromFile(imagePath.string());
	
	loadAnimatedTiles(firstgid, tilesetVal["tiles"]);
}

void TileSet::save(Json::Value &tilesets) const
{
	Json::Value tilesetVal;

	tilesetVal["firstgid"] = firstgid;
	tilesetVal["image"] = image.string();
	tilesetVal["firstgid"] = firstgid;
	tilesetVal["name"] = name;
	tilesetVal["imagewidth"] = width;
	tilesetVal["imageheight"] = height;
	tilesetVal["tilewidth"] = tileSize.x;
	tilesetVal["tileheight"] = tileSize.y;
	tilesetVal["spacing"] = tileSize.s;
	tilesetVal["margin"] = tileSize.m;

	saveAnimatedTiles(tilesetVal["tiles"]);

	tilesets.append(tilesetVal);
}

void TileSet::loadAnimatedTiles(int firstGid, const Json::Value &tileset) // Store info on animated tiles
{
	for (const auto &tile : tileset)
	{
		int tileid = tile["id"].asInt();

		std::vector<std::pair<int, sf::Time>> tileSetAnimations;
		for (const auto &animation : tile["animation"])
		{
			int animationTileId = animation["tileid"].asInt();
			int animationTileDuration_ms = animation["duration"].asInt();

			tileSetAnimations.emplace_back(animationTileId, sf::milliseconds(animationTileDuration_ms));
		}

		animatedTiles.try_emplace(firstGid + tileid, tileSetAnimations);
	}
}

void TileSet::saveAnimatedTiles(Json::Value &tileset) const
{
	for (auto[animationtileid, animationdata] : animatedTiles)
	{
		Json::Value tilesetAnimation;
		tilesetAnimation["id"] = animationtileid - firstgid;

		for (auto[tileid, duration] : animationdata)
		{
			Json::Value animationFrameVal;
			animationFrameVal["duration"] = static_cast<int>(duration.asMilliseconds());
			animationFrameVal["tileid"] = tileid;

			tilesetAnimation["animation"].append(animationFrameVal);
		}

		if (!tilesetAnimation.empty())
			tileset.append(tilesetAnimation);
	}
}
