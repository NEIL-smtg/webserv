/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:31:26 by suchua            #+#    #+#             */
/*   Updated: 2023/09/17 20:34:30 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PutResponse.hpp"

PutResponse::PutResponse(const HttpRequest& req, const int& clientSocket, const ServerBlock& sb)
:_req(req), _clientSocket(clientSocket), _sb(sb)
{
	if (!urlPathFound() || !methodAllowed())
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

	this->_response = generateErrorResponse(this->_sb, NOT_FOUND);
	return (false);
}

bool	PutResponse::methodAllowed()
{
	str							reqMethod;
	std::vector<str>			validMethods;
	std::vector<str>::iterator	it;

	reqMethod = this->_req.getMethodStr();
	validMethods = _location.getMethods();
	for (it = validMethods.begin(); it != validMethods.end(); it++)
	{
		if ((*it) == reqMethod)
			return (true);
	}
	this->_response = generateErrorResponse(this->_sb, NOT_ALLOWED);
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

std::string	generateErrorResponse(const ServerBlock& sb, const httpError errNum)
{
	std::string			errHtmlFilePath;
	std::string			line;
	std::stringstream	response;
	std::ifstream		errHtml;
	std::stringstream	body;

	errHtmlFilePath = sb.getErrorPage().find(errNum)->second;
	errHtml.open(errHtmlFilePath.c_str());
	
	while (std::getline(errHtml, line))
		body << line;

	if (errNum == NOT_FOUND) response << ERR_404_HEAD;
	else if (errNum == NOT_ALLOWED) response << ERR_405_HEAD;

	response << "Content-Type: " << errHtmlFilePath + "\r\n";
	response << "Content-Length: " << body.str().length() << "\r\n\r\n";
	response << body.str();
	return (response.str());
}
