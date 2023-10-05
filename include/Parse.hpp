/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 23:25:25 by suchua            #+#    #+#             */
/*   Updated: 2023/10/05 17:43:31 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSE_HPP__
# define __PARSE_HPP__

# include "Webserv.hpp"
# include "ServerBlock.hpp"
# include "Location.hpp"

class Parse
{
	private:
		typedef std::vector<std::string>::iterator	iterator;
		typedef std::string str;

		std::vector<std::string>			token;
		std::map<int, ServerBlock>			block;
		std::map<int, int>					_socketFD;
			
		void	tokennize(std::string line);
		void	tokenValidation();
		void	serverCheck(iterator &i);
		void	pathValidation();
		int		getAvailablePort(iterator i);

		void	setMethod(iterator &i, ServerBlock& sb);
		void	setSocketFD(int port, int sockfd);
	public:
		Parse(std::string fileName);
		~Parse();
		Parse(const Parse& other);
		Parse& operator=(const Parse& other);

		std::map<int, ServerBlock>			&getBlock();
		std::map<int, int>					&getSocketFD();
};

#endif
