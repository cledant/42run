/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IGravity.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 15:09:19 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IGRAVITY_HPP
# define IGRAVITY_HPP

# include "glm/glm.hpp"

class IGravity
{
	public :

		IGravity(void);
		virtual ~IGravity(void);
		IGravity(IGravity const &src) = delete;
		IGravity &operator=(IGravity const &rhs) = delete;

		virtual void update_gravity(glm::vec3 const &vec_gravity, float delta) = 0;
};

#endif
