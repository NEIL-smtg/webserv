/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 13:31:31 by lzi-xian          #+#    #+#             */
/*   Updated: 2023/09/17 20:43:52 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "PostResponse.hpp"

PostResponse::PostResponse(const HttpRequest& req, const int& clientSocket, const ServerBlock& sb)
:_req(req), _clientSocket(clientSocket), _sb(sb)
{
	if (!urlPathFound() || !methodAllowed())
		return ;
	//check content type: (Error 400)
	//check for is it multipart/form-data (Error 400)
	//check content length exceed max/min (Error 413/Error 400)
	//path = strjoin server path and request path
	//check path is end with / or not if not add
	//get boundary numbers from content type content line (Error 400)
		/*	THIS IS AN EXAMPLE
			Content-Type: multipart/form-data; boundary=--------------------------764881834332229428462343
		*/
	//treat body part can split with the boundary numbers
		//then iter through to find "name"
		/*	THIS IS AN EXAMPLE
			----------------------------764881834332229428462343
			Content-Disposition: form-data; name="filename"
			newfile.txt
		*/
		//if found, can do map<string, string> (map["filename"] = "newfile.txt")
		//if any fails like invalid mapping response error (Error 400)
	//check for is there a map["filename"] && map["content"] (Error 400/Error 500(No content to fill))
	//if target file dont exist (Error 400) (IGNORE THIS IN PUT)
	//do uploading ifstream and ofstream (Error 500)
	//return response (200, Success)
	//Note: (Error 400) = give reponse on error code 400 if fails
}

bool	PostResponse::urlPathFound()
{
	if (this->_req.getPath() == "/")
		return (true);

	std::vector<Location>			loc;
	std::vector<Location>::iterator	it;
	
	loc = this->_sb.getLocation();
	for (it = loc.begin(); it != loc.end(); ++it)
	{
		if (it->getDirectory() == this->_req.getPath())
		{
			this->_location = (*it);
			return (true);
		}
	}

	// this->_response = generateErrorResponse(this->_sb, NOT_FOUND);
	return (false);
}

bool	PostResponse::methodAllowed()
{
	str							reqMethod;
	std::vector<str>			validMethods;
	std::vector<str>::iterator	it;

	reqMethod = this->_req.getMethodStr();
	validMethods = _location.getMethods();
	for (it = validMethods.begin(); it != validMethods.end(); it++)
	{
		if ((*it) == reqMethod)
			return (true);
	}
	// this->_response = generateErrorResponse(this->_sb, NOT_ALLOWED);
	return (false);
}

PostResponse::~PostResponse() {}

PostResponse::PostResponse(const PostResponse& other)
:_req(other._req), _clientSocket(other._clientSocket), _sb(other._sb) {*this = other;}

PostResponse&	PostResponse::operator=(const PostResponse& other)
{
	if (this == &other)
		return (*this);
	this->_response = other._response;
	return (*this);
}

std::string	PostResponse::getResponse() const {return this->_response;}

// std::string	generateErrorResponse(const ServerBlock& sb, const httpError errNum)
// {
// 	std::string			errHtmlFilePath;
// 	std::string			line;
// 	std::stringstream	response;
// 	std::ifstream		errHtml;
// 	std::stringstream	body;

// 	errHtmlFilePath = sb.getErrorPage().find(errNum)->second;
// 	errHtml.open(errHtmlFilePath.c_str());
	
// 	while (std::getline(errHtml, line))
// 		body << line;

// 	if (errNum == NOT_FOUND) response << ERR_404_HEAD;
// 	else if (errNum == NOT_ALLOWED) response << ERR_405_HEAD;

// 	response << "Content-Type: " << errHtmlFilePath + "\r\n";
// 	response << "Content-Length: " << body.str().length() << "\r\n\r\n";
// 	response << body.str();
// 	return (response.str());
// }
