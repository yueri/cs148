#include "common/Rendering/Renderer/Photon/PhotonMappingRenderer.h"
#include "common/Scene/Scene.h"
#include "common/Sampling/ColorSampler.h"
#include "common/Scene/Lights/Light.h"
#include "common/Scene/Geometry/Primitives/Primitive.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Rendering/Material/Material.h"
#include "common/Intersection/IntersectionState.h"
#include "common/Scene/SceneObject.h"
#include "common/Scene/Geometry/Mesh/MeshObject.h"
#include "common/Rendering/Material/Material.h"
#include "glm/gtx/component_wise.hpp"

#define VISUALIZE_PHOTON_MAPPING 1

static const float EPSILON = 0.001f;

PhotonMappingRenderer::PhotonMappingRenderer(std::shared_ptr<class Scene> scene, std::shared_ptr<class ColorSampler> sampler):
    BackwardRenderer(scene, sampler), 
    diffusePhotonNumber(1000000),
    maxPhotonBounces(1000)
{
    srand(static_cast<unsigned int>(time(NULL)));
}

void PhotonMappingRenderer::InitializeRenderer()
{
    // Generate Photon Maps
    GenericPhotonMapGeneration(diffuseMap, diffusePhotonNumber);
    diffuseMap.optimise();
}

void PhotonMappingRenderer::GenericPhotonMapGeneration(PhotonKdtree& photonMap, int totalPhotons)
{
    float totalLightIntensity = 0.f;
    size_t totalLights = storedScene->GetTotalLights();
    for (size_t i = 0; i < totalLights; ++i) {
        const Light* currentLight = storedScene->GetLightObject(i);
        if (!currentLight) {
            continue;
        }
        totalLightIntensity += glm::length(currentLight->GetLightColor());
    }

    // Shoot photons -- number of photons for light is proportional to the light's intensity relative to the total light intensity of the scene.
    for (size_t i = 0; i < totalLights; ++i) {
        const Light* currentLight = storedScene->GetLightObject(i);
        if (!currentLight) {
            continue;
        }

        const float proportion = glm::length(currentLight->GetLightColor()) / totalLightIntensity;
        const int totalPhotonsForLight = static_cast<const int>(proportion * totalPhotons);
        const glm::vec3 photonIntensity = currentLight->GetLightColor() / static_cast<float>(totalPhotonsForLight);
        for (int j = 0; j < totalPhotonsForLight; ++j) {
            Ray photonRay;
            std::vector<char> path;
            path.push_back('L');
            currentLight->GenerateRandomPhotonRay(photonRay);
            TracePhoton(photonMap, &photonRay, photonIntensity, path, 1.f, maxPhotonBounces);
        }
    }
}

void PhotonMappingRenderer::TracePhoton(PhotonKdtree& photonMap, Ray* photonRay, glm::vec3 lightIntensity, std::vector<char>& path, float currentIOR, int remainingBounces)
{
    /*
     * Assignment 8 TODO: Trace a photon into the scene and make it bounce.
     *    
     *    How to insert a 'Photon' struct into the photon map.
     *        Photon myPhoton;
     *        ... set photon properties ...
     *        photonMap.insert(myPhoton);
     */

    assert(photonRay);
    IntersectionState state(0, 0);
    state.currentIOR = currentIOR;

	// done with photon
	if (remainingBounces < 0 || !storedScene->Trace(photonRay, &state)) {
		return;
	}

	// create photon that doesn't come directly from light
	if (path.size() > 1) {
		Photon newPhoton;
		newPhoton.position = state.intersectionRay.GetRayPosition(state.intersectionT) - EPSILON * photonRay->GetRayDirection();
		newPhoton.intensity = lightIntensity;
		newPhoton.toLightRay.SetRayDirection(-1.0f * photonRay->GetRayDirection());
		photonMap.insert(newPhoton);
	}

	const MeshObject *hitMeshObject = state.intersectedPrimitive->GetParentMeshObject();
	const Material *hitMaterial = hitMeshObject->GetMaterial();
	const glm::vec3 diffuse = hitMaterial->GetBaseDiffuseReflection();

	float r = diffuse[0];
	float g = diffuse[1];
	float b = diffuse[2];

	float m = (r < g) ? g : r;
	float p = (m < b) ? b : m;

	float gen = 1.0 * rand() / RAND_MAX;

	// absorb photon
	if (gen > p) {
		return;
	}

	// scatter photon
	// sampling
	float u1 = 1.0 * rand() / RAND_MAX;
	float u2 = 1.0 * rand() / RAND_MAX;

	float rad = sqrt(u1);
	float theta = 2 * PI * u2;

	float x = rad * cos(theta);
	float y = rad * sin(theta);
	float z = sqrt(1 - u1);

	glm::vec3 ray = normalize(glm::vec3(x, y, z));
	
	// transformation
	// std::cout << "transform" << std::endl;
	glm::vec3 nv = state.ComputeNormal();
	glm::vec3 tv;
	if (abs(dot(nv, glm::vec3(1, 0, 0))) < 0.99) {
		tv = normalize(cross(nv, glm::vec3(1, 0, 0)));
	}
	else {
		tv = normalize(cross(nv, glm::vec3(0, 1, 0)));
	}
	glm::vec3 bv = normalize(cross(nv, tv));
	glm::vec3 transformedRay = glm::mat3(tv, bv, nv) * ray;

	// std::cout << nv[0] << ", " << nv[1] << ", " << nv[2] << std::endl;
	// std::cout << tv[0] << ", " << tv[1] << ", " << tv[2] << std::endl;
	// std::cout << bv[0] << ", " << bv[1] << ", " << bv[2] << std::endl;

	// recurse
	path.push_back('S');
	remainingBounces -= 1;
	photonRay->SetRayDirection(transformedRay);
	photonRay->SetRayPosition(state.intersectionRay.GetRayPosition(state.intersectionT) + EPSILON * photonRay->GetRayDirection());
	TracePhoton(photonMap, photonRay, lightIntensity, path, currentIOR, remainingBounces);
}

glm::vec3 PhotonMappingRenderer::ComputeSampleColor(const struct IntersectionState& intersection, const class Ray& fromCameraRay) const
{
    glm::vec3 finalRenderColor = BackwardRenderer::ComputeSampleColor(intersection, fromCameraRay);
#if VISUALIZE_PHOTON_MAPPING
    Photon intersectionVirtualPhoton;
    intersectionVirtualPhoton.position = intersection.intersectionRay.GetRayPosition(intersection.intersectionT);

    std::vector<Photon> foundPhotons;
    diffuseMap.find_within_range(intersectionVirtualPhoton, 0.003f, std::back_inserter(foundPhotons));
    if (!foundPhotons.empty()) {
        finalRenderColor += glm::vec3(1.f, 0.f, 0.f);
    }
#endif
    return finalRenderColor;
}

void PhotonMappingRenderer::SetNumberOfDiffusePhotons(int diffuse)
{
    diffusePhotonNumber = diffuse;
}
