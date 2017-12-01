/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Audio.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:31:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Audio.hpp"

Audio::Audio(void)
{
}

Audio::~Audio(void)
{
	std::map<std::string, sf::Sound>::iterator it;

	for (it = this->_sound_list.begin(); it != this->_sound_list.end(); ++it)
		it->second.stop();
}

void Audio::loadSound(std::string const &name, std::string const &file,
					  bool loop, float volume)
{
	sf::SoundBuffer buff;
	sf::Sound       sound;
	sf::SoundBuffer *sb = nullptr;

	std::cout << "Loading : " + file << std::endl;
	if (!buff.loadFromFile(file))
		throw Audio::FileOpenException(file);
	this->_buffer_list.insert(std::pair<std::string, sf::SoundBuffer>(name, buff));
	this->_getSoundBuffer(name, &sb);
	sound.setBuffer(*sb);
	sound.setVolume(volume);
	sound.setLoop(loop);
	this->_sound_list.insert(std::pair<std::string, sf::Sound>(name, sound));
}

void Audio::playSound(std::string const &name)
{
	sf::Sound *sound = nullptr;

	if (!this->_getSound(name, &sound))
	{
		std::cout << "Sound unknown : " + name << std::endl;
		return;
	}
	sound->play();
}

void Audio::stopSound(std::string const &name)
{
	sf::Sound *sound = nullptr;

	if (!this->_getSound(name, &sound))
	{
		std::cout << "Sound unknown : " + name << std::endl;
		return;
	}
	sound->stop();
}

void Audio::pauseSound(std::string const &name)
{
	sf::Sound *sound = nullptr;

	if (!this->_getSound(name, &sound))
	{
		std::cout << "Sound unknown : " + name << std::endl;
		return;
	}
	sound->pause();
}

void Audio::setLoopSound(std::string const &name, bool value)
{
	sf::Sound *sound = nullptr;

	if (!this->_getSound(name, &sound))
	{
		std::cout << "Sound unknown : " + name << std::endl;
		return;
	}
	sound->setLoop(value);
}

void Audio::setVolumeSound(std::string const &name, float value)
{
	sf::Sound *sound = nullptr;

	if (!this->_getSound(name, &sound))
	{
		std::cout << "Sound unknown : " + name << std::endl;
		return;
	}
	sound->setVolume(value);
}
/*
void Audio::loadTheme(std::string const &file, t_theme_list slot,
					  bool loop, float volume)
{
	sf::SoundBuffer buff;
	sf::Sound       sound;
	sf::SoundBuffer *sb = nullptr;

	std::cout << "Loading : " + file << std::endl;
	if (!buff.loadFromFile(file))
		throw Audio::FileOpenException(file);
	this->_buffer_list.insert(std::pair<std::string, sf::SoundBuffer>(name, buff));
	this->_getSoundBuffer(name, &sb);
	sound.setBuffer(*sb);
	sound.setVolume(volume);
	sound.setLoop(loop);
	this->_sound_list.insert(std::pair<std::string, sf::Sound>(name, sound));
}
*/
bool Audio::_getSoundBuffer(std::string const &name, sf::SoundBuffer **buff)
{
	std::map<std::string, sf::SoundBuffer>::iterator it;

	if ((it = this->_buffer_list.find(name)) == this->_buffer_list.end())
	{
		*buff = nullptr;
		return (false);
	}
	*buff   = &(it->second);
	return (true);
}

bool Audio::_getSound(std::string const &name, sf::Sound **buff)
{
	std::map<std::string, sf::Sound>::iterator it;

	if ((it = this->_sound_list.find(name)) == this->_sound_list.end())
	{
		*buff = nullptr;
		return (false);
	}
	*buff   = &(it->second);
	return (true);
}

Audio::FileOpenException::FileOpenException(std::string const &path)
{
	this->_msg = "Audio : Failed to find to open file : ";
	this->_msg += path.c_str();
}

Audio::FileOpenException::FileOpenException(void)
{
	this->_msg = "Audio : Failed to find to open file";
}

Audio::FileOpenException::~FileOpenException(void) throw()
{
}