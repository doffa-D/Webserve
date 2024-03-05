/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:40:20 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/04 18:01:08 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

#ifndef CGI_HPP
#define CGI_HPP
#define CGI_WAITING -2          
#define CGI_SUCCESS 0           // "200 OK" and return body
#define CGI_TIMEOUT -1          // "504 Gateway Timeout"
#define CGI_ERROR -3            // "500 Internal Server Error"
#define CGI_SIGNALED -4         // "500 Internal Server Error"
#define CGI_UNKNOWN_ERROR -5    // "500 Internal Server Error"


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
