/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:49:01 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/05 12:23:51 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/exception.hpp"

exception::status_code_exception::status_code_exception(const std::string &msg) : message(msg) {}

const char *exception::status_code_exception::what() const
{
    return message.c_str();
}