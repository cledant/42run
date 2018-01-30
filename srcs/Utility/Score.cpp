/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Score.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utility/WorldSelect.hpp"

long int load_score(std::string const &path)
{
	std::fstream fs;
	std::string  content;
	long int     score;

	try
	{
		fs.exceptions(std::fstream::failbit | std::fstream::badbit);
		fs.open(path, std::fstream::in);
		std::getline(fs, content);
		fs.close();
		score = std::stol(content);
	}
	catch (std::exception &e)
	{
		std::cout << "Can't load High Score" << std::endl;
		return (0);
	}
	return (score);
}

void save_score(std::string const &path, long int to_save)
{
	std::ofstream ofs;
	std::string   str_to_save;

	try
	{
		str_to_save = std::to_string(to_save);
		ofs.open(path, std::ofstream::out | std::ofstream::trunc);
		ofs << str_to_save << std::endl;
		ofs.close();
	}
	catch (std::exception &e)
	{
		std::cout << "Can't save High Score" << std::endl;
		return;
	}
}