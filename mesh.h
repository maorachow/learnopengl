#pragma once
#include "vertex.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "shader.cpp"

class Mesh {
public:
    unsigned int VAO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    Mesh(){}
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,std::vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
         SetupMesh();
    }
    void Draw(Shader& shader) {
       
        unsigned int diffuseNr = 1;
     /*   for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse")
                number = std::to_string(diffuseNr++);


            // now set the sampler to the correct texture unit
         //  
            shader.setInt("texture1", 1);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }*/
        shader.use();
        glActiveTexture(GL_TEXTURE0 );
        glUniform1i(glad_glGetUniformLocation(shader.ID, "texture1"), 0);
        glBindTexture(GL_TEXTURE_2D, textures[0].id);
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
      //  glDrawArrays(GL_TRIANGLES, 0, 36);
       
        
        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(0);
        //std::cout << "draw" << std::endl;
    }
    ~Mesh() {
        std::vector<unsigned int>().swap(indices);
        std::vector<Vertex>().swap(vertices);
    }
private:
    unsigned int VBO, EBO;
    void SetupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glBindVertexArray(0);
      //  std::cout << vertices[3].Position.s << std::endl;
    }
};