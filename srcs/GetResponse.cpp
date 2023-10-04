/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 00:20:17 by suchua            #+#    #+#             */
/*   Updated: 2023/10/04 19:41:20 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetResponse.hpp"
#include <algorithm>

// the main GET method function
GetResponse::GetResponse(const HttpRequest req, const Location target)
{
	this->_path = req.getPath();

	if (isImgFile(this->_path))
	{
		this->_response = sendFile(this->_path, target);
		return;
	}
	this->_response = locationRequest(this->_path, target);
}

// check whether the GET request is an image file
bool	GetResponse::isImgFile(std::string &file)
{
	const std::string	type[8] = {".avif", ".gif", ".jpeg", ".png", ".svg", ".csv", ".txt", ".jpg"};
	std::string			check;

	size_t dotPos = file.find_last_of(".");
    if (dotPos != std::string::npos) {
        check = file.substr(dotPos);
    }

	for (size_t i = 0; i < 8; i++)
	{
		if (type[i] == check)
			return (true);
	}
	return (false);
}

std::string readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return "";
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// send img file
std::string GetResponse::sendFile(std::string &path, Location target)
{
	std::string file = path.substr(1);
	std::string type ;
	std::string tmp ;
	std::string httpResponse;
	size_t 		dotPos = file.find_last_of(".");
	
	if (dotPos != std::string::npos) {
		type = file.substr(dotPos);
	}

	int numberOfSlashes = std::count(file.begin(), file.end(), '/');
	if (numberOfSlashes > 0)
	{
		size_t slashPos = file.find_first_of("/");
		tmp = file.substr(0, slashPos);
		if (tmp != "img" || tmp != "error_page")
			file = file.substr(slashPos + 1);
		// std::cout << file << std::endl;
	}

	// Path to the img file
	std::string svgFilePath = target.getRoot();
	svgFilePath += "/";
	svgFilePath += file;

	if (type != ".csv" && type != ".txt")
	{
		// Open the img file
		std::string gifData = readFile(svgFilePath);
		
		// Construct the HTTP response
		httpResponse = "HTTP/1.1 200 OK\r\n";
		if (type == ".svg" || type == ".xml")
			httpResponse += "Content-Type: image/svg+xml\r\n";
		else
		{
			httpResponse += "Content-Type: image/";
			httpResponse += type;
			httpResponse += "\r\n";
		}
		
		httpResponse += "Content-Length: " + std::to_string(gifData.size()) + "\r\n";
		httpResponse += "\r\n"; // End of headers
		httpResponse += gifData;
	}
	else
	{
		std::ifstream svgFile(svgFilePath.c_str());

		if (!svgFile.is_open()) {
			perror("Couldn't open img file");
			std::cerr << "failed to open "<< svgFilePath << " file" << std::endl;
		}

		std::string str;
		char		c;
		while (!svgFile.eof() && svgFile >> std::noskipws >> c){
			str += c;
		}
		svgFile.close();

		std::stringstream	len;
		len << str.length();

		httpResponse = "HTTP/1.1 200 OK\r\n";
		httpResponse += "Content-Type: text/html\r\n";
		httpResponse += "Content-Length: " + len.str() + "\r\n";
		httpResponse += "\r\n";
		httpResponse += str;
				
	}
	return (httpResponse);
}

// execute in Location
std::string	GetResponse::locationRequest(std::string &path, Location target)
{
	std::string						index = "index.html", httpResponse, str, dir;
	std::ifstream					input_file;
	char							c;

	(void)path;
	
	//open the index.html
	dir = target.getRoot();
	dir += "/";
	dir += target.getIndex();
	
	input_file.open(dir.c_str());
	if (!input_file)
	{
		perror("Couldn't open dir file");
		std::cerr << "failed to open " << dir << " file" << std::endl;
		return ("");
	}
	while (!input_file.eof() && input_file >> std::noskipws >> c){
		str += c;
	}
	input_file.close();
	
	// Construct the HTTP response
	std::stringstream	len;
	len << str.length();

	httpResponse = "HTTP/1.1 200 OK\r\n";
	httpResponse += "Content-Type: text/html\r\n";
	httpResponse += "Content-Length: " + len.str() + "\r\n";
	httpResponse += "\r\n";
	httpResponse += str;
	return (httpResponse);
}

std::string	GetResponse::getResponse() const {return this->_response;}

GetResponse::~GetResponse() {}

GetResponse::GetResponse(const GetResponse& other) {*this = other;}

GetResponse&	GetResponse::operator=(const GetResponse& other)
{
	if (this == &other)
		return (*this);
	this->_response = other._response;
	return (*this);
}
