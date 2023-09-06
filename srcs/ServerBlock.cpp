/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 22:28:55 by suchua            #+#    #+#             */
/*   Updated: 2023/09/07 00:20:45 by suchua           ###   ########.fr       */
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
