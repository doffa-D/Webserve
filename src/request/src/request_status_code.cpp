/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_status_code.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:03:42 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/06 17:55:56 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parseHttpRequest.hpp"

bool parseHttpRequest::URI_check(std::string URI)
{
    std::string ValidChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=";
    for (size_t i = 0; i < URI.length(); i++)
    {
        if (ValidChars.find(URI[i]) == std::string::npos)
        {
            return false;
        }
    }
    return true;
}

void parseHttpRequest::Check_Request_Status()
{
    if (URI_check(URI) == false)
        throw MY_exception::status_code_exception("400 Bad Request");
    else if (URI.length() > 2048)
        throw MY_exception::status_code_exception("414 Request-URI Too Long");
    // else if (Method == "POST")
    // {
    // if (get_Header("Content-Length") == "")
    //     status = "411 Length Required";
    // else if (get_Header("Content-Type") != "application/x-www-form-urlencoded")
    //     status = "415 Unsupported Media Type";
    // else
    //     status = "200 OK";
    // }
    // else if (Method != "GET")
    //     status = "501 Not Implemented";
    // else
    //     status = "200 OK";
}