/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 11:19:31 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/09 16:32:01 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Parser.hpp"
#include "../include/StringExtensions.hpp"
// #include "../../../include/header.hpp"


// int main(int ac, char **av)
// {
// 	try
// 	{
// 		Parser parsedData(ac, av);
// 		Server ser = parsedData.getServerbyHost("loc.alh.os.t:80");
// 		if (ser.isNull())
// 			(void)ser;//do somthiong
// 		parsedData.dump();
// 		// Location l = parser.getDefaultServer().getLocationByPath("/app/app1/app2/app3");
// 		// cout << "Root: " << l.getRoot() << endl; 
// 		// cout << "Index: " << l.getIndexes()[0] << endl; 
// 		// cout << "AutoIndex: " << l.getAutoIndex() << endl; 
// 	}
// 	catch (CustomException ce)
// 	{
// 		std::cout << ce.what() << std::endl; 
// 	}
// 	return (0);
// }


int main(int ac, char **av)
{
	try
	{
		Parser parsedData(ac, av);
        // std::vector<Server> servers = parsedData.getServers();
        // std::vector<std::pair<Uint, int> > Host;
        // for (size_t i = 0; i < servers.size();i++)
        // {
        //     VecInt s = servers[i].getPorts();
        //     for (size_t j = 0; j < s.size(); j++)
        //     {
        //         Host.push_back(std::make_pair(servers[i].getIpAddress(), s[j]));
        //     }
        // }
        
        // for (size_t i = 0; i < Host.size();i++)
        //     cout << "ip: " << Host[i].first << " port: " << Host[i].second << endl;
        
		// Server ser = parsedData.getServerbyHost("loc.alh.os.t:80");
		// if (ser.isNull())
		// 	(void)ser;//do somthiong
		// parsedData.dump();
		
		Server s = parsedData.getDefaultServer();
		cout << "Index: " << s.getIndexFilePathByRoot("/Users/kchaouki/Desktop/doffa_rep") << endl;
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
