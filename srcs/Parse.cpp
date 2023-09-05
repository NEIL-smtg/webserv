/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 23:28:21 by suchua            #+#    #+#             */
/*   Updated: 2023/09/06 01:15:23 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"

Parse::Parse(std::string fileName)
{
	std::ifstream	infile(fileName.c_str());

	if (!infile.is_open())
		throw InvalidFileException("Error opening file.");

	std::string	line;
	while (std::getline(infile, line))
		tokennize(line);
	
	for (std::vector<std::string>::iterator i = token.begin(); i != token.end(); i++)
		std::cout << *i << std::endl;
}

/*
	end = find the first index of sep
	start = find the next index of not sep
	push the string between start and end into list

	works like ft_split with sep==isspace
*/
void	Parse::tokennize(std::string line)
{
	const std::string sep(" \t\v\r\n\f");
	size_t	start = 0, end = 0;
	size_t	npos = std::string::npos;

	while (start != npos && end != npos)
	{
		end = line.find_first_of(sep, start);
		if (end == npos && line[start] != 0)
		{
			token.push_back(&line[start]);
			break ;
		}
		if (start != npos && end - start != 0 && line[start] != 0)
			token.push_back(line.substr(start, end - start + 1));
		start = line.find_first_not_of(sep, end);
	}
}

Parse::~Parse(){}
