#pragma once
#include "Core/tspch.h"
#include "Core/Object.h"
#include "Primitive/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"
#include "Renderer/RenderCommand.h"

namespace TS_ENGINE {

	class Mesh;
	class Object;
	class GameObject : public Object
	{
	public:
		GameObject();
		GameObject(const std::string& name);
		~GameObject();

		// Inherited via Object
		virtual void Initialize() override;
		virtual void Update(Ref<Shader> shader, float deltaTime) override;
		virtual void DeleteMeshes() override;

		void Destroy();

		void AddMesh(Ref<Mesh> mesh);
		void AddMeshes(std::vector<Ref<Mesh>> _meshes);
		std::vector<Ref<Mesh>> GetMeshes();	
	protected:
		std::vector<Ref<Mesh>> mMeshes;
	};
}

