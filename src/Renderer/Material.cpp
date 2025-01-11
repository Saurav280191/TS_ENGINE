#include "tspch.h"
#include "Material.h"
#include "Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

#ifdef TS_ENGINE_EDITOR
#include <imgui.h>
#include "imgui_internal.h"
#endif

#include "Application.h"

namespace TS_ENGINE {

	Material::Material()
		: mAmbientColor(1.0f),
		mDiffuseColor(1.0f),
		mDiffuseMapOffset(0),
		mDiffuseMapTiling(1),
		mSpecularColor(1.0f),
		mSpecularMapOffset(0),
		mSpecularMapTiling(1),
		mShininess(16.0f),
		mNormalMapOffset(0),
		mNormalMapTiling(1),
		mBumpValue(1.0f),
		mDepthTestEnabled(true),
		mAlphaBlendingEnabled(true)
	{

	}

	Material::Material(const std::string& name, Ref<Shader> shader)
		: mAmbientColor(1.0f),
		mDiffuseColor(1.0f),
		mDiffuseMapOffset(0),
		mDiffuseMapTiling(1),
		mSpecularColor(1.0f),
		mSpecularMapOffset(0),
		mSpecularMapTiling(1),
		mShininess(16.0f),
		mNormalMapOffset(0),
		mNormalMapTiling(1),
		mBumpValue(1.0f),
		mDepthTestEnabled(true),
		mAlphaBlendingEnabled(true)
	{
		auto n = name;
		auto s = shader;

		this->mName = name;
		this->mShader = shader;
	}

	Material::Material(const Ref<Material>& material)
	{
		this->mName = material->mName;
		this->mShader = material->mShader;

		this->mAmbientColor = material->mAmbientColor;

		this->mDiffuseColor = material->mDiffuseColor;
		this->mDiffuseMap = material->mDiffuseMap;
		this->mDiffuseMapOffset = material->mDiffuseMapOffset;
		this->mDiffuseMapTiling = material->mDiffuseMapTiling;

		this->mSpecularColor = material->mSpecularColor;
		this->mSpecularMap = material->mSpecularMap;
		this->mSpecularMapOffset = material->mSpecularMapOffset;
		this->mSpecularMapTiling = material->mSpecularMapTiling;
		this->mShininess = material->mShininess;

		this->mNormalMap = material->mNormalMap;
		this->mNormalMapOffset = material->mNormalMapOffset;
		this->mNormalMapTiling = material->mNormalMapTiling;
		this->mBumpValue = material->mBumpValue;

		this->mAlphaBlendingEnabled = material->mAlphaBlendingEnabled;
		this->mDepthTestEnabled = material->mDepthTestEnabled;
	}

	void Material::CloneMaterialProperties(Ref<Material> material)
	{
		this->mName = material->mName;
		this->mShader = material->mShader;

		this->mAmbientColor = material->mAmbientColor;
		
		this->mDiffuseColor = material->mDiffuseColor;
		this->mDiffuseMap = material->mDiffuseMap;
		this->mDiffuseMapOffset = material->mDiffuseMapOffset;
		this->mDiffuseMapTiling = material->mDiffuseMapTiling;

		this->mSpecularColor = material->mSpecularColor;
		this->mSpecularMap = material->mSpecularMap;
		this->mSpecularMapOffset = material->mSpecularMapOffset;
		this->mSpecularMapTiling = material->mSpecularMapTiling;
		this->mShininess = material->mShininess;

		this->mNormalMap = material->mNormalMap;
		this->mNormalMapOffset = material->mNormalMapOffset;
		this->mNormalMapTiling = material->mNormalMapTiling;
		this->mBumpValue = material->mBumpValue;

		this->mAlphaBlendingEnabled = material->mAlphaBlendingEnabled;
		this->mDepthTestEnabled = material->mDepthTestEnabled;
	}

	const Ref<Shader> Material::GetShader()
	{
		return mShader;
	}

#ifdef  TS_ENGINE_EDITOR
	void Material::Render(int _entityID, bool _enableTextures)
#else
	void Material::Render(bool _enableTextures)
#endif
	{																// *** Set Render Commands ***

		RenderCommand::EnableDepthTest(mDepthTestEnabled);					// Depth Test
		RenderCommand::EnableAlphaBlending(mAlphaBlendingEnabled);			// Alpha Blending

		if (mShader)												// *** Set Shader Properties ***
		{
#ifdef  TS_ENGINE_EDITOR											
			mShader->SetInt("u_EntityID", _entityID);						// Entity ID
#endif

			mShader->SetVec4("u_AmbientColor", mAmbientColor);				// Ambient Color
			mShader->SetVec4("u_DiffuseColor", mDiffuseColor);				// Diffuse Color
			mShader->SetVec4("u_SpecularColor", mSpecularColor);			// Specular Color

			if (Ref<Texture2D> diffuseMap = mDiffuseMap)
			{
				diffuseMap->Bind();											// Bind Diffuse Map

				mShader->SetBool("u_HasDiffuseTexture", _enableTextures);	// HasDiffuseTexture

				mShader->SetVec2("u_DiffuseMapOffset", mDiffuseMapOffset);	// DiffuseMapOffset
				mShader->SetVec2("u_DiffuseMapTiling", mDiffuseMapTiling);	// DiffuseMapTiling
			}
			else
			{
				mShader->SetBool("u_HasDiffuseTexture", false);				// HasDiffuseTexture
			}
		}
	}

#ifdef TS_ENGINE_EDITOR
	void Material::SetMaterialGui(MaterialGui materialGui)
	{
		mMaterialGui = materialGui;
	}

	void Material::ShowGUI(int meshIndex, bool treeOpen)
	{	
		//Material name		
		if (ImGui::TreeNodeEx(mName.c_str(), treeOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None))
		{
			//Material context menu
			{
				if (ImGui::BeginPopup("DiffuseTexturePopUp"))
				{
					if (ImGui::Button("Delete"))
					{
						mDiffuseMap = nullptr;
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopup("SpecularTexturePopUp"))
				{
					if (ImGui::Button("Delete"))
					{
						mSpecularMap = nullptr;
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				if (ImGui::BeginPopup("NormalTexturePopUp"))
				{
					if (ImGui::Button("Delete"))
					{
						mNormalMap = nullptr;
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}

			ImGui::Separator();
			
			//Ambient 
			{
				ImGui::Text("Ambient");
				float* ambientColor = mAmbientColor.data;
				if (ImGui::ColorEdit4((std::string("##AmbientColor") + std::to_string(meshIndex)).c_str(), ambientColor))
				{
					mMaterialGui.mAmbientColor = Vector4(ambientColor[0], ambientColor[1], ambientColor[2], ambientColor[3]);
					mAmbientColor = mMaterialGui.mAmbientColor;
				}

				ImGui::Spacing();
			}

			// Diffuse 
			{
				ImGui::Text("Diffuse");
				// Color
				float* diffuseColor = mDiffuseColor.data;
				if (ImGui::ColorEdit4((std::string("##DiffuseColor") + std::to_string(meshIndex)).c_str(), diffuseColor))
				{
					mMaterialGui.mDiffuseColor = Vector4(diffuseColor[0], diffuseColor[1], diffuseColor[2], diffuseColor[3]);
					mDiffuseColor = mMaterialGui.mDiffuseColor;
				}

				ImGui::Spacing();
				// Texture
				if (mDiffuseMap)
				{
					ImGui::ImageButton((void*)(intptr_t)mDiffuseMap->GetRendererID(), ImVec2(40, 40));
					ImGui::OpenPopupOnItemClick("DiffuseTexturePopUp", ImGuiPopupFlags_MouseButtonRight);
					DropContentBrowserTexture(TextureType::DIFFUSE, mMaterialGui, meshIndex);
				}
				else
				{
					ImGui::ImageButton((void*)(intptr_t)0, ImVec2(40, 40));
					DropContentBrowserTexture(TextureType::DIFFUSE, mMaterialGui, meshIndex);
				}
				ImGui::SameLine();
				//Offset and tiling
				ImGui::SetNextItemWidth(100.0f);
				ImVec2 diffuseMapOffsetUiPos = ImGui::GetCursorPos();
				if (ImGui::DragFloat2((std::string("##DiffuseMapOffset") + std::to_string(meshIndex)).c_str(), mMaterialGui.mDiffuseMapOffset))
				{
					mDiffuseMapOffset = Vector2(mMaterialGui.mDiffuseMapOffset[0], mMaterialGui.mDiffuseMapOffset[1]);
				}
				ImGui::SameLine();
				ImGui::Text("Offset");
				ImGui::SetCursorPos(ImVec2(diffuseMapOffsetUiPos.x + 0.0f, diffuseMapOffsetUiPos.y + 26.0f));
				ImGui::SetNextItemWidth(100.0f);
				if (ImGui::DragFloat2((std::string("##DiffuseMapTiling") + std::to_string(meshIndex)).c_str(), mMaterialGui.mDiffuseMapTiling))
				{
					mDiffuseMapTiling = Vector2(mMaterialGui.mDiffuseMapTiling[0], mMaterialGui.mDiffuseMapTiling[1]);
				}
				ImGui::SameLine();
				ImGui::Text("Tiling");

				ImGui::Spacing();
			}

			// Specular 
			{
				ImGui::Text("Specular");
				// Color
				float* specularColor = mSpecularColor.data;
				if (ImGui::ColorEdit4((std::string("##SpecularColor") + std::to_string(meshIndex)).c_str(), specularColor))
				{
					mMaterialGui.mSpecularColor = Vector4(specularColor[0], specularColor[1], specularColor[2], specularColor[3]);
					mSpecularColor = mMaterialGui.mSpecularColor;
				}

				ImGui::Spacing();

				// Texture
				if (mSpecularMap)
				{
					ImGui::ImageButton((void*)(intptr_t)mSpecularMap->GetRendererID(), ImVec2(40, 40));
					ImGui::OpenPopupOnItemClick("SpecularTexturePopUp", ImGuiPopupFlags_MouseButtonRight);
					DropContentBrowserTexture(TextureType::SPECULAR, mMaterialGui, meshIndex);
				}
				else
				{
					ImGui::ImageButton((void*)(intptr_t)0, ImVec2(40, 40));
					DropContentBrowserTexture(TextureType::SPECULAR, mMaterialGui, meshIndex);
				}
				ImGui::SameLine();
				//Offset and tiling
				ImGui::SetNextItemWidth(100.0f);
				ImVec2 specularMapOffsetUiPos = ImGui::GetCursorPos();
				if (ImGui::DragFloat2((std::string("##SpecularMapOffset") + std::to_string(meshIndex)).c_str(), mMaterialGui.mSpecularMapOffset))
				{
					mSpecularMapOffset = Vector2(mMaterialGui.mSpecularMapOffset[0], mMaterialGui.mSpecularMapOffset[1]);
				}
				ImGui::SameLine();
				ImGui::Text("Offset");
				ImGui::SetCursorPos(specularMapOffsetUiPos + ImVec2(0.0f, 26.0f));
				ImGui::SetNextItemWidth(100.0f);
				if (ImGui::DragFloat2((std::string("##SpecularMapTiling") + std::to_string(meshIndex)).c_str(), mMaterialGui.mSpecularMapTiling))
				{
					mSpecularMapTiling = Vector2(mMaterialGui.mSpecularMapTiling[0], mMaterialGui.mSpecularMapTiling[1]);
				}
				ImGui::SameLine();
				ImGui::Text("Tiling");

				ImGui::Spacing();

				// Shininess Slider
				if (ImGui::SliderFloat((std::string("##Shininess") + std::to_string(meshIndex)).c_str(), &mMaterialGui.mShininess, 0, 20.0f))
				{
					mShininess = mMaterialGui.mShininess;
				}

				ImGui::Spacing();
			}

			// Normal
			{
				ImGui::Text("Normal");
				//Texture						
				if (mNormalMap)
				{
					ImGui::ImageButton((void*)(intptr_t)mNormalMap->GetRendererID(), ImVec2(40, 40));
					ImGui::OpenPopupOnItemClick("NormalTexturePopUp", ImGuiPopupFlags_MouseButtonRight);
					DropContentBrowserTexture(TextureType::NORMAL, mMaterialGui, meshIndex);
				}
				else
				{
					ImGui::ImageButton((void*)(intptr_t)0, ImVec2(40, 40));
					DropContentBrowserTexture(TextureType::NORMAL, mMaterialGui, meshIndex);
				}
				ImGui::SameLine();
				//Offset and tiling
				ImGui::SetNextItemWidth(100.0f);
				ImVec2 normalMapOffsetUiPos = ImGui::GetCursorPos();
				if (ImGui::DragFloat2((std::string("##NormalMapOffset") + std::to_string(meshIndex)).c_str(), mMaterialGui.mNormalMapOffset))
				{
					mNormalMapOffset = Vector2(mMaterialGui.mNormalMapOffset[0], mMaterialGui.mNormalMapOffset[1]);
				}
				ImGui::SameLine();
				ImGui::Text("Offset");
				ImGui::SetCursorPos(normalMapOffsetUiPos + ImVec2(0.0f, 26.0f));
				ImGui::SetNextItemWidth(100.0f);
				if (ImGui::DragFloat2((std::string("##NormalMapTiling") + std::to_string(meshIndex)).c_str(), mMaterialGui.mNormalMapTiling))
				{
					mNormalMapTiling = Vector2(mMaterialGui.mNormalMapTiling[0], mMaterialGui.mNormalMapTiling[1]);
				}
				ImGui::SameLine();
				ImGui::Text("Tiling");

				ImGui::Spacing();

				// Bump Slider
				if (ImGui::SliderFloat((std::string("##Bump") + std::to_string(meshIndex)).c_str(), &mMaterialGui.mBumpValue, 0, 20.0f))
				{
					mBumpValue = mMaterialGui.mBumpValue;
				}
			}

			ImGui::TreePop();
		}

		ImGui::Separator();
	}

	void Material::DropContentBrowserTexture(TextureType textureType, Material::MaterialGui& materialGui, int meshIndex)
	{
		//Ref<Material> material = mSelectedNode->GetMeshes()[meshIndex]->GetMaterial();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("_CONTENTBROWSER_TEXTURE"))
			{
				const char* draggedTexturePath = reinterpret_cast<const char*>(payload->Data);
				TS_CORE_INFO("Dropped {0} on {1}", draggedTexturePath, "DiffuseTextureDropZone");

				Ref<Texture2D> texture = Texture2D::Create(draggedTexturePath);

				if (textureType == TextureType::DIFFUSE)
				{
					materialGui.mDiffuseMap = texture;
					mDiffuseMap = texture;
				}
				else if (textureType == TextureType::SPECULAR)
				{
					materialGui.mSpecularMap = texture;
					mSpecularMap = texture;
				}
				else if (textureType == TextureType::NORMAL)
				{
					materialGui.mNormalMap = texture;
					mNormalMap = texture;
				}
			}

			ImGui::EndDragDropTarget();
		}
	}
#endif
}
