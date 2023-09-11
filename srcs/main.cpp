/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzi-xian <lzi-xian@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:47:59 by suchua            #+#    #+#             */
/*   Updated: 2023/09/11 17:08:49 by lzi-xian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Parse.hpp"

int	main(int ac, char **av)
{
	std::string ConfigFilename = "config/easy.conf";

	if (ac > 2)
	{
		std::cerr << "Error : Incorrect number of arguments\n";
		return (1);
	}
	else if (ac == 2)
		ConfigFilename = av[1];
	try
	{
		Parse	parse(ConfigFilename);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
