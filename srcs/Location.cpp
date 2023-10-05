/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:43:02 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/10/05 18:12:29 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(std::vector<std::string>::iterator &i, std::vector<std::string> &token)
{
	conf	_conf = NONE;
	
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
			_conf = ROOT;
		else if (*i == "index")
			_conf = INDEX;
		else if (*i == "cgi_script")
			_conf = CGI_SCRIPT;
		else if (*i == "upload")
			_conf = UPLOAD;
		else if (*i == "error_page")
			_conf = ERROR_PAGE;
		else if (*i == "client_max_body_size")
			_conf = CLIENT_MAX_BODY_SIZE;
		else if (*i == "client_min_body_size")
			_conf = CLIENT_MIN_BODY_SIZE;
		else if (*i == "autoindex")
			_conf = AUTOINDEX;
		else if (*i == "allow_methods" || (_conf == LIMIT_EXCEPT && isMethod(*i)))
			_conf = LIMIT_EXCEPT;
		else if (*i == "limit_except" || (_conf == LIMIT_EXCEPT && isMethod(*i)))
			_conf = LIMIT_EXCEPT;
		if (_conf == NONE)
		{
			std::cerr << "Error : Invalid syntax : " + *i;
			throw InvalidFileException("");
		}
		++i;
		if (_conf == ROOT)
			setRoot(*i);
		else if (_conf == INDEX)
			setIndex(*i);
		else if (_conf == CGI_SCRIPT)
			setCgiScript(*i);
		if (_conf == UPLOAD)
			setUpload(*i);
		else if (_conf == ERROR_PAGE)
			addErrorPage(i);
		else if (_conf == CLIENT_MAX_BODY_SIZE)
			setBodySize(*i, _conf);
		else if (_conf == CLIENT_MIN_BODY_SIZE)
			setBodySize(*i, _conf);
		else if (_conf == AUTOINDEX)
			validateAutoIndex(*i);
		else if (_conf == ALLOW_METHOD)
			addLimitExcept(i, token);
		else if (_conf == LIMIT_EXCEPT)
			addLimitExcept(i, token);
		if (_conf != ALLOW_METHOD && _conf != LIMIT_EXCEPT)
			_conf = NONE;
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
	this->upload = other.upload;
	this->errorPage = other.errorPage;
	this->clientMaxBodySize = other.clientMaxBodySize;
	this->clientMinBodySize = other.clientMinBodySize;
	this->autoIndex = other.autoIndex;
	this->methods = other.methods;
	this->init = other.init;
	return (*this);
}

void	Location::setDirectory(std::string directory) {this->directory = directory;}

void	Location::setIndex(std::string index) {this->index = index;}

void	Location::setRoot(std::string root) {this->root = root;}

void	Location::setCgiScript(std::string cgiScript) {this->cgiScript = cgiScript;}

void	Location::setErrorPage(std::map<int, std::string> errorPage) {this->errorPage = errorPage;}

void	Location::setMethods(std::vector<std::string> methods) {this->methods = methods;}

void	Location::validateAutoIndex(std::string _auto)
{
	if (_auto == "on")			this->autoIndex = true;
	else if (_auto == "off")	this->autoIndex = false;
	else
	{
		std::cerr << "Error : Invalid syntax : autoindex" << _auto;
		throw InvalidFileException("");
	}
}

void	Location::setUpload(std::string uploadPath) {this->upload = uploadPath;}

void	Location::setAutoIndex(bool _auto) {this->autoIndex = _auto;}

void	Location::setInit() {this->init = true;}

void	Location::addErrorPage(std::vector<std::string>::iterator &i)
{
	int			err_value;
	std::string err = (*i);
	char		*str;

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

void	Location::setBodySize(std::string value, conf minMax)
{
	for (size_t i = 0; i < value.length(); i++)
	{
		if (value.at(i) < '0' || value.at(i) > '9')
		{
			std::cout << "Error : Invalid arguments at body size : " << value << std::endl;
			throw InvalidFileException("");
		}
	}
	if (minMax == CLIENT_MAX_BODY_SIZE)
		setClientMaxBodySize(std::atoi(value.c_str()));
	else
		setClientMinBodySize(std::atoi(value.c_str()));	
}

void	Location::setClientMaxBodySize(int clientSize) {this->clientMaxBodySize = clientSize;}

void	Location::setClientMinBodySize(int clientSize) {this->clientMinBodySize = clientSize;}

void	Location::addLimitExcept(std::vector<std::string>::iterator &i, std::vector<std::string> &token)
{
	while (i != token.end() && *i != "}" && isMethod(*i))
		this->methods.push_back(*i++);
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

std::string	Location::getDirectory() const {return this->directory;}

std::string	Location::getRoot() const {return this->root;}

std::string	Location::getIndex() const {return this->index;}

std::string	Location::getCgiScript() const {return this->cgiScript;}

std::vector<std::string>	Location::getMethods() const {return this->methods;}

int	Location::getClientMaxBodySize() const {return this->clientMaxBodySize;}

int	Location::getClientMinBodySize() const {return this->clientMinBodySize;}

std::map<int, std::string>	Location::getErrorPage() const {return this->errorPage;}

bool	Location::getAutoIndex() const {return this->autoIndex;}

std::string	Location::getUpload() const {return this->upload;}

bool	Location::isInit() const {return this->init;}

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
