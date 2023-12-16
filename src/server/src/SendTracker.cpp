/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendTracker.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/16 15:40:29 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/12/16 15:56:32 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/SendTracker.hpp"

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
        bytesWritten += send(this->fd, response.c_str(), response.length(), 0);
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
        std::string res(&response[this->bytesWritten_], response.length() - this->bytesWritten_);

        bytesWritten += send(this->fd, res.c_str(), res.length(), 0);
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
