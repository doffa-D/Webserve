/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TmpServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:41:42 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/01/26 10:28:14 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


void server(const std::string& fileName);