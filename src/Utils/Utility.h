#pragma once
#include "tspch.h"
#include <filesystem>
#include <wchar.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#define GL_SILENCE_DEPRECATION

#include <ImGuizmo.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/epsilon.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace TS_ENGINE
{
	namespace Utility
	{
		static Quaternion EulerToQuaternion(float pitchDegrees, float yawDegrees, float rollDegrees)
		{
			float pitchRadians = pitchDegrees * glm::pi<float>() / 180.0f;
			float yawRadians = yawDegrees * glm::pi<float>() / 180.0f;
			float rollRadians = rollDegrees * glm::pi<float>() / 180.0f;

			Quaternion q;
			float halfPitch = pitchRadians * 0.5f;
			float halfYaw = yawRadians * 0.5f;
			float halfRoll = rollRadians * 0.5f;

			float sinHalfPitch = sin(halfPitch);
			float cosHalfPitch = cos(halfPitch);
			float sinHalfYaw = sin(halfYaw);
			float cosHalfYaw = cos(halfYaw);
			float sinHalfRoll = sin(halfRoll);
			float cosHalfRoll = cos(halfRoll);

			q.x = sinHalfYaw * cosHalfPitch * cosHalfRoll - cosHalfYaw * sinHalfPitch * sinHalfRoll;
			q.y = cosHalfYaw * sinHalfPitch * cosHalfRoll + sinHalfYaw * cosHalfPitch * sinHalfRoll;
			q.z = cosHalfYaw * cosHalfPitch * sinHalfRoll - sinHalfYaw * sinHalfPitch * cosHalfRoll;
			q.w = cosHalfYaw * cosHalfPitch * cosHalfRoll + sinHalfYaw * sinHalfPitch * sinHalfRoll;

			return q;
		}

		static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
		{
			// From glm::decompose in matrix_decompose.inl

			using namespace glm;
			using T = float;

			mat4 LocalMatrix(transform);

			// Normalize the matrix.
			if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
				return false;

			// First, isolate perspective.  This is the messiest.
			if (
				epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
			{
				// Clear the perspective partition
				LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
				LocalMatrix[3][3] = static_cast<T>(1);
			}

			// Next take care of translation (easy).
			translation = vec3(LocalMatrix[3]);
			LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

			vec3 Row[3], Pdum3;

			// Now get scale and shear.
			for (length_t i = 0; i < 3; ++i)
				for (length_t j = 0; j < 3; ++j)
					Row[i][j] = LocalMatrix[i][j];

			// Compute X scale factor and normalize first row.
			scale.x = length(Row[0]);
			v3Scale(Row[0], static_cast<T>(1));
			scale.y = length(Row[1]);
			v3Scale(Row[1], static_cast<T>(1));
			scale.z = length(Row[2]);
			v3Scale(Row[2], static_cast<T>(1));

			// At this point, the matrix (in rows[]) is orthonormal.
			// Check for a coordinate system flip.  If the determinant
			// is -1, then negate the matrix and the scaling factors.
#if 0
			Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
			if (dot(Row[0], Pdum3) < 0)
			{
				for (length_t i = 0; i < 3; i++)
				{
					scale[i] *= static_cast<T>(-1);
					Row[i] *= static_cast<T>(-1);
				}
			}
#endif

			rotation.y = asin(-Row[0][2]);
			if (cos(rotation.y) != 0) {
				rotation.x = atan2(Row[1][2], Row[2][2]);
				rotation.z = atan2(Row[0][1], Row[0][0]);
			}
			else {
				rotation.x = atan2(-Row[2][0], Row[1][1]);
				rotation.z = 0;
			}


			return true;
		}

#pragma region Matrix Decompose
		struct DecomposedData
		{
			glm::vec3 scale;
			glm::vec3 eulerAngles;
			glm::vec3 translation;
		};
		
		/*Vector3 EulerAngle(vec4 q, vec3 v)
		{
			return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
		}*/

		static glm::mat4 GetLookatAtRotationMatrix(const glm::vec3& objectPos, const glm::vec3& targetPos, const glm::vec3& up)
		{
			glm::vec3 forward = glm::normalize(targetPos - objectPos);
			glm::vec3 right = glm::normalize(glm::cross(forward, up));
			glm::vec3 newUp = glm::cross(right, forward);

			glm::mat4 rotationMatrix(1.0f);
			rotationMatrix[0] = glm::vec4(right, 0.0f);
			rotationMatrix[1] = glm::vec4(newUp, 0.0f);
			rotationMatrix[2] = glm::vec4(forward, 0.0f);

			return rotationMatrix;
		}

		static void DecomposeMtx(const glm::mat4& m, glm::vec3& pos, glm::quat& rot, glm::vec3& scale)
		{
			pos = Vector3(m[3].x, m[3].y, m[3].z);
			
			for (int i = 0; i < 3; i++)
				scale[i] = glm::length(glm::vec3(m[i]));
			
			const glm::mat3 rotMtx(
				glm::vec3(m[0]) / scale[0],
				glm::vec3(m[1]) / scale[1],
				glm::vec3(m[2]) / scale[2]);

			rot = glm::quat_cast(rotMtx);
		}

		static DecomposedData* Decompose(float* matrix)
		{
			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

			DecomposedData* dd = new DecomposedData();
			dd->translation = Vector3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
			dd->eulerAngles = Vector3(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
			dd->scale = Vector3(matrixScale[0], matrixScale[1], matrixScale[2]);

			return dd;
		}
		static DecomposedData* Decompose(Matrix4 matrix)
		{
			float* mat = glm::value_ptr(matrix);
			return Decompose(mat);
		}
#pragma endregion

		static bool resourcesInitialized = false;
		static const char* resourcePath;
		static std::filesystem::path path;

		static void GetExtension(const std::string& fullFileName, std::string& extension)
		{
			size_t dotPos = fullFileName.find_last_of('.');

			if (dotPos != std::string::npos)
			{
				extension = fullFileName.substr(dotPos + 1);
			}
			else
			{				
				extension = "";
			}
		}

		static void GetFilenameAndExtension(const std::string& fullFileName, std::string& name, std::string& extension) 
		{
			size_t dotPos = fullFileName.find_last_of('.');

			if (dotPos != std::string::npos) 
			{
				name = fullFileName.substr(0, dotPos);
				extension = fullFileName.substr(dotPos + 1);
			}
			else 
			{
				name = fullFileName;
				extension = "";
			}
		}

		static void GetDirectoryFilenameAndExtension(const std::string& fullFileName, std::string& directory, std::string& fileName, std::string& extension)
		{
			// Find the last directory separator
			size_t lastSeparator = fullFileName.find_last_of("/\\");

			// Extract the directory
			directory = fullFileName.substr(0, lastSeparator);

			// Extract the file name
			fileName = fullFileName.substr(lastSeparator + 1);

			// Find the last dot (for the extension)
			size_t lastDot = fileName.find_last_of(".");

			// Extract the extension
			extension = fileName.substr(lastDot + 1);
		}

		static void GetDirectory(const std::string& fullFileName, std::string& directory)
		{
			// Find the last directory separator
			size_t lastSeparator = fullFileName.find_last_of("/\\");

			// Extract the directory
			directory = fullFileName.substr(0, lastSeparator);
		}

		static std::string GetTruncatedString(const std::string& input, size_t characterLimit) 
		{
			std::string truncatedStr = "";

			for (size_t i = 0; i < characterLimit; i ++)
			{
				truncatedStr.push_back(input[i]);
			}

			return truncatedStr;
		}

		static const char* GetVec3CStr(Vector3 vec3)
		{
			int arr_len = 3;
			float x[3] = { vec3.x, vec3.y, vec3.z };
			char* bytes = new char[arr_len * sizeof(float)];
			memcpy(bytes, x, arr_len * sizeof(float));

			return bytes;
		}
		
		static const char* ConvertWideToChar(const wchar_t* wideStr) 
		{
			if (!wideStr) 
				return nullptr;

			// Get the required size for the buffer
			size_t len = std::wcstombs(nullptr, wideStr, 0);

			if (len == static_cast<size_t>(-1)) 
			{
				TS_CORE_ERROR("Conversion error");
				return nullptr;
			}

			// Allocate memory for the narrow string
			char* charStr = new char[len + 1]; // +1 for the null terminator
			std::wcstombs(charStr, wideStr, len + 1);

			return charStr; // Caller is responsible for freeing the memory
		}

		static const char* GetResourcePath()
		{
			static std::filesystem::path path = std::filesystem::current_path();

			if (!resourcesInitialized)
			{
				std::filesystem::path root = path.parent_path().parent_path().parent_path().parent_path();
				std::wstring resourcePathWString = std::wstring(root.c_str()) + L"\\ExternalDependencies\\resources";
				resourcePath = ConvertWideToChar(resourcePathWString.c_str());

				resourcesInitialized = true;
			}

			return resourcePath;
		}

		template<typename T>
		static std::vector<T> GetVector(T* arrayOfElements, int count)
		{
			std::vector<T> dest;// (std::begin(arrayOfElements), std::end(arrayOfElements));
			for (int i = 0; i < count; i++)
				des.push_back(arrayOfElements[i]);
			return dest;
		}
	};
}
