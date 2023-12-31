/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 22:26:46 by suchua            #+#    #+#             */
/*   Updated: 2023/09/13 16:27:54 by lzi-xian         ###   ########.fr       */
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
		str			name;
		int			port;
		str			root;
		str			index;
		std::map<int, std::string>	error_page;
		std::vector<str>		methods;
		std::vector<Location>	location;

	public:
		ServerBlock();
		~ServerBlock();
		ServerBlock(const ServerBlock& other);
		ServerBlock& operator=(const ServerBlock& other);

		void	addMethod(str method);
		void	addErrorPage(std::vector<std::string>::iterator &i);
		void	addLocation(Location& location);
		void	setName(str name);
		void	setPort(int port);
		void	setRoot(str root);
		void	setIndex(str index);

		str		getName() const;
		int		getPort() const;
		str		getRoot() const;
		str		getIndex() const;
		std::map<int, std::string>	getErrorPage() const;
		std::vector<str>		getMethods() const;
		std::vector<Location>	getLocation() const;
		empty	somethingEmpty();
		void	printMethods();
};

std::ostream&	operator<<(std::ostream& out, ServerBlock& sb);

#endif
