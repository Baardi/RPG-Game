#pragma once

using Textures = std::unordered_map<std::filesystem::path, sf::Texture>;

class ResourceManager
{
	friend ResourceManager &resources();

	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager &) = delete;
	ResourceManager(ResourceManager &&) = delete;
	ResourceManager &operator=(const ResourceManager &) = delete;
	ResourceManager &operator=(ResourceManager &&) = delete;
		
	sf::Font m_font;
	Textures m_textures;

public:
	auto &font() { return m_font; }
	auto &textures() { return m_textures; }
};

ResourceManager &resources();
