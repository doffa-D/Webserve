/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:49 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/27 13:50:31 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

int main(int ac, char **av)
{
	// signal(SIGPIPE, SIG_IGN);
	try
	{
		Parser parsedData(ac, av);
		// parsedData.dump();
        Wb_Server _Wb_Server(parsedData);
	}
	catch (CustomException ce)
	{
		std::cerr << ce.what() << std::endl;
	    return (1);
	}
	return (0);
}
