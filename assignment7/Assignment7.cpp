#include "assignment7/Assignment7.h"
#include "common/core.h"

std::shared_ptr<Camera> Assignment7::CreateCamera() const
{
    const glm::vec2 resolution = GetImageOutputResolution();
    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(1280.0f / 720.0f, 60.0f);
    camera->SetPosition(glm::vec3(15.f, 8.0f, 12.f));
    camera->Rotate(glm::vec3(0.f, -1.0f, 0.f), -PI / 4.0f);
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
    cubeMaterial->SetReflectivity(0.f);

    // Objects
    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials1;
	std::vector<std::shared_ptr<MeshObject>> tableObjects = MeshLoader::LoadMesh("Table/Table.obj", &loadedMaterials1);
    for (size_t i = 0; i < tableObjects.size(); ++i) {
        std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
        materialCopy->LoadMaterialFromAssimp(loadedMaterials1[i]);
		materialCopy->SetTexture("diffuseTexture", TextureLoader::LoadTexture("Table/Table.jpg"));
		materialCopy->SetTexture("specularTexture", TextureLoader::LoadTexture("Table/Table.jpg"));
		tableObjects[i]->SetMaterial(materialCopy);

        std::shared_ptr<SceneObject> tableSceneObject = std::make_shared<SceneObject>();
		tableSceneObject->AddMeshObject(tableObjects[i]);
		tableSceneObject->SetPosition(glm::vec3(0.0f, 0.f, 0.f));
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
	std::vector<std::shared_ptr<MeshObject>> roomObjects = MeshLoader::LoadMesh("Room/Room.obj", &loadedMaterials2);
	for (size_t i = 0; i < roomObjects.size(); ++i) {
		std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
		materialCopy->LoadMaterialFromAssimp(loadedMaterials2[i]);
		materialCopy->SetTexture("diffuseTexture", TextureLoader::LoadTexture("Room/wall_carpet_2.jpg"));
		materialCopy->SetTexture("specularTexture", TextureLoader::LoadTexture("Room/wall_carpet_2.jpg"));
		roomObjects[i]->SetMaterial(materialCopy);

		std::shared_ptr<SceneObject> roomSceneObject = std::make_shared<SceneObject>();
		roomSceneObject->AddMeshObject(roomObjects[i]);
		roomSceneObject->SetPosition(glm::vec3(0.0f, 0.f, 0.f));
		// cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);

		roomSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		roomSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		roomSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});
		newScene->AddSceneObject(roomSceneObject);
	}

	std::vector<std::shared_ptr<aiMaterial>> loadedMaterials3;
	std::vector<std::shared_ptr<MeshObject>> plantObjects = MeshLoader::LoadMesh("Plant/plant.obj", &loadedMaterials3);
	for (size_t i = 0; i < plantObjects.size(); ++i) {
		std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
		materialCopy->LoadMaterialFromAssimp(loadedMaterials3[i]);
		plantObjects[i]->SetMaterial(materialCopy);

		std::shared_ptr<SceneObject> plantSceneObject = std::make_shared<SceneObject>();
		plantSceneObject->AddMeshObject(plantObjects[i]);
		plantSceneObject->SetPosition(glm::vec3(0.0f, 0.f, 0.f));
		// cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);

		plantSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		plantSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		plantSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});
		newScene->AddSceneObject(plantSceneObject);
	}

	std::vector<std::shared_ptr<aiMaterial>> loadedMaterials4;
	std::vector<std::shared_ptr<MeshObject>> frameObjects = MeshLoader::LoadMesh("frame/merry go round.obj", &loadedMaterials4);
	for (size_t i = 0; i < frameObjects.size(); ++i) {
		std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
		materialCopy->LoadMaterialFromAssimp(loadedMaterials4[i]);
		materialCopy->SetTexture("diffuseTexture", TextureLoader::LoadTexture("frame/poster.jpeg"));
		materialCopy->SetTexture("specularTexture", TextureLoader::LoadTexture("frame/poster.jpeg"));
		frameObjects[i]->SetMaterial(materialCopy);

		std::shared_ptr<SceneObject> frameSceneObject = std::make_shared<SceneObject>();
		frameSceneObject->AddMeshObject(frameObjects[i]);
		frameSceneObject->SetPosition(glm::vec3(0.0f, 0.f, 0.f));
		// cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);

		frameSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		frameSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		frameSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});
		newScene->AddSceneObject(frameSceneObject);
	}

	std::vector<std::shared_ptr<aiMaterial>> loadedMaterials5;
	std::vector<std::shared_ptr<MeshObject>> lampObjects = MeshLoader::LoadMesh("lamp/eb_lamp_01.obj", &loadedMaterials5);
	for (size_t i = 0; i < lampObjects.size(); ++i) {
		std::shared_ptr<Material> materialCopy = cubeMaterial->Clone();
		materialCopy->LoadMaterialFromAssimp(loadedMaterials5[i]);
		materialCopy->SetTexture("diffuseTexture", TextureLoader::LoadTexture("lamp/eb_lamp_01_c.tga"));
		materialCopy->SetTexture("specularTexture", TextureLoader::LoadTexture("lamp/eb_lamp_01_c.tga"));
		lampObjects[i]->SetMaterial(materialCopy);

		std::shared_ptr<SceneObject> lampSceneObject = std::make_shared<SceneObject>();
		lampSceneObject->AddMeshObject(lampObjects[i]);
		lampSceneObject->SetPosition(glm::vec3(0.0f, 0.f, 0.f));
		// cubeSceneObject->Rotate(glm::vec3(1.f, 0.f, 0.f), PI / 2.f);

		lampSceneObject->CreateAccelerationData(AccelerationTypes::BVH);
		lampSceneObject->ConfigureAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});

		lampSceneObject->ConfigureChildMeshAccelerationStructure([](AccelerationStructure* genericAccelerator) {
			BVHAcceleration* accelerator = dynamic_cast<BVHAcceleration*>(genericAccelerator);
			accelerator->SetMaximumChildren(2);
			accelerator->SetNodesOnLeaves(2);
		});
		newScene->AddSceneObject(lampSceneObject);
	}

    // Lights
    std::shared_ptr<Light> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3(30.0f, 30.0f, 30.0f));
    pointLight->SetLightColor(glm::vec3(0.8f, 0.8f, 0.8f));

	// std::shared_ptr<Light> directionalLight = std::make_shared<DirectionalLight>();
	// directionalLight->SetLightColor(glm::vec3(0.5f, 0.5f, 0.5f));
	// directionalLight->Rotate(glm::vec3(0.f, -1.0f, 0.f), -PI / 4.0f);

	std::shared_ptr<Light> lampLight = std::make_shared<PointLight>();
	lampLight->SetPosition(glm::vec3(11.5f, 12.0f, -2.0f));
	lampLight->SetLightColor(glm::vec3(0.8f, 0.8f, 0.8f));


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
	// newScene->AddLight(directionalLight);
	newScene->AddLight(lampLight);

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
