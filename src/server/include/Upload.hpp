/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 09:48:02 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/03 13:13:14 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../../include/header.hpp"



std::string upload_file(std::string const &body,std::string const &location,MapStringString const &header);
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