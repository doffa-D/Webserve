/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendTracker.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 15:40:29 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/26 10:31:45 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../include/SendTracker.hpp"
#include "../../../include/header.hpp" // in this header file we have all the libraries that we need


int SendTracker::getFd()
{
    return this->fd;
}

void SendTracker::setFd(int fd)
{
    this->fd = fd;
}

bool SendTracker::writeNextChunk()
{
    if (this->has_finished == true)
    {
        ssize_t bytesWritten = 0;
        size_t byt = response.length() - this->bytesWritten_;
        size_t size_to_send = std::min(static_cast<size_t>(SEND_BUFFER_SIZE), byt);

        std::string res(&response[this->bytesWritten_], size_to_send);
        bytesWritten += send(this->fd, res.c_str(), size_to_send, 0);
        if (bytesWritten > -1)
        {
            this->bytesWritten_ += bytesWritten;
        }

        if (this->bytesWritten_ < static_cast<ssize_t>(response.length()))
        {
            this->has_finished = false;
            return false;
        }
        else if (this->bytesWritten_ >= static_cast<ssize_t>(response.length()))
        {
            this->has_finished = true;
            return true;
        }
    }
    else if (this->has_finished == false)
    {
        ssize_t bytesWritten = 0;
        size_t byt = response.length() - this->bytesWritten_;
        size_t size_to_send = std::min(static_cast<size_t>(SEND_BUFFER_SIZE), byt);

        std::string res(&response[this->bytesWritten_], size_to_send);
        bytesWritten += send(this->fd, res.c_str(), size_to_send, 0);

        if (bytesWritten > -1)
        {
            this->bytesWritten_ += bytesWritten;
        }
        // std::cout << "this is the second time " << this->bytesWritten_ << "  " << response.length() << std::endl;
        if (this->bytesWritten_ < static_cast<ssize_t>(response.length()))
        {
            this->has_finished = false;
            return false;
        }
        else if (this->bytesWritten_ >= static_cast<ssize_t>(response.length()))
        {
            // std::cout << this->bytesWritten_ << " " << response.length() << std::endl;

            // std::cout << "Data sent successfully" << std::endl;
            // exit(0);

            this->has_finished = true;
            return true;
        }
    }
    return false;
}
