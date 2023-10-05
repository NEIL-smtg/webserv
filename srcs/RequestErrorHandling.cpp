/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestErrorHandling.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:55:41 by suchua            #+#    #+#             */
/*   Updated: 2023/10/06 04:13:34 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestErrorHandling.hpp"

bool	RequestErrorHandling::ErrorHandler()
{
	if (this->_req.getMethodEnum() == GET)
		return (urlPathFound() && allowMethod());
	return (urlPathFound() && allowMethod() && validContent());
}

bool	RequestErrorHandling::urlPathFound()
{
	std::vector<Location>			loc;
	std::vector<Location>::iterator	it;
	std::string						urlDir;

	urlDir = this->_req.getPath();
	if (urlDir == "/")
	{
		setTargetBlock();
		return true;
	}
	this->_tokennizePath = this->_req.getTokennizePath();
	urlDir = this->_tokennizePath.front();
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
	return (accessDir());
}

bool	RequestErrorHandling::accessDir()
{
	std::ifstream	infile;
	std::string		rootToUse;
	struct stat		fileStat;

	rootToUse = this->_target.getRoot();
	std::cout << "root to use = " << rootToUse << std::endl;
	if (!stat(rootToUse.c_str(), &fileStat))
	{
		if (S_ISREG(fileStat.st_mode) || S_ISDIR(fileStat.st_mode))
		{
			std::cout << "File exists\n";
			return (true);
		}
	}
	std::cout << "File not found\n";
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

bool	RequestErrorHandling::validBoundary()
{
	std::string					boundary;
	size_t						maxSize;
	std::vector<std::string>	formBoundary;

	boundary = this->_req.getBoundary();
	maxSize = this->_req.getDataKeys().size();
	formBoundary = this->_req.getFormBoundary();
	
	//	+1 for ending
	if ((maxSize + 1) != formBoundary.size())
	{
		generateErrResponse(400);
		return (false);
	}
	for (size_t i = 0; i < maxSize + 1; i++)
	{
		std::string	toTest = "--" + boundary;

		if (i + 1 == maxSize + 1)
			toTest += "--";
		if (boundary != toTest)
		{
			generateErrResponse(400);
			return (false);
		}
	}
	return (true);
}

bool	isValidContentType(std::string ct)
{
	const std::string	contentType[4] = {"application/x-www-form-urlencoded", "multipart/form-data", "application/json"};

	for (size_t i = 0; i < 4; i++)
	{
		if (ct == contentType[i])
			return (true);
	}
	return (false);
}

bool	RequestErrorHandling::validContent()
{
	std::map<std::string, std::string>				head;
	std::map<std::string, std::string>::iterator	key;
	std::string	value;
	std::string	contentType;
	size_t		len = 0;

	head = this->_req.getHeader();
	key = head.find("Content-Type");
	if (key != head.end())
		value = head.find("Content-Type")->second;
	if (len == std::string::npos || !isValidContentType(contentType))
	{
		generateErrResponse(415);
		return (false);
	}
	return (validContentLen(head.find("Content-Length")->second) && validBoundary());
}

void	RequestErrorHandling::generateErrResponse(int statusCode)
{
	std::map<int, std::string>	errPage;
	std::string					errHtmlFilePath;
	std::string					statusMsg;
	std::stringstream			res;
	std::ifstream				htmlFile;
	std::string					htmlBody;
	std::string					line;

	errPage = this->_target.getErrorPage();
	if (errPage.find(statusCode) == errPage.end())
		errHtmlFilePath = errPage.find(404)->second;
	else
		errHtmlFilePath = errPage.find(statusCode)->second;
	htmlFile.open(errHtmlFilePath.c_str());
	while (std::getline(htmlFile, line))
		htmlBody += line;

	res << this->_req.getHttpStatusMsg().find(statusCode)->second;
	res << "Content-Type: " << "text/html" << "\r\n";
	res << "Content-Length: " << htmlBody.length() << "\r\n\r\n";
	res << htmlBody;
	this->_errResponse = res.str();
	std::cout << this->_errResponse;
}

void	RequestErrorHandling::setTargetBlock()
{
	std::string	rootToUse  = getRootToUse();

	if (this->_currLoc.isInit())
	{
		_target = this->_currLoc;
		_target.setRoot(rootToUse);
		if (_target.getErrorPage().empty())
			_target.setErrorPage(this->_sb.getErrorPage());
		return ;
	}
	_target.setInit();
	_target.setDirectory(this->_req.getPath());	
	_target.setRoot(rootToUse);
	_target.setClientMinBodySize(this->_sb.getClientMinBodySize());
	_target.setClientMaxBodySize(this->_sb.getClientMaxBodySize());
	_target.setIndex(this->_sb.getIndex());
	_target.setErrorPage(this->_sb.getErrorPage());
	_target.setMethods(this->_sb.getMethods());
	_target.setCgiScript(this->_sb.getCgiScript());
	_target.setAutoIndex(this->_sb.getAutoIndex());
}

std::string	RequestErrorHandling::getRootToUse()
{
	std::string		rootToUse;
	std::vector<std::string>::iterator	it;

	if (_currLoc.isInit() && !_currLoc.getRoot().empty() && this->_tokennizePath.size() <= 1)
		return (_currLoc.getRoot());
	if (this->_tokennizePath.size() == 0)
		return (this->_sb.getRoot() + this->_req.getPath());
	rootToUse = this->_sb.getRoot();	
	for (it = this->_tokennizePath.begin(); it != this->_tokennizePath.end(); it++)
		rootToUse += *it;
	return (rootToUse);
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
