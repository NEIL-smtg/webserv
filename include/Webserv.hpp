/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 23:23:31 by suchua            #+#    #+#             */
/*   Updated: 2023/09/16 01:13:19 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WEBSERV_HPP__
# define __WEBSERV_HPP__

// streamm
# include <iostream>
# include <fstream>
# include <sstream>

//	std lib
# include <exception>
# include <string>
# include <cstring>
# include <cstdlib>
# include <cstdio>
# include <unistd.h>

//	container
# include <vector>
# include <map>
# include <algorithm>

//	networking
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# define	LOCAL_HOST				"127.0.0.1"
# define	DEFAULT_CLIENT_SIZE		42

//	HTTP ERROR METHOD MSG
# define	ERR_404_HEAD			"HTTP/1.1 404 Not Found\r\n"
# define	ERR_405_HEAD			"HTTP/1.1 405 Not Allowed\r\n"

# ifndef	UINT16_MAX
#  define	UINT16_MAX	65536
# endif

# define	RESET		"\001\033[0;39m\002"
# define	RED			"\001\033[1;91m\002"
# define	GREEN		"\001\033[1;32m\002"
# define	YELLOW		"\001\033[1;93m\002"
# define	MAGENTA		"\001\033[1;95m\002"
# define	GRAY		"\001\033[1;90m\002"
# define	BLUE		"\033[1;34m"

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

typedef enum httpMethod
{
	GET, HEAD, DELETE, OPTIONS, POST, PUT, TRACE
}	httpMethod;

typedef enum httpError
{
	NOT_FOUND		= 404,
	NOT_ALLOWED		= 405
}	httpError;

bool	isNum(std::string line);
bool	isMethod(std::string method);
bool	isHead(std::string line);

#endif
