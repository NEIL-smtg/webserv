/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestErrorHandling.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:55:41 by suchua            #+#    #+#             */
/*   Updated: 2023/10/03 12:24:50 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestErrorHandling.hpp"

bool	RequestErrorHandling::ErrorHandler()
{
	if (this->_req.getMethodEnum() == GET)
		return (urlPathFound() && allowMethod());
	return (urlPathFound() && allowMethod() && validContent());
}

void	RequestErrorHandling::tokennizeReqUrlPath()
{
	const std::string	sep("/");
	size_t	start = 0, end = 0;
	size_t	npos = std::string::npos;
	std::string	path = this->_req.getPath();

	while (start != npos && end != npos)
	{
		end = path.find_first_of(sep, start);
		if (end == npos && path[start] != 0)
		{
			this->_reqPath.push_back(sep + &path[start]);
			break ;
		}
		if (start != npos && end - start != 0 && path[start] != 0)
			this->_reqPath.push_back(sep + path.substr(start, end - start));
		start = path.find_first_not_of(sep, end);
	}
}

bool	RequestErrorHandling::urlPathFound()
{
	std::vector<Location>			loc;
	std::vector<Location>::iterator	it;
	std::string						urlDir;
	std::ifstream					infile;
	std::string						rootToUse;

	urlDir = this->_req.getPath();
	if (urlDir == "/")
	{
		setTargetBlock();
		return true;
	}
	tokennizeReqUrlPath();
	urlDir = this->_reqPath.front();
	loc = this->_sb.getLocation();
	for (it = loc.begin(); it != loc.end(); it++)
	{
		if (it->getDirectory() == urlDir)
		{
			this->_currLoc = *it;
			break ;
		}
	}
	setTargetBlock();
	rootToUse = this->_target.getRoot() + this->_req.getPath();
	infile.open(rootToUse.c_str());
	if (infile.is_open())
	{
		infile.close();
		_target.setRoot(rootToUse);
		return (true);
	}
	generateErrResponse(404);
	return (false);
}

bool	RequestErrorHandling::allowMethod()
{
	std::string							reqMethod;
	std::vector<std::string>			validMethod;
	std::vector<std::string>::iterator	it;

	reqMethod = this->_req.getMethodStr();
	validMethod = this->_target.getMethods();
	for (it = validMethod.begin(); it != validMethod.end(); it++)
	{
		if ((*it) == reqMethod)
			return (true); 
	}
	generateErrResponse(405);
	return (false);
}

bool	RequestErrorHandling::validContentLen(std::string contentLen)
{
	const int	reqLen = std::atoi(contentLen.c_str());
	int			min;
	int			max;

	min = this->_target.getClientMinBodySize();
	max = this->_target.getClientMaxBodySize();
	if (reqLen > max)
	{
		generateErrResponse(413);
		return (false);
	}
	if (reqLen < min)
	{
		generateErrResponse(400);
		return (false);
	}
	return (true);
}

bool	RequestErrorHandling::validBoundary(std::string boundary)
{
	(void) boundary;
	// return (true);
	// std::vector<std::string>			body;
	// std::vector<std::string>::iterator	it;

	// body = this->_req.getBody();
	// for (it = body.begin(); it != body.end(); it++)
	// {
	// 	// if ((*it).at(0) == '-' && (*it) != boundary)
			
	// }
	return true;
}

bool	RequestErrorHandling::validContent()
{
	std::map<std::string, std::string>				head;
	std::map<std::string, std::string>::iterator	key;
	std::string	value;
	std::string	boundary;
	std::string	multi;
	size_t		len = 0;

	head = this->_req.getHeader();
	key = head.find("Content-Type");
	if (key != head.end())
	{
		value = head.find("Content-Type")->second;
		len = value.find_first_of(';');
		if (len != std::string::npos)
		{
			multi = value.substr(0, len);
			boundary = value.substr(len);
			len = boundary.find_first_of('-');
			boundary = boundary.substr(len);
		}
	}
	if (len == std::string::npos || multi != "multipart/form-data")
	{
		generateErrResponse(415);
		return (false);
	}
	if (!validContentLen(head.find("Content-Length")->second))
		return (false);
	if (!validBoundary(boundary))
		return (false);
	return (true);
}

void	RequestErrorHandling::generateErrResponse(int statusCode)
{
	std::map<int, std::string>	errPage;
	std::string					errHtmlFilePath;
	std::string					statusMsg;
	std::stringstream			res;
	std::ifstream				htmlFile;
	std::stringstream			htmlBody;
	std::string					line;

	errPage = this->_target.getErrorPage();
	if (errPage.find(statusCode) == errPage.end())
		errHtmlFilePath = this->_sb.getErrorPage().find(404)->second;

	htmlFile.open(errHtmlFilePath.c_str());
	while (std::getline(htmlFile, line))
		htmlBody << line;

	res << this->_req.getHttpStatusMsg().find(statusCode)->second;
	res << "Content-Type: " << errHtmlFilePath << "\r\n";
	res << "Content-Length: " << htmlBody.str().length() << "\r\n\r\n";
	res << htmlBody.str();
	this->_errResponse = res.str();
}

void	RequestErrorHandling::setTargetBlock()
{
	if (this->_currLoc.isInit())
	{
		_target = this->_currLoc;
		if (_target.getRoot().empty())
			_target.setRoot(this->_sb.getRoot());
		if (_target.getErrorPage().empty())
			_target.setErrorPage(this->_sb.getErrorPage());
		return ;
	}
	_target.setInit();
	_target.setDirectory(this->_req.getPath());
	_target.setRoot(this->_sb.getRoot());
	_target.setClientMinBodySize(this->_sb.getClientMinBodySize());
	_target.setClientMaxBodySize(this->_sb.getClientMaxBodySize());
	_target.setIndex(this->_sb.getIndex());
	_target.setErrorPage(this->_sb.getErrorPage());
	_target.setMethods(this->_sb.getMethods());
	_target.setCgiScript(this->_sb.getCgiScript());
	_target.setAutoIndex(this->_sb.getAutoIndex());
}

Location	RequestErrorHandling::getTargetBlock() const {return this->_target;}

std::string	RequestErrorHandling::getErrResponse() const {return this->_errResponse;}

/*CONSTRUCTORSS*/
RequestErrorHandling::RequestErrorHandling(const HttpRequest& req, const ServerBlock& sb):
_req(req), _sb(sb) {}

RequestErrorHandling::~RequestErrorHandling() {}

RequestErrorHandling::RequestErrorHandling(const RequestErrorHandling& other)
:_req(other._req) , _sb(other._sb) {*this = other;}

RequestErrorHandling&	RequestErrorHandling::operator=(const RequestErrorHandling& other)
{
	if (this == &other)
		return (*this);
	this->_errResponse = other._errResponse;
	this->_currLoc = other._currLoc;
	return (*this);
}
