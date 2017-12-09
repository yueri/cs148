#include "common/Scene/Lights/Point/PointLight.h"


void PointLight::ComputeSampleRays(std::vector<Ray>& output, glm::vec3 origin, glm::vec3 normal) const
{
    origin += normal * LARGE_EPSILON;
    const glm::vec3 lightPosition = glm::vec3(GetPosition());
    const glm::vec3 rayDirection = glm::normalize(lightPosition - origin);
    const float distanceToOrigin = glm::distance(origin, lightPosition);
    output.emplace_back(origin, rayDirection, distanceToOrigin);
}

float PointLight::ComputeLightAttenuation(glm::vec3 origin) const
{
    return 1.f;
}

void PointLight::GenerateRandomPhotonRay(Ray& ray) const
{
    // Assignment 8 TODO: Fill in the random point light samples here.
	ray.SetRayPosition(glm::vec3(GetPosition()));

	float x, y, z = 0;
	do {
		x = (2.0 * rand() / RAND_MAX) - 1;
		y = (2.0 * rand() / RAND_MAX) - 1;
		z = (2.0 * rand() / RAND_MAX) - 1;
	} while (x*x + y*y + z*z > 1);

	glm::vec3 rayDirection = normalize(glm::vec3(x, y, z));
	ray.SetRayDirection(rayDirection);
}
