/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ITranslatable.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/04 11:44:29 by cledant           #+#    #+#             */
/*   Updated: 2017/09/06 15:09:19 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITRANSLATABLE_HPP
# define ITRANSLATABLE_HPP

# include "glm/glm.hpp"

class ITranslatable
{
	public :

		ITranslatable(void);
		virtual ~ITranslatable(void);
		ITranslatable(ITranslatable const &src) = delete;
		ITranslatable &operator=(ITranslatable const &rhs) = delete;

		virtual void translateObject(glm::vec3 const &vec) = 0;
		virtual void scaleObject(glm::vec3 const &vec) = 0;
};

#endif
