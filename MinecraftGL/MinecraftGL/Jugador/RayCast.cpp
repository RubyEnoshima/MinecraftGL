#include "RayCast.h"

RayCast::RayCast(Camera* cam, Renderer* renderer)
{
	camera = cam;
	r = renderer;
}

glm::vec3 RayCast::calcularRay()
{
	float xCentre = r->obtenirCentre().first;
	float yCentre = r->obtenirCentre().second;

	float x = (2.0f * xCentre) / r->obtenirTamany().first - 1.0f;
	float y = 1.0f - (2.0f * yCentre) / r->obtenirTamany().second;

	glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(camera->getProjection()) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	glm::vec4 ray_wor4 = glm::inverse(camera->getView()) * ray_eye;
	glm::vec3 ray_wor = glm::normalize(glm::vec3(ray_wor4.x,ray_wor4.y,ray_wor4.z));
	Dir = ray_wor;
	return ray_wor;
}

bool RayCast::intersecta(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2) const
{
	glm::vec3 Origen = camera->obtPos();
	
	// Calculem la normal del pla del triangle
	glm::vec3 v0v1 = v1 - v0;
	glm::vec3 v0v2 = v2 - v0;
	glm::vec3 N = glm::cross(v0v1,v0v2);

	float denom = glm::dot(N, Dir);


	if (fabs(denom) > 0.01f) // !parallel
	{
		float t = glm::dot(N, v0 - Origen) / denom;
		cout << v0 << v1 << v2 << N << t << endl;
		return t>=0; // hit, intersection value
	}
	return false;

	
	float NdotRayDirection = glm::dot(N,Dir);
	if (fabs(NdotRayDirection) < 0.001f)  //almost 0 
		return false;  //they are parallel so they don't intersect ! 

	// compute d parameter using equation 2
	float d = glm::dot(- N,v0);

	// compute t (equation 3)
	float t = -(glm::dot(N,Origen) + d) / NdotRayDirection;

	// check if the triangle is in behind the ray
	if (t < 0) return false;  //the triangle is behind 
	return true;
}

float RayCast::calcT(const glm::vec3& O, const glm::vec3& D, const glm::vec3 n) const {
    return -((glm::dot(O, n)) / (glm::dot(D, n)));
}

glm::vec3 RayCast::calcPunt(const glm::vec3& O, const glm::vec3& D, float t) const {
    return O + (D*t);
}