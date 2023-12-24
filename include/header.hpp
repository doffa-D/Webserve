/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:24:29 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/12/23 14:41:46 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <iostream>     // For cout
#include <unistd.h>     // For read
#include <arpa/inet.h>  // For inet_ntoa
#include <netdb.h>      // For gethostbyname
#include <fstream>      // For ifstream
#include <fcntl.h>      // For open
#include <vector>       // For vector
#include <string>       // For string

// ======================== //
#include "../src/exception/include/exception.hpp"
// #include "../src/config/include/ServerConfigFile.hpp"
#include "../src/request/include/parseHttpRequest.hpp"
#include "../src/server/include/Upload.hpp"
#include "../src/server/include/SendTracker.hpp"
#include "../src/server/include/cgi.hpp"

// ======================== //
#include "../src1/Includes/Location.hpp"
#include "../src1/Includes/Parser.hpp"
#include "../src1/Includes/ServerModel.hpp"
#include "../src1/Includes/ServerData.hpp"
#include "../src1/Includes/Logger.hpp"

// ======================== //

#endif
