/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:28:08 by suchua            #+#    #+#             */
/*   Updated: 2023/09/27 17:59:41 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
	fd_set	readFds;
	int		maxFD;
	int		sockfd;
	int		port;
	int		activity;
	int		newSocket;

	std::cout << std::endl;
	std::cout << MAGENTA << " --------------------------------- " << RESET << std::endl;
	std::cout << std::endl;
	while (true)
	{
		FD_ZERO(&readFds);

		maxFD = -1;
		for (ite = _socketFD.begin(); ite != _socketFD.end(); ite++)
		{
			sockfd = ite->second;
			FD_SET(sockfd, &readFds);
			if (sockfd > maxFD)
				maxFD = sockfd;
		}

		activity = select(maxFD + 1, &readFds, NULL, NULL, NULL);
		if (activity == -1)
		{
			perror("select");
			exit(EXIT_FAILURE);
		}

		std::map<int, ServerBlock>::const_iterator	it;
		
		for (it = _conf.begin(); it != _conf.end(); it++)
		{
			port = it->first;
			sockfd = _socketFD.find(port)->second;
			
			if (FD_ISSET(sockfd, &readFds))
			{
				struct sockaddr_in	clientAddr;
				socklen_t			addrLen;
				
				memset(&clientAddr, 0, sizeof(clientAddr));
				addrLen = sizeof(clientAddr);
				newSocket = accept(sockfd, (struct sockaddr *) &clientAddr, &addrLen);
				
				if (newSocket == -1)
				{
					perror("accept");
					std::cerr << "Failed to accept new connection at port " << port << std::endl;
				}
				else
				{
					runRequest(clientAddr, newSocket, it->second);
				}
			}
		}
	}
}

void	Server::runRequest(struct sockaddr_in&	clientAddr, int newSocket, ServerBlock sb)
{

	char				server_message[1024];
	char				client_message[1024];
	int					receivedBytes;
	std::string 		receivedData;
	const int			port = sb.getPort();
 	
	memset(server_message, 0, 1024);

	std::cout << GREEN << "[ ✅ ] New connection" << YELLOW << " : client with IP "  << BLUE << inet_ntoa(clientAddr.sin_addr) << YELLOW ;
	std::cout << ", accepted on port " << BLUE << port << RESET << " ==> "<< MAGENTA << "FD -> " << newSocket << RESET << std::endl;

	while (1)
	{
		memset(client_message, 0, 8);
		receivedBytes = recv(newSocket, client_message, 8, 0);
		if (receivedBytes == -1)
		{
			perror("Couldn't receive");
			std::cerr << "Couldn't receive message at " << newSocket << " client fd socket" << std::endl;
			return ;
		}
		receivedData.append(client_message, receivedBytes);
		if (receivedBytes < 8)
			break ;
	}

	std::cout << YELLOW << "[ * ]  Msg from client: \n\n" << RESET << std::endl;
	std::cout << receivedData;
	
	std::string	httpResponse = this->_httpReq.generateHttpResponse(receivedData, newSocket, sb);
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
		return ;
	}
	std::cout << GREEN << "[ ✅ ] Msg sent to client!" << RESET << std::endl;
	std::cout << std::endl;

	std::cout << MAGENTA << " --------------------------------- " << RESET << std::endl;
	std::cout << std::endl;

	close(newSocket);
}


/***********************************
 * Constructors & Destructors
***********************************/

Server::Server(std::map<int, ServerBlock>& conf, std::map<int, int>& socketFD)
: _conf(conf), _socketFD(socketFD) {this->_httpReq = HttpRequest();}

Server::Server(const Server& other)
: _conf(other._conf), _socketFD(other._socketFD) {*this = other;}

Server&	Server::operator=(const Server& other)
{
	if (this == &other)
		return (*this);
	return (*this);
}

Server::~Server(){}
