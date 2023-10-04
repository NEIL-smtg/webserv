/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestErrorHandling.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 15:50:33 by suchua            #+#    #+#             */
/*   Updated: 2023/10/04 11:45:25 by mmuhamad         ###   ########.fr       */
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
		Location			_currLoc;
		Location			_target;
		std::vector<std::string>			_reqPath;

		void			generateErrResponse(int statusCode);
		bool			urlPathFound();
		bool			allowMethod();
		bool			validContent();
		bool			validContentLen(std::string contentLen);
		void			tokennizeReqUrlPath();
		bool			validBoundary(std::string boundary);
	public:
		RequestErrorHandling(const HttpRequest& _req, const ServerBlock& _sb);
		~RequestErrorHandling();
		RequestErrorHandling(const RequestErrorHandling& other);
		RequestErrorHandling&	operator=(const RequestErrorHandling& other);

		bool			ErrorHandler();
		std::string		getErrResponse() const;
		void			setTargetBlock();
		Location		getTargetBlock() const;
};

#endif
