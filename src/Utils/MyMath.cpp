#include "D:/Documents/ThinkSideways/WorkInProgress/TS_ENGINE Related/TS_ENGINE_Editor/build/x64/Debug/TS_ENGINE/CMakeFiles/TS_ENGINE.dir/Debug/cmake_pch.hxx"
#include "MyMath.h"

Vector3 TS_ENGINE::MyMath::ClampEulerAngles(Vector3 rotationInDegrees)
{
	float x = fmod(rotationInDegrees.x, 360.0f); 
	float y = fmod(rotationInDegrees.y, 360.0f); 
	float z = fmod(rotationInDegrees.z, 360.0f); 

	if(x < 0.0f)
		x += 360.0f;
	if (y < 0.0f)
		y += 360.0f;
	if (z < 0.0f)
		z += 360.0f;

	return Vector3(x, y, z);
}
