/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 23:28:21 by suchua            #+#    #+#             */
/*   Updated: 2023/09/11 19:44:18 by suchua           ###   ########.fr       */
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
	pathValidation();
	std::cout << "Parsing completed.." << std::endl;
	std::vector<ServerBlock>::iterator	i;

	for (i = block.begin(); i != block.end(); i++)
	{
		std::cout << *i << std::endl;
	}
	
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

bool	isNum(std::string line)
{
	return (line[0] >= '0' && line[0] <= '9');
}

int		Parse::getPort(iterator i)
{
	Parse::iterator	it = i;

	for (; isNum(*i); i++)
	{
		for (size_t j = 0; (*i)[j]; j++)
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
	while (isNum(*it))
	{
		int port = std::atoi((*it).c_str());

		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(static_cast<uint16_t>(port));
		serverAddr.sin_addr.s_addr = INADDR_ANY;

		if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) >= 0)
		{
			return (port);
		}
		/*
			Not closing socket fd is in case other
			server block wants to use the same port
			hence keeping it occupied
		*/
		it++;
	}
	throw InvalidFileException("Error : All ports not available.");
}

static bool	isMethod(std::string method)
{
	const std::string	met[5] = {"GET", "POST", "HEAD", "OPTION"};

	for (size_t i = 0; i < met->size(); i++)
	{
		if (met[i] == method)
			return (true);
	}
	return (false);
}

void	ExceptionDecider(empty type)
{
	if (type == EMPTY_PORT)
		throw InvalidFileException("Error : Invalid port.");
 	if (type == EMPTY_NAME)
		throw InvalidFileException("Error : empty name.");
	else if (type == EMPTY_INDEX)
		throw InvalidFileException("Error : empty index.");
	else if (type == EMPTY_ROOT)
		throw InvalidFileException("Error : empty ROOT.");
	else
		return ;
}

bool	isHead(std::string line)
{
	const std::string head[6] = {"server_name", "listen", "root", "index", "allow_methods", "location"};

	for (size_t i = 0; i < 6; i++)
	{
		if (line == head[i])
			return (true);
	}
	return (false);
}

void	Parse::setMethod(Parse::iterator &i, ServerBlock& sb)
{
	while (i != token.end() && *i != "}" && isMethod(*i))
		sb.addMethod(*i++);
	if (i == token.end() || *i == "}")
	{
		--i;
		return ;
	}
	if (!isHead(*i))
	{
		std::cerr << "Error : Invalid syntax : " + *i;
		throw InvalidFileException("");
	}
	--i;
}

void	Parse::serverCheck(Parse::iterator &i)
{
	conf		_conf = NONE;
	ServerBlock	block;
	
	if (*(++i) != "{")
		throw InvalidFileException("Missing {");
	++i;
	for (; i != token.end(); i++)
	{
		if (*i == "}")
			break ;
		if (*i == "server_name")
			_conf = SERVER_NAME;
		else if (*i == "listen")
			_conf = LISTEN;
		else if (*i == "root")
			_conf = ROOT;
		else if (*i == "index")
			_conf = INDEX;
		else if (*i == "allow_methods" || (_conf == ALLOW_METHOD && isMethod(*i)))
			_conf = ALLOW_METHOD;
		else if (*i == "location")
			_conf = LOCATION;
		if (_conf == NONE)
		{
			std::cerr << "Error : Invalid syntax : " + *i << std::endl;
			throw InvalidFileException("");
		}
		++i;
		if (_conf == SERVER_NAME)
			block.setName(*i);
		else if (_conf == LISTEN)
		{
			block.setPort(getPort(i));
			while (isNum(*(i + 1)))
				++i;
		}
		else if (_conf == ROOT)
			block.setRoot(*i);
		else if (_conf == INDEX)
			block.setIndex(*i);
		else if (_conf == ALLOW_METHOD)
			setMethod(i, block);
		else if (_conf == LOCATION)
			block.addLocation(Location(i, token));
		if (_conf != ALLOW_METHOD)
			_conf = NONE;
	}
	if (i == token.end())
		throw InvalidFileException("Error : Missing }");
	ExceptionDecider(block.somethingEmpty());
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

void	Parse::pathValidation()
{
	std::vector<ServerBlock>::iterator	i;

	for (i = block.begin(); i != block.end(); i++)
	{
		std::string	folder = (*i).getRoot();
		std::string	index = (*i).getRoot() + std::string("/") + (*i).getIndex();
		std::string	errMsg;

		std::ifstream	in(folder);
		std::ifstream	in2(index);
		
		if (!in)
			errMsg = folder;
		else if (!in2)
			errMsg = index;
		if (!errMsg.empty())	
		{
			std::cerr << "Error : no such file or directory : " << errMsg;
			throw InvalidFileException("");
		}
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
