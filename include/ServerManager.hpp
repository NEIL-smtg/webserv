/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suchua <suchua@student.42kl.edu.my>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 16:05:33 by suchua            #+#    #+#             */
/*   Updated: 2023/09/11 16:06:42 by suchua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_MANAGER_HPP__
# define __SERVER_MANAGER_HPP__

# include "Webserv.hpp"

class ServerManager
{
	public:
		ServerManager(std::string config);
		~ServerManager();
};

#endif
