/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PutResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:31:26 by suchua            #+#    #+#             */
/*   Updated: 2023/10/03 04:09:00 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PutResponse.hpp"

PutResponse::PutResponse(const HttpRequest& req, const int& clientSocket, const Location& target)
:_req(req), _clientSocket(clientSocket), _target(target)
{
}

PutResponse::~PutResponse() {}

PutResponse::PutResponse(const PutResponse& other)
:_req(other._req), _clientSocket(other._clientSocket), _target(other._target) {*this = other;}

PutResponse&	PutResponse::operator=(const PutResponse& other)
{
	if (this == &other)
		return (*this);
	return (*this);
}

std::string	PutResponse::getResponse() const
{
	std::string			header;
	std::stringstream	res;

	res << this->_req.getHttpStatusMsg().find(200)->second;
	return (res.str());
}
