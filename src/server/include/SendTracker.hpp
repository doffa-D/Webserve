/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendTracker.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 15:38:24 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/27 13:42:43 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../../include/header.hpp" // in this header file we have all the libraries that we need
#define SEND_BUFFER_SIZE 1024
// #include <algorithm>

class SendTracker
{
private:
    int fd;
    std::string response;
    ssize_t bytesWritten_;
    bool has_finished;

public:
    SendTracker(std::string Response) : response(Response), bytesWritten_(0), has_finished(true) {}
    void setFd(int fd);
    int getFd();
    std::string getPort();
    bool writeNextChunk();
};