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

        Model(std::string const &name, std::string const &path);
        Model(Model const &src);
        Model		&operator=(Model const &rhs);
        virtual ~Model(void);

        std::string const		        &getName(void) const;
        std::vector<Texture> const      &getTextureList(void);
        std::vector<Mesh> const         &getMeshList(void);

    private :

        std::string             _name;
        std::vector<Mesh>       _mesh_list;
        std::vector<Texture>    _texture_list;

        void    _load_model(std::string const &path);
        void    _load_node(aiNode *node, const aiScene *scene);
        void    _load_mesh(aiMesh *mesh, const aiScene *scene);
        void    _load_material();
};

#endif