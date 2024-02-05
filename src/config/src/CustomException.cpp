/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CustomException.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 10:06:24 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/05 09:29:07 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "CustomException.hpp"
#include "../../../include/header.hpp"


CustomException::CustomException(const std::string& _message) : message(_message)
{
	message = "webserve: " + message;
}

CustomException::CustomException(const std::string& _message, const std::string& _arg) : message(_message)
{
	message = "webserve: [" + _arg + "] " + message;
}

const char* CustomException::what() const throw(){return (message.c_str());}
