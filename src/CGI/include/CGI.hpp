/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:40:20 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/02 17:56:29 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"
#include <time.h>

#include <signal.h>
#ifndef CGI_HPP
#define CGI_HPP
#define CGI_SUCCESS 0
#define CGI_TIMEOUT 1
#define CGI_WAITING 2

#define RUN_TIME(BeginTime) (time(0) - BeginTime > 5)
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
