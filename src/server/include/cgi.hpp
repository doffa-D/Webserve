/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:40:20 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/12/24 11:33:12 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"
#define CGI "cgi-bin/php-cgi"

#pragma once

class cgi
{
private:
    std::map<std::string, std::string> env;
    char **envp;
    std::string head;
    std::string body;

public:
    cgi(std::string head, std::string body, std::string SCRIPT_NAME, std::string SCRIPT_FILENAME, std::string CONTENT_TYPE, std::string REQUEST_METHOD, std::string CONTENT_LENGTH, std::string QUERY_STRING, std::string SERVER_PROTOCOL, std::string SERVER_SOFTWARE, std::string SERVER_NAME, std::string GATEWAY_INTERFACE, std::string REDIRECT_STATUS);

    ~cgi();
    void fill_env(std::string SCRIPT_FILENAME);
};
