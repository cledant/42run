/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IEntity.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 15:09:19 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IENTITY_HPP
# define IENTITY_HPP

class IEntity
{
	public :

		IEntity(void);
		virtual ~IEntity(void);

		virtual void	update(float time) = 0;
		virtual void	draw(void) = 0;

	private :

		IEntity(IEntity const &src);
		virtual IEntity		&operator=(IEntity const &rhs);
};

#endif
