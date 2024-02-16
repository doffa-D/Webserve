/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:49 by hdagdagu          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/02/16 22:45:13 by kchaouki         ###   ########.fr       */
=======
/*   Updated: 2024/02/15 14:39:54 by hdagdagu         ###   ########.fr       */
>>>>>>> 76793f526db815417796d3f1fe88e7e40cec5b60
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
