/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:49 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/08 09:19:16 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/server/include/Server.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    Parser parser(argv[1]);
    ServerData servers(parser.getServers());
    ServerModel smodel = servers.getServer("localhost");
    std::vector<Data> data = smodel.getData("listen");
    std::vector<Data>::iterator ibegin = data.begin();
	std::vector<Data>::iterator iend = data.end();
    while (ibegin < iend)
	{
        String str(""); 
		Data::printData(*ibegin, str);
		ibegin++;
	}
    // ServerModel::printServerModelInfo(smodel);
    Server server;
    return (0);
}