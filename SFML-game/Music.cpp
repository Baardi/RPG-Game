#include "stdafx.h"
#include "Music.hpp"
#include <iostream>

Music::Music()
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

	return m_currentFile != "";
}

void Music::play()
{
	if (m_currentFile != "")
		m_music.play();
}

void Music::pause()
{
	if (m_currentFile != "")
		m_music.pause();
}

void Music::stop()
{
	if (m_currentFile != "")
		m_music.stop();
}

void Music::reset()
{
	stop();
	m_currentFile = "";
}

void Music::mute()
{
	m_muted = true;
	m_music.setVolume(0);
}

void Music::unmute()
{
	m_muted = false;
	m_music.setVolume(m_volume);
}

void Music::toggle()
{
	m_muted ? unmute() : mute();
}

void Music::incVolume()
{
	m_music.setVolume(++m_volume);
}

void Music::decVolume()
{
	m_music.setVolume(--m_volume);
}

void Music::setVolume(double volume)
{
	m_music.setVolume(m_volume = volume);
}

