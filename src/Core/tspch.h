#pragma once
#include <set>
#include <map>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <memory>
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>
//#include <function>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLM
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\quaternion.hpp>
#include <GLM\gtx\quaternion.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\gtx\matrix_decompose.hpp>
#include <GLM\gtx\matrix_interpolation.hpp>

#define Vector2 glm::vec2 
#define Vector3 glm::vec3 
#define Vector4 glm::vec4
#define Matrix4 glm::mat4 
#define Matrix3 glm::mat3
#define Quaternion glm::quat

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Base.h"

#ifdef TS_PLATFORM_WINDOWS
#include <Windows.h>
#endif