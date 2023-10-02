/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:00:18 by suchua            #+#    #+#             */
/*   Updated: 2023/10/02 15:42:35 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef __GET_RESPONSE_HPP__
# define __GET_RESPONSE_HPP__

# include "Webserv.hpp"
# include "ServerBlock.hpp"
# include "HttpRequest.hpp"

class	HttpRequest;

class GetResponse
{
	typedef std::string	str;
	private:
		str		_response;
		str		_path;

		bool	isImgFile(std::string &file);
		str		sendFile(std::string &path);
		bool	isLocation(std::string &path, ServerBlock sb);
		str		locationRequest(std::string &path, ServerBlock sb);
	public:
		GetResponse(const HttpRequest req, const ServerBlock sb);
		~GetResponse();
		GetResponse(const GetResponse& other);
		GetResponse&	operator=(const GetResponse& other);
		
		str	getResponse() const;
};


#endif
