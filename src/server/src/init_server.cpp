/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:24:20 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/09 12:05:14 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void init_server(Server &server,char *argv)
{
    int i = 0;
    (void)server;
    (void)argv;
    Parser parser(argv);
    ServerData servers(parser.getServers());
    ServerModel smodel = servers.getServerByServerName("Default_server");
    std::vector<Data> data = smodel.getData("listen");
    std::vector<Data>::iterator ibegin = data.begin();
	std::vector<Data>::iterator iend = data.end();
    while (ibegin < iend)
	{
        // String str(""); 
        // Data::printData(*ibegin, str);
        server.set_ports(std::stoi(ibegin->getValue()), ibegin - data.begin());
        // std::cout << (ibegin - data.begin()) + i << std::endl;
		ibegin++;
	}
    i = ibegin - data.begin();
    // ======================>> 
    smodel = servers.getServerByServerName("localhost");
    data = smodel.getData("listen");
    ibegin = data.begin();
	iend = data.end();
    while (ibegin < iend)
	{
        // String str(""); 
        // Data::printData(*ibegin, str);
        server.set_ports(std::stoi(ibegin->getValue()), (ibegin - data.begin()) + i);
		ibegin++;
	}
    server.set_Number_of_ports(ibegin - data.begin() + i); 

    // exit(0);
    

}