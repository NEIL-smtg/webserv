/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:31:26 by suchua            #+#    #+#             */
/*   Updated: 2023/10/12 16:49:24 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PutResponse.hpp"

static int RequestHeaderChecking(const HttpRequest& req, std::string &boundary, const Location& sb)
{
    std::map<std::string, std::string> header = req.getHeader();
	std::map<std::string, std::string>::iterator it = header.find("Content-Type");
	if (it == header.end())
		return (400);
	size_t found = it->second.find("multipart/form-data");
	if (found == std::string::npos)
		return (415);
	found = it->second.find("boundary=");
	if (found + 9 >= std::string::npos)
		return (400);
	boundary = "--" + it->second.substr(found + 9);
	it = header.find("Content-Length");
	if (found == std::string::npos)
		return (400);
	std::string s = header["Content-Length"];
	if (std::stol(s) > sb.getClientMaxBodySize())
		std::cout << "Exceeded max" << std::endl;
	if (std::stol(s) < sb.getClientMinBodySize())
		std::cout << "Exceeded min" << std::endl;
    return (0);
}

static int RequestBodyExtract(std::map<std::string, std::string> &body_extract, const HttpRequest& req, std::string boundary)
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
            pos = body_key.find(';');
            if (pos != std::string::npos)
                body_key = body_key.substr(0, pos - 1);
            else
                body_key = body_key.substr(0, body_key.length() - 1);
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

static int FilenameContentCheck(std::map<std::string, std::string> body_extract, std::string &path)
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

static int FileCheckingWriting(std::map<std::string, std::string> body_extract, std::string path, httpMethod method)
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
	std::cout << path.c_str() << std::endl;
    outfile.open(path.c_str());
    if (!outfile.is_open())
	{
        return (500);
	}
    outfile << body_extract["content"];
    outfile.close();
    return (0);
}

PutResponse::PutResponse(const HttpRequest& req, const int& clientSocket, const Location& sb, RequestErrorHandling err)
:_req(req), _clientSocket(clientSocket), _sb(sb)
{
	std::string boundary;
    int status_code;
    status_code = RequestHeaderChecking(req, boundary, sb);
    if (status_code)
	{
		err.generateErrResponse(status_code, sb);
		this->_response = err.getErrResponse();
		return ;
	}
    std::map<std::string, std::string> body_extract;
    status_code = RequestBodyExtract(body_extract, req, boundary);
    if (status_code)
	{
		err.generateErrResponse(status_code, sb);
		this->_response = err.getErrResponse();
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
		err.generateErrResponse(status_code, sb);
		this->_response = err.getErrResponse();
		return ;
	}
    status_code = FileCheckingWriting(body_extract, path, PUT);
    if (status_code)
	{
		err.generateErrResponse(status_code, sb);
		this->_response = err.getErrResponse();
		return ;
	}
    err.generateErrResponse(200, sb);
	this->_response = err.getErrResponse();
}

PutResponse::~PutResponse() {}

PutResponse::PutResponse(const PutResponse& other)
:_req(other._req), _clientSocket(other._clientSocket), _sb(other._sb) {*this = other;}

PutResponse&	PutResponse::operator=(const PutResponse& other)
{
	if (this == &other)
		return (*this);
	return (*this);
}

std::string	PutResponse::getResponse() const {return this->_response;}
