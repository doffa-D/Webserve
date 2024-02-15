/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:49 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/15 12:56:56 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

int main(int ac, char **av)
{
	try
	{
		Parser parsedData(ac, av);
		parsedData.dump();
        Wb_Server(parsedData.getHostsAndPorts());

	}
	catch (CustomException ce)
	{
		std::cerr << ce.what() << std::endl;
	    return (1);
	}
	return (0);
}
