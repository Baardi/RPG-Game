#pragma once
#include <filesystem>

class Music
{
/**
* Wrapper for sf::Music class
*/
public:
	Music() = default;
	~Music() = default;

	bool load(const std::filesystem::path &file);
	void play();
	void pause();
	void stop();
	void reset();
	void mute();
	void unmute();
	void toggle();
	
	auto current_file() const { return m_current_file; }
	auto muted() const { return m_muted; }

private:
	std::filesystem::path m_current_file;
	sf::Music m_music;
	bool m_muted;
};

