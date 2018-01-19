/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cubemap.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 16:07:59 by cledant           #+#    #+#             */
/*   Updated: 2017/09/10 14:43:03 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBEMAP_HPP
# define CUBEMAP_HPP

#include "Interfaces/IEntity.hpp"
#include "OpenGL/oGL_module.hpp"
#include "OpenGL/Texture.hpp"
#include "Cameras/Camera.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Interfaces/ITranslatable.hpp"

class Cubemap : public IEntity, public ITranslatable
{
	public :

		Cubemap(Shader const *shader, glm::mat4 const *perspec_mult_view,
				Model const *model, glm::vec3 const &pos, glm::vec3 const &scale);
		virtual ~Cubemap(void);
		Cubemap(Cubemap const &src);
		Cubemap &operator=(Cubemap const &rhs);

		/*
 		* Interface ITranslatable
 		*/

		void translateObject(glm::vec3 const &vec);
		void scaleObject(glm::vec3 const &vec);

		/*
		 * Interface IEntity
		 */

		void update(float time);
		void draw(void);

		/*
		 * Setter
		 */

		void setPosition(glm::vec3 const &pos);
		void setScale(glm::vec3 const &scale);

		/*
		 * Getter
		 */

		glm::mat4 const &getTotalMatrix(void) const;
		Shader const *getShader(void) const;
		Model const *getModel(void) const;
		glm::mat4 const *getPerspecMultView(void) const;
		glm::vec3 const &getPos(void) const;
		glm::vec3 const &getScale(void) const;

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual        ~InitException(void) throw();
		};

		static float  vertices[];
		static size_t nb_faces;

	private :

		Shader const    *_shader;
		glm::mat4 const *_perspec_mult_view;
		Model const     *_model;
		glm::vec3       _pos;
		glm::vec3       _scale;
		glm::mat4       _total;
};

#endif
