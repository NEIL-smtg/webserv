/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 00:20:17 by suchua            #+#    #+#             */
/*   Updated: 2023/10/24 14:48:58 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GetResponse.hpp"

GetResponse::GetResponse(const HttpRequest req, const int clientSocket, const ServerBlock sb)
{
	this->_path = req.getPath();

	if (isLocation(this->_path, sb))
		this->_response = locationRequest(this->_path, sb);
	else if (isImgFile(this->_path))
		this->_response = sendFile(this->_path, clientSocket);
	else
	{
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

bool	GetResponse::isImgFile(std::string &file)
{
	const std::string	type[5] = {".avif", ".gif", ".jpeg", ".png", ".svg" };
	std::string			check;

	size_t dotPos = file.find_last_of(".");
    if (dotPos != std::string::npos) {
        check = file.substr(dotPos);
    }

	for (size_t i = 0; i < 5; i++)
	{
		if (type[i] == check)
			return (true);
	}
	return (false);
}

std::string GetResponse::sendFile(std::string &path, int clientSocket)
{
	std::string file = path.substr(1);
	std::string type ;
	size_t dotPos = file.find_last_of(".");
	if (dotPos != std::string::npos) {
		type = file.substr(dotPos + 1);
	}

	// Path to the img file
	std::string svgFilePath = "";
	svgFilePath += file;

	// Open the img file
	std::ifstream svgFile(svgFilePath.c_str(), std::ios::binary | std::ios::ate);

	if (!svgFile.is_open()) {
		perror("Couldn't open img file");
		std::cerr << "failed to open "<< svgFilePath << " file" << std::endl;
	}

	// Get the size of the img file
	std::streamsize fileSize = svgFile.tellg();
	svgFile.seekg(0, std::ios::beg);

	// Construct the HTTP response
	std::string httpResponse = "HTTP/1.1 200 OK\r\n";
	if (type == "svg" || type == "xml")
		httpResponse += "Content-Type: image/svg+xml\r\n";
	else
	{
		httpResponse += "Content-Type: image/";
		httpResponse += type;
		httpResponse += "\r\n";
	}

	std::stringstream	len;
	len << fileSize;
	
	httpResponse += "Content-Length: " + len.str() + "\r\n";
	httpResponse += "\r\n"; // End of headers

	// Send the response headers to the client
	ssize_t bytesSent = send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);

	if (bytesSent == -1) {
		perror("Couldn't send");
		std::cerr << "Couldn't send message at " << clientSocket << " server fd socket" << std::endl;
	}

	// Send the img file content to the client
	char buffer[4096];
	while (true) {
		svgFile.read(buffer, sizeof(buffer));
		ssize_t bytesRead = svgFile.gcount();

		if (bytesRead <= 0)
			break;

		ssize_t bytesSent = send(clientSocket, buffer, bytesRead, 0);

		if (bytesSent == -1) {
			perror("Couldn't send");
			std::cerr << "Couldn't send message at " << clientSocket << " server fd socket" << std::endl;
			break;
		}
	}

	// Close the img file
	svgFile.close();
	return ("");
}

bool	GetResponse::isLocation(std::string &path, ServerBlock sb)
{
	std::vector<Location>	loc = sb.getLocation();
	std::vector<Location>::iterator	it = loc.begin();
	std::string			check = "";
	std::string			dir = "";

	if (path == "/")
		return (true);
	for (; it != loc.end(); it++)
	{
		size_t dotPos = path.find_last_of(".");
		if (dotPos != std::string::npos) {
			check = path.substr(dotPos);
		}
		
		if (check != ""  && check != ".html")
		{
			std::ifstream	input_file;
			
			dir = it->getRoot();
			dir += path;
			input_file.open(dir.c_str());
			if (input_file){
				input_file.close();
				return (true);
			}
		}
		if (path == it->getDirectory())
			return (true);
	}
	return (false);
}

std::string	GetResponse::locationRequest(std::string &path, ServerBlock sb)
{
	std::string 	httpResponse;
	std::ifstream	input_file;
	std::string		str;
	char			c;
	std::string		dir;
	std::vector<Location>	loc = sb.getLocation();
	std::vector<Location>::iterator	it = loc.begin();
	std::string			check = "";


	if (path == "/")
	{
	dir = sb.getRoot();
	dir += "/";
	dir += sb.getIndex();

	input_file.open(dir.c_str());
	if (!input_file){
		perror("Couldn't open file file");
		std::cerr << "failed to open " << dir << " file" << std::endl;
	}
	while (!input_file.eof() && input_file >> std::noskipws >> c){
		str += c;
	}
	input_file.close();

	std::stringstream	len;
	len << str.length();

	httpResponse = "HTTP/1.1 200 OK\r\n";
	httpResponse += "Content-Type: text/html\r\n";
	httpResponse += "Content-Length: " + len.str() + "\r\n";
	httpResponse += "\r\n";
	httpResponse += str;
	return (httpResponse);
}
	size_t dotPos = path.find_last_of(".");
	if (dotPos != std::string::npos) {
		check = path.substr(dotPos);
	}

	else if (check != ""  && check != ".html")
	{
		dir = it->getRoot();
		dir += "/";
		dir += path;
	}
	else
	{
		dir = sb.getRoot();
		dir += path;
		dir += "/";
		dir += it->getIndex();
	}
	
	input_file.open(dir.c_str());
	if (!input_file){
		perror("Couldn't open dir file");
		std::cerr << "failed to open " << dir << " file" << std::endl;
	}
	while (!input_file.eof() && input_file >> std::noskipws >> c){
		str += c;
	}
	input_file.close();

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
