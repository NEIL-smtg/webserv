/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:33:01 by suchua            #+#    #+#             */
/*   Updated: 2023/10/03 12:24:28 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

/*
	Parse the request string and split it into words
	and store them
*/
void	HttpRequest::parseHttpRequest(const str& req)
{
	std::istringstream	stream(req);
	str	line;

	if (std::getline(stream, line))
	{
		std::istringstream	lineStream(line);
		lineStream >> this->_methodStr >> this->_path;
		setMethod(this->_methodStr);
	}

	size_t	separatorPos;
	str		key;
	str		value;

	while (std::getline(stream, line))
	{
		separatorPos = line.find(':');
		if (separatorPos != std::string::npos)
		{
			key = line.substr(0, separatorPos);
			value = line.substr(separatorPos + 2);
			setHeader(key, value);
		}
		if (key == "Content-Type")
			str	boundary = value.substr(value.find_first_of('=') + 1);
		if (key == "Content-Length")
			break ;
	}
	while (std::getline(stream, line))
		setBody(line);
}

std::string	HttpRequest::generateHttpResponse(const str& req, const int clientSocket, const ServerBlock sb)
{
	parseHttpRequest(req);	
	RequestErrorHandling	err(*this, sb);
	if (!err.ErrorHandler())
		return (err.getErrResponse());
	
	// Location	target(err.getTargetBlock());
	str			response;

	switch (this->_methodEnum)
	{
		case GET:
			response = GetResponse(*this, sb).getResponse();
			break;
		case PUT:
			// response = PutResponse(*this, newSocket, target).getResponse();
			break;
		case POST:
			response = PostResponse(*this, clientSocket, sb).getResponse();
			break;
		case DELETE:
			response = DeleteResponse(*this, clientSocket, sb).getResponse();
			break;
		default:
			break;
	}
	return (response);
}

HttpRequest::HttpRequest()
{
	this->_httpStatusMsg[200] = "HTTP/1.1 200 OK\r\n";

	this->_httpStatusMsg[400] = "HTTP/1.1 400 Bad Request\r\n";
	this->_httpStatusMsg[404] = "HTTP/1.1 404 Not Found\r\n";
	this->_httpStatusMsg[405] = "HTTP/1.1 405 Not Allowed\r\n";
	this->_httpStatusMsg[413] = "HTTP/1.1 413 Request Entity Too Large\r\n";
	this->_httpStatusMsg[415] = "HTTP/1.1 415 Unsupported Media Type\r\n";
}

HttpRequest::~HttpRequest() {}

HttpRequest::HttpRequest(const HttpRequest& other) {*this = other;}

HttpRequest&	HttpRequest::operator=(const HttpRequest& other)
{
	if (this == & other)
		return (*this);
	this->_methodStr = other._methodStr;
	this->_methodEnum = other._methodEnum;
	this->_path = other._path;
	this->_body = other._body;
	this->_header = other._header;
	return (*this);
}

void	HttpRequest::setMethod(str method)
{
	if (method == "GET")			_methodEnum = GET;
	else if (method == "HEAD")		_methodEnum = HEAD;
	else if (method == "DELETE")	_methodEnum = DELETE;
	else if (method == "OPTIONS")	_methodEnum = OPTIONS;
	else if (method == "POST")		_methodEnum = POST;
	else if (method == "PUT")		_methodEnum = PUT;
	else							_methodEnum = TRACE;
}

void	HttpRequest::setPath(str path) {this->_path = path;}

void	HttpRequest::setBody(str line) {this->_body.push_back(line);}

void	HttpRequest::setHeader(str key, str value){this->_header[key] = value;}

HttpRequest::body	HttpRequest::getBody() const {return this->_body;}

std::string	HttpRequest::getPath() const {return this->_path;}

std::string	HttpRequest::getMethodStr() const {return this->_methodStr;}

httpMethod	HttpRequest::getMethodEnum() const {return this->_methodEnum;}

HttpRequest::header	HttpRequest::getHeader() const {return this->_header;}

std::map<int, std::string>	HttpRequest::getHttpStatusMsg() const {return this->_httpStatusMsg;}

