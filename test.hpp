/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 15:28:25 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/01/26 17:50:49 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
// #include <filesystem>
#include <fstream>

class Response
{
	private:
		std::string Eroor_Page;
	public:
		Response();
		std::string	Error_HmlPage(const std::string& stat_code, const std::string& stat_msg);
		// void	setEroor_Page(const std::string& stat_code, const std::string& stat_msg);
		std::string getEroor_Page();
};