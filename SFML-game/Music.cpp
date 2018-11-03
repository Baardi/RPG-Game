#include "stdafx.h"
#include "Music.hpp"
#include <iostream>

Music::Music()
{
}


Music::~Music()
{
}

bool Music::load(const std::filesystem::path &file)
{
	m_music.setLoop(true);
	
	if (file.has_filename() && m_music.openFromFile(file.string()))
	{
		play();
		m_current_file = std::filesystem::canonical(file);
	}
	else
	{
		reset();
	}

	return m_current_file != "";
}

void Music::play()
{
	if (!m_muted)
		m_music.play();
}

void Music::pause()
{
	m_music.pause();
}

void Music::stop()
{
	m_music.stop();
}

void Music::reset()
{
	m_music.openFromFile("");
	m_current_file = "";
}

void Music::mute()
{
	m_muted = true;
	pause();
}

void Music::unmute()
{
	m_muted = false;
	play();
}

void Music::toggle()
{
	m_muted ? unmute() : mute();
}

