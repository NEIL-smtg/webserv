/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:24:59 by suchua            #+#    #+#             */
/*   Updated: 2023/10/06 04:00:49 by suchua           ###   ########.fr       */
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
		std::vector<str>	_tokennizePath;
		std::vector<str>	_dataKeys;
		std::vector<str>	_dataVal;
		std::string			_boundary;
		std::vector<str>	_formBoundary;

		void	clearAll();
		void				tokennizeReqUrlPath();
		std::vector<std::string>	tokennizer(const std::string line, const std::string sep);
		bool	setWWWFormData(std::string line);
		void	setBoundaryFormData(std::string line);
	public:
		HttpRequest();
		HttpRequest(const HttpRequest& other);
		HttpRequest&	operator=(const HttpRequest& other);		
		~HttpRequest();

		void				setMethod(str method);
		void				setPath(str path);
		void				setBody(str line);
		void				setHeader(str head, str content);
		str					getMethodStr() const;
		httpMethod			getMethodEnum() const;
		body				getBody() const;
		header				getHeader() const;
		str					getPath() const;
		std::map<int, str>	getHttpStatusMsg() const;
		std::vector<str>	getTokennizePath() const;
		std::vector<str>	getDataKeys() const;
		std::vector<str>	getDataValues() const;
		str					getBoundary() const;
		std::vector<str>	getFormBoundary() const;

		void			parseHttpRequest(const str& req);
		str				generateHttpResponse(const str& req, const int clientSocket,const ServerBlock sb);
};

#endif
