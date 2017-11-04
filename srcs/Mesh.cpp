/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/04 11:52:23 by cledant           #+#    #+#             */
/*   Updated: 2017/11/04 11:52:23 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mesh.hpp"
#include "oGL_module.hpp"

Mesh::Mesh(void) : _vao(0), _vbo(0), _ebo(0)
{
}

Mesh::Mesh(aiMesh *mesh, const aiScene *scene) : _vao(0), _vbo(0), _ebo(0)
{
	if (mesh == NULL)
		throw Mesh::InvalidMeshException();
	this->_load_mesh(mesh, scene);
	this->_load_indice(mesh, scene);
	this->_load_material(mesh, scene);
	try
	{
		this->_allocate_set_GL_ressources();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		oGL_module::oGL_delete_vao(this->_vao);
		oGL_module::oGL_delete_vbo(this->_vbo);
		oGL_module::oGL_delete_vbo(this->_ebo);
		throw Mesh::GLInitException();
	}
}

void Mesh::_load_mesh(aiMesh *mesh, const aiScene *scene)
{
	struct Mesh::Vertex tmp;

	for (size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		std::memset(&tmp, 0, sizeof(Mesh::Vertex));
		tmp.Position.x  = mesh->mVertices[i].x;
		tmp.Position.y  = mesh->mVertices[i].y;
		tmp.Position.z  = mesh->mVertices[i].z;
		tmp.Normal.x    = mesh->mNormals[i].x;
		tmp.Normal.y    = mesh->mNormals[i].y;
		tmp.Normal.z    = mesh->mNormals[i].z;
		tmp.Tangent.x   = mesh->mTangents[i].x;
		tmp.Tangent.y   = mesh->mTangents[i].y;
		tmp.Tangent.z   = mesh->mTangents[i].z;
		tmp.Bitangent.x = mesh->mBitangents[i].x;
		tmp.Bitangent.y = mesh->mBitangents[i].y;
		tmp.Bitangent.z = mesh->mBitangents[i].z;
		if (mesh->mTextureCoords[0] != NULL)
		{
			tmp.TexCoords.x = mesh->mTextureCoords[0][i].x;
			tmp.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		this->_vertex_list.push_back(tmp);
	}
}

void Mesh::_load_indice(aiMesh *mesh, const aiScene *scene)
{
	aiFace face;

	for (size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; ++j)
			this->_indice_list.push_back(face.mIndices[j]);
	}
}

void Mesh::_load_material(aiMesh *mesh, const aiScene *scene)
{
	aiMaterial *mat = NULL;

	if ((mat = scene->mMaterials[mesh->mMaterialIndex]) == NULL)
		throw Mesh::InvalidMaterialException();
	this->_load_texture(mat, aiTextureType_DIFFUSE, Texture::TEX_DIFFUSE);
	this->_load_texture(mat, aiTextureType_SPECULAR, Texture::TEX_SPECULAR);
	this->_load_texture(mat, aiTextureType_HEIGHT, Texture::TEX_NORMAL);
	this->_load_texture(mat, aiTextureType_AMBIENT, Texture::TEX_HEIGHT);
}

void Mesh::_allocate_set_GL_ressources(void)
{
	this->_vao = oGL_module::oGL_create_vao();
	this->_vbo = oGL_module::oGL_create_vbo(this->_vertex_list.size() * sizeof(Vertex), &(this->_vertex_list[0]));
	this->_ebo = oGL_module::oGL_create_vbo(this->_indice_list.size() * sizeof(unsigned int), &(this->_indice_list[0]));
	oGL_module::oGL_set_vao_parameters(this->_vbo, this->_vao, 0, 3, sizeof(Vertex), 0);
	oGL_module::oGL_set_vao_parameters(this->_vbo, this->_vao, 1, 3, sizeof(Vertex), sizeof(GLfloat) * 3);
	oGL_module::oGL_set_vao_parameters(this->_vbo, this->_vao, 2, 3, sizeof(Vertex), sizeof(GLfloat) * 6);
	oGL_module::oGL_set_vao_parameters(this->_vbo, this->_vao, 3, 3, sizeof(Vertex), sizeof(GLfloat) * 8);
	oGL_module::oGL_set_vao_parameters(this->_vbo, this->_vao, 4, 3, sizeof(Vertex), sizeof(GLfloat) * 11);
}
