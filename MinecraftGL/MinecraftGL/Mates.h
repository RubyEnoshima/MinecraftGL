#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <limits>
#include <vector>
using namespace std;
struct Pla {
    Pla(){}
    Pla(const glm::vec3 _pos, const glm::vec3 _normal) {
        pos = _pos;
        normal = _normal;
        d = glm::dot(normal, pos);
    }
    glm::vec3 pos = { 0,0,0 }, normal = { 0,1,0 };
    float d = 0;

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
        float exitTime = std::min(xExit, std::min(yExit, zExit));

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

};
