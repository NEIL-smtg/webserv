/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:28:08 by suchua            #+#    #+#             */
/*   Updated: 2023/09/14 19:51:38 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <fstream>
#include <sstream>
#include <map>

/***********************************
 * SERVER
***********************************/
void	Server::startServer()
{
	std::map<int, int>::const_iterator	it;
	int	sockfd;

	std::cout << std::endl;
	for (it = _socketFD.begin(); it != _socketFD.end(); it++)
	{
		sockfd = it->second;
		if (listen(sockfd, SOMAXCONN) == -1)
		{
			perror("listen");
			std::cerr << "Error : Failed to listen at port " << it->first << std::endl;
			exit(EXIT_FAILURE);
		}
		std::cout << YELLOW << "[ * ] Listening for incoming connections on port " << BLUE << it->first << YELLOW << " ... " << RESET << std::endl;
	}
	acceptConnection();
}

void	Server::acceptConnection()
{
	std::map<int, int>::const_iterator	ite;

	std::cout << std::endl;
	std::cout << MAGENTA << " --------------------------------- " << RESET << std::endl;
	std::cout << std::endl;
	while (true)
	{
		fd_set	readFds;
		FD_ZERO(&readFds);

		int	maxFD = -1;
		for (ite = _socketFD.begin(); ite != _socketFD.end(); ite++)
		{
			int sockfd = ite->second;
			FD_SET(sockfd, &readFds);
			if (sockfd > maxFD)
				maxFD = sockfd;
		}

		int activity = select(maxFD + 1, &readFds, NULL, NULL, NULL);
		if (activity == -1)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}

		std::vector<ServerBlock>::const_iterator	it;
		
		for (it = _conf.begin(); it != _conf.end(); it++)
		{
			int	port = it->getPort();
			int sockfd = _socketFD.find(port)->second;
			
			
			if (FD_ISSET(sockfd, &readFds))
			{
				struct sockaddr_in	clientAddr;
				socklen_t	addrLen = sizeof(clientAddr);
				memset(&clientAddr, 0, sizeof(clientAddr));
				int newSocket = accept(sockfd, (struct sockaddr *) &clientAddr, &addrLen);
				
				if (newSocket == -1)
				{
					perror("accept");
					std::cerr << "Failed to accept new connection at port " << port << std::endl;
				}
				else
				{
					runRequest(clientAddr, newSocket, it);
				}
			}
		}
	}
}

/***********************************
 * Http Request
***********************************/

HttpRequest parseHttpRequest(const std::string& httpRequest)
{
	HttpRequest request;
	const std::string	met[7] = {"GET", "HEAD", "DELETE", "OPTIONS", "POST", "PUT", "TRACE" };


	// Split the request into lines
	std::istringstream requestStream(httpRequest);
	std::string line;

	// Parse the first line (Request-Line)
	if (std::getline(requestStream, line)) {
		std::istringstream requestLineStream(line);
		requestLineStream >> request.method >> request.path;
	}

	for (size_t i = 0; i < 7; i++)
	{
		if (met[i] == request.method)
		{
			
			switch (i)
			{
				case 0:
					request.met = GET;
					break;
				case 1:
					request.met = HEAD;
					break;
				case 2:
					request.met = DELETE;
					break;
				case 3:
					request.met = OPTIONS;
					break;
				case 4:
					request.met = POST;
					break;
				case 5:
					request.met = PUT;
					break;
				case 6:
					request.met = TRACE;
					break;
				
				default:
					break;
			}
			break;
		}
	}

	// Parse headers
	while (std::getline(requestStream, line) && !line.empty()) {
		size_t separatorPos = line.find(':');
		if (separatorPos != std::string::npos) {
			std::string key = line.substr(0, separatorPos);
			std::string value = line.substr(separatorPos + 2); // Skip the ': ' characters
			request.headers[key] = value;
		}
	}
	return (request);
}

void	Server::runRequest(struct sockaddr_in&	clientAddr, int newSocket, std::vector<ServerBlock>::const_iterator it)
{

	char server_message[2000], client_message[2000];
	int	port = it->getPort();;
	
    
	// Clean buffers:
	memset(server_message, '\0', sizeof(server_message));
	memset(client_message, '\0', sizeof(client_message));

	std::cout << GREEN << "[ ✅ ] New connection" << YELLOW << " : client with IP "  << BLUE << inet_ntoa(clientAddr.sin_addr) << YELLOW ;
	std::cout << ", accepted on port " << BLUE << port << RESET << " ==> "<< MAGENTA << "FD -> " << newSocket << RESET << std::endl;

	if (recv(newSocket, client_message, sizeof(client_message), 0) < 0)
	{
		perror("Couldn't receive");
		std::cerr << "Couldn't receive message at " << newSocket << " client fd socket" << std::endl;
	}

	std::cout << YELLOW << "[ * ]  Msg from client: \n\n" << RESET << client_message << std::endl;
	
	std::string	httpResponse = generateHttpResponse(client_message, newSocket, it);
	if	(httpResponse == "")
	{
		std::cout << GREEN << "[ ✅ ] Msg sent to client!" << RESET << std::endl;
		std::cout << std::endl;

		std::cout << MAGENTA << " --------------------------------- " << RESET << std::endl;
		std::cout << std::endl;
		close(newSocket);
		return ;
	}

	// Send the response over the network connection
	strcpy(server_message, httpResponse.c_str());

	if (send(newSocket, server_message, strlen(server_message), 0) < 0){
		perror("Couldn't send");
		std::cerr << "Couldn't send message at " << newSocket << " server fd socket" << std::endl;
	}
	std::cout << GREEN << "[ ✅ ] Msg sent to client!" << RESET << std::endl;
	std::cout << std::endl;

	std::cout << MAGENTA << " --------------------------------- " << RESET << std::endl;
	std::cout << std::endl;

	close(newSocket);
}

/***********************************
 * Http Response
***********************************/

std::string	Server::generateHttpResponse(char *client_message, int newSocket, std::vector<ServerBlock>::const_iterator it)
{
	std::string	ret;

	HttpRequest parsedRequest = parseHttpRequest(client_message);

	switch (parsedRequest.met)
	{
		case GET:
			ret = doGetMethod(parsedRequest, newSocket, it);
			break;
		// case HEAD:
		// 	doHeadMethod();
		// 	break;
		// case DELETE:
		// 	doDeleteMethod();
		// 	break;
		// case OPTIONS:
		// 	doOptionsMethod();
		// 	break;
		// case POST:
		// 	doPostMethod();
		// 	break;
		// case PUT:
		// 	doPutMethod();
		// 	break;
		// case TRACE:
		// 	doTraceMethod();
		// 	break;
		default:
			break;
	}
	return (ret);
}

/***********************************
 * Methods
***********************************/

std::string	Server::doGetMethod(HttpRequest parsedRequest, int clientSocket, std::vector<ServerBlock>::const_iterator it)
{
	std::string httpResponse;

	if (isLocation(parsedRequest.path, it))
		httpResponse = locationRequest(parsedRequest.path, it);
	else if (isImgFile(parsedRequest.path))
		httpResponse = sendFile(parsedRequest.path, clientSocket);
	else
	{
		std::ifstream	input_file;
		std::string		str;
		char			c;
		
		input_file.open(it->getErrorPage().find(404)->second);
		if (!input_file){
			perror("Couldn't open ./error_page/Error_404.html file");
			std::cerr << "failed to open ./error_page/Error_404.html file" << std::endl;
		}
		while (!input_file.eof() && input_file >> std::noskipws >> c){
			str += c;
		}
		input_file.close();

		// Construct the HTTP response
		httpResponse = "HTTP/1.1 404 Not Found\r\n";
		httpResponse += "Content-Type: text/html\r\n";
		httpResponse += "Content-Length: " + std::to_string(str.length()) + "\r\n";
		httpResponse += "\r\n";
		httpResponse += str;
	}
	return (httpResponse);
}

/***********************************
 * utils
***********************************/

bool Server::isImgFile(std::string &file)
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

std::string Server::sendFile(std::string &path, int clientSocket)
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
	std::ifstream svgFile(svgFilePath, std::ios::binary | std::ios::ate);

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
	httpResponse += "Content-Length: " + std::to_string(fileSize) + "\r\n";
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

bool	Server::isLocation(std::string &path, std::vector<ServerBlock>::const_iterator sb)
{
	std::vector<Location>	loc = (*sb).getLocation();
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
			input_file.open(dir);
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

std::string	Server::locationRequest(std::string &path, std::vector<ServerBlock>::const_iterator sb)
{
	std::string 	httpResponse;
	std::ifstream	input_file;
	std::string		str;
	char			c;
	std::string		dir;
	std::vector<Location>	loc = (*sb).getLocation();
	std::vector<Location>::iterator	it = loc.begin();
	std::string			check = "";


	if (path == "/")
	{
	dir = sb->getRoot();
	dir += "/";
	dir += sb->getIndex();

	input_file.open(dir);
	if (!input_file){
		perror("Couldn't open file file");
		std::cerr << "failed to open " << dir << " file" << std::endl;
	}
	while (!input_file.eof() && input_file >> std::noskipws >> c){
		str += c;
	}
	input_file.close();

	httpResponse = "HTTP/1.1 200 OK\r\n";
	httpResponse += "Content-Type: text/html\r\n";
	httpResponse += "Content-Length: " + std::to_string(str.length()) + "\r\n";
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
		dir = sb->getRoot();
		dir += path;
		dir += "/";
		dir += it->getIndex();
	}
	
	input_file.open(dir);
	if (!input_file){
		perror("Couldn't open dir file");
		std::cerr << "failed to open " << dir << " file" << std::endl;
	}
	while (!input_file.eof() && input_file >> std::noskipws >> c){
		str += c;
	}
	input_file.close();

	httpResponse = "HTTP/1.1 200 OK\r\n";
	httpResponse += "Content-Type: text/html\r\n";
	httpResponse += "Content-Length: " + std::to_string(str.length()) + "\r\n";
	httpResponse += "\r\n";
	httpResponse += str;
	return (httpResponse);
}

/***********************************
 * Constructors & Destructors
***********************************/

Server::Server(std::vector<ServerBlock>& conf, std::map<int, struct sockaddr_in>& socketAddr, std::map<int, int>& socketFD, std::map<int, struct sockaddr_in>&	socketFdAddr)
: _conf(conf), _socketAddr(socketAddr), _socketFD(socketFD), _socketFdAddr(socketFdAddr){}

Server::~Server(){}

Server::Server(const Server& other)
: _conf(other._conf), _socketAddr(other._socketAddr), _socketFD(other._socketFD), _socketFdAddr(other._socketFdAddr) {*this = other;}

Server&	Server::operator=(const Server& other)
{
	if (this == &other)
		return (*this);
	return (*this);
}
