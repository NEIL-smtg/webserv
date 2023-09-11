/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:43:02 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/09/12 03:57:45 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(std::vector<std::string>::iterator &i, std::vector<std::string> &token)
{
	locconf		_conf = LC_NONE;
	
	setName(*i);
	if (*(++i) != "{")
		throw InvalidFileException("Missing {");
	i++;
	for (; i != token.end(); i++)
	{
		if (*i == "}")
			break ;
		// if (*i == "root")
		// 	_conf = LC_ROOT;
		else if (*i == "index")
			_conf = LC_INDEX;
		// else if (*i == "include")
		// 	_conf = LC_INCLUDE;
		// else if (*i == "cgi_script")
		// 	_conf = LC_CGI_SCRIPT;
		// else if (*i == "upload")
		// 	_conf = LC_UPLOAD;
		// else if (*i == "error_page")
		// 	_conf = LC_ERROR_PAGE;
		// else if (*i == "client_max_body_size")
		// 	_conf = LC_CLIENT_MAX_BODY_SIZE;
		// else if (*i == "autoindex")
		// 	_conf = LC_AUTOINDEX;
		// else if (*i == "return")
		// 	_conf = LC_RETURN;
		else if (*i == "allow_methods" || (_conf == LC_ALLOW && isMethod(*i)))
			_conf = LC_LIMIT_EXCEPT;
		else if (*i == "limit_except" || (_conf == LC_LIMIT_EXCEPT && isMethod(*i)))
			_conf = LC_LIMIT_EXCEPT;
		// else if (*i == "allow")
		// 	_conf = LC_ALLOW;
		// else if (*i == "deny")
		// 	_conf = LC_DENY;
		if (_conf == LC_NONE)
		{
			std::cerr << "Error : Invalid syntax : " + *i;
			throw InvalidFileException("");
		}
		++i;
		// if (_conf == LC_ROOT)
		// 	setRoot(*i);
		if (_conf == LC_INDEX)
			setIndex(*i);
		// if (_conf == LC_INCLUDE)
		// 	setInclude(*i);
		// if (_conf == LC_CGI_SCRIPT)
		// 	setCgiScript(*i);
		// if (_conf == LC_UPLOAD)
		// 	setUpload(*i);
		// if (_conf == LC_ERROR_PAGE)
		// 	setErrorPage(*i);
		// if (_conf == LC_CLIENT_MAX_BODY_SIZE)
		// 	setClientMaxBodySize(*i);
		// if (_conf == LC_AUTOINDEX)
		// 	setAutoIndex(*i);
		// if (_conf == LC_RETURN)
		// 	setReturn(*i);
		if (_conf == LC_ALLOW)
			addLimitExcept(*i);
		if (_conf == LC_LIMIT_EXCEPT)
			addLimitExcept(*i);
		// if (_conf == LC_ALLOW)
		// 	setAllow(*i);
		// if (_conf == LC_DENY)
		// 	setDeny(*i);
	}
	if (i == token.end())
		throw InvalidFileException("Error : Missing }");
}

Location::~Location(){}

Location::Location(const Location& other){*this = other;}

Location& Location::operator=(const Location& other)
{
	if (this == &other)
		return (*this);
	this->name = other.name;
	this->index = other.index;
	this->methods = other.methods;
	return (*this);
}

void	Location::setIndex(std::string index)
{
	this->index = index;
}

void	Location::setName(std::string name)
{
	this->name = name;
}

void	Location::addLimitExcept(std::string method)
{
	this->methods.push_back(method);
}

std::string	Location::getIndex() const
{
	return this->index;
}

std::string	Location::getName() const
{
	return this->name;
}

std::vector<std::string>	Location::getMethods() const
{
	return this->methods;
}

void	Location::printLimitExcept()
{
	std::vector<std::string>::iterator it;
    for (it = methods.begin(); it != methods.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

std::ostream&	operator<<(std::ostream& out, Location& loc)
{
	out << "Location : " << loc.getName() << std::endl;
	out << "INDEX : " << loc.getIndex() << std::endl;

	std::vector<std::string>	met = loc.getMethods();
	std::vector<std::string>::iterator it;

    for (it = met.begin(); it != met.end(); ++it)
	{
        if ((it + 1) != met.end())
			out << ", ";
    }
	out << "\n";
	return (out);
}
