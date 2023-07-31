#pragma once

namespace TS_ENGINE
{
	class MyMath
	{

	public:
		MyMath();
		~MyMath();

		float Lerp(float value, float finalValue, float rate);
		float Clamp(float value, float minValue, float maxValue, float rate);
		float PingPong(float value, float minValue, float maxValue, float rate);
	};
}

