/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:28:08 by suchua            #+#    #+#             */
/*   Updated: 2023/10/03 12:25:08 by mmuhamad         ###   ########.fr       */
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
	std::cout << "\n" << MAGENTA << " --------------------------------- " << RESET << "\n\n";
	while (true)
		acceptConnection();
}

void	Server::acceptConnection()
{
	std::map<int, int>::const_iterator	ite;
	fd_set			readFds;
	int				maxFD;
	int				sockfd;
	int				port;
	int				clientSocket;
	struct timeval	timeout;

	FD_ZERO(&readFds);
	maxFD = -1;
	for (ite = _socketFD.begin(); ite != _socketFD.end(); ite++)
	{
		sockfd = ite->second;
		FD_SET(sockfd, &readFds);
		if (sockfd > maxFD)
			maxFD = sockfd;
	}

	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	if (select(maxFD + 1, &readFds, NULL, NULL, &timeout) == -1)
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
			clientSocket = accept(sockfd, (struct sockaddr *) &clientAddr, &addrLen);
			if (clientSocket == -1)
			{
				perror("accept");
				std::cerr << "Failed to accept new connection at port " << port << std::endl;
			}
			FD_SET(clientSocket, &readFds);

			if (clientSocket > maxFD)
				maxFD = clientSocket;

			timeout.tv_sec = 5;
			timeout.tv_usec = 0;

			if (select(maxFD + 1, &readFds, NULL, NULL, &timeout) == -1)
			{
				perror("select");
				exit(EXIT_FAILURE);
			}
			if (FD_ISSET(clientSocket, &readFds))
			{
				if (fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
				{
					std::cerr << "Error setting non-blocking mode\n";
					close(clientSocket);
				}
				else
				{
					runRequest(clientAddr, clientSocket, it->second);
				}
			}
		}
	}
}

void	Server::runRequest(struct sockaddr_in&	clientAddr, int clientSocket, ServerBlock sb)
{
	char				client_message[1024];
	int					receivedBytes;
	std::string 		receivedData;
	const int			port = sb.getPort();

	std::cout << GREEN << "[ ✅ ] New connection" << YELLOW << " : client with IP "  << BLUE << inet_ntoa(clientAddr.sin_addr) << YELLOW ;
	std::cout << ", accepted on port " << BLUE << port << RESET << " ==> "<< MAGENTA << "FD -> " << clientSocket << RESET << std::endl;
	while (1)
	{
		memset(client_message, 0, 24);
		receivedBytes = recv(clientSocket, client_message, 24, 0);
		if (receivedBytes == -1)
		{
			perror("Couldn't receive");
			std::cerr << "Couldn't receive message at " << clientSocket << " client fd socket" << std::endl;
			close(clientSocket);
			return ;
		}
		receivedData.append(client_message, receivedBytes);
		if (receivedBytes < 24)
			break ;
	}

	std::cout << YELLOW << "[ * ]  Msg from client: \n\n" << RESET << std::endl;
	std::cout << receivedData;
	
	
	std::string	httpResponse = this->_httpReq.generateHttpResponse(receivedData, clientSocket, sb);
	if	(httpResponse == "")
	{
		std::cout << RED << "[ ❌ ] Msg not sent to client!" << RESET << std::endl;
		std::cout << std::endl;

		std::cout << MAGENTA << " --------------------------------- " << RESET << std::endl;
		std::cout << std::endl;
		close(clientSocket);
		return ;
	}
	sendResponse(httpResponse, clientSocket);
}

void	Server::sendResponse(std::string response, int clientSocket)
{
	const char	*server_msg = response.c_str();
	ssize_t	msg_len = static_cast<ssize_t>(response.length());
	ssize_t	total_sent = 0;
	ssize_t	sent;

	while (total_sent < msg_len)
	{
		sent = send(clientSocket, server_msg + total_sent, msg_len - total_sent, 0);
		if (sent < 0)
		{
			perror("Couldn't send");
			std::cerr << "Couldn't send message at " << clientSocket << " server fd socket" << std::endl;
			return ;
		}
		if (total_sent == msg_len)
			break ;
		total_sent += sent;
	}
	std::cout << GREEN << "[ ✅ ] Msg sent to client!" << RESET << "\n\n";
	std::cout << MAGENTA << " --------------------------------- " << RESET << "\n\n";
	close(clientSocket);
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
