/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TraceResponse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 13:31:31 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/10/12 18:58:55 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "TraceResponse.hpp"

TraceResponse::TraceResponse(const HttpRequest& req, const int& clientSocket, const Location& sb, RequestErrorHandling err)
:_req(req), _clientSocket(clientSocket), _sb(sb)
{
	std::string ser_path = _sb.getRoot();
	std::string path;
	if (ser_path[ser_path.length() - 1] == '/')
		ser_path.erase(ser_path.length() - 1);	
	path = ser_path + _req.getPath();
	if (path[path.length() - 1] == '/')
    {
		err.generateErrResponse(400, sb);
		this->_response = err.getErrResponse();
		this->_response +=  req.getfull();
		return ;
    }
	std::ifstream	infile(fileName.c_str());
	std::string	line;

	if (!infile.is_open())
		throw InvalidFileException("Error opening file.");
    err.generateErrResponse(200, sb);
	this->_response = err.getErrResponse();
	this->_response +=  req.getfull();
	return ;
}

TraceResponse::~TraceResponse() {}

TraceResponse::TraceResponse(const TraceResponse& other)
:_req(other._req), _clientSocket(other._clientSocket), _sb(other._sb) {*this = other;}

TraceResponse&	TraceResponse::operator=(const TraceResponse& other)
{
	if (this == &other)
		return (*this);
	this->_response = other._response;
	return (*this);
}

std::string	TraceResponse::getResponse() const {return this->_response;}
