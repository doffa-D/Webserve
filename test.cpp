/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:30:48 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/01/26 17:51:31 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.hpp"

Response::Response()
{
	// Eroor_Page = "error " + Stat_Code + "  " + Stat_Msg + " \n";
}

std::string Response::getEroor_Page()
{
	return Eroor_Page;
}
std::string	Response::Error_HmlPage(const std::string& stat_code, const std::string& stat_msg)
{
	std::string	Eroor_Page = "error " + stat_code + "  " + stat_msg + " \n";
	return Eroor_Page;
}
// void	Response::setEroor_Page(const std::string& stat_code, const std::string& stat_msg)
// {
// 	Eroor_Page = "error " + stat_code + "  " + stat_msg + " \n";
// }

int main()
{
	Response	response;
	std::string err = response.Error_HmlPage("508", "yess");

	std::cout << err << std::endl;

	
	// std::ifstream File("./test.html");
	// if(File.is_open())
	// 	std::cout << "open\n" << std::endl;
	// else
	// 	std::cout << "not open" << std::endl;
	return 0;
}