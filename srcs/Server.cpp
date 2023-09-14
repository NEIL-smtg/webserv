/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:28:08 by suchua            #+#    #+#             */
/*   Updated: 2023/09/14 13:13:58 by mmuhamad         ###   ########.fr       */
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
	std::map<int, int>::const_iterator	it;

	std::cout << std::endl;
	std::cout << MAGENTA << " --------------------------------- " << RESET << std::endl;
	std::cout << std::endl;
	while (true)
	{
		fd_set	readFds;
		FD_ZERO(&readFds);

		int	maxFD = -1;
		for (it = _socketFD.begin(); it != _socketFD.end(); it++)
		{
			int sockfd = it->second;
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

		for (it = _socketFD.begin(); it != _socketFD.end(); it++)
		{
			int	port = it->first;
			int sockfd = it->second;
			
			
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
					runRequest(clientAddr, port, newSocket);
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

void	Server::runRequest(struct sockaddr_in&	clientAddr, int	port, int newSocket)
{

	char server_message[2000], client_message[2000];
    
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
	
	std::string	httpResponse = generateHttpResponse(client_message, newSocket);
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

std::string	Server::generateHttpResponse(char *client_message, int newSocket)
{
	std::string	ret;

	HttpRequest parsedRequest = parseHttpRequest(client_message);

	switch (parsedRequest.met)
	{
		case GET:
			ret = doGetMethod(parsedRequest, newSocket);
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

std::string	Server::doGetMethod(HttpRequest parsedRequest, int clientSocket)
{
	std::string httpResponse;
	if (parsedRequest.path == "/")
	{
		std::ifstream	input_file;
		std::string		str;
		char			c;
		
		input_file.open("./test/index.html");
		if (!input_file){
			perror("Couldn't open index.html file");
			std::cerr << "failed to open index.html file" << std::endl;
		}
		while (!input_file.eof() && input_file >> std::noskipws >> c){
			str += c;
		}
		input_file.close();

		// Construct the HTTP response
		httpResponse = "HTTP/1.1 200 OK\r\n";
		httpResponse += "Content-Type: text/html\r\n";
		httpResponse += "Content-Length: " + std::to_string(str.length()) + "\r\n";
		httpResponse += "\r\n"; // End of headers
		httpResponse += str;
	}
	else if (parsedRequest.path == "/404.svg")
	{
		// Path to the .svg file you want to send
		std::string svgFilePath = "./error_page/404.svg";

		// Open the .svg file
		std::ifstream svgFile(svgFilePath, std::ios::binary | std::ios::ate);

		if (!svgFile.is_open()) {
			perror("Couldn't open ./error_page/404.svg file");
			std::cerr << "failed to open ./error_page/404.svg file" << std::endl;
		}

		// Get the size of the .svg file
		std::streamsize fileSize = svgFile.tellg();
		svgFile.seekg(0, std::ios::beg);

		// Construct the HTTP response
		std::string httpResponse = "HTTP/1.1 200 OK\r\n";
		httpResponse += "Content-Type: image/svg+xml\r\n";
		httpResponse += "Content-Length: " + std::to_string(fileSize) + "\r\n";
		httpResponse += "\r\n"; // End of headers

		// Send the response headers to the client
		ssize_t bytesSent = send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);

		if (bytesSent == -1) {
			perror("Couldn't send");
			std::cerr << "Couldn't send message at " << clientSocket << " server fd socket" << std::endl;
		}

		// Send the .svg file content to the client
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

		// Close the .svg file
		svgFile.close();
		return ("");
	}
	else
	{
		std::ifstream	input_file;
		std::string		str;
		char			c;
		
		input_file.open("./error_page/Error_404.html");
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
		httpResponse += "\r\n"; // End of headers
		httpResponse += str;
	}
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
