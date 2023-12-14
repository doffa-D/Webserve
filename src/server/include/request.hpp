/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 10:05:22 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/12/14 10:54:41 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>

class request
{
public:
    int fd;
    long double contentLength;
    bool isChunked;
    bool isFile;
    size_t start;
    size_t bytes_read;
    size_t startFContent;
    size_t endFile;
    std::vector<std::string> parts;
    std::string method;
    std::string fisrtboundaryValue;
    std::string lastboundaryValue;
    std::string buffer;
    std::string header;

    // request();
};
