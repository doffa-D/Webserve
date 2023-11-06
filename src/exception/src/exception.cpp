/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:49:01 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/06 12:55:36 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/exception.hpp"

MY_exception::MY_exception(){}


MY_exception::MY_exception(const std::string &msg) : message(msg) {}

MY_exception::~MY_exception() {}

MY_exception::status_code_exception::status_code_exception(const std::string &msg) : message(msg) {}

const char *MY_exception::status_code_exception::what() const throw()
{
    return message.c_str();
}

const char *MY_exception::what() const throw()
{
    return message.c_str();
}