/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:24:59 by suchua            #+#    #+#             */
/*   Updated: 2023/09/16 00:01:27 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	__HTTP_REQUEST_HPP__
# define __HTTP_REQUEST_HPP__

# include "Webserv.hpp"
# include "ServerBlock.hpp"
# include "GetResponse.hpp"
# include "PutResponse.hpp"

// class	GetResponse;
// class	PutResponse;

class HttpRequest
{
	typedef std::string	str;
	typedef std::map<str, str>	header;
	
	private:
		str			_methodStr;
		str			_path;			//url dir
		str			_body;
		header		_header;
		httpMethod	_methodEnum;
	public:
		HttpRequest();
		HttpRequest(const HttpRequest& other);
		HttpRequest&	operator=(const HttpRequest& other);		
		~HttpRequest();

		void			setMethod(str method);
		void			setPath(str path);
		void			setBody(str body);
		void			setHeader(str head, str content);
		str				getMethodStr() const;
		httpMethod		getMethodEnum() const;
		str				getBody() const;
		header			getHeader() const;
		str				getPath() const;

		void			parseHttpRequest(const str& req);
		str				generateHttpResponse(const str& req, const int newSocket,const ServerBlock sb);
};

#endif
