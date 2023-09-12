/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:02:28 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/09/12 19:30:30 by lzi-xian         ###   ########.fr       */
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
		std::string					directory;
		std::string					root;
		std::string					index;
		std::string					cgi_script;
		std::map<int, std::string>	error_page;
		int							client_max_body_size;
		std::vector<std::string>	methods;
	public:
		Location(std::vector<std::string>::iterator &i, std::vector<std::string> &token);
		~Location();
		Location(const Location& other);
		Location& operator=(const Location& other);

		void	setDirectory(std::string directory);
		void	setRoot(std::string root);
		void	setIndex(std::string index);
		void	setCgiScript(std::string cgi_script);
		void	addErrorPage(std::vector<std::string>::iterator &i);
		void	getSize(std::string size);
		void	setClientMaxBodySize(int client_max_body_size);
		void	addLimitExcept(std::vector<std::string>::iterator &i, std::vector<std::string> &token);
		void	printLimitExcept();
		void	printErrorPage();
		
		std::string	getDirectory() const;
		std::string	getRoot() const;
		std::string	getIndex() const;
		std::string	getCgiScript() const;
		int	getClientMaxBodySize() const;
		std::map<int, std::string>	getErrorPage() const;
		std::vector<std::string>	getMethods() const;
};

std::ostream&	operator<<(std::ostream& out, Location& loc);

#endif
