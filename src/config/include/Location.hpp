/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki < kchaouki@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:15:28 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/18 22:49:10 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../../include/header.hpp"


class Location : public CommonDirectives
{
	private:
		string		alias;
		bool		nullObject;
	public:
		Location();
		~Location();
		Location(const Location& _copy);
		Location(const CommonDirectives& _copy);
		Location& operator=(const Location& _assignment);

		static Location	createNullObject();
		bool			isNull();
		void			setAlias(const string& _alias);
		string		getIndexFilePathByRoot(const string& _value) const;
		const string&	getAlias() const;
};
