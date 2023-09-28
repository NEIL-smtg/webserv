/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestErrorHandling.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:50:33 by suchua            #+#    #+#             */
/*   Updated: 2023/09/27 18:32:38 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __REQUEST_ERROR_HANDLING__
# define __REQUEST_ERROR_HANDLING__

# include "Webserv.hpp"
# include "HttpRequest.hpp"
# include "ServerBlock.hpp"

class RequestErrorHandling
{
	private:
		const HttpRequest&	_req;
		const ServerBlock&	_sb;
		std::string			_errResponse;
		Location			*_currLoc;
		const std::map<int, std::string>	_httpStatusMsg;
		
		void			generateErrResponse(int statusCode);
		bool			urlPathFound();
		bool			allowMethod();
		bool			validContent();
		bool			validContentLen(std::string contentLen);
	public:
		RequestErrorHandling(const HttpRequest& _req, const ServerBlock& _sb);
		~RequestErrorHandling();
		RequestErrorHandling(const RequestErrorHandling& other);
		RequestErrorHandling&	operator=(const RequestErrorHandling& other);
		
		std::string		getErrResponse() const;
};

#endif
