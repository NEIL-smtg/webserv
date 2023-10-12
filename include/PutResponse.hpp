/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:15:38 by suchua            #+#    #+#             */
/*   Updated: 2023/10/12 16:49:03 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PUT_RESPONSE__HPP__
# define __PUT_RESPONSE__HPP__

# include "Webserv.hpp"
# include "HttpRequest.hpp"
# include "RequestErrorHandling.hpp"

class	HttpRequest;
class	RequestErrorHandling;

class	PutResponse
{
	typedef std::string	str;

	private:
		const HttpRequest&	_req;
		const int&			_clientSocket;
		const Location&		_sb;
		str					_response;

	public:
		PutResponse(const HttpRequest& req, const int& clientSocket, const Location& sb, RequestErrorHandling err);
		~PutResponse();
		PutResponse(const PutResponse& other);
		PutResponse&	operator=(const PutResponse& other);
		
		str		getResponse() const;
};

#endif
