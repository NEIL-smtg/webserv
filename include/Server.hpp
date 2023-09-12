/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 21:20:51 by suchua            #+#    #+#             */
/*   Updated: 2023/09/12 04:10:10 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include "ServerBlock.hpp"

class Server
{
	private:
		const std::vector<ServerBlock>&	_conf;
	public:
		Server(std::vector<ServerBlock>& conf);
		~Server();
		Server(const Server& other);
		Server&	operator=(const Server& other);
		void	startServer();
};

#endif
