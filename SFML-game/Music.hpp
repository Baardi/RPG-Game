#pragma once
#include <filesystem>

class Music
{
/**
* Wrapper for sf::Music class
*/
public:
	Music();
	~Music() = default;

	bool load(const std::filesystem::path &file);
	void play();
	void pause();
	void stop();
	void reset();
	void mute();
	void unmute();
	void toggle();
	
	auto currentFile() const { return m_currentFile; }
	auto muted() const { return m_muted; }

	auto volume() const { return m_volume; }
	void incVolume();
	void decVolume();
	void setVolume(double volume);

private:
	double m_volume = 100;
	std::filesystem::path m_currentFile;
	sf::Music m_music;
	bool m_muted;
};
