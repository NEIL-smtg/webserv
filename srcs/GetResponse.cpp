/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 18:54:44 by suchua            #+#    #+#             */
/*   Updated: 2023/10/06 01:50:21 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetResponse.hpp"

static bool	isImg(std::string img)
{
	const std::string	type[7] = {"jpeg", "jpg", "png", "gif", "xml", "svg"};

	for (size_t i = 0; i < 7; i++)
	{
		if (img == type[i])
			return (true);
	}
	return (false);	
}

GetResponse::GetResponse(const HttpRequest& req, const Location& target):
_req(req), _target(target) 
{
	struct stat	fileStat;
	std::string	pathToFile = this->_target.getRoot();

	if (!stat(pathToFile.c_str(), &fileStat))
	{
		if (S_ISREG(fileStat.st_mode))
			responseHandler("");
		else if (S_ISDIR(fileStat.st_mode) && !pathToFile.empty())
			responseHandler(this->_target.getIndex());
	}
	else
		std::cerr << "Error checking status for " << pathToFile << std::endl;
}

void	GetResponse::responseHandler(std::string file)
{
	std::string	pathToFile = this->_target.getRoot();
	std::string type;

	if (!file.empty() && file.at(file.length() - 1) != '/')
		pathToFile += "/";
	pathToFile += file;
	if (pathToFile.find_last_of('.') + 1 != std::string::npos)
		type = pathToFile.substr(pathToFile.find_last_of('.') + 1);	
	if (isImg(type) && (type == "xml" || type == "svg"))
		this->_contentType = "image/svg+xml";
	else if (isImg(type))
		this->_contentType = "image/" + type;
	else if (type == "html")
		this->_contentType = "text/html";
	else
		this->_contentType = "text/plain";
	if (isImg(type))
		setImgResponse(pathToFile, type);
	else
		setNormalResponse(pathToFile);
	std::cout << "file = " << file << std::endl;
	std::cout << "this->_contentType = " << this->_contentType << std::endl;
	std::cout << "_pathToFile = " << pathToFile << std::endl;
}

void	GetResponse::setNormalResponse(std::string pathToFile)
{
	std::string			line;
	std::string			content;
	std::ifstream		infile;

	infile.open(pathToFile.c_str());
	while (std::getline(infile, line))
		content += line;
	infile.close();
	this->_response << "HTTP/1.1 200 OK\r\n";
	this->_response << "Content-Type: " << this->_contentType << "\r\n";
	this->_response << "Content-Length: " << content.length() << "\r\n";
	this->_response << "\r\n";
	this->_response << content;
}

void	GetResponse::setImgResponse(std::string pathToFile, std::string type)
{
	std::string			imgData;
	std::ifstream		infile;
	std::ostringstream	imgBuffer;

	if (type == "svg" || type == "xml")
		infile.open(pathToFile.c_str());
	else
		infile.open(pathToFile.c_str(), std::ios::binary);
	imgBuffer << infile.rdbuf();
	imgData = imgBuffer.str();
	infile.close();
	this->_response << "HTTP/1.1 200 OK\r\n";
	this->_response << "Content-Type: " << this->_contentType << "\r\n";
	this->_response << "Content-Length: " << imgData.length() << "\r\n";
	this->_response << "\r\n";
	this->_response << imgData;
}

std::string	GetResponse::getResponse() const {return this->_response.str();}

GetResponse::~GetResponse(){}

GetResponse::GetResponse(const GetResponse& other):
_req(other._req), _target(other._target) {*this = other;}

GetResponse&	GetResponse::operator=(const GetResponse& other)
{
	if (this == &other)
		return (*this);
	this->_contentType = other._contentType;
	return (*this);
}
