/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/18 21:18:15 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef	__HTTP_REQUEST_HPP__
# define __HTTP_REQUEST_HPP__

# include "Webserv.hpp"
# include "ServerBlock.hpp"
# include "GetResponse.hpp"
# include "PutResponse.hpp"
# include "PostResponse.hpp"

// class	GetResponse;
// class	PutResponse;

class HttpRequest
{
	typedef std::string			str;
	typedef std::map<str, str>	header;
	
	private:
		str					_methodStr;
		str					_path;			//url dir
		str					_body;
		header				_header;
		httpMethod			_methodEnum;
		std::map<httpError, str>	_HttpErrorMsg;
	public:
		HttpRequest();
		HttpRequest(const HttpRequest& other);
		HttpRequest&	operator=(const HttpRequest& other);		
		~HttpRequest();

		void			setMethod(str method);
		void			setPath(str path);
		void			setBody(str body);
		void			setHeader(str head, str content);
		void			setContentDisposition(str line);
		str				getMethodStr() const;
		httpMethod		getMethodEnum() const;
		str				getBody() const;
		header			getHeader() const;
		str				getPath() const;
		str				getContentDisposition() const;
		std::map<httpError, str>	getHttpErrorMsg() const;

		void			parseHttpRequest(const str& req);
		str				generateHttpResponse(const str& req, const int newSocket,const ServerBlock sb);
};

#endif
