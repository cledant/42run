/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OrientableShaderSurface.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/09 11:06:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/09 11:06:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ORIENTABLE_SHADERSURFACE_HPP
# define ORIENTABLE_SHADERSURFACE_HPP

# include "UI/ShaderSurface.hpp"
# include "glm/gtc/matrix_transform.hpp"

class OrientableShaderSurface : public ShaderSurface
{
	public :

		struct Params
		{
			Params(void);
			~Params(void);

			GLFW_Window const *win;
			Input const       *input;
			Shader const      *shader;
			glm::mat4 const   *perspec_mult_view;
			glm::vec3         pos;
			glm::vec3         offset;
			glm::vec3         scale;
			glm::vec3         orientation;
		};

		OrientableShaderSurface(OrientableShaderSurface::Params const &params);
		virtual ~OrientableShaderSurface(void);
		OrientableShaderSurface(OrientableShaderSurface const &src) = delete;
		OrientableShaderSurface &operator=(OrientableShaderSurface const &rhs) = delete;
		OrientableShaderSurface(OrientableShaderSurface &&src);
		OrientableShaderSurface &operator=(OrientableShaderSurface &&rhs);

		/*
		 * Setter
		 */

		void setPosition(glm::vec3 const &pos);
		void setOffset(glm::vec3 const &offset);
		void setScale(glm::vec3 const &scale);
		void setOrientation(glm::vec3 const &orientation);

		/*
		 * Getter
		 */

		glm::mat4 const *getPerspecMultView(void) const;
		glm::mat4 const &getTotalMatrix(void) const;
		glm::vec3 const &getPos(void) const;
		glm::vec3 const &getScale(void) const;
		glm::vec3 const &getOffset(void) const;
		glm::vec3 const &getOrientation(void) const;

		/*
		 * Interface IEntity
		 */

		virtual void update(float time);
		virtual void draw(void);

	protected :

		glm::mat4 const *_perspec_mult_view;
		glm::mat4       _total;
		glm::vec3       _pos;
		glm::vec3       _offset;
		glm::vec3       _scale;
		glm::vec3       _orientation;

		/*
		 * Orientation values :
		 *
		 * pitch = x
		 * yaw = y
		 * roll = z
		 */
};

#endif
