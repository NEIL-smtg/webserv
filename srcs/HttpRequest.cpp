/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmuhamad <suchua@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 23:33:01 by suchua            #+#    #+#             */
/*   Updated: 2023/10/24 19:00:10 by mmuhamad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#define TEMP_FILE_IN ".tempIn"
#define TEMP_FILE_OUT ".tempOut"
#define BUFFER_SIZE 100000

/*
	Parse the request string and split it into words
	and store them
*/
void	HttpRequest::parseHttpRequest(const str& req)
{
	std::istringstream	stream(req);
	str	line;

	setfull(req);

	if (std::getline(stream, line))
	{
		std::istringstream	lineStream(line);
		lineStream >> this->_methodStr >> this->_path;
		setMethod(this->_methodStr);
	}

	size_t	separatorPos;
	str		key;
	str		value;

	while (std::getline(stream, line))
	{
		separatorPos = line.find(':');
		if (separatorPos != std::string::npos)
		{
			key = line.substr(0, separatorPos);
			value = line.substr(separatorPos + 2);
			setHeader(key, value);
		}
		if (key == "Content-Type")
			str	boundary = value.substr(value.find_first_of('=') + 1);
		if (key == "Content-Length")
			break ;
	}
	while (std::getline(stream, line))
		setBody(line);
}

bool	isCGI(const HttpRequest req)
{
	if (req.getPath().find(".bla") != std::string::npos)
	{
		return (true);
	}
	return (false);
}

std::string	HttpRequest::generateHttpResponse(const str& req, const int clientSocket, const ServerBlock sb)
{
	parseHttpRequest(req);	
	RequestErrorHandling	err(*this, sb);
	if (!err.ErrorHandler())
		return (err.getErrResponse());
	
	Location	target(err.getTargetBlock());
	str			response;

	if (isCGI(*this))
	{
		std::string		inFileName = TEMP_FILE_IN;
		std::string		outFileName = TEMP_FILE_OUT;
		std::string		toWrite = this->getfull();
		std::remove(inFileName.c_str());
		int tempFd = open(inFileName.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0777);
		write(tempFd, toWrite.c_str(), toWrite.size());
		close(tempFd);

		std::vector<const char*> env_vars;

		env_vars.push_back("HTTP_X_SECRET_HEADER_FOR_TEST=1");
		env_vars.push_back("REDIRECT_STATUS=200");
		env_vars.push_back("CONTENT_LENGTH=100000");
		env_vars.push_back("CONTENT_TYPE=*/*");
		env_vars.push_back("GATEWAY_INTERFACE=CGI/1.1");
		env_vars.push_back("PATH_INFO=cgi_tester");
		env_vars.push_back("PATH_TRANSLATED=cgi_tester");
		env_vars.push_back("QUERY_STRING=");
		env_vars.push_back("REMOTE_ADDR=127.0.0.1");
		env_vars.push_back("REQUEST_URI=cgi_tester");
		env_vars.push_back("REQUEST_METHOD=POST");
		env_vars.push_back("SCRIPT_NAME=cgi_tester");
		env_vars.push_back("SERVER_PROTOCOL=HTTP/1.1");
		env_vars.push_back("SERVER_SOFTWARE=webserv");
		env_vars.push_back("SERVER_PORT=8000");

		char** envp = new char*[env_vars.size() + 1]; // +1 for the NULL terminator

		for (std::size_t i = 0; i < env_vars.size(); ++i) {
			envp[i] = (char *)env_vars[i];
		}

		envp[env_vars.size()] = nullptr; // NULL-terminate the char** array

		std::remove(outFileName.c_str());
		int	stdinFd = dup(STDIN_FILENO);
		int	stdoutFd = dup(STDOUT_FILENO);
		pid_t pid = fork();
		if (pid == 0)
		{
			int inFd = open(inFileName.c_str(), O_RDONLY, 0777);
			int outFd = open(outFileName.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0777);
			dup2(inFd, STDIN_FILENO);
			close(inFd);
			dup2(outFd, STDOUT_FILENO);
			close(outFd);
			rlimit	cpuLimit;
			cpuLimit.rlim_cur = 10;
			cpuLimit.rlim_max = 10;
			if (setrlimit(RLIMIT_CPU, &cpuLimit) == -1)
			{
				perror("setrlimit");
				exit(EXIT_FAILURE);
			}
			char	*args[3] = {(char *)"cgi_tester", (char *)"/YoupiBanane/youpi.bla", NULL};
			execve(args[0], args, envp);
			std::remove(inFileName.c_str());
			std::remove(outFileName.c_str());
			std::cerr << RED << "Execve failed..." << RESET << std::endl;
			exit(EXIT_FAILURE);
		}
		int status;
		waitpid(-1, &status, 0);
		if (WIFEXITED(status) == 0)
			std::cerr << RED << "CGI exited abnormally" << RESET << std::endl;
		std::string output = "";
		int     outfd2 = open(outFileName.c_str(), O_RDWR, 0777);
		char    *buffer = new char[BUFFER_SIZE + 1];
		std::memset(buffer, 0, BUFFER_SIZE + 1);
		long    bytesRead = 0, total = 0;
		while ((bytesRead = read(outfd2, buffer, BUFFER_SIZE)) > 0)
		{
			output.append(buffer, bytesRead);
			std::memset(buffer, 0, BUFFER_SIZE + 1);
			total += bytesRead;
		}
		close(outfd2);
		size_t  startPos = output.find("\r\n\r\n");
		if (startPos == std::string::npos)
		{
			err.generateErrResponse(200, target);
			return (err.getErrResponse());
		}
		else
		{
			startPos += std::strlen("\r\n\r\n");
			std::string newOutput = output.substr(startPos);
			response= "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
			response += newOutput;
			std::cout << GREEN << "CGI ran successfully!" << RESET << std::endl;
			std::remove(inFileName.c_str());
			std::remove(outFileName.c_str());
			dup2(stdinFd, STDIN_FILENO);
			close(stdinFd);
			dup2(stdoutFd, STDOUT_FILENO);
			close(stdoutFd);
		}

		delete[] envp;
		
		return (response);
	}

	switch (this->_methodEnum)
	{
		case GET:
			response = GetResponse(*this, target).getResponse();
			break;
		case PUT:
			response = PutResponse(*this, clientSocket, target, err).getResponse();
			break;
		case POST:
			response = PostResponse(*this, clientSocket, target, err).getResponse();
			break;
		case DELETE:
			response = DeleteResponse(*this, clientSocket, target, err).getResponse();
			break;
		case HEAD:
			err.generateErrResponse(200, target);
			response = err.getErrResponse();
			break;
		case TRACE:
			response = TraceResponse(*this, clientSocket, target, err).getResponse();
			break;
		default:
			break;
	}
	return (response);
}

HttpRequest::HttpRequest()
{
	this->_httpStatusMsg[200] = "200 OK\r\n";
	this->_httpStatusMsg[201] = "201 Created\r\n";
	this->_httpStatusMsg[400] = "400 Bad Request\r\n";
	this->_httpStatusMsg[404] = "404 Not Found\r\n";
	this->_httpStatusMsg[405] = "405 Not Allowed\r\n";
	this->_httpStatusMsg[413] = "413 Request Entity Too Large\r\n";
	this->_httpStatusMsg[415] = "415 Unsupported Media Type\r\n";
	this->_httpStatusMsg[500] = "500 Internal Server Error\r\n";
}

HttpRequest::~HttpRequest() {}

HttpRequest::HttpRequest(const HttpRequest& other) {*this = other;}

HttpRequest&	HttpRequest::operator=(const HttpRequest& other)
{
	if (this == & other)
		return (*this);
	this->_methodStr = other._methodStr;
	this->_methodEnum = other._methodEnum;
	this->_path = other._path;
	this->_body = other._body;
	this->_header = other._header;
	return (*this);
}

void	HttpRequest::setMethod(str method)
{
	if (method == "GET")			_methodEnum = GET;
	else if (method == "HEAD")		_methodEnum = HEAD;
	else if (method == "DELETE")	_methodEnum = DELETE;
	else if (method == "OPTIONS")	_methodEnum = OPTIONS;
	else if (method == "POST")		_methodEnum = POST;
	else if (method == "PUT")		_methodEnum = PUT;
	else							_methodEnum = TRACE;
}

void	HttpRequest::setfull(str full) {this->_full = full;}

void	HttpRequest::setPath(str path) {this->_path = path;}

void	HttpRequest::setBody(str line) {this->_body.push_back(line);}

void	HttpRequest::setHeader(str key, str value){this->_header[key] = value;}

HttpRequest::body	HttpRequest::getBody() const {return this->_body;}

std::string	HttpRequest::getPath() const {return this->_path;}

std::string	HttpRequest::getfull() const {return this->_full;}

std::string	HttpRequest::getMethodStr() const {return this->_methodStr;}

httpMethod	HttpRequest::getMethodEnum() const {return this->_methodEnum;}

HttpRequest::header	HttpRequest::getHeader() const {return this->_header;}

std::map<int, std::string>	HttpRequest::getHttpStatusMsg() const {return this->_httpStatusMsg;}

