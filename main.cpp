/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:49 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/12/12 12:37:34 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/server/include/Server.hpp"

int main(int argc, char **argv)
{
    (void)argc;

    Server server(argv[1]);
    return (0);
}