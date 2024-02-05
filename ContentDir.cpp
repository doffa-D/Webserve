/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ContentDir.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:35:22 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/05 21:48:54 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <dirent.h>

int main() {
    DIR *directory;
    struct dirent *entry;

    // Open the current directory (replace "." with the path to your desired directory)
    if ((directory = opendir(".")) != NULL) {
        // Read each entry in the directory
        while ((entry = readdir(directory)) != NULL) {
            std::cout << entry->d_name  << "  " <<  << " " <<  std::endl;
        }

        // Close the directory
        closedir(directory);
    } else {
        // Handle error opening directory
        std::cerr << "Error opening directory." << std::endl;
        return 1;
    }

    return 0;
}
