/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:43:02 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/11/07 16:39:57 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

static bool isLocoHead(std::string locohead)
{
	const std::string head[13] = {"root", "index", "include", "cgi_script", "upload", "error_page", "client_max_body_size", "client_min_body_size", "autoindex", "return", "limit_except", "allow", "deny"};

	for (size_t i = 0; i < 13; i++)
	{
		if (locohead == head[i])
			return (true);
	}
	return (false);
}

Location::Location(std::vector<std::string>::iterator &i, std::vector<std::string> &token)
{
	locconf		_conf = LC_NONE;
	
	this->clientMaxBodySize = DEFAULT_CLIENT_MAX_SIZE;
	this->clientMinBodySize = DEFAULT_CLIENT_MIN_SIZE;
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
		else if (*i == "client_min_body_size")
			_conf = LC_CLIENT_MIN_BODY_SIZE;
		// else if (*i == "autoindex")
		// 	_conf = LC_AUTOINDEX;
		// else if (*i == "return")
		// 	_conf = LC_RETURN;
		else if (*i == "allow_methods" || (_conf == LC_LIMIT_EXCEPT && isMethod(*i)))
			_conf = LC_LIMIT_EXCEPT;
		else if (*i == "limit_except" || (_conf == LC_LIMIT_EXCEPT && isMethod(*i)))
			_conf = LC_LIMIT_EXCEPT;
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
			addCgiScript(i);
		// if (_conf == LC_UPLOAD)
		// 	setUpload(*i);
		if (_conf == LC_ERROR_PAGE)
			addErrorPage(i);
		if (_conf == LC_CLIENT_MAX_BODY_SIZE)
			getSize(*i, _conf);
		if (_conf == LC_CLIENT_MIN_BODY_SIZE)
			getSize(*i, _conf);
		// if (_conf == LC_AUTOINDEX)
		// 	setAutoIndex(*i);
		// if (_conf == LC_RETURN)
		// 	setReturn(*i);
		if (_conf == LC_LIMIT_EXCEPT)
			addLimitExcept(i, token);
		if (_conf == LC_LIMIT_EXCEPT)
			addLimitExcept(i, token);
	}
	if (i == token.end())
		throw InvalidFileException("Error : Missing }");
	setInit();
}

Location::Location(){this->init = false;}

Location::~Location(){}

Location::Location(const Location& other){*this = other;}

Location& Location::operator=(const Location& other)
{
	if (this == &other)
		return (*this);
	this->directory = other.directory;
	this->root = other.root;
	this->index = other.index;
	this->cgiScript = other.cgiScript;
	this->errorPage = other.errorPage;
	this->clientMaxBodySize = other.clientMaxBodySize;
	this->clientMinBodySize = other.clientMinBodySize;
	this->methods = other.methods;
	this->autoIndex = other.autoIndex;
	this->init = other.init;
	return (*this);
}

void	Location::setDirectory(std::string directory) {this->directory = directory;}

void	Location::setIndex(std::string index) {this->index = index;}

void	Location::setRoot(std::string root) {this->root = root;}

void	Location::addCgiScript(std::vector<std::string>::iterator &i)
{
	std::string file = (*i);
	char *str;

	try
	{
		str = (char *)file.c_str();
		if (str[0] != '.')
			throw InvalidFileException("");
		i++;
		cgiScript[file] = *i;
	}
	catch(std::exception &e)
	{
		throw InvalidFileException("Error : Invalid argument in error page }");
	}
}

void	Location::setCgiScript(std::map<std::string, std::string> cgiScript) {this->cgiScript = cgiScript;}

void	Location::setErrorPage(std::map<int, std::string> errorPage) {this->errorPage = errorPage;}

void	Location::setMethods(std::vector<std::string> methods) {this->methods = methods;}

void	Location::setAutoIndex(bool _auto) {this->autoIndex = _auto;}

void	Location::setInit() {this->init = true;}

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
		errorPage[err_value] = *i;
	}
	catch(std::exception &e)
	{
		throw InvalidFileException("Error : Invalid argument in error page }");
	}
}

void	Location::getSize(std::string size, locconf minMax)
{
	int clientSize;
	char *str;

	try
	{
		clientSize = std::atoi(size.c_str());
		int i = 0;
		str = (char *)size.c_str();
		while (str[i])
		{
			if (str[i] < '0' || str[i] > '9')
				throw InvalidFileException("");
			i++;
		}
        if (minMax == LC_CLIENT_MAX_BODY_SIZE)
		    setClientMaxBodySize(clientSize);
        else if (minMax == LC_CLIENT_MIN_BODY_SIZE)
		    setClientMinBodySize(clientSize);
	}
	catch(std::exception &e)
	{
		throw InvalidFileException("Error : Invalid argument in client_max_body_size }");
	}
	
}

void	Location::setClientMaxBodySize(int clientSize){this->clientMaxBodySize = clientSize;}

void	Location::setClientMinBodySize(int clientSize){this->clientMinBodySize = clientSize;}

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

std::string	Location::getDirectory() const {return this->directory;}

std::string	Location::getRoot() const {return this->root;}

std::string	Location::getIndex() const {return this->index;}

std::map<std::string, std::string>	Location::getCgiScript() const {return this->cgiScript;}

std::vector<std::string>	Location::getMethods() const {return this->methods;}

int	Location::getClientMaxBodySize() const {return this->clientMaxBodySize;}

int	Location::getClientMinBodySize() const {return this->clientMinBodySize;}

std::map<int, std::string>	Location::getErrorPage() const {return this->errorPage;}

bool	Location::getAutoIndex() const {return this->autoIndex;}

bool	Location::isInit() const {return this->init;}

void	Location::printErrorPage()
{
    for (std::map<int, std::string>::iterator it = errorPage.begin(); it != errorPage.end(); ++it) {
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
	// if (!loc.getCgiScript().empty())
	// 	out << "CGI_SCRIPT : " << loc.getCgiScript() << std::endl;

	std::map<std::string, std::string>	cgi = loc.getCgiScript();
	std::map<std::string, std::string>::iterator	it3;
	for (it3 = cgi.begin(); it3 != cgi.end(); it3++)
	{
		out << "file : " << (*it3).first << std::endl;
		out << "script : " << (*it3).second;
		if (it3 != cgi.end())
			out << "\n";
	}
	out << "\n";

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
