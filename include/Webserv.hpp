/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 23:23:31 by suchua            #+#    #+#             */
/*   Updated: 2023/10/03 17:10:09 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __WEBSERV_HPP__
# define __WEBSERV_HPP__

// streamm
# include <iostream>
# include <fstream>
# include <sstream>
# include <fcntl.h>

//	std lib
# include <exception>
# include <string>
# include <cstring>
# include <cstdlib>
# include <cstdio>
# include <unistd.h>
# include <ctime>

//	container
# include <vector>
# include <map>
# include <algorithm>

//	networking
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# define	LOCAL_HOST				"127.0.0.1"
# define	DEFAULT_CLIENT_MAX_SIZE	1048576 
# define	DEFAULT_CLIENT_MIN_SIZE	0

# ifndef	UINT16_MAX
#  define	UINT16_MAX				65536
# endif

# define	RESET					"\001\033[0;39m\002"
# define	RED						"\001\033[1;91m\002"
# define	GREEN					"\001\033[1;32m\002"
# define	YELLOW					"\001\033[1;93m\002"
# define	MAGENTA					"\001\033[1;95m\002"
# define	GRAY					"\001\033[1;90m\002"
# define	BLUE					"\033[1;34m"

class InvalidFileException : public std::exception
{
	private:
		const char *msg;
	public:
		InvalidFileException(const char *msg):msg(msg){}
		const char *what() const throw(){return this->msg;}
};

typedef enum httpMethod
{
	GET, HEAD, DELETE, OPTIONS, POST, PUT, TRACE
}	httpMethod;

typedef enum httpError
{
	SWITCHING_PORT	= 101,
	OK				= 200,
	CREATED			= 201,
	ACCEPTED		= 202,
	NO_CONTETNT		= 204,
	BAD_REQUEST		= 400,
	NOT_FOUND		= 404,
	NOT_ALLOWED		= 405,
	NOT_ACCEPTABLE	= 406,
	AUTH_REQUIRED	= 407,
	REQUEST_TIMEOUT	= 408,
	LENGTH_REQUIRED	= 411,
	URI_TOO_LONG	= 414,
	UNSUPPORTED_MEDIA_TYPE	= 415,
	SERER_ERR		= 500,
	NOT_IMPLEMENTED	= 501,
	BAD_GATEWAY		= 502,
	GATEWAY_TIMEOUT	= 504,
	HTTP_VER_NOT_SUPPORTED	= 505
}	httpError;

enum empty
{
	EMPTY_NAME,
	EMPTY_PORT,
	EMPTY_ROOT,
	EMPTY_INDEX,
	EMPTY_METHOD,
	FILLED
};

//	UTILS
bool	isNum(std::string line);
bool	isMethod(std::string method);
bool	isHead(std::string line);

#endif
