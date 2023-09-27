/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:31:26 by suchua            #+#    #+#             */
/*   Updated: 2023/09/26 19:03:18 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PutResponse.hpp"

PutResponse::PutResponse(const HttpRequest& req, const int& clientSocket, const ServerBlock& sb)
:_req(req), _clientSocket(clientSocket), _sb(sb)
{
	if (!urlPathFound() || !methodAllowed() || !validContentType())
		return ;
}

bool	PutResponse::urlPathFound()
{
	if (this->_req.getPath() == "/")
		return (true);

	std::vector<Location>			loc;
	std::vector<Location>::iterator	it;
	
	loc = this->_sb.getLocation();
	for (it = loc.begin(); it != loc.end(); ++it)
	{
		if (it->getDirectory() == this->_req.getPath())
		{
			this->_location = (*it);
			return (true);
		}
	}
	this->_response = generateErrorResponse(404);
	return (false);
}

bool	PutResponse::methodAllowed()
{
	str							reqMethod;
	std::vector<str>			validMethods;
	std::vector<str>::iterator	it;

	reqMethod = this->_req.getMethodStr();
	validMethods = this->_location.getMethods();
	for (it = validMethods.begin(); it != validMethods.end(); it++)
	{
		if ((*it) == reqMethod)
			return (true);
	}
	this->_response = generateErrorResponse(405);
	return (false);
}

/*
value = multipart/form-data; boundary=--------------------------287650079002050513765839
Content-Length: 287
*/
bool	PutResponse::validContentType()
{
	str		value;
	str		contentType;
	size_t	len;
	std::map<str, str>	head;
	std::map<str, str>::iterator	key;

	head = this->_req.getHeader();
	key = head.find("Content-Type");
	if (key != head.end()) 
	{
		value = head.find("Content-Type")->second;
		len = value.find_first_of(';');
		if (len != str::npos)
			contentType = value.substr(0, len);
		if (len != str::npos && contentType == "multipart/form-data")
		{
			str boundary = value.substr(len + 2 + str("boundary=").length());\
			// int contentLen = std::atoi(head.find("Content-Length")->second.c_str());
			
			// std::cout << head.find("Content-Disposition")->second << "\n\n\n";
			return (true);
		}
	}
	this->_response = generateErrorResponse(415);
	return (false);
}

PutResponse::~PutResponse() {}

PutResponse::PutResponse(const PutResponse& other)
:_req(other._req), _clientSocket(other._clientSocket), _sb(other._sb) {*this = other;}

PutResponse&	PutResponse::operator=(const PutResponse& other)
{
	if (this == &other)
		return (*this);
	this->_response = other._response;
	return (*this);
}

std::string	PutResponse::getResponse() const {return this->_response;}

std::string	PutResponse::generateErrorResponse(const int errCode)
{
	std::map<int, std::string>	errPage;
	std::string					errHtmlFilePath;
	std::string					line;
	std::stringstream			response;
	std::ifstream				errHtml;
	std::stringstream			body;

	if (errCode != 404 && this->_location.getErrorPage().size() > 0)
		errPage = this->_location.getErrorPage();
	else
		errPage = this->_sb.getErrorPage();

	if (errPage.find(errCode) != errPage.end())
		errHtmlFilePath = errPage.find(errCode)->second;
	else
		errHtmlFilePath = errPage.find(404)->second;
	
	errHtml.open(errHtmlFilePath.c_str());
	
	while (std::getline(errHtml, line))
		body << line;

	response << this->_req.getHttpErrorMsg().find(errCode)->second;
	response << "Content-Type: " << errHtmlFilePath + "\r\n";
	response << "Content-Length: " << body.str().length() << "\r\n\r\n";
	response << body.str();
	return (response.str());
}
