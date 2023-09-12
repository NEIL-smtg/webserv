/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:28:08 by suchua            #+#    #+#             */
/*   Updated: 2023/09/13 04:42:30 by suchua           ###   ########.fr       */
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
					std::cout << "New connection accepted" << std::endl;
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