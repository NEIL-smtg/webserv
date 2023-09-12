/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:28:08 by suchua            #+#    #+#             */
/*   Updated: 2023/09/12 19:56:35 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::vector<ServerBlock>& conf, std::map<int, struct sockaddr_in>& _socketAddr, std::map<int, int>& _socketFD) : _conf(conf) , _socketAddr(_socketAddr) , _socketFD(_socketFD)
{
	
}

Server::~Server(){}

Server::Server(const Server& other) : _conf(other._conf) , _socketAddr(other._socketAddr) , _socketFD(other._socketFD) {}

Server&	Server::operator=(const Server& other)
{
	if (this == &other)
		return (*this);
	return (const_cast<Server&>(other));
}

void	Server::startServer()
{	
	iterator	end = _conf.end();
	int			serverBlock_num = 0;
	for (iterator	start = _conf.begin(); start < end; start++)
	{
		// _conf.at(serverBlock_num).getPort(); // example of accessing the element

		int socket_desc, client_sock, client_size;
		struct sockaddr_in server_addr, client_addr;
		fd_set rset;
		char server_message[2000], client_message[2000];
		
		// Clean buffers:
		memset(server_message, '\0', sizeof(server_message));
		memset(client_message, '\0', sizeof(client_message));
		
		// Create socket:
		socket_desc = _socketFD.at(_conf.at(serverBlock_num).getPort());
		
		// if(socket_desc < 0){
		// 	printf("Error while creating socket\n");
		// 	return -1;
		// }
		// printf("Socket created successfully, FD %d\n", socket_desc);
		
		// Set port and IP:
		// server_addr.sin_family = AF_INET;
		// server_addr.sin_port = htons(_conf.at(serverBlock_num).getPort());
		// server_addr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
		server_addr = _socketAddr.at(_conf.at(serverBlock_num).getPort());
		
		// Bind to the set port and IP:
		if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		{
			std::cerr << "Couldn't bind to the port" << std::endl;
			throw (InvalidFileException(""));
		}
		printf("Done with binding\n");
		
		// Listen for clients:
		if(listen(socket_desc, 1) < 0){
			std::cerr << "Error while listening" << std::endl;
			throw (InvalidFileException(""));
		}
		printf("\nListening for incoming connections on port 2000 .....\n");
		
		// clear the descriptor set
		FD_ZERO(&rset);
		FD_SET(socket_desc, &rset);

		int *client_fds = (int *)malloc(100 * sizeof(int));
		int max_client = 0;
		int num_avail_fds;
		struct timeval	timeout;

		num_avail_fds = 0;
		
		while (1)
		{
			fd_set			read_fds_copy;

			// while num of avail fds is not 1, keep selecting
			while (!num_avail_fds)
			{
				timeout.tv_sec  = 2;
				timeout.tv_usec = 0;
				memcpy(&read_fds_copy, &rset, sizeof(rset));
				num_avail_fds =  select(6, &read_fds_copy, NULL, NULL, &timeout);
			}
			
			printf("FD_ISSET(3, &read_fds_copy) %d\n",FD_ISSET(3, &read_fds_copy));
			printf("FD_ISSET(4, &read_fds_copy) %d\n",FD_ISSET(4, &read_fds_copy));
			printf("FD_ISSET(5, &read_fds_copy) %d\n",FD_ISSET(5, &read_fds_copy));

			// handle connection
			if (FD_ISSET(socket_desc, &read_fds_copy))
			{
				int addrlen = sizeof(client_addr);
				// Accept an incoming connection:
				client_size = sizeof(client_addr);
				client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, (socklen_t*)&addrlen);

				if (client_sock < 0){
					printf("Can't accept\n");
					continue;
				}
				FD_SET(client_sock, &rset);
				num_avail_fds = 0;
				client_fds[max_client++] = client_sock;
				printf("Client connected at IP: %s and port: %i. FD %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_sock);
				continue;    
			}

			// handle read
			for (size_t i = 0; i < max_client; i++)
			{
				if (FD_ISSET(client_fds[i], &read_fds_copy))
				{
					printf("handling read from fd %d\n", client_fds[i]);
					// Receive client's message:
					if (recv(client_fds[i], client_message, sizeof(client_message), 0) < 0){
						std::cout << "Couldn't receive" << std::endl;
						throw (InvalidFileException(""));;
					}
					printf("Msg from client: %s\n", client_message);

					// after read, write then close connection and remove from fdset
					// Respond to client:
					strcpy(server_message, "This is the server's message.");
					
					if (send(client_fds[i], server_message, strlen(server_message), 0) < 0){
						std::cout << "Couldn't send" << std::endl;
						throw (InvalidFileException(""));
					}

					// Closing the socket:
					close(client_fds[i]);

					int fd_save = client_fds[i];
					// move the sockets back in our client_fds
					for (size_t j = i; j < max_client - 1; j++)
						client_fds[j] = client_fds[j + 1];
					FD_CLR(fd_save, &rset);
					client_fds[max_client--] = -1;
					num_avail_fds = 0;
					break;
				}
			}
			
		}
		
		// closing server socket
		close(socket_desc);
			serverBlock_num ++;
	}
	
	
}
