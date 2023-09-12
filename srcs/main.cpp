/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:47:59 by suchua            #+#    #+#             */
/*   Updated: 2023/09/12 18:18:42 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parse.hpp"
#include "Server.hpp"

int	main(int ac, char **av)
{
	if (ac != 1 && ac != 2)
	{
		std::cerr << "Error : Incorrect number of arguments\n";
		std::cerr << "Usage : ./webserv <path_to_config_file>\n";
		return (1);
	}

	std::string	ConfigFilename;

	if (ac == 1)
		ConfigFilename = "config/easy.conf";
	else
		ConfigFilename = av[1];
	try
	{
		Parse	parse(ConfigFilename);
		Server	server(parse.getBlock());
		server.startServer();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cerr << "Usage : ./webserv <path_to_config_file>\n";
		return (1);
	}
	return (0);
}
