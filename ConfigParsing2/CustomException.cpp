/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CustomException.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 10:06:24 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/21 07:54:41 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CustomException.hpp"

CustomException::CustomException(const std::string& _message) : message(_message)
{
	message = "webserve: " + message;
}

CustomException::CustomException(const std::string& _message, const std::string& _arg) : message(_message)
{
	message = "webserve: [" + _arg + "] " + message;
}

const char* CustomException::what() const throw(){return (message.c_str());}
