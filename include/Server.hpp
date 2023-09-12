/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:20:51 by suchua            #+#    #+#             */
/*   Updated: 2023/09/12 19:44:02 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "ServerBlock.hpp"
# include "Webserv.hpp"
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <netinet/in.h>

class Server
{
	private:
		typedef std::vector<const ServerBlock>::iterator	iterator;
		const std::vector<ServerBlock>&				_conf;
		const std::map<int, struct sockaddr_in>&	_socketAddr;
		const std::map<int, int>&					_socketFD;
	public:
		Server(std::vector<ServerBlock>& conf, std::map<int, struct sockaddr_in>& _socketAddr, std::map<int, int>& _socketFD);
		~Server();
		Server(const Server& other);
		Server&	operator=(const Server& other);
		void	startServer();

};

#endif
