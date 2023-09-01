#pragma once
#include <vector>

//GameObject and primitives
#include "Primitive/Quad.h"
#include "Primitive/Cube.h"
#include "Primitive/Sphere.h"
#include "Primitive/Cylinder.h"
#include "Primitive/Cone.h"
#include "Primitive/Model.h"
#include "ModelLoader.h"

#include "SceneManager/Node.h"
#include "SceneManager/SceneManager.h"
#include "Renderer/Camera/SceneCamera.h"
#include "Renderer/Lighting/Light.h"

namespace TS_ENGINE
{
	class Scene;
	class SceneCamera;
	class Factory
	{
	public:
		static Factory* GetInstance();

		Ref<SceneCamera> InstantitateSceneCamera(const std::string& name, Scene* scene);
		Ref<SceneCamera> InstantitateDuplicateSceneCamera(Ref<SceneCamera> sceneCamera);
		
		Ref<Node> InstantiateLine(const std::string& name, Ref<Node> parentNode, const std::vector<Vector3>& points);
		Ref<Node> InstantiateQuad(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateCube(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateSphere(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateCylinder(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateCone(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateModel(const std::string& modelPath, Ref<Node> parentNode);

		void ChangeMeshForNode(Ref<Node> node, PrimitiveType primitive);
		//Ref<Light> CreateLight(Light::Type lightType);
	private:
		static Factory* mInstance;
		std::unordered_map<std::string, Ref<Node>> mLoadedModelNodeMap;//Model path and Model Node map
	};
}