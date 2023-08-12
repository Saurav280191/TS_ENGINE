#pragma once
#include "tspch.h"

namespace TS_ENGINE
{
	class Node;
	class Transform
	{
	public:
		Transform();
		Transform(const char* createdFrom);
		~Transform();

		float* data;
		Vector3 m_Pos;
		Vector3 m_EulerAngles;
		Vector3 m_Scale;
		Vector3 m_Right;
		Vector3 m_Up;
		Vector3 m_Forward;
		Matrix4 m_TransformationMatrix;
		
		void Follow(Ref<Node> targetNode);
		void LookAt(Node* parentNode, const Ref<Transform> target);
	
		void ComputeTransformationMatrix(TS_ENGINE::Node* node, TS_ENGINE::Node* parentNode);
		void SetTransformationMatrix(Matrix4 transformationMatrix);

		void SetLocalPosition(const Vector3& newPosition);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(const float* newPosition);
		void SetLocalEulerAngles(const Vector3& newLocalEulerAngles);
		void SetLocalEulerAngles(float x, float y, float z);
		void SetLocalEulerAngles(const float* newLocalEulerAngles);
		void SetLocalScale(const Vector3& newScale);
		void SetLocalScale(float x, float y, float z);
		void SetLocalScale(const float* newScale);

		void Reset();

		const Vector3& GetLocalPosition() const;
		const Vector3& GetLocalEulerAngles() const;
		const Vector3& GetLocalScale() const;

		Matrix4 GetInverseParentMat(Matrix4 newTransformMatrix);

		//const Vector3& GetGlobalPosition() const;
		//const Vector3& GetGlobalEulerAngles(Vector3 parentEulerAngles) const;
		//const Vector3& GetGlobalScale() const;

		const Matrix4 GetTransformationMatrix() const
		{
			return m_TransformationMatrix;
		}

		Vector3 GetRight() const;
		Vector3 GetUp() const;
		Vector3 GetBackward() const;
		Vector3 GetForward() const;

		void MoveFwd(float moveSpeed, float deltaTime);
		void MoveBack(float moveSpeed, float deltaTime);
		void MoveLeft(float moveSpeed, float deltaTime);
		void MoveRight(float moveSpeed, float deltaTime);
		void MoveUp(float moveSpeed, float deltaTime);
		void MoveDown(float moveSpeed, float deltaTime);
		void Yaw(float yawSpeed, float deltaTime);
		void Pitch(float pitchSpeed, float deltaTime);
		void Roll(float rollSpeed, float deltaTime);

		Ref<Transform> mLookAtTarget;
		bool mLookAtEnabled = false;
	};
}