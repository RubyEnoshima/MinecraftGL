#include "RayCast.h"

RayCast::RayCast(Camera* cam, Renderer* renderer, GLFWwindow* _window)
{
	camera = cam;
	window = _window;
	r = renderer;
}

glm::vec3 RayCast::calcularRay() const
{
	float xCentre = r->obtenirCentre().first;
	float yCentre = r->obtenirCentre().second;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

	float x = (2.0f * xCentre) / r->obtenirTamany().first - 1.0f;
	float y = 1.0f - (2.0f * yCentre) / r->obtenirTamany().second;

	glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(camera->getProjection()) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	glm::vec4 ray_wor4 = glm::inverse(camera->getView()) * ray_eye;
	glm::vec3 ray_wor = glm::normalize(glm::vec3(ray_wor4.x,ray_wor4.y,ray_wor4.z));
	
	return ray_wor;
}

glm::vec3 RayCast::calcularRayV2() const
{
    return glm::vec3();
}

bool RayCast::rayTriangleIntersect(
    const glm::vec3& orig, const glm::vec3& dir,
    const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
    float& t)
{
    // compute plane's normal
    glm::vec3 v0v1 = v1 - v0;
    glm::vec3 v0v2 = v2 - v0;
    // no need to normalize
    glm::vec3 N = glm::cross(v0v1,v0v2);  //N 
    float area2 = N.length();

    // Step 1: finding P

    // check if ray and plane are parallel.
    float NdotRayDirection = glm::dot(N,dir);
    if (fabs(NdotRayDirection) < 0.0000001f)  //almost 0 
        return false;  //they are parallel so they don't intersect ! 

    // compute d parameter using equation 2
    float d = glm::dot(-N,v0);

    // compute t (equation 3)
    t = -(glm::dot(N,orig) + d) / NdotRayDirection;

    // check if the triangle is in behind the ray
    if (t < 0) return false;  //the triangle is behind 

    // compute the intersection point using equation 1
    glm::vec3 P = orig + t * dir;

    // Step 2: inside-outside test
    glm::vec3 C;  //vector perpendicular to triangle's plane 

    // edge 0
    glm::vec3 edge0 = v1 - v0;
    glm::vec3 vp0 = P - v0;
    C = glm::cross(edge0,vp0);
    if (glm::dot(N,C) < 0) return false;  //P is on the right side 

    // edge 1
    glm::vec3 edge1 = v2 - v1;
    glm::vec3 vp1 = P - v1;
    C = glm::cross(edge1,vp1);
    if (glm::dot(N,C) < 0)  return false;  //P is on the right side 

    // edge 2
    glm::vec3 edge2 = v0 - v2;
    glm::vec3 vp2 = P - v2;
    C = glm::cross(edge2,vp2);
    if (glm::dot(N,C) < 0) return false;  //P is on the right side; 

    return true;  //this ray hits the triangle 
}