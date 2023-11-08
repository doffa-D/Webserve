/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 09:24:20 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/08 10:40:14 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void init_server(Server &server,char *argv)
{
    (void)server;
    Parser parser(argv);
    ServerData servers(parser.getServers());
    ServerModel smodel = servers.getServer("localhost");
    std::vector<Data> data = smodel.getData("listen");
    std::vector<Data>::iterator ibegin = data.begin();
	std::vector<Data>::iterator iend = data.end();
    while (ibegin < iend)
	{
        String str(""); 
		// Data::printData(*ibegin, str);
        // std::cout << ibegin->getValue() << std::endl;
        // std::cout << ibegin - data.begin() << std::endl;
        server.set_ports(std::stoi(ibegin->getValue()), ibegin - data.begin());
		ibegin++;
	}
    server.set_Number_of_ports(ibegin - data.begin());
    // std::cout << "==< "<< ibegin - data.begin() << std::endl;
    // ServerModel::printServerModelInfo(smodel);
    // exit(0);
}