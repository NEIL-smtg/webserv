/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 13:29:51 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/09/17 20:33:47 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __POST_RESPONSE__HPP__
# define __POST_RESPONSE__HPP__

# include "Webserv.hpp"
# include "HttpRequest.hpp"

class	HttpRequest;

class	PostResponse
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
		PostResponse(const HttpRequest& req, const int& clientSocket, const ServerBlock& sb);
		~PostResponse();
		PostResponse(const PostResponse& other);
		PostResponse&	operator=(const PostResponse& other);
		
		str		getResponse() const;
};

// std::string		generateErrorResponse(const ServerBlock& sb, const httpError errNum);

#endif
