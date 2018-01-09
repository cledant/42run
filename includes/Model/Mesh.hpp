/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESH_HPP
# define MESH_HPP

# include "glad/glad.h"
# include "glfw3.h"
# include "assimp/Importer.hpp"
# include "assimp/scene.h"
# include "assimp/postprocess.h"
# include "OpenGL/Texture.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>

class Mesh
{
	public :

		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
		};

		Mesh(void);
		Mesh(aiMesh *mesh, const aiScene *scene, std::string const &directory);
		Mesh(Mesh const &src) = delete;
		Mesh &operator=(Mesh const &rhs) = delete;
		Mesh(Mesh &&src);
		Mesh &operator=(Mesh &&rhs);
		virtual ~Mesh(void);

		std::vector<Mesh::Vertex> const &getVertexList(void) const;
		std::vector<Texture> const &getTextureList(void) const;
		std::vector<unsigned int> const &getIndiceList(void) const;
		std::string const &getDirectory(void) const;
		std::vector<Texture> moveTextureList(void);
		GLuint getVAO(void) const;
		GLuint moveVAO(void);
		GLuint moveVBO(void);
		GLuint moveEBO(void);

		class GLInitException : public GeneralException
		{
			public :

				explicit GLInitException(void);
				virtual        ~GLInitException(void) throw();
		};

		class InvalidMeshException : public GeneralException
		{
			public :

				explicit InvalidMeshException(void);
				virtual ~InvalidMeshException(void) throw();
		};

		class InvalidMaterialException : public GeneralException
		{
			public :

				explicit InvalidMaterialException(void);
				virtual ~InvalidMaterialException(void) throw();
		};

	private :

		std::vector<Mesh::Vertex> _vertex_list;
		std::vector<Texture>      _texture_list;
		std::vector<unsigned int> _indice_list;
		GLuint                    _vao;
		GLuint                    _vbo;
		GLuint                    _ebo;
		std::string               _directory;

		void _load_mesh(aiMesh *mesh);
		void _load_indice(aiMesh *mesh);
		void _load_material(aiMesh *mesh, const aiScene *scene);
		void _load_texture(aiMaterial *mat, aiTextureType type, Texture::t_tex_type tex_type);
		bool _find_texture(std::string const &name) const;
		void _allocate_set_GL_ressources(void);
};

#endif
