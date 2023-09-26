/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 13:31:31 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/09/26 16:21:02 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostResponse.hpp"

int RequestHeaderChecking(const HttpRequest& req, std::string &boundary)
{
    std::map<std::string, std::string> header = req.getHeader();
	std::map<std::string, std::string>::iterator it = header.find("Content-Type");
	if (it == header.end())
		return (400);
	size_t found = it->second.find("multipart/form-data");
	if (found == std::string::npos)
		return (415);
	//check content length exceed max/min (Error 413/Error 400)
	found = it->second.find("boundary=");
	if (found + 9 >= std::string::npos)
		return (400);
	boundary = "--" + it->second.substr(found + 9);
    return (0);
}

int RequestBodyExtract(std::map<std::string, std::string> &body_extract, const HttpRequest& req, std::string boundary)
{
    std::string boundary_end = boundary.substr(0, boundary.length() - 1) + "--\r";
    size_t pos = 0;
    std::vector<std::string> body = req.getBody();
    std::vector<std::string>::iterator it2;
    for(it2 = body.begin(); it2 != body.end(); it2++)
    {
        while (it2 != body.end() && (*it2) != boundary)
            it2++;
        if (it2 == body.end())
            break;
        it2++;
        pos = (*it2).find("Content-Disposition: form-data;");
        if (pos != std::string::npos)
		{
            (*it2) = (*it2).substr(0, (*it2).length() - 1);
            pos = (*it2).find(';');
            std::string body_key = (*it2).substr(pos + 8);
            if (body_key == "\"")
                return (400);
            body_key = body_key.substr(0, body_key.length() - 1);
            if (pos == std::string::npos)
               return (400);
            if (it2 + 1 != body.end() && (*(it2 + 1)) != boundary && (*(it2 + 1)) != boundary_end)
                it2++;
            std::string body_value = "";
            while (it2 + 1 != body.end() && (*(it2 + 1)) != boundary && (*(it2 + 1)) != boundary_end)
            {
                it2++;
                body_value += (*it2).substr(0, (*it2).length() - 1);
            }
            if (it2 + 1 == body.end())
                return (400);
            if ((*it2) == "\r")
                return (400);
            body_extract[body_key] = body_value;
        }
    }
    return (0);
}

int FilenameContentCheck(std::map<std::string, std::string> body_extract, std::string &path)
{
    std::map<std::string, std::string>::iterator file_key_it = body_extract.find("filename");
    if (file_key_it == body_extract.end())
        return (400);
    std::map<std::string, std::string>::iterator content_key_it = body_extract.find("content");
    if (content_key_it == body_extract.end())
        return (500);
    path += body_extract["filename"];
    return (0);
}

int FileCheckingWriting(std::map<std::string, std::string> body_extract, std::string path, httpMethod method)
{
    std::ifstream infile;
    std::ofstream outfile;
    infile.open(path.c_str());
    if (infile.is_open())
    {
        outfile.close();
        if (method == POST)
            return (400);
    }
    else
    {
        if (method == PUT)
            return (400); 
    }
    outfile.open(path.c_str());
    if (!outfile.is_open())
        return (500);
    outfile << body_extract["content"];
    outfile.close();
    return (0);
}

PostResponse::PostResponse(const HttpRequest& req, const int& clientSocket, const ServerBlock& sb)
:_req(req), _clientSocket(clientSocket), _sb(sb)
{
	// if (!urlPathFound() || !methodAllowed())
	// 	return ;
    std::string boundary;
    int status_code;
    status_code = RequestHeaderChecking(req, boundary);
    if (status_code)
    {
        std::cout << "Error " << status_code << std::endl;
        return ;
    }
    std::map<std::string, std::string> body_extract;
    if (RequestBodyExtract(body_extract, req, boundary))
    {
        std::cout << "Error 400" << std::endl;
        return ;
    }
    std::string ser_path = _sb.getRoot();
	std::string path;
	if (ser_path[ser_path.length() - 1] == '/')
		ser_path.erase(ser_path.length() - 1);	
	path = ser_path + _req.getPath();
	if (path[path.length() - 1] != '/')
		path += '/';
    status_code = FilenameContentCheck(body_extract, path);
    if (status_code)
    {
        std::cout << "Error " << status_code << std::endl;
        return ;
    }
    status_code = FileCheckingWriting(body_extract, path, POST);
    if (status_code)
    {
        std::cout << "Error " << status_code << std::endl;
        return ;
    }
    std::cout << "200, Success" << std::endl;
}

bool	PostResponse::urlPathFound()
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

bool	PostResponse::methodAllowed()
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

PostResponse::~PostResponse() {}

PostResponse::PostResponse(const PostResponse& other)
:_req(other._req), _clientSocket(other._clientSocket), _sb(other._sb) {*this = other;}

PostResponse&	PostResponse::operator=(const PostResponse& other)
{
	if (this == &other)
		return (*this);
	this->_response = other._response;
	return (*this);
}

std::string	PostResponse::getResponse() const {return this->_response;}

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
