/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 11:19:31 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/24 16:59:08 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "StringExtensions.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return (std::cout << "Usage: \n\t./webserv [configuration file]" << std::endl, 1);
	try
	{
		Parser parser(av[1]);
		parser.dump();
	}
	catch (CustomException ce)
	{
		std::cout << ce.what() << std::endl; 
	}
	return (0);
}