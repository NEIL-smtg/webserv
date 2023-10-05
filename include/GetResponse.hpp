/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GetResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:00:18 by suchua            #+#    #+#             */
/*   Updated: 2023/10/06 01:24:58 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef __GET_RESPONSE_HPP__
# define __GET_RESPONSE_HPP__

# include "Webserv.hpp"
# include "HttpRequest.hpp"

class	HttpRequest;

class GetResponse
{
	private:
		const HttpRequest&			_req;
		const Location&				_target;
		std::stringstream			_response;
		std::string					_contentType;

		void		responseHandler(std::string file);
		void		setImgResponse(std::string pathToFile, std::string type);
		void		setNormalResponse(std::string pathToFile);
	public:
		GetResponse(const HttpRequest& req, const Location& target);
		~GetResponse();
		GetResponse(const GetResponse& other);
		GetResponse&	operator=(const GetResponse& other);
		std::string		getResponse() const;
};


#endif
