/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestErrorHandling.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:55:41 by suchua            #+#    #+#             */
/*   Updated: 2023/09/27 18:35:26 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestErrorHandling.hpp"

RequestErrorHandling::RequestErrorHandling(const HttpRequest& req, const ServerBlock& sb):
_req(req), _sb(sb), _httpStatusMsg(req.getHttpStatusMsg()) {this->_currLoc = NULL;}

std::string	RequestErrorHandling::getErrResponse() const {return this->_errResponse;}

RequestErrorHandling::~RequestErrorHandling() {}

RequestErrorHandling::RequestErrorHandling(const RequestErrorHandling& other)
:_req(other._req) , _sb(other._sb), _httpStatusMsg(other._req.getHttpStatusMsg()) {*this = other;}

RequestErrorHandling&	RequestErrorHandling::operator=(const RequestErrorHandling& other)
{
	if (this == &other)
		return (*this);
	this->_errResponse = other._errResponse;
	this->_currLoc = other._currLoc;
	return (*this);
}

bool	RequestErrorHandling::urlPathFound()
{
	std::vector<Location>			loc;
	std::vector<Location>::iterator	it;
	std::string						urlDir;

	urlDir = this->_req.getPath();
	if (urlDir == "/")
		return true;
	
	loc = this->_sb.getLocation();
	for (it = loc.begin(); it != loc.end(); it++)
	{
		if (it->getDirectory() == this->_req.getPath())
		{
			this->_currLoc = &(*it);
			return (true);
		}
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
	validMethod = this->_currLoc->getMethods();
	if (validMethod.size() == 0)
		validMethod = this->_sb.getMethods();
	
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

	if (this->_currLoc)
	{
		min = this->_currLoc->getClientMinBodySize();
		max = this->_currLoc->getClientMaxBodySize();
	}
	else
	{
		min = this->_sb.getClientMinBodySize();
		max = this->_sb.getClientMaxBodySize();
	}
	return (reqLen <= max && reqLen >= max);
}

bool	RequestErrorHandling::validContent()
{
	std::map<std::string, std::string>				head;
	std::map<std::string, std::string>::iterator	key;
	std::string	value;
	std::string	boundary;
	std::string	multi;
	size_t		len;
	int			contentLen;

	head = this->_req.getHeader();
	key = head.find("Content-Type");
	if (key != head.end())
	{
		value = head.find("Content-Type")->second;
		len = value.find_first_of(';');
		if (len != std::string::npos)
		{
			multi = value.substr(0, len);
			len = value.find_first_of('-');
			boundary = value.substr(len);
		}
	}
	if (len == std::string::npos || multi != "multipart/form-data" || !validContentLen(head.find("Content-Length")->second))
	{
		generateErrResponse(415);
		return (false);
	}
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

	errPage = this->_currLoc->getErrorPage();
	if (errPage.find(statusCode) == errPage.end())
	{
		errPage = this->_sb.getErrorPage();
		if (errPage.find(statusCode) == errPage.end())
			errHtmlFilePath = this->_sb.getErrorPage().find(404)->second;
	}

	htmlFile.open(errHtmlFilePath.c_str());
	while (std::getline(htmlFile, line))
		htmlBody << line;
	
	res << this->_httpStatusMsg.find(statusCode)->second;
	res << "Content-Type: " << errHtmlFilePath << "\r\n";
	res << "Content-Length: " << htmlBody.str().length() << "\r\n\r\n";
	res << htmlBody.str();

	this->_errResponse = res.str();
}
