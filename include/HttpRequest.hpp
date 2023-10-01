/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:24:59 by suchua            #+#    #+#             */
/*   Updated: 2023/10/01 02:13:11 by suchua           ###   ########.fr       */
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
# include "RequestErrorHandling.hpp"

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
		std::map<int, str>	_httpStatusMsg;
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
		std::map<int, str>	getHttpStatusMsg() const;

		void			parseHttpRequest(const str& req);
		str				generateHttpResponse(const str& req, const int newSocket,const ServerBlock sb);
};

#endif
