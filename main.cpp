/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 11:19:31 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/26 10:27:49 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./ConfigParsing/Parser.hpp"
#include "./ConfigParsing/StringExtensions.hpp"
#include "./Request_Response/TmpServer.hpp"

int main(int ac, char **av)
{
	(void)av;
	if (ac != 2)
		return (std::cout << "Usage: \n\t./webserv [configuration file]" << std::endl, 1);
	try
	{
		// Parser parser(av[1]);
		// std::cout <<  parser.getServers()[0].getServerName() << std::endl;
		server(av[1]);
		// parser.dump();
	}
	catch (CustomException ce)
	{
		std::cout << ce.what() << std::endl; 
	}
	return (0);
}
