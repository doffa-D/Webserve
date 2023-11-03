/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseHttpRequest.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 15:18:41 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/03 18:27:44 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEHTTPREQUEST_HPP
#define PARSEHTTPREQUEST_HPP
#include <iostream>
#include <sstream>
#include <map>
class parseHttpRequest
{
    private:
        std::string Method;                                  // Method is the method of the request
        std::string Version;                                 // Version is the version of the request
        std::string Host;                                    // Host is the host of the request
        std::string URI;                                     // URI is the URI of the request
        std::multimap<std::string, std::string> parsed_data; // parsed_data is the map that will be used to store the parsed data
        std::string status;                                  // status is the status of the request

    public:
        parseHttpRequest(); // this constructor is used to parse the request and store the parsed data in the map
        ~parseHttpRequest();
        void parseRequest(std::string request); // this constructor is used to parse the request and store the parsed data in the map

        std::string get_Method();                // get_Method function is used to get the method of the request
        std::string get_Version();               // get_Version function is used to get the version of the request
        std::string get_Host();                  // get_Host function is used to get the host of the request
        std::string get_URI();                   // get_URI function is used to get the URI of the request
        std::string get_Header(std::string key); // get_Header function is used to get the value of a header just by passing the key
        void clear_all();                        // clear_all function is used to clear the map
        bool URI_check(std::string URI);         // URI_check function is used to check if the URI is valid
        std::string get_status();                // get_status function is used to get the status of the request
};

#endif