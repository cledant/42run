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
# include "GeneralException.hpp"
# include "glm/glm.hpp"
# include <iostream>

class Mesh
{
    public :
        Mesh();

    struct Vertex
    {
        glm::vec3   Position;
        glm::vec3   Normal;
        glm::vec2   TexCoords;
        glm::vec3   Tangent;
        glm::vec3   Bitangent;
    };

    private :
};

#endif
