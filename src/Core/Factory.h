#pragma once
#include <vector>

//GameObject and primitives
#include "Primitive/Quad.h"
#include "Primitive/Cube.h"
#include "Primitive/Sphere.h"
#include "Primitive/Cylinder.h"
#include "Primitive/Cone.h"
#include "Components/Terrain.h"
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

		Ref<Node> InstantitateEmptyNode(const std::string& name, Ref<Node> parentNode);

		Ref<Node> InstantitateSceneCamera(const std::string& name 
#ifdef TS_ENGINE_EDITOR
			, Ref<EditorCamera> editorCamera
#endif
		);

		Ref<Node> InstantitateDuplicateSceneCamera(Ref<SceneCamera> sceneCamera);

		Ref<Node> InstantiateLine(const std::string& name, Ref<Node> parentNode, const std::vector<Vector3>& points);
		Ref<Node> InstantiateQuad(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateCube(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateSphere(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateSphere(const std::string& _name, float _radius, Ref<Node> _parentNode);
		Ref<Node> InstantiateCylinder(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateCone(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateTerrain(const std::string& name, Ref<Node> parentNode);				
		Ref<Node> InstantiateTerrain(const std::string& name, double _width, double _depth, double _gridSpacing, Ref<Node> parentNode);

		Ref<Node> InstantiateSphereGui(const std::string& name, Ref<Node> parentNode);
		Ref<Node> InstantiateBone(const std::string& name, Ref<Node> parentNode);

		std::pair<Ref<Node>, Ref<Model>> InstantiateModel(const std::string& modelPath, Ref<Node> parentNode);

		void ChangeMeshForNode(Ref<Node> node, PrimitiveType primitive);
		//Ref<Light> CreateLight(Light::Type lightType);
		void Clear();

		const Ref<Node> FindNodeByName(const std::string& _name);

		int GetBoneIdByName(std::string& _name);

		std::unordered_map<std::string, std::pair<Ref<Node>, Ref<Model>>> mLoadedModelNodeMap;//Model path and Model Node map
	private:
		static Factory* mInstance;
	};
}