/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MY_exception.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 11:30:36 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/05 12:33:34 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_exception_HPP
#define MY_exception_HPP
#include <string>

class MY_exception
{
    private:
        std::string message;
    public:
        MY_exception();
        MY_exception(const std::string &msg);
        ~MY_exception();
        class status_code_exception
        {
            private:
                std::string message;

            public:
                status_code_exception(const std::string &msg);
                const char *what() const throw();
        };
        const char *what() const throw();
        
};

#endif
