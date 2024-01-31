/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:15:28 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/29 16:24:23 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonDirectives.hpp"

class Location : public CommonDirectives
{
	private:
		bool	nullObject;
	public:
		Location();
		~Location();
		Location(const Location& _copy);
		Location(const CommonDirectives& _copy);
		Location& operator=(const Location& _assignment);
		static Location createNullObject();
		bool	isNull();
};
