/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/09/25 18:25:31 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef	__HTTP_REQUEST_HPP__
# define __HTTP_REQUEST_HPP__

# include "Webserv.hpp"
# include "ServerBlock.hpp"
# include "GetResponse.hpp"
# include "PutResponse.hpp"
# include "PostResponse.hpp"
# include "DeleteResponse.hpp"

// class	GetResponse;
// class	PutResponse;

class HttpRequest
{
	typedef std::string			str;
	typedef std::map<str, str>	header;
    typedef std::vector<str>	body;
	
	private:
		str					_methodStr;
		str					_path;			//url dir
		body				_body;
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
		void			setBody(str line);
		void			setHeader(str head, str content);
		str				getMethodStr() const;
		httpMethod		getMethodEnum() const;
		body			getBody() const;
		header			getHeader() const;
		str				getPath() const;
		std::map<httpError, str>	getHttpErrorMsg() const;

		void			parseHttpRequest(const str& req);
		str				generateHttpResponse(const str& req, const int newSocket,const ServerBlock sb);
};

#endif
