/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 10:10:15 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/03 11:08:21 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parse.hpp"

parse::parse()
{
    // std::cout << "parse constructor called. " << std::endl;
}

parse::~parse()
{
    // std::cout << "parse destructor called. " << std::endl;
}

void parse::parse_data(std::string data)
{
    std::stringstream ss(data);
    std::string line;
    std::string key;
    std::string Value; 
    std::getline(ss,line,'\n');
    key = line.substr(0,line.find(" "));
    Value = line.substr(line.find(" ") + 1);
    parsed_data.insert(std::make_pair(key,Value));
    // std::cout << "=> " << line << std::endl;
    while(std::getline(ss,line,'\n'))
    {
        size_t pos = line.find(":");
        if(pos != std::string::npos)
        {
            key = line.substr(0,pos);
            Value = line.substr(pos + 2);
            parsed_data.insert(std::make_pair(key,Value));
        }
    }

}
std::multimap<std::string,std::string> parse::parse_data()
{
    return parsed_data;
}
void parse::print_data()
{
    for(std::multimap<std::string,std::string>::iterator it = parsed_data.begin(); it != parsed_data.end();it++)
    {
        std::cout << "Key: " << it->first << " Value: " << it->second << std::endl;
    }
}
void parse::clear_data()
{
    parsed_data.clear();
}