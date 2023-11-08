/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseHttpRequest.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 15:40:04 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/06 19:20:43 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parseHttpRequest.hpp"

/*
function parseHttpRequest(requestString):
    Split the requestString into lines
    Extract the first line as the requestLine
    Split the requestLine by spaces into method, uri, and version

    Initialize an empty map for headers

    For each line in the requestString after the requestLine:
        If the line is empty, break the loop (end of headers)
        Split the line by the first colon into headerName and headerValue
        Trim whitespace from headerName and headerValue
        Add headerName and headerValue to the headers map

    If a Content-Length header is present:
        Read the number of bytes specified by Content-Length from the requestString as the body

    Return an object containing the method, uri, version, headers, and possibly the body
*/

void parseHttpRequest::parseRequest(std::string request)
{
    std::stringstream ss(request);
    std::string line;

    std::getline(ss, line, '\n');
    std::istringstream iss(line);
    iss >> Method >> URI >> Version;
    Check_Request_Status();

    while (std::getline(ss, line, '\n') && line != "\r")
    {
        size_t pos = line.find(":");
        if (pos != std::string::npos)
        {
            parsed_data.insert(std::make_pair(line.substr(0, pos), line.substr(pos + 2)));
        }
    }
}

parseHttpRequest::parseHttpRequest()
{
}
parseHttpRequest::~parseHttpRequest()
{
}

std::string parseHttpRequest::get_Method()
{
    return Method;
}

std::string parseHttpRequest::get_Version()
{
    return Version;
}
std::string parseHttpRequest::get_URI()
{
    return URI;
}
std::string parseHttpRequest::get_status()
{
    return status;
}

std::string parseHttpRequest::get_Header(std::string key)
{
    std::multimap<std::string, std::string>::iterator it = parsed_data.find(key);
    if (it != parsed_data.end())
    {
        return it->second;
    }
    return "";
}

void parseHttpRequest::clear_all()
{
    Method.clear();
    Version.clear();
    URI.clear();
    parsed_data.clear();
    status.clear();
}

void parseHttpRequest::set_status(std::string status)
{
    this->status = status;
}

void parseHttpRequest::print_all_parseRequest()
{
    std::cout << "==============================" << std::endl;
    std::cout << "Method: " << Method << std::endl;
    std::cout << "Version: " << Version << std::endl;
    std::cout << "URI: " << URI << std::endl;
    std::cout << "Status: " << status << std::endl;
    
    // std::map<std::string, std::string>::iterator it = parsed_data.begin();
    // while (it != parsed_data.end())
    // {
    //     std::cout << it->first << " => " << it->second << '\n';
    //     it++;
    // }
    std::cout << "==============================" << std::endl;
}