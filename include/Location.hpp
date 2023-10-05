/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 00:51:12 by suchua            #+#    #+#             */
/*   Updated: 2023/10/05 18:06:16 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __LOCATION_HPP__
# define __LOCATION_HPP__

# include "Webserv.hpp"

class Location
{
	private:
		std::string					directory;
		std::string					root;
		std::string					index;
		std::string					cgiScript;
		std::string					upload;
		std::map<int, std::string>	errorPage;
		int							clientMaxBodySize;
		int							clientMinBodySize;
		bool						autoIndex;
		std::vector<std::string>	methods;
		bool						init;
	public:
		Location();
		Location(std::vector<std::string>::iterator &i, std::vector<std::string> &token);
		~Location();
		Location(const Location& other);
		Location& operator=(const Location& other);

		void	setDirectory(std::string directory);
		void	setRoot(std::string root);
		void	setIndex(std::string index);
		void	setCgiScript(std::string cgiScript);
		void	addErrorPage(std::vector<std::string>::iterator &i);
		void	setClientMaxBodySize(int clientMaxBodySize);
		void	setClientMinBodySize(int clientMinBodySize);
		void	setErrorPage(std::map<int, std::string> errorPage);
		void	setMethods(std::vector<std::string> methods);
		void	validateAutoIndex(std::string _auto);
		void	setAutoIndex(bool _auto);
		void	setInit();
		void	setUpload(std::string uploadPath);
		void	setBodySize(std::string value, conf minMax);
		void	addLimitExcept(std::vector<std::string>::iterator &i, std::vector<std::string> &token);
	
		std::string					getDirectory() const;
		std::string					getRoot() const;
		std::string					getIndex() const;
		std::string					getCgiScript() const;
		int							getClientMaxBodySize() const;
		int							getClientMinBodySize() const;
		bool						getAutoIndex() const;
		bool						isInit() const;
		std::map<int, std::string>	getErrorPage() const;
		std::vector<std::string>	getMethods() const;
		std::string					getUpload() const;
};

std::ostream&	operator<<(std::ostream& out, Location& loc);

#endif
