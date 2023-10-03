/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:00:18 by suchua            #+#    #+#             */
/*   Updated: 2023/10/03 16:32:22 by mmuhamad         ###   ########.fr       */
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
		str		sendFile(std::string &path, Location target);
		// bool	isLocation(std::string &path, Location sb);
		str		locationRequest(std::string &path, Location target);
	public:
		GetResponse(const HttpRequest req, const Location target);
		~GetResponse();
		GetResponse(const GetResponse& other);
		GetResponse&	operator=(const GetResponse& other);
		
		str	getResponse() const;
};


#endif
