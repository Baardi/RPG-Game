#pragma once
#include <filesystem>

/*
*	Wrapper for sf::Music class
*/
class Music
{
public:
	Music();
	~Music() = default;

	bool load(const std::filesystem::path &file);
	void reset();
	auto currentFile() const { return m_currentFile; }

	void play();
	void pause();
	void stop();
	
	void mute();
	void unmute();
	void toggle();
	bool muted() const { return m_muted; }

	double volume() const { return m_volume; }
	void incVolume();
	void decVolume();
	void setVolume(double volume);

private:
	double m_volume;
	std::filesystem::path m_currentFile;
	sf::Music m_music;
	bool m_muted;
};
