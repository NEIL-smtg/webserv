/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:43:02 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/09/26 18:23:53 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

static bool isLocoHead(std::string locohead)
{
	const std::string head[12] = {"root", "index", "include", "cgi_script", "upload", "error_page", "client_max_body_size", "autoindex", "return", "limit_except", "allow", "deny"};

	for (size_t i = 0; i < 12; i++)
	{
		if (locohead == head[i])
			return (true);
	}
	return (false);
}

Location::Location(std::vector<std::string>::iterator &i, std::vector<std::string> &token)
{
	locconf		_conf = LC_NONE;
	
	this->clientMaxBodySize = DEFAULT_CLIENT_SIZE;
	setDirectory(*i);
	if (*(++i) != "{")
		throw InvalidFileException("Missing {");
	i++;
	for (; i != token.end(); i++)
	{
		if (*i == "}")
			break ;
		if (*i == "root")
			_conf = LC_ROOT;
		else if (*i == "index")
			_conf = LC_INDEX;
		// else if (*i == "include")
		// 	_conf = LC_INCLUDE;
		else if (*i == "cgi_script")
			_conf = LC_CGI_SCRIPT;
		// else if (*i == "upload")
		// 	_conf = LC_UPLOAD;
		else if (*i == "error_page")
			_conf = LC_ERROR_PAGE;
		else if (*i == "client_max_body_size")
			_conf = LC_CLIENT_MAX_BODY_SIZE;
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
		if (_conf == LC_ROOT)
			setRoot(*i);
		if (_conf == LC_INDEX)
			setIndex(*i);
		// if (_conf == LC_INCLUDE)
		// 	setInclude(*i);
		if (_conf == LC_CGI_SCRIPT)
			setCgiScript(*i);
		// if (_conf == LC_UPLOAD)
		// 	setUpload(*i);
		if (_conf == LC_ERROR_PAGE)
			addErrorPage(i);
		if (_conf == LC_CLIENT_MAX_BODY_SIZE)
			getSize(*i);
		// if (_conf == LC_AUTOINDEX)
		// 	setAutoIndex(*i);
		// if (_conf == LC_RETURN)
		// 	setReturn(*i);
		if (_conf == LC_ALLOW)
			addLimitExcept(i, token);
		if (_conf == LC_LIMIT_EXCEPT)
			addLimitExcept(i, token);
		// if (_conf == LC_ALLOW)
		// 	setAllow(*i);
		// if (_conf == LC_DENY)
		// 	setDeny(*i);
	}
	if (i == token.end())
		throw InvalidFileException("Error : Missing }");
}

Location::Location(){}

Location::~Location(){}

Location::Location(const Location& other){*this = other;}

Location& Location::operator=(const Location& other)
{
	if (this == &other)
		return (*this);
	this->directory = other.directory;
	this->index = other.index;
	this->methods = other.methods;
	this->root = other.root;
	this->cgi_script = other.cgi_script;
	this->error_page = other.error_page;
	this->clientMaxBodySize = other.clientMaxBodySize;
	return (*this);
}

void	Location::setDirectory(std::string directory)
{
	this->directory = directory;
}

void	Location::setIndex(std::string index)
{
	this->index = index;
}

void	Location::setRoot(std::string root)
{
	this->root = root;
}

void	Location::setCgiScript(std::string cgi_script)
{
	this->cgi_script = cgi_script;
}

void	Location::addErrorPage(std::vector<std::string>::iterator &i)
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

void	Location::getSize(std::string size)
{
	int client_max_body_size;
	char *str;

	try
	{
		client_max_body_size = std::atoi(size.c_str());
		int i = 0;
		str = (char *)size.c_str();
		while (str[i])
		{
			if (str[i] < '0' || str[i] > '9')
				throw InvalidFileException("");
			i++;
		}
		setClientMaxBodySize(client_max_body_size);
	}
	catch(std::exception &e)
	{
		throw InvalidFileException("Error : Invalid argument in client_max_body_size }");
	}
	
}

void	Location::setClientMaxBodySize(int clientMaxBodySize)
{
	this->clientMaxBodySize = clientMaxBodySize;
}

void	Location::addLimitExcept(std::vector<std::string>::iterator &i, std::vector<std::string> &token)
{
	while (i != token.end() && *i != "}" && isMethod(*i))
		this->methods.push_back(*i++);
	if (i == token.end() || *i == "}")
	{
		--i;
		return ;
	}
	if (!isLocoHead(*i))
	{
		std::cerr << "Error : Invalid syntax : " + *i;
		throw InvalidFileException("");
	}
	--i;
}

std::string	Location::getDirectory() const
{
	return this->directory;
}

std::string	Location::getRoot() const
{
	return this->root;
}

std::string	Location::getIndex() const
{
	return this->index;
}

std::string	Location::getCgiScript() const
{
	return this->cgi_script;
}

std::vector<std::string>	Location::getMethods() const
{
	return this->methods;
}

int	Location::getClientMaxBodySize() const
{
	return this->clientMaxBodySize;
}

std::map<int, std::string>	Location::getErrorPage() const
{
	return this->error_page;
}

void	Location::printErrorPage()
{
    for (std::map<int, std::string>::iterator it = error_page.begin(); it != error_page.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
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
	out << "Location : " << loc.getDirectory() << std::endl;
	if (!loc.getRoot().empty())
		out << "ROOT : " << loc.getRoot() << std::endl;
	if (!loc.getIndex().empty())
		out << "INDEX : " << loc.getIndex() << std::endl;
	if (!loc.getCgiScript().empty())
		out << "CGI_SCRIPT : " << loc.getCgiScript() << std::endl;

	std::vector<std::string>	met = loc.getMethods();
	std::vector<std::string>::iterator it;

    for (it = met.begin(); it != met.end(); ++it)
	{
		out << *it;
        if ((it + 1) != met.end())
			out << ", ";
    }
	out << "\n";
	std::map<int, std::string>	errorPage = loc.getErrorPage();
	std::map<int, std::string>::iterator	it2;
	for (it2 = errorPage.begin(); it2 != errorPage.end(); it2++)
	{
		out << "ERROR : " << (*it2).first << std::endl;
		out << "PAGE : " << (*it2).second;
		if (it2 != errorPage.end())
			out << "\n";
	}
	return (out);
}
