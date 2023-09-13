/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:28:08 by suchua            #+#    #+#             */
/*   Updated: 2023/09/13 18:17:14 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <fstream>

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
	
	// generating server respond (index.html)
	std::ifstream	input_file;
	std::string		str;
	char			c;
	
	input_file.open("./error_page/Error_404.html");
	if (!input_file){
		perror("Couldn't open index.html file");
		std::cerr << "failed to open index.html file" << std::endl;
	}
	while (!input_file.eof() && input_file >> std::noskipws >> c){
		str += c;
	}
	input_file.close();
	/// image file
	// std::ifstream	input_file;
	// std::string		str;
	// char			c;
	
	// input_file.open("./error_page/Error_404.html");
	// if (!input_file){
	// 	perror("Couldn't open index.html file");
	// 	std::cerr << "failed to open index.html file" << std::endl;
	// }
	// while (!input_file.eof() && input_file >> std::noskipws >> c){
	// 	str += c;
	// }
	// input_file.close();

	// Construct the HTTP response
	std::string httpResponse = "HTTP/1.1 200 OK\r\n";
	httpResponse += "Content-Type: text/html\r\n";
	httpResponse += "Content-Length: " + std::to_string(str.length()) + "\r\n";
	httpResponse += "\r\n"; // End of headers
	httpResponse += str;

	// Send the response over the network connection
	strcpy(server_message, httpResponse.c_str());

	if (send(newSocket, server_message, strlen(server_message), 0) < 0){
		perror("Couldn't send");
		std::cerr << "Couldn't send message at " << newSocket << " server fd socket" << std::endl;
	}
	std::cout << GREEN << "[ ✅ ] Msg sent to client!" << RESET << std::endl;

	std::cout << MAGENTA << " --------------------------------- " << RESET << std::endl;
	std::cout << std::endl;

	close(newSocket);
}

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
