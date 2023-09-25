/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteResponse.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:15:38 by suchua            #+#    #+#             */
/*   Updated: 2023/09/25 17:44:51 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __DELETE_RESPONSE__HPP__
# define __DELETE_RESPONSE__HPP__

# include "Webserv.hpp"
# include "HttpRequest.hpp"

class	HttpRequest;

class	DeleteResponse
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
		bool	validContentType();

		str		generateErrorResponse(const httpError err);
	public:
		DeleteResponse(const HttpRequest& req, const int& clientSocket, const ServerBlock& sb);
		~DeleteResponse();
		DeleteResponse(const DeleteResponse& other);
		DeleteResponse&	operator=(const DeleteResponse& other);
		
		str		getResponse() const;
};

#endif
