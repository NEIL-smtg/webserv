/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 22:28:55 by suchua            #+#    #+#             */
/*   Updated: 2023/09/11 17:32:59 by suchua           ###   ########.fr       */
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
	this->methods = other.methods;
	return (*this);
}

void	ServerBlock::addMethod(str method)
{
	this->methods.push_back(method);
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
	return (out);	
}
