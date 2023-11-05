/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:30:36 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/05 12:16:52 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <exception>
#include <string>

class exception
{
    public:
        exception();
        ~exception();
        class status_code_exception
        {
            private:
                std::string message;

            public:
                status_code_exception(const std::string &msg) : message(msg) {}
                const char *what() const throw()
                {
                    return message.c_str();
                }
        };
};

#endif
