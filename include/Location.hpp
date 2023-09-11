/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:02:28 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/09/12 01:30:14 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include "Webserv.hpp"

typedef enum	locconf
{
	LC_ROOT,
	LC_INDEX,
	LC_INCLUDE,
	LC_CGI_SCRIPT,
	LC_UPLOAD,
	LC_ERROR_PAGE,
	LC_CLIENT_MAX_BODY_SIZE,
	LC_AUTOINDEX,
	LC_RETURN,
	LC_LIMIT_EXCEPT,
	LC_ALLOW,
	LC_DENY,
	LC_NONE
}	locconf;

class Location
{
	private:
		std::string		name;
		std::string		index;
		std::vector<std::string>	methods;
	public:
		Location(std::vector<std::string>::iterator &i, std::vector<std::string> &token);
		~Location();
		Location(const Location& other);
		Location& operator=(const Location& other);

		void	setIndex(std::string index);
		void	setName(std::string name);
		void	addLimitExcept(std::string method);
		void	printLimitExcept();
		
		std::string	getIndex() const;
		std::string	getName() const;
		std::vector<std::string>	getMethods() const;
};

std::ostream&	operator<<(std::ostream& out, Location& loc);

#endif
