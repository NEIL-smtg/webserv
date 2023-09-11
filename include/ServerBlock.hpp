/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <lzi-xian@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 22:26:46 by suchua            #+#    #+#             */
/*   Updated: 2023/09/11 18:57:45 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_BLOCK_HPP__
# define __SERVER_BLOCK_HPP__

# include "Webserv.hpp"
# include "Location.hpp"

class ServerBlock
{
	private:
		typedef std::string	str;
		str		name;
		int		port;
		str		root;
		str		index;
		std::vector<str>	methods;
		std::vector<Location>	location;

	public:
		ServerBlock();
		~ServerBlock();
		ServerBlock(const ServerBlock& other);
		ServerBlock& operator=(const ServerBlock& other);

		void	addMethod(str method);
		void	addLocation(Location location);
		void	setName(str name);
		void	setPort(int port);
		void	setRoot(str root);
		void	setIndex(str index);
		empty	somethingEmpty();
		void	getMethods();
};

#endif
