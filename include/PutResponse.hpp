/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:15:38 by suchua            #+#    #+#             */
/*   Updated: 2023/09/16 01:13:52 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PUT_RESPONSE__HPP__
# define __PUT_RESPONSE__HPP__

# include "Webserv.hpp"
# include "HttpRequest.hpp"

class	HttpRequest;

class	PutResponse
{
	typedef std::string	str;

	private:
		const HttpRequest&	_req;
		const int&			_clientSocket;
		const ServerBlock&	_sb;
		Location			_location;
		str					_response;

		bool	urlPathFound();
		bool	methodAllowed();
	public:
		PutResponse(const HttpRequest& req, const int& clientSocket, const ServerBlock& sb);
		~PutResponse();
		PutResponse(const PutResponse& other);
		PutResponse&	operator=(const PutResponse& other);
		
		str		getResponse() const;
};

std::string		generateErrorResponse(const ServerBlock& sb, const httpError errNum);

#endif
