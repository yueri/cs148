#include "assignment7/Assignment7.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment7::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(1280.0f / 720.0f, 60.0f);
    // camera->SetPosition(glm::vec3(0.f, -4.1469f, 0.73693f));
    // camera->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);
    return camera;
}


// Assignment 7 Part 1 TODO: Change the '1' here.
// 0 -- Naive.
// 1 -- BVH.
// 2 -- Grid.
#define ACCELERATION_TYPE 2

std::shared_ptr<Scene> Assignment7::CreateScene() const
{
    std::shared_ptr<Scene> newScene = std::make_shared<Scene>();

    // Material
    std::shared_ptr<BlinnPhongMaterial> cubeMaterial = std::make_shared<BlinnPhongMaterial>();
    cubeMaterial->SetDiffuse(glm::vec3(1.f, 1.f, 1.f));
    cubeMaterial->SetSpecular(glm::vec3(0.6f, 0.6f, 0.6f), 40.f);
    cubeMaterial->SetReflectivity(0.3f);

    // Objects
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
	std::vector<std::shared_ptr<MeshObject>> tableObjects = MeshLoader::LoadMesh("Table/Table.obj", &loadedMaterials);
    for (size_t i = 0; i < tableObjects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(loadedMaterials[i]);
		materialCopy->SetTexture("diffuseTexture", TextureLoader::LoadTexture("Table/pCube6Shape_color.png"));
		materialCopy->SetTexture("specularTexture", TextureLoader::LoadTexture("Table/pCube6Shape_color.png"));
		tableObjects[i]->SetMaterial(materialCopy);

        std::shared_ptr<SceneObject> tableSceneObject = std::make_shared<SceneObject>();
		tableSceneObject->AddMeshObject(tableObjects[i]);
		tableSceneObject->SetPosition(glm::vec3(0.0f, -8.0f, -20.0f));
        // cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);

		tableSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		tableSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });

		tableSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
            BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
            accelerator->SetMaximumChildren(2);
            accelerator->SetNodesOnLeaves(2);
        });
        newScene->AddSceneObject(tableSceneObject);
    }

	std::vector<std::shared_ptr<aiMaterial>> loadedMaterials2;
	std::vector<std::shared_ptr<MeshObject>> fishObjects = MeshLoader::LoadMesh("Room/Room.obj", &loadedMaterials2);
	for (size_t i = 0; i < fishObjects.size(); ++i) {
		std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
		materialCopy->LoadMaterialFromAssimp(loadedMaterials2[i]);
		materialCopy->SetTexture("diffuseTexture", TextureLoader::LoadTexture("Room/wallpaper.jpg"));
		materialCopy->SetTexture("specularTexture", TextureLoader::LoadTexture("Room/wallpaper.jpg"));
		fishObjects[i]->SetMaterial(materialCopy);

		std::shared_ptr<SceneObject> fishSceneObject = std::make_shared<SceneObject>();
		fishSceneObject->AddMeshObject(fishObjects[i]);
		fishSceneObject->SetPosition(glm::vec3(0.0f, 8.0f, -20.0f));
		// cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);

		fishSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		fishSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		fishSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});
		newScene->AddSceneObject(fishSceneObject);
	}

    // Lights
    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    pointLight->SetLightColor(glm::vec3(1.f, 1.f, 1.f));

	std::shared_ptr<Light> pointLight2 = std::make_shared<PointLight>();
	pointLight2->SetPosition(glm::vec3(0.0f, 0.0f, -20.0f));
	pointLight2->SetLightColor(glm::vec3(1.f, 1.f, 1.f));

	std::shared_ptr<Light> pointLight3 = std::make_shared<PointLight>();
	pointLight3->SetPosition(glm::vec3(10.0f, 0.0f, -30.0f));
	pointLight3->SetLightColor(glm::vec3(1.f, 1.f, 1.f));

	std::shared_ptr<Light> pointLight4 = std::make_shared<PointLight>();
	pointLight4->SetPosition(glm::vec3(-10.0f, 0.0f, -30.0f));
	pointLight4->SetLightColor(glm::vec3(1.f, 1.f, 1.f));

	std::shared_ptr<Light> pointLight5 = std::make_shared<PointLight>();
	pointLight5->SetPosition(glm::vec3(0.0f, -10.0f, -30.0f));
	pointLight5->SetLightColor(glm::vec3(1.f, 1.f, 1.f));

	std::shared_ptr<Light> pointLight6 = std::make_shared<PointLight>();
	pointLight6->SetPosition(glm::vec3(0.0f, 10.0f, -30.0f));
	pointLight6->SetLightColor(glm::vec3(1.f, 1.f, 1.f));

#if ACCELERATION_TYPE == 0
    newScene->GenerateAccelerationData(AccelerationTypes::NONE);
#elif ACCELERATION_TYPE == 1
    newScene->GenerateAccelerationData(AccelerationTypes::BVH);
#else
    UniformGridAcceleration* accelerator = dynamic_cast<UniformGridAcceleration*>(newScene->GenerateAccelerationData(AccelerationTypes::UNIFORM_GRID));
    assert(accelerator);
    // Assignment 7 Part 2 TODO: Change the glm::ivec3(10, 10, 10) here.
    accelerator->SetSuggestedGridSize(glm::ivec3(10, 10, 10));
#endif    
    newScene->AddLight(pointLight);
	newScene->AddLight(pointLight2);

    return newScene;

}
std::shared_ptr<ColorSampler> Assignment7::CreateSampler() const
{
    std::shared_ptr<JitterColorSampler> jitter = std::make_shared<JitterColorSampler>();
    jitter->SetGridSize(glm::ivec3(1, 1, 1));
    return jitter;
}

std::shared_ptr<class Renderer> Assignment7::CreateRenderer(std::shared_ptr<Scene> scene, std::shared_ptr<ColorSampler> sampler) const
{
    return std::make_shared<BackwardRenderer>(scene, sampler);
}

int Assignment7::GetSamplesPerPixel() const
{
    return 1;
}

bool Assignment7::NotifyNewPixelSample(glm::vec3 inputSampleColor, int sampleIndex)
{
    return true;
}

int Assignment7::GetMaxReflectionBounces() const
{
    return 2;
}

int Assignment7::GetMaxRefractionBounces() const
{
    return 4;
}

glm::vec2 Assignment7::GetImageOutputResolution() const
{
    return glm::vec2(640.f, 480.f);
}
