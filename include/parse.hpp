/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 10:10:12 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/03 11:08:28 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_HPP
#define PARSE_HPP
#include <map>
#include <string>
#include <iostream>
#include <sstream>

class parse
{
    private:
        std::multimap<std::string,std::string> parsed_data;
    public:
        parse();
        ~parse();
        void parse_data(std::string data);
        std::multimap<std::string,std::string> parse_data();
        void print_data();
        void clear_data();

};


#endif