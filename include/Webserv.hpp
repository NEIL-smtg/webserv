/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <lzi-xian@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 23:23:31 by suchua            #+#    #+#             */
/*   Updated: 2023/09/11 18:38:53 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WEBSERV_HPP__
# define __WEBSERV_HPP__

# include <iostream>
# include <fstream>
# include <exception>
# include <string>
# include <cstring>
# include <cstdlib>
# include <cstdio>
# include <vector>
# include <algorithm>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>

# define LOCAL_HOST		"127.0.0.1"

class InvalidFileException : public std::exception
{
	private:
		const char *msg;
	public:
		InvalidFileException(const char *msg):msg(msg){}
		const char *what() const throw(){return this->msg;}
};

enum empty
{
	EMPTY_NAME,
	EMPTY_PORT,
	EMPTY_ROOT,
	EMPTY_INDEX,
	EMPTY_METHOD,
	FILLED
};

#endif
