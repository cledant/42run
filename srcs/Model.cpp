/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Model.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/03 17:06:31 by cledant           #+#    #+#             */
/*   Updated: 2017/11/03 17:06:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Model.hpp"

Model::Model(void) : _name("")
{
}

Model::Model(std::string const &name, std::string const &path) : _name(name)
{
	this->_load_model(path);
}

Model::Model(Model &&src) : _name("")
{
	this->_name      = src.getName();
	this->_mesh_list = src.getMeshList(); //not sure => check later
}

Model &Model::operator=(Model &&rhs)
{
	this->_name      = rhs.getName();
	this->_mesh_list = rhs.getMeshList(); //not sure => check later
	return (*this);
}

Model::~Model(void)
{
}

std::string const &Model::getName(void) const
{
	return (this->_name);
}

std::vector<Mesh> const &Model::getMeshList() const
{
	return (this->_mesh_list);
}

void Model::draw(void) const
{
	std::vector<Mesh>::const_iterator it;

	for (it = this->_mesh_list.begin(); it != this->_mesh_list.end(); ++it)
		it->draw();
}

void Model::_load_model(std::string const &path)
{
	Assimp::Importer importer;
	const aiScene    *scene;

	scene = importer.ReadFile(path,
							  aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		throw Model::FileOpenException(path);
	this->_load_node(scene->mRootNode, scene);
}

void Model::_load_node(aiNode *node, const aiScene *scene)
{
	if (node == NULL)
		throw Model::InvalidNodeException();
	for (size_t i = 0; i < node->mNumMeshes; ++i)
		this->_mesh_list.push_back({scene->mMeshes[node->mMeshes[i]], scene});
	for (size_t j = 0; j < node->mNumChildren; ++j)
		this->_load_node(node->mChildren[j], scene);
}

Model::FileOpenException::FileOpenException(std::string const &path)
{
	this->_msg = "Model : Failed to find to open file : ";
	this->_msg += path.c_str();
}

Model::FileOpenException::FileOpenException(void)
{
	this->_msg = "Model : Failed to find to open file";
}

Model::FileOpenException::~FileOpenException(void) throw()
{
}

Model::InvalidNodeException::InvalidNodeException(void)
{
	this->_msg = "Model : Invalid Node";
}

Model::InvalidNodeException::~InvalidNodeException(void) throw()
{
}