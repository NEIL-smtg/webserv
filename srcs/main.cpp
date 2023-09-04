/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:47:59 by suchua            #+#    #+#             */
/*   Updated: 2023/09/05 04:25:41 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Error : Incorrect number of arguments\n";
		return (1);
	}
	std::cout << av[1] << std::endl;
	return (0);
}
