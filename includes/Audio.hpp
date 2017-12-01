/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Audio.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 16:13:19 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 17:47:48 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIO_HPP
# define AUDIO_HPP

# include "SFML/Audio/Sound.hpp"
# include "SFML/Audio/SoundBuffer.hpp"
# include "SFML/Audio/Music.hpp"
# include "GeneralException.hpp"
# include <string>
# include <iostream>
# include <map>

class Audio
{
	public :

		enum theme_list
		{
			THEME_1,
			THEME_2,
		};

		Audio(void);
		virtual ~Audio(void);
		Audio(const Audio &src) = delete;
		Audio &operator=(Audio const &rhs) = delete;

		void loadSound(std::string const &name, std::string const &file,
					   bool loop, float volume);
		void playSound(std::string const &name);
		void stopSound(std::string const &name);
		void pauseSound(std::string const &name);
		void setLoopSound(std::string const &name, bool value);
		void setVolumeSound(std::string const &name, float value);
		void loadTheme(std::string const &file, Audio::theme_list slot,
					   bool loop, float volume);
		void playTheme(Audio::theme_list slot);
		void stopTheme(Audio::theme_list slot);
		void pauseTheme(Audio::theme_list slot);
		void setLoopTheme(std::string const &name, bool value);
		void setVolumeTheme(std::string const &name, float value);

		class FileOpenException : public GeneralException
		{
			public :

				explicit FileOpenException(void);
				explicit FileOpenException(std::string const &path);
				virtual ~FileOpenException(void) throw();
		};

	private :

		std::map<std::string, sf::SoundBuffer> _buffer_list;
		std::map<std::string, sf::Sound>       _sound_list;
		sf::Music                              _theme_1;
		sf::Music                              _theme_2;

		bool _getSoundBuffer(std::string const &name, sf::SoundBuffer **buff);
		bool _getSound(std::string const &name, sf::Sound **buff);
};

#endif
