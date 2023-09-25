/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:33:01 by suchua            #+#    #+#             */
/*   Updated: 2023/09/26 02:38:26 by suchua           ###   ########.fr       */
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

	this->_body = "";
	while (std::getline(stream, line))
	{
		this->_body += line + str("\n");
		if (line.find_first_of(":") != str::npos)
		{
			size_t	start;
			size_t	end;

			start = line.find_first_of("\"") + 1;
			end = line.find_first_of(start);
			this->_body += static_cast<int>(end - start) + 1;
		}
	}
}

std::string	HttpRequest::generateHttpResponse(const str& req, const int newSocket, const ServerBlock sb)
{
	parseHttpRequest(req);

	str	response;

	switch (this->_methodEnum)
	{
		case GET:
			return GetResponse(*this, newSocket, sb).getResponse();
			break;
		case PUT:
			return PutResponse(*this, newSocket, sb).getResponse();
			break;
		// case POST:
		// 	response = PostResponse();
			// break;
		default:
			break;
	}
	return (response);
}

HttpRequest::HttpRequest()
{
	this->_HttpErrorMsg[200] = "HTTP/1.1 200 OK\r\n";
	
	this->_HttpErrorMsg[400] = "HTTP/1.1 400 Bad Request\r\n";
	this->_HttpErrorMsg[404] = "HTTP/1.1 404 Not Found\r\n";
	this->_HttpErrorMsg[405] = "HTTP/1.1 405 Not Allowed\r\n";
	this->_HttpErrorMsg[415] = "HTTP/1.1 415 Unsupported Media Type\r\n";		
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

void	HttpRequest::setBody(str body) {this->_body = body;}

void	HttpRequest::setHeader(str key, str value){this->_header[key] = value;}

std::string	HttpRequest::getBody() const {return this->_body;}

std::string	HttpRequest::getPath() const {return this->_path;}

std::string	HttpRequest::getMethodStr() const {return this->_methodStr;}

httpMethod	HttpRequest::getMethodEnum() const {return this->_methodEnum;}

HttpRequest::header	HttpRequest::getHeader() const {return this->_header;}

std::map<int, std::string>	HttpRequest::getHttpErrorMsg() const {return this->_HttpErrorMsg;}
