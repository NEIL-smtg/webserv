/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 22:28:55 by suchua            #+#    #+#             */
/*   Updated: 2023/09/13 16:44:45 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerBlock.hpp"

ServerBlock::ServerBlock()
{
	this->name.clear();
	this->port = -1;
	this->root.clear();
	this->index.clear();
}

ServerBlock::~ServerBlock(){}

ServerBlock::ServerBlock(const ServerBlock& other){*this = other;}

ServerBlock&	ServerBlock::operator=(const ServerBlock& other)
{
	if (this == &other)
		return (*this);
	this->name = other.name;
	this->port = other.port;
	this->root = other.root;
	this->index = other.index;
	this->error_page = other.error_page;
	this->methods = other.methods;
	this->location = other.location;
	return (*this);
}

void	ServerBlock::addMethod(str method)
{
	this->methods.push_back(method);
}

void	ServerBlock::addLocation(Location& location)
{
	this->location.push_back(location);
}

void	ServerBlock::setName(str name)
{
	this->name = name;
}

void	ServerBlock::setPort(int port)
{
	this->port = port;
}

void	ServerBlock::setRoot(str root)
{
	this->root = root;
}

void	ServerBlock::setIndex(str index)
{
	this->index = index;
}

empty	ServerBlock::somethingEmpty()
{
	if (port == -1)
		return (EMPTY_PORT);
	else if (name.empty())
		return (EMPTY_NAME);
	else if (root.empty())
		return (EMPTY_ROOT);
	else if (index.empty())
		return (EMPTY_INDEX);
	else
		return (FILLED);
}

std::string	ServerBlock::getName() const
{
	return this->name;
}

int	ServerBlock::getPort() const
{
	return this->port;
}

std::string	ServerBlock::getRoot() const
{
	return this->root;
}

std::string	ServerBlock::getIndex() const
{
	return this->index;
}

std::vector<std::string>	ServerBlock::getMethods() const
{
	return this->methods;
}

std::vector<Location>	ServerBlock::getLocation() const
{
	return this->location;
}

std::ostream&	operator<<(std::ostream& out, ServerBlock& sb)
{
	out << "Server name : " << sb.getName() << std::endl;
	out << "Listening : " << sb.getPort() << std::endl;
	out << "ROOT : " << sb.getRoot() << std::endl;
	out << "INDEX FILE : " << sb.getIndex() << std::endl;
	
	out << "ALLOW METHODS : ";
	std::vector<std::string> met = sb.getMethods();
	std::vector<std::string>::iterator	it = met.begin();
	for  (; it != met.end(); it++)
	{
		std::cout << *it;
		if ((it + 1) != met.end())
			std::cout << ", ";
	}
	out << "\n";
	
	std::map<int, std::string>	errorPage = sb.getErrorPage();
	std::map<int, std::string>::iterator	it2;
	for (it2 = errorPage.begin(); it2 != errorPage.end(); it2++)
	{
		out << "ERROR : " << (*it2).first << std::endl;
		out << "PAGE : " << (*it2).second;
		if (it2 != errorPage.end())
			out << "\n";
	}
	out << "\n";

	std::vector<Location>	loc = sb.getLocation();
	std::vector<Location>::iterator	it3 = loc.begin();
	for (; it3 != loc.end(); it3++)
	{
		std::cout << *it3;
		out << "\n";
	}
	return (out);
}

void	ServerBlock::printMethods()
{
	std::vector<std::string>::iterator it;
    for (it = methods.begin(); it != methods.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void	ServerBlock::addErrorPage(std::vector<std::string>::iterator &i)
{
	int err_value;
	std::string err = (*i);
	char *str;

	try
	{
		err_value = std::atoi(err.c_str());
		int j = 0;
		str = (char *)err.c_str();
		while (str[j])
		{
			if (str[j] < '0' || str[j] > '9')
				throw InvalidFileException("");
			j++;
		}
		i++;
		err = (*i);
		str = (char *)err.c_str();
		std::ifstream	in(err.c_str());
		if (!in)
		{
			std::cerr << "Error : no such file or directory : " << *i << std::endl;
			throw InvalidFileException("");
		}
		error_page[err_value] = *i;
	}
	catch(std::exception &e)
	{
		throw InvalidFileException("Error : Invalid argument in error page }");
	}
}

std::map<int, std::string>	ServerBlock::getErrorPage() const
{
	return this->error_page;
}
