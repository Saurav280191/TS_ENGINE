#pragma once

namespace TS_ENGINE
{
	class MyMath
	{

	public:
		//static float Lerp(float value, float finalValue, float rate);
		//static float Clamp(float value, float minValue, float maxValue, float rate);
		//static float PingPong(float value, float minValue, float maxValue, float rate);

		static Vector3 ClampEulerAngles(Vector3 rotationInDegrees);
	};
}

