/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 09:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 09:38:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODEL_HPP
# define MODEL_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "assimp/Importer.hpp"
# include "assimp/scene.h"
# include "assimp/postprocess.h"
# include "Mesh.hpp"
# include "Texture.hpp"
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>

class Model
{
	public :

		Model(void);
		Model(std::string const &name, std::string const &path);
		Model(Model const &src) = delete;
		Model &operator=(Model const &rhs) = delete;
		Model(Model &&src);
		Model &operator=(Model &&rhs);
		virtual ~Model(void);

		std::string const &getName(void) const;
		std::vector<Mesh> const &getMeshList(void) const;
		void draw(void) const;

		class FileOpenException : public GeneralException
		{
			public :

				explicit FileOpenException(void);
				explicit FileOpenException(std::string const &path);
				virtual ~FileOpenException(void) throw();
		};

		class InvalidNodeException : public GeneralException
		{
			public :

				explicit InvalidNodeException(void);
				virtual ~InvalidNodeException(void) throw();
		};

	private :

		std::string       _name;
		std::vector<Mesh> _mesh_list;

		void _load_model(std::string const &path);
		void _load_node(aiNode *node, const aiScene *scene, std::string const &directory);
};

#endif