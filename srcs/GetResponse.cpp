/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 00:20:17 by suchua            #+#    #+#             */
/*   Updated: 2023/10/02 19:10:46 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetResponse.hpp"

// the main GET method function
GetResponse::GetResponse(const HttpRequest req, const ServerBlock sb)
{
	this->_path = req.getPath();

	if (isLocation(this->_path, sb))
		this->_response = locationRequest(this->_path, sb);
	else if (isImgFile(this->_path))
		this->_response = sendFile(this->_path);
	else
	{
		//error
		std::ifstream	input_file;
		std::string		str;
		char			c;
		
		input_file.open(sb.getErrorPage().find(404)->second.c_str());
		if (!input_file){
			perror("Couldn't open ./error_page/Error_404.html file");
			std::cerr << "failed to open ./error_page/Error_404.html file" << std::endl;
		}
		while (!input_file.eof() && input_file >> std::noskipws >> c){
			str += c;
		}
		input_file.close();

		// Construct the HTTP response
		std::stringstream	len;
		len << str.length();

		this->_response = "HTTP/1.1 404 Not Found\r\n";
		this->_response += "Content-Type: text/html\r\n";
		this->_response += "Content-Length: " + len.str() + "\r\n";
		this->_response += "\r\n";
		this->_response += str;
	}
}

// check whether the GET request is an image file
bool	GetResponse::isImgFile(std::string &file)
{
	const std::string	type[7] = {".avif", ".gif", ".jpeg", ".png", ".svg", ".csv", ".txt"};
	std::string			check;

	size_t dotPos = file.find_last_of(".");
    if (dotPos != std::string::npos) {
        check = file.substr(dotPos);
    }

	for (size_t i = 0; i < 7; i++)
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
std::string GetResponse::sendFile(std::string &path)
{
	std::string file = path.substr(1);
	std::string type ;
	std::string httpResponse;
	size_t 		dotPos = file.find_last_of(".");
	
	if (dotPos != std::string::npos) {
		type = file.substr(dotPos);
	}

	// Path to the img file
	std::string svgFilePath = "";
	if (type == ".csv" || type == ".txt")
		svgFilePath = "test/";
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

// check whether the GET request is a Loc
bool	GetResponse::isLocation(std::string &path, ServerBlock sb)
{
	std::vector<Location>	loc = sb.getLocation();
	std::vector<Location>::iterator	it = loc.begin();

	if (path == "/")
		return (true);
	for (; it != loc.end(); it++)
	{
		if (path == it->getDirectory())
			return (true);
	}
	return (false);
}

// execute in Location
std::string	GetResponse::locationRequest(std::string &path, ServerBlock sb)
{
	std::vector<Location>			loc = sb.getLocation();
	std::vector<Location>::iterator	it = loc.begin();
	std::string						index = "index.html", httpResponse, str, dir;
	std::ifstream					input_file;
	char							c;

	// get the location
	for (; it != loc.end(); it++)
	{
		if (path == it->getDirectory())
			break ;
	}
	// std::cout << "test::" << it->getDirectory() << it->getIndex() << "\n";
	
	//open the index.html
	if (path == "/")
	{
		dir = "test/index.html";
	} else {
		dir = "test";
		dir += path;
		dir += "/";
		dir += it->getIndex();
	}
	
	
	
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
