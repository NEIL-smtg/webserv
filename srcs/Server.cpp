/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:28:08 by suchua            #+#    #+#             */
/*   Updated: 2023/09/13 16:58:19 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::startServer()
{
	std::map<int, int>::const_iterator	it;
	int	sockfd;

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
		
		char server_message[2000], client_message[2000];
    
		// Clean buffers:
		memset(server_message, '\0', sizeof(server_message));
		memset(client_message, '\0', sizeof(client_message));

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
					std::cout << GREEN << "[ ✅ ] New connection" << YELLOW << " : client with IP "  << BLUE << inet_ntoa(clientAddr.sin_addr) << YELLOW ;
					std::cout << ", accepted on port " << BLUE << port << RESET << " ==> "<< MAGENTA << "FD -> " << newSocket << RESET << std::endl;

					if (recv(newSocket, client_message, sizeof(client_message), 0) < 0)
					{
						perror("Couldn't receive");
						std::cerr << "Couldn't receive message at " << newSocket << " client fd socket" << std::endl;
                	}

					std::cout << YELLOW << "[ * ] Msg from client: \n" << RESET << client_message << std::endl;

					strcpy(server_message, "This is the server's message.");
                
					if (send(newSocket, server_message, strlen(server_message), 0) < 0){
						perror("Couldn't send");
						std::cerr << "Couldn't send message at " << newSocket << " server fd socket" << std::endl;
					}

					close(newSocket);
				}
			}
		}
	}
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
