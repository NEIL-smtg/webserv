/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerBlock.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 22:26:46 by suchua            #+#    #+#             */
/*   Updated: 2023/09/11 17:30:40 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_BLOCK_HPP__
# define __SERVER_BLOCK_HPP__

# include "Webserv.hpp"

class ServerBlock
{
	private:
		typedef std::string	str;
		str		name;
		int		port;
		str		root;
		str		index;
		std::vector<str>	methods;

	public:
		ServerBlock();
		~ServerBlock();
		ServerBlock(const ServerBlock& other);
		ServerBlock& operator=(const ServerBlock& other);

		void	addMethod(str method);
		void	setName(str name);
		void	setPort(int port);
		void	setRoot(str root);
		void	setIndex(str index);

		str		getName() const;
		int		getPort() const;
		str		getRoot() const;
		str		getIndex() const;
		std::vector<str>	getMethods() const;
		empty	somethingEmpty();
};

std::ostream&	operator<<(std::ostream& out, ServerBlock& sb);

#endif
