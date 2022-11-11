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

bool RayCast::intersecta(const glm::vec3& O, const glm::vec3& D, const glm::vec3 n) const
{
    //float t = calcT(O,D,n);
    //return t > 0;

    float denominator = glm::dot(n,D);

    if (denominator >= 1e-6) // 1e-6 = 0.000001
    {
        glm::vec3 vector_subtraction = glm::vec3(0,0,0) - O;
        float distance = glm::dot(vector_subtraction,n);

        return (distance >= 0);
    }

    return false;
}

glm::vec3 RayCast::calcularRay2() const
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    cout << xpos << ypos;

    float mouseX = xpos / (r->obtenirTamany().first * 0.5f) - 1.0f;
    float mouseY = ypos / (r->obtenirTamany().second * 0.5f) - 1.0f;

    glm::mat4 proj = camera->getProjection();
    glm::mat4 view = camera->getView();

    glm::mat4 invVP = glm::inverse(proj * view);
    glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
    glm::vec4 worldPos = invVP * screenPos;

    glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

    return dir;
}

float RayCast::calcT(const glm::vec3& O, const glm::vec3& D, const glm::vec3 n) const {
    return -((glm::dot(O, n)) / (glm::dot(D, n)));
}

glm::vec3 RayCast::calcPunt(const glm::vec3& O, const glm::vec3& D, float t) const {
    return O + (D*t);
}