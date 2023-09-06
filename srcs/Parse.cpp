/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 23:28:21 by suchua            #+#    #+#             */
/*   Updated: 2023/09/06 23:12:47 by suchua           ###   ########.fr       */
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
	tokenValidation();
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
			token.push_back(line.substr(start, end - start));
		start = line.find_first_not_of(sep, end);
	}
}

int	Parse::getPort(iterator i)
{
	Parse::iterator	it = i;

	for (; i != token.end(); i++)
	{
		for (int j = 0; (*i)[j]; j++)
		{
			if ((*i)[j] < '0' || (*i)[j] > '9')
				throw InvalidFileException("Error : Invalid port.");		
		}
	}
	
	int	sockfd;
	struct sockaddr_in	serverAddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::cerr << "Error creating socket." << std::endl;
		exit(1);
	}
	while (it != token.end())
	{
		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(std::atoi((*it).c_str()));
		serverAddr.sin_addr.s_addr = INADDR_ANY;

		if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) >= 0)
			return (std::atoi((*it).c_str()));

		/*
			Not closing socket fd is in case other
			server block wants to use the same port
			hence keeping it occupied
		*/

		it++;
	}
	throw InvalidFileException("Error : socket not available.");
}

void	Parse::serverCheck(Parse::iterator &i)
{
	const std::string	head[6] = {"server_name", "listen", "root", "index", "allow_methods"};
	const std::string	met[5] = {"GET", "POST", "HEAD", "OPTION"};
	conf	_conf = NONE;
	ServerBlock	block;
	
	if (*(++i) != "{")
		throw InvalidFileException("Missing {");
	++i;
	for (; i != token.end(); i++)
	{
		if (_conf == SERVER_NAME)
			block.setName(*i);
		else if (_conf == LISTEN)
		{
			block.setPort(getPort(i));
		}
		else if (_conf == ROOT)
			block.setRoot(*i);
		else if (_conf == INDEX)
			block.setIndex(*i);
		else if (_conf == ALLOW_METHOD)
			block.addMethod(*i);
		else if (*i == "server_name")
			_conf = SERVER_NAME;
		else if (*i == "listen")
			_conf = LISTEN;
		else if (*i == "root")
			_conf = ROOT;
		else if (*i == "index")
			_conf = INDEX;
		else if (*i == "allow_methods")
			_conf = ALLOW_METHOD;
		else if (*i == "}")
			break ;
		else
			throw InvalidFileException(std::strcat("Invalid syntax : ", (*i).c_str()));
	}
	this->block.push_back(block);
}

void	Parse::tokenValidation()
{
	iterator	i;

	for (i = token.begin(); i != token.end(); i++)
	{
		if (*i == "server")
			serverCheck(i);
	}
}

Parse::~Parse(){}

Parse::Parse(const Parse& other) {*this = other;}

Parse&	Parse::operator=(const Parse& other)
{
	if (this == &other)
		return (*this);
	this->token = other.token;
	return (*this);
}
