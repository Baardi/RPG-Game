#include "stdafx.h"
#include "TileSet.hpp"
#include <print>

bool TileSet::load(const Json::Value &tilesetVal, const std::filesystem::path &directory, Textures &textures)
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
	auto [it, inserted] = textures.try_emplace(imagePath);
	if (inserted)
	{
		if (!it->second.loadFromFile(imagePath))
		{
			std::println("Failed loading texture");

			return false;
		}
	}
	
	texture = &it->second;
	loadAnimatedTiles(firstgid, tilesetVal["tiles"]);
	
	return true;
}

bool TileSet::save(Json::Value &tilesets) const
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

	return true;
}

void TileSet::loadAnimatedTiles(int firstGid, const Json::Value &tileset) // Store info on animated tiles
{
	for (const auto &tile : tileset)
	{
		int tileid = tile["id"].asInt();

		Animation tileSetAnimations;
		for (const auto &animation : tile["animation"])
		{
			int animationTileId = animation["tileid"].asInt();
			int animationTileDuration_ms = animation["duration"].asInt();

			tileSetAnimations.push_back({ animationTileId, sf::milliseconds(animationTileDuration_ms) });
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
