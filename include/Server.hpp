/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:20:51 by suchua            #+#    #+#             */
/*   Updated: 2023/10/02 18:17:59 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "ServerBlock.hpp"
# include "Webserv.hpp"
# include "HttpRequest.hpp"

class Server
{
	private:
		const std::map<int, ServerBlock>&	_conf;
		const std::map<int, int>&			_socketFD;
		HttpRequest							_httpReq;

		void			acceptConnection();
		void			runRequest(struct sockaddr_in&	clientAddr, int newSocket, ServerBlock sb);
		void			sendResponse(std::string response, int newSocket);
	public:
		Server(std::map<int, ServerBlock>& conf, std::map<int, int>& socketFD);
		~Server();
		Server(const Server& other);
		Server&	operator=(const Server& other);
		void	startServer();

};

#endif
