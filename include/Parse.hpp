/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 23:25:25 by suchua            #+#    #+#             */
/*   Updated: 2023/09/12 19:48:17 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSE_HPP__
# define __PARSE_HPP__

# include "Webserv.hpp"
# include "ServerBlock.hpp"
# include "Location.hpp"

typedef enum	conf
{
	SERVER_NAME,
	LISTEN,
	ROOT,
	INDEX,
	ALLOW_METHOD,
	LOCATION,
	NONE
}	conf;

class Parse
{
	private:
		typedef std::vector<std::string>::iterator	iterator;
		typedef std::string str;

		std::vector<std::string>			token;
		std::vector<ServerBlock>			block;
		std::map<int, struct sockaddr_in>	_socketAddr;
		std::map<int, int>					_socketFD;
			
		void	tokennize(std::string line);
		void	tokenValidation();
		void	serverCheck(iterator &i);
		void	pathValidation();

		void	setMethod(iterator &i, ServerBlock& sb);
		void	setSocketAddr(int port, struct sockaddr_in addr);
		void	setSocketFD(int port, int sockfd);

		int		getPort(iterator i);
		std::map<int, struct sockaddr_in>	&getSocketAddr();
		std::map<int, int>					&getSocketFD();
	public:
		Parse(std::string fileName);
		~Parse();
		Parse(const Parse& other);
		Parse& operator=(const Parse& other);
		std::vector<ServerBlock>&	getBlock();
};

#endif
