#pragma once
#include <vector>
//GameObject and primitives
#include "GameObject.h"
#include "Primitive/Cube.h"
#include "Primitive/Sphere.h"
#include "Primitive/Quad.h"
#include "Primitive/Model.h"

//Cameras
#include "Renderer/SceneCamera.h"

#include "SceneManager/SceneManager.h"
#include "SceneManager/Node.h"
#include "Renderer/Lighting/Light.h"

namespace TS_ENGINE
{
	class Factory
	{
	public:
	private:
		static Factory* mInstance;
		std::vector<Ref<Object>> mInstantiatedObjects;
	public:
		static Factory* GetInstance();

		Ref<GameObject> CreateGameObject(GameObject::Type objectType);
#ifdef TS_ENGINE_EDITOR
		Ref<SceneCamera> CreateSceneCamera(Ref<Camera> editorCamera);
#else
		Ref<SceneCamera> CreateSceneCamera();
#endif
		Ref<Light> CreateLight(Light::Type lightType);
	};
}