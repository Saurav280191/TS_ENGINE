#pragma once
#include <vector>
//GameObject and primitives
#include "Primitive/Cube.h"
#include "Primitive/Sphere.h"
#include "Primitive/Quad.h"
#include "Primitive/Model.h"
#include "ModelLoader.h"

//Cameras
#include "Renderer/Camera/SceneCamera.h"

#include "SceneManager/SceneManager.h"
#include "SceneManager/Node.h"
#include "Renderer/Lighting/Light.h"

namespace TS_ENGINE
{
	class Factory
	{
	public:
		static Factory* GetInstance();

		Ref<SceneCamera> InstantitateSceneCamera(const std::string& name, Scene* scene);
		
		Ref<Node> InstantiateLine(const std::string& name, Ref<Node> parentNode, const std::vector<Vector3>& points);
		Ref<Node> InstantiateQuad(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateCube(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateSphere(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateModel(const std::string& modelPath, Ref<Node> parentNode);

		//Ref<GameObject> CreateGameObject(PrimitiveType type);
		//Ref<Model> LoadModel(std::string& modelPath);
		//void ChangeMeshForNode(Ref<Node> node, int primitiveIndex);
		
//#ifdef TS_ENGINE_EDITOR
		//Ref<SceneCamera> CreateSceneCamera(Ref<Camera> editorCamera);
//#else
		//Ref<SceneCamera> CreateSceneCamera();
//#endif
		//Ref<Light> CreateLight(Light::Type lightType);
	public:
		//std::vector<Ref<Object>> mInstantiatedObjects;
	private:
		static Factory* mInstance;
		std::unordered_map<std::string, Ref<Node>> mLoadedModelNodeMap;//Model path and Model Node map
	};
}