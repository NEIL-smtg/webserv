/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 23:25:25 by suchua            #+#    #+#             */
/*   Updated: 2023/09/11 19:36:37 by suchua           ###   ########.fr       */
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

		std::vector<std::string>	token;
		std::vector<ServerBlock>	block;
		
		void	tokennize(std::string line);
		void	tokenValidation();
		void	serverCheck(iterator &i);
		int		getPort(iterator i);
		void	setMethod(iterator &i, ServerBlock& sb);
		void	pathValidation();
	public:
		Parse(std::string fileName);
		~Parse();
		Parse(const Parse& other);
		Parse& operator=(const Parse& other);
};

#endif
