/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 11:19:31 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/31 19:56:52 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "StringExtensions.hpp"

int main(int ac, char **av)
{
	try
	{
		Parser parser(ac, av);
		Server ser = parser.getServerbyHost("loc.alh.os.t:80");
		if (ser.isNull())
			(void)ser;//do somthiong
		parser.dump();
		// Location l = parser.getDefaultServer().getLocationByPath("/app/app1/app2/app3");
		// cout << "Root: " << l.getRoot() << endl; 
		// cout << "Index: " << l.getIndexes()[0] << endl; 
		// cout << "AutoIndex: " << l.getAutoIndex() << endl; 
	}
	catch (CustomException ce)
	{
		std::cout << ce.what() << std::endl; 
	}
	return (0);
}
