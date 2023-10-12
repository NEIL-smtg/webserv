/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TraceResponse.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:15:38 by suchua            #+#    #+#             */
/*   Updated: 2023/10/12 14:25:26 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __TRACE_RESPONSE__HPP__
# define __TRACE_RESPONSE__HPP__

# include "Webserv.hpp"
# include "HttpRequest.hpp"

class	HttpRequest;
class	RequestErrorHandling;

class	TraceResponse
{
	typedef std::string	str;

	private:
		const HttpRequest&	_req;
		const int&			_clientSocket;
		const Location&		_sb;
		Location			_location;
		str					_response;

		// bool	urlPathFound();
		// bool	methodAllowed();
		// bool	validContentType();

		// str		generateErrorResponse(const httpError err);
	public:
		TraceResponse(const HttpRequest& req, const int& clientSocket, const Location& sb, RequestErrorHandling err);
		~TraceResponse();
		TraceResponse(const TraceResponse& other);
		TraceResponse&	operator=(const TraceResponse& other);
		
		str		getResponse() const;
};

#endif
