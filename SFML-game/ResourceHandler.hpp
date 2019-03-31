#pragma once

class ResourceHandler
{
	friend ResourceHandler &resourceHandler();

	ResourceHandler() = default;
	~ResourceHandler() = default;
	ResourceHandler(const ResourceHandler &) = delete;
	ResourceHandler(ResourceHandler &&) = delete;
	ResourceHandler &operator=(const ResourceHandler &) = delete;
	ResourceHandler &operator=(ResourceHandler &&) = delete;
		
	sf::Font m_font;
	std::map<std::string, sf::Texture> m_textures;

public:
	auto &font() { return m_font; }
	auto &textures() { return m_textures; }
};

ResourceHandler &resourceHandler();
