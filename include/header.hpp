/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:24:29 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/07 10:10:59 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

#include <time.h>
#include <signal.h>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <sys/wait.h> // For waitpid
#include <sys/statvfs.h>

// ===========Config============= //

#include "../src/config/include/StringExtensions.hpp"
#include "../src/config/include/CommonDirectives.hpp"
#include "../src/config/include/CustomException.hpp"
#include "../src/config/include/Location.hpp"
#include "../src/config/include/Parser.hpp"
#include "../src/config/include/Server.hpp"

// ===========Server============= //

#include "../src/server/include/Upload.hpp"
#include "../src/server/include/SendTracker.hpp"
#include "../src/server/include/Wb_Server.hpp" 
#include "../src/CGI/include/CGI.hpp"

// ===========Request_Response============ //

#include "../src/Request_Response/include/Request.hpp"
#include "../src/Request_Response/include/Response.hpp"




#endif
