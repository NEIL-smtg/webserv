/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 00:51:12 by suchua            #+#    #+#             */
/*   Updated: 2023/11/06 18:15:02 by mmuhamad         ###   ########.fr       */
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
	LC_CLIENT_MIN_BODY_SIZE,
	LC_AUTOINDEX,
	LC_RETURN,
	LC_LIMIT_EXCEPT,
	LC_NONE
}	locconf;

class Location
{
	private:
		std::string							directory;
		std::string							root;
		std::string							index;
		std::map<std::string, std::string>	cgiScript;
		std::map<int, std::string>			errorPage;
		int									clientMaxBodySize;
		int									clientMinBodySize;
		bool								autoIndex;
		std::vector<std::string>			methods;
		bool								init;
	public:
		Location();
		Location(std::vector<std::string>::iterator &i, std::vector<std::string> &token);
		~Location();
		Location(const Location& other);
		Location& operator=(const Location& other);

		void	setDirectory(std::string directory);
		void	setRoot(std::string root);
		void	setIndex(std::string index);
		void	setCgiScript(std::map<std::string, std::string> cgiScript);
		void	addErrorPage(std::vector<std::string>::iterator &i);
		void	addCgiScript(std::vector<std::string>::iterator &i);
		void	setClientMaxBodySize(int clientMaxBodySize);
		void	setClientMinBodySize(int clientMinBodySize);
		void	setErrorPage(std::map<int, std::string> errorPage);
		void	setMethods(std::vector<std::string> methods);
		void	setAutoIndex(bool _auto);
		void	setInit();

		void	getSize(std::string size, locconf minMax);
		void	addLimitExcept(std::vector<std::string>::iterator &i, std::vector<std::string> &token);
		void	printLimitExcept();
		void	printErrorPage();
		
		std::string							getDirectory() const;
		std::string							getRoot() const;
		std::string							getIndex() const;
		std::map<std::string, std::string>	getCgiScript() const;
		int									getClientMaxBodySize() const;
		int									getClientMinBodySize() const;
		bool								getAutoIndex() const;
		std::map<int, std::string>			getErrorPage() const;
		std::vector<std::string>			getMethods() const;
		bool								isInit() const;
};

std::ostream&	operator<<(std::ostream& out, Location& loc);

#endif
