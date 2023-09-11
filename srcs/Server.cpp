/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:28:08 by suchua            #+#    #+#             */
/*   Updated: 2023/09/12 04:26:20 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::vector<ServerBlock>& conf) : _conf(conf){}

Server::~Server(){}

Server::Server(const Server& other) : _conf(other._conf) {}

Server&	Server::operator=(const Server& other)
{
	if (this == &other)
		return (*this);
	return (const_cast<Server&>(other));
}

void	Server::startServer()
{
	int	serverSocket;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1)
	{
		std::cerr << "Error creating server socket." << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(serverSocket, 1) == -1)
	{
		std::cerr << "Error listening to server socket." << std::endl;
		exit(EXIT_FAILURE);
	}

	int	clientSocket;
	sockaddr_in	clientAddr;
	socklen_t	clientAddrLen = sizeof(clientAddr);
	char	buffer[1024];

	while (true)
	{
		clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen);
		if (clientSocket == -1)
		{
			std::cerr << "Error : Failed to accept connection." << std::endl;
			continue ;
		}

		memset(buffer, 0, 1024);
		if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1)
		{
			std::cerr << "Error : Client failed to receive." << std::endl;
			continue ;
		}
		std::cout << buffer << std::endl;

		close(clientSocket);
	}
	close(serverSocket);
}
