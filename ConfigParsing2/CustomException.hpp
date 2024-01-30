/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CustomException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 10:01:56 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/21 07:55:22 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

class CustomException
{
    private:
    	std::string message;
    public:
		CustomException(const std::string& _message);
		CustomException(const std::string& _message, const std::string& _arg);
		const char *what() const throw();
};

#endif