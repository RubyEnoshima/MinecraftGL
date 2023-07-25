#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <limits>

struct Pla {
    Pla(){}
    Pla(const glm::vec3 _pos, const glm::vec3 _normal) {
        pos = _pos;
        normal = _normal;
        d = glm::dot(normal, pos);
    }
    glm::vec3 pos = { 0,0,0 }, normal = { 0,1,0 };
    float d = 0;

    float distancia(const glm::vec3& p) const{
        return glm::dot(normal, p) - d;
    }
};

struct Frustum {
    Pla topFace;
    Pla bottomFace;

    Pla rightFace;
    Pla leftFace;

    Pla farFace;
    Pla nearFace;

    vector<Pla> obtPlans() const {
        return {topFace,bottomFace,rightFace,leftFace,farFace,nearFace};
    }
};

struct AABB {
	glm::vec3 pos; //cantonada esquerra superior
	glm::vec3 tamany;
	glm::vec3 vel;
    ShaderProgram* shader = NULL;

    pair<glm::vec3,glm::vec3> MinMax() const{
        return {pos - tamany * 0.5f, pos + tamany * 0.5f};
    }

    bool AABBtoAABB(const AABB& tBox2)
    {
        pair<glm::vec3, glm::vec3> minmax1 = MinMax(), minmax2 = tBox2.MinMax();

        //Check if Box1's max is greater than Box2's min and Box1's min is less than Box2's max
        return(minmax1.second.x > minmax2.first.x &&
            minmax1.first.x < minmax2.second.x &&
            minmax1.second.y > minmax2.first.y &&
            minmax1.first.y < minmax2.second.y &&
            minmax1.second.z > minmax2.first.z &&
            minmax1.first.z < minmax2.second.z);

        //If not, it will return false

    }

    // Editat de https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/
    float sweptAABB(const AABB& b2, float& normalx, float& normaly, float& normalz) {
        float xInvEntry, yInvEntry, zInvEntry;
        float xInvExit, yInvExit, zInvExit;

        // find the distance between the objects on the near and far sides for both x and y 
        if (vel.x > 0.0f)
        {
            xInvEntry = b2.pos.x - (pos.x + tamany.x);
            xInvExit = (b2.pos.x + b2.tamany.x) - pos.x;
        }
        else
        {
            xInvEntry = (b2.pos.x + b2.tamany.x) - pos.x;
            xInvExit = b2.pos.x - (pos.x + tamany.x);
        }

        if (vel.y > 0.0f)
        {
            yInvEntry = b2.pos.y - (pos.y + tamany.y);
            yInvExit = (b2.pos.y + b2.tamany.y) - pos.y;
        }
        else
        {
            yInvEntry = (b2.pos.y + b2.tamany.y) - pos.y;
            yInvExit = b2.pos.y - (pos.y + tamany.y);
        }

        if (vel.z > 0.0f)
        {
            zInvEntry = b2.pos.z - (pos.z + tamany.z);
            zInvExit = (b2.pos.z + b2.tamany.z) - pos.z;
        }
        else
        {
            zInvEntry = (b2.pos.z + b2.tamany.z) - pos.z;
            zInvExit = b2.pos.z - (pos.z + tamany.z);
        }

        // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero) 
        float xEntry, yEntry, zEntry;
        float xExit, yExit, zExit;

        if (vel.x == 0.0f)
        {
            xEntry = -std::numeric_limits<float>::infinity();
            xExit = std::numeric_limits<float>::infinity();
        }
        else
        {
            xEntry = xInvEntry / vel.x;
            xExit = xInvExit / vel.x;
        }

        if (vel.y == 0.0f)
        {
            yEntry = -std::numeric_limits<float>::infinity();
            yExit = std::numeric_limits<float>::infinity();
        }
        else
        {
            yEntry = yInvEntry / vel.y;
            yExit = yInvExit / vel.y;
        }

        if (vel.z == 0.0f)
        {
            zEntry = -std::numeric_limits<float>::infinity();
            zExit = std::numeric_limits<float>::infinity();
        }
        else
        {
            zEntry = zInvEntry / vel.z;
            zExit = zInvExit / vel.z;
        }

        // find the earliest/latest times of collisionfloat 
        float entryTime = std::max(xEntry, std::max(yEntry, zEntry));
        float exitTime = std::min(xExit, std::max(yExit, zExit));

        // if there was no collision
        if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f && zEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f || zEntry > 1.0f)
        {
            normalx = 0.0f;
            normaly = 0.0f;
            normalz = 0.0f;
            return 1.0f;
        }
        else // if there was a collision 
        {
            // calculate normal of collided surface
            if (xEntry > yEntry && xEntry > zEntry)
            {
                if (xInvEntry < 0.0f)
                {
                    normalx = 1.0f;
                    normaly = 0.0f;
                    normalz = 0.0f;

                }
                else
                {
                    normalx = -1.0f;
                    normaly = 0.0f;
                    normalz = 0.0f;

                }
            }
            else if(yEntry > zEntry)
            {
                if (yInvEntry < 0.0f)
                {
                    normalx = 0.0f;
                    normaly = 1.0f;
                    normalz = 0.0f;

                }
                else
                {
                    normalx = 0.0f;
                    normaly = -1.0f;
                    normalz = 0.0f;

                }
            } 
            else {
                if (zInvEntry < 0.0f)
                {
                    normalx = 0.0f;
                    normaly = 0.0f;
                    normalz = 1.0f;

                }
                else
                {
                    normalx = 0.0f;
                    normaly = 0.0f;
                    normalz = -1.0f;

                }
            }
            // return the time of collision
            return entryTime;
        }
    }

	void render(bool centrat = false) const{
        static const float vertices[] = {
            pos.x,   pos.y,   pos.z,
            pos.x +  tamany.x,  pos.y,   pos.z,
            pos.x +  tamany.x,  pos.y -  tamany.y,     pos.z,
            pos.x,   pos.y -  tamany.y,     pos.z,
            pos.x,   pos.y,   pos.z -  tamany.z,
            pos.x +  tamany.x,  pos.y,   pos.z -  tamany.z,
            pos.x +  tamany.x,  pos.y -  tamany.y,     pos.z -  tamany.z,
            pos.x,   pos.y -  tamany.y,     pos.z -  tamany.z
        };

        static const unsigned int indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0, // Aristas de la cara frontal
        4, 5, 5, 6, 6, 7, 7, 4, // Aristas de la cara trasera
        0, 4, 1, 5, 2, 6, 3, 7  // Aristas que conectan las caras frontal y trasera
        };

        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Renderiza el AABB utilizando líneas
        //glUseProgram(shaderProgram);
        //glUniform3f(glGetUniformLocation(shaderProgram, "color"), box.r, box.g, box.b);

        glm::mat4 model = glm::mat4(1);
        if(centrat)model = glm::translate(glm::mat4(1), glm::vec3(pos.x - tamany.x / 2, 0, pos.z + tamany.z / 2));
        else model = glm::translate(glm::mat4(1), glm::vec3(pos.x, 0, pos.z));
        shader->colocarMat4("model", model);

        glDrawElements(GL_LINES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
	}
};
