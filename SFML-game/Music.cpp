#include "stdafx.h"
#include "Music.hpp"
#include <iostream>

Music::Music() :
	m_muted(false),
	m_volume(100)
{
	m_music.setLoop(true);
}

bool Music::load(const std::filesystem::path &file)
{	
	if (file.has_filename() && m_music.openFromFile(file.string()))
	{
		m_currentFile = std::filesystem::canonical(file);
		play();
	}
	else
	{
		reset();
	}

	return m_currentFile.has_value();
}

void Music::reset()
{
	stop();
	m_currentFile.reset();
}

void Music::play()
{
	if (m_currentFile.has_value())
		m_music.play();
}

void Music::pause()
{
	if (m_currentFile.has_value())
		m_music.pause();
}

void Music::stop()
{
	if (m_currentFile.has_value())
		m_music.stop();
}

void Music::mute()
{
	m_muted = true;
	m_music.setVolume(0.0);
}

void Music::unmute()
{
	m_muted = false;
	m_music.setVolume(float(m_volume));
}

void Music::toggle()
{
	m_muted ? unmute() : mute();
}

void Music::incVolume()
{
	if (m_volume < 100.0)
		m_music.setVolume(float(++m_volume));
}

void Music::decVolume()
{
	if (m_volume > 0.0)
		m_music.setVolume(float(--m_volume));
}

void Music::setVolume(double volume)
{
	if (volume >= 0.0 && volume <= 100.0)
		m_music.setVolume(float(m_volume = volume));
}

