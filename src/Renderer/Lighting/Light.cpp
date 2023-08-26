#include "tspch.h"
#include "Renderer/Lighting/Light.h"

namespace TS_ENGINE {

	Light::Light() :
		mType(Type::DIRECTIONAL)
	{
		//mEntityType = EntityType::LIGHT;
	}

	Light::~Light()
	{		
		
	}

	//void Light::Initialize()
	//{
	//	mEntityID = EntityManager::GetInstance()->Instantiate(mName, mEntityType);
	//}

	//void Light::Update(Ref<Shader> shader, float deltaTime)
	//{
	//	//this->linear = 1.0f;			
	//	//this->constant = 0.09f;			
	//	//this->quadratic = 0.032f;			
	//	//this->cutOff = glm::cos(glm::radians(12.5f));
	//	//this->outerCutOff = glm::cos(glm::radians(17.5f));

	//	mShader->SetVec3("u_LightPos", this->position);
	//	mShader->SetVec3("dirLight.position", this->position);
	//	mShader->SetVec3("dirLight.direction", this->direction);
	//	mShader->SetVec3("dirLight.ambient", this->ambient);
	//	mShader->SetVec3("dirLight.diffuse", this->diffuse);
	//	mShader->SetVec3("dirLight.specular", this->specular);
	//	//mShader->SetFloat("light.linear", this->linear);
	//	//mShader->SetFloat("light.constant", this->constant);
	//	//mShader->SetFloat("light.quadratic", this->quadratic);
	//	//mShader->SetFloat("light.cutOff", this->cutOff);
	//	//mShader->SetFloat("light.outerCutOff", this->outerCutOff);
	//}

	void Light::SetCommonParams(const Ref<Shader>& shader, const Vector3& position, const Vector3& direction, const Vector3& ambient, const Vector3& diffuse, const Vector3& specular)
	{
		this->mShader = shader;
		this->position = position;
		this->direction = direction;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}
}