/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 13:31:31 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/10/12 16:46:51 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "DeleteResponse.hpp"

DeleteResponse::DeleteResponse(const HttpRequest& req, const int& clientSocket, const Location& sb, RequestErrorHandling err)
:_req(req), _clientSocket(clientSocket), _sb(sb)
{
	// if (!urlPathFound() || !methodAllowed())
	// 	return ;
	std::string ser_path = _sb.getRoot();
	std::string path;
	if (ser_path[ser_path.length() - 1] == '/')
		ser_path.erase(ser_path.length() - 1);	
	path = ser_path + _req.getPath();
	if (path[path.length() - 1] == '/')
    {
		err.generateErrResponse(400, sb);
		this->_response = err.getErrResponse();
		return ;
    }
    int status;
    status = remove(path.c_str());
    if (status != 0)
    {
		err.generateErrResponse(400, sb);
		this->_response = err.getErrResponse();
		return ;
    }
    err.generateErrResponse(200, sb);
	this->_response = err.getErrResponse();
	return ;
}

DeleteResponse::~DeleteResponse() {}

DeleteResponse::DeleteResponse(const DeleteResponse& other)
:_req(other._req), _clientSocket(other._clientSocket), _sb(other._sb) {*this = other;}

DeleteResponse&	DeleteResponse::operator=(const DeleteResponse& other)
{
	if (this == &other)
		return (*this);
	this->_response = other._response;
	return (*this);
}

std::string	DeleteResponse::getResponse() const {return this->_response;}

// std::string	generateErrorResponse(const ServerBlock& sb, const httpError errNum)
// {
// 	std::string			errHtmlFilePath;
// 	std::string			line;
// 	std::stringstream	response;
// 	std::ifstream		errHtml;
// 	std::stringstream	body;

// 	errHtmlFilePath = sb.getErrorPage().find(errNum)->second;
// 	errHtml.open(errHtmlFilePath.c_str());
	
// 	while (std::getline(errHtml, line))
// 		body << line;

// 	if (errNum == NOT_FOUND) response << ERR_404_HEAD;
// 	else if (errNum == NOT_ALLOWED) response << ERR_405_HEAD;

// 	response << "Content-Type: " << errHtmlFilePath + "\r\n";
// 	response << "Content-Length: " << body.str().length() << "\r\n\r\n";
// 	response << body.str();
// 	return (response.str());
// }

// HOLDING AREA
// found = body.find(boundary, pos);
// while (found != std::string::npos) {
//     data_split.push_back(body.substr(pos, found - pos));
//     pos = found + boundary.length();
//     found = body.find(boundary, pos);
// }
// data_split.push_back(body.substr(pos));
