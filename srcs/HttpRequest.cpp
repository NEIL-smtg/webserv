/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:33:01 by suchua            #+#    #+#             */
/*   Updated: 2023/10/06 04:01:29 by suchua           ###   ########.fr       */
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
			if (key == "Content-Type" && !strncmp(value.c_str(), "multipart/form-data", strlen("multipart/form-data")))
			{
				size_t	index = value.find_first_of('=') + 1;
				_boundary = value.substr(index);
				value = "multipart/form-data";
			}
			setHeader(key, value);
		}
		if (key == "Content-Length")
			break ;
	}
	while (std::getline(stream, line))
	{
		if (isspace(line.at(0)))
			continue;
		if (_boundary.empty() && !setWWWFormData(line))
			continue ;
		if (!_boundary.empty())
			setBoundaryFormData(line);
	}
}

bool	HttpRequest::setWWWFormData(std::string line)
{
	size_t				toSep;
	std::vector<str>	tokens;

	toSep = line.find('=');
	if (toSep == std::string::npos)
		return (false);
	tokens = tokennizer(line, "=&");
	for (size_t i = 0; i < tokens.size(); i++)
	{
		if (i % 2 == 0)
			this->_dataKeys.push_back(tokens.at(i));
		else
			this->_dataVal.push_back(tokens.at(i));
	}
	return (true);
}

void	HttpRequest::setBoundaryFormData(std::string line)
{
	if (line.at(0) == '-')
		_formBoundary.push_back(line);
	else if (this->_dataKeys.size() == this->_dataVal.size())
	{
		size_t	start = line.find('\"') + 1;
		size_t	end = line.find('\"', start);
		this->_dataKeys.push_back(line.substr(start, end - start));
	}
	else
		this->_dataVal.push_back(line);
}

std::string	HttpRequest::generateHttpResponse(const str& req, const int clientSocket, const ServerBlock sb)
{
	clearAll();
	parseHttpRequest(req);
	tokennizeReqUrlPath();

	RequestErrorHandling	err(*this, sb);
	if (!err.ErrorHandler())
		return (err.getErrResponse());

	Location	target(err.getTargetBlock());
	str			response;

	switch (this->_methodEnum)
	{
		case GET:
			response = GetResponse(*this, target).getResponse();
			break;			
		case PUT:
			response = PutResponse(*this, clientSocket, target).getResponse();
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

void	HttpRequest::clearAll()
{
	this->_header.clear();
	this->_tokennizePath.clear();
	this->_dataKeys.clear();
	this->_dataVal.clear();
	this->_boundary.clear();
	this->_formBoundary.clear();
}

HttpRequest::HttpRequest()
{
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

void	HttpRequest::tokennizeReqUrlPath()
{
	const std::string	sep("/");
	size_t	start = 0, end = 0;
	size_t	npos = std::string::npos;
	std::string	path = _path;

	while (start != npos && end != npos)
	{
		end = path.find_first_of(sep, start);
		if (end == npos && path[start] != 0)
		{
			this->_tokennizePath.push_back(sep + &path[start]);
			break ;
		}
		if (start != npos && end - start != 0 && path[start] != 0)
			this->_tokennizePath.push_back(sep + path.substr(start, end - start));
		start = path.find_first_not_of(sep, end);
	}
}

std::vector<std::string>	HttpRequest::tokennizer(const std::string line, const std::string sep)
{
	size_t	start = 0, end = 0;
	size_t	npos = std::string::npos;
	std::vector<std::string>	tokens;

	while (start != npos && end != npos)
	{
		end = line.find_first_of(sep, start);
		if (end == npos && line[start] != 0)
		{
			tokens.push_back(&line[start]);
			break ;
		}
		if (start != npos && end - start != 0 && line[start] != 0)
			tokens.push_back(line.substr(start, end - start));
		start = line.find_first_not_of(sep, end);
	}
	return (tokens);
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

std::vector<std::string>	HttpRequest::getTokennizePath() const {return this->_tokennizePath;}

std::vector<std::string>	HttpRequest::getDataKeys() const {return this->_dataKeys;}

std::vector<std::string>	HttpRequest::getDataValues() const {return this->_dataVal;}

std::string	HttpRequest::getBoundary() const {return this->_boundary;}

std::vector<std::string>	HttpRequest::getFormBoundary() const {return this->_formBoundary;}
