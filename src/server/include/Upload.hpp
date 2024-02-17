/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 09:48:02 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/17 21:53:14 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../../include/header.hpp"

// struct request
// {
//     int fd;
//     std::string method;
//     std::string fisrtboundaryValue;
//     std::string lastboundaryValue;
//     std::string buffer;
//     long double contentLength;
//     size_t start;
//     size_t bytes_read;
//     bool isChunked;
//     size_t startFContent;
//     size_t endFile;
//     std::vector<std::string> parts;
//     bool isFile;
//     std::string header;
// };

// class Upload
// {
// private:
//     std::string full_request;
//     std::vector<request> clients;

// public:
//     int find_clinet(int socket_fd);
//     void create_client(std::string body, int socket_fd);
//     // void handle_chunked_data();
//     // void handle_non_chunked_data();
// };