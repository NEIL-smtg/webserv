/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 13:31:31 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/09/25 18:26:05 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "DeleteResponse.hpp"

DeleteResponse::DeleteResponse(const HttpRequest& req, const int& clientSocket, const ServerBlock& sb)
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
		std::cout << "Error 400" << std::endl;
        return;
    }
    int status;
    status = remove(path.c_str());
    if (status != 0)
    {
		std::cout << "Error 400" << std::endl;
        return;
    }
    std::cout << "200, Success" << std::endl;
}

bool	DeleteResponse::urlPathFound()
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

	// this->_response = generateErrorResponse(this->_sb, NOT_FOUND);
	return (false);
}

bool	DeleteResponse::methodAllowed()
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
	// this->_response = generateErrorResponse(this->_sb, NOT_ALLOWED);
	return (false);
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
