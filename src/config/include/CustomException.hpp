/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CustomException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 10:01:56 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/18 00:30:02 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include "../../../include/header.hpp"


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