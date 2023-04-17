#pragma once
#include "Core/Base.h"

namespace TS_ENGINE
{
	class Transform
	{
	public:
		Transform();
		Transform(const char* createdFrom);
		~Transform();
	public:
		float* data;
		Vector3 m_Pos;
		Vector3 m_EulerAngles;
		Vector3 m_Scale;
		Vector3 m_Right;
		Vector3 m_Up;
		Vector3 m_Forward;
		//Dirty flag
		bool m_isDirty;
	private:
		//Global space information concatenated in matrix
		Matrix4 mModelMatrix;
	protected:
		Matrix4 GetLocalModelMatrix() const;
	public:
		void SetIsDirty(bool isDirty);
		
		void ComputeModelMatrix(const Matrix4& parentGlobalModelMatrix);
		void ComputeModelMatrix();

		void SetLocalPosition(const Vector3& newPosition);
		void SetLocalPosition(const float* newPosition);		
		void SetLocalEulerAngles(const Vector3& newLocalEulerAngles);
		void SetLocalEulerAngles(const float* newLocalEulerAngles);		
		void SetLocalScale(const Vector3& newScale);
		void SetLocalScale(const float* newScale);

		void SetPosition(const Vector3& pos);
		void SetPosition(float x, float y, float z);
		void SetEulerAngles(const Vector3& eulerAngles);
		void SetEulerAngles(float x, float y, float z);
		void SetScale(const Vector3& scale);
		void SetScale(float x, float y, float z);
		void SetScale(float x);

		void Reset();

		const Vector3& GetLocalPosition() const;		
		const Vector3& GetLocalEulerAngles() const;		
		const Vector3& GetLocalScale() const;	

		Matrix4 GetModelMatrix();
		
		void OverrideModelMatrix(float* mat);	

		Matrix4 GetInverseParentMat(Matrix4 newModelMatrix);

		void OverrrideModelMatrix(Matrix4 newModelMatrix);

		const Vector3& GetGlobalPosition() const;

		const Vector3& GetGlobalEulerAngles(Vector3 parentEulerAngles) const;
		const Vector3& GetGlobalScale() const;
		const Matrix4& GetModelMatrix() const;		

		Vector3 GetRight() const;
		Vector3 GetUp() const;
		Vector3 GetBackward() const;
		Vector3 GetForward() const;

		bool IsDirty() const;
	};
}