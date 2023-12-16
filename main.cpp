/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:49 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/12/15 11:33:40 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/server/include/Server.hpp"

int main(int argc, char **argv)
{
    (void)argc;
    if (argc != 2)
    {
        std::cout << "Usage: ./webserv [config_file]" << std::endl;
        return (1);
    }
    Server server(argv[1]);
    return (0);
}