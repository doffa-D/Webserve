/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:40:20 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/09 18:56:12 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

#ifndef CGI_HPP
#define CGI_HPP
#define CGI_WAITING -2          
#define CGI_TIMEOUT 266          // "504 Gateway Timeout"
#define CGI_ERROR 260    			// "500 Internal Server Error"


class CGI
{
	private:
		std::map<std::string, std::string> env;
		char **envp;
		std::string body;
		std::string bin;

	public:
		CGI(std::string const &body, std::map<std::string, std::string> const &env, std::string const &bin);
		~CGI();

	std::pair<std::string, int> fill_env();
};

#endif // CGI_HPP
