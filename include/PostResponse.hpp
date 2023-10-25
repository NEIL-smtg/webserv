/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 13:29:51 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/10/04 19:25:26 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __POST_RESPONSE__HPP__
# define __POST_RESPONSE__HPP__

# include "Webserv.hpp"
# include "HttpRequest.hpp"
# include "RequestErrorHandling.hpp"

class	HttpRequest;
class	RequestErrorHandling;

class	PostResponse
{
	typedef std::string	str;

	private:
		const HttpRequest&	_req;
		const int&			_clientSocket;
		const Location&	_sb;
		str					_response;

	public:
		PostResponse(const HttpRequest& req, const int& clientSocket, const Location& sb, RequestErrorHandling err);
		~PostResponse();
		PostResponse(const PostResponse& other);
		PostResponse&	operator=(const PostResponse& other);
		
		str		getResponse() const;
};

#endif
