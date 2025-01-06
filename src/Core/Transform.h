#pragma once
#include "tspch.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE
{
	class Node;

	class Transform
	{
	public:
		Transform();
		Transform(const char* createdFrom);
		~Transform();

		Vector3 mLocalPosition;
		Quaternion mLocalRotation;
		Vector3 mLocalScale;

		Vector3 mPosition;
		Quaternion mRotation;
		Vector3 mScale;

		Vector3 mRight;
		Vector3 mUp;
		Vector3 mForward;

		Matrix4 mLocalTransformationMatrix;
		Matrix4 mWorldTransformationMatrix;

		void Follow(Ref<Node> targetNode);
		void LookAt(Ref<Node> parentNode, const Ref<Transform> target);

		void ComputeTransformationMatrix(Ref<Node> parentNode);
		void SetLocalTransformationMatrix(const Matrix4& transformationMatrix);
		void SetGlobalTransformationMatrix(const Matrix4& transformationMatrix);

		// Sets local position
		void SetLocalPosition(const Vector3& newPosition);
		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(const float* newPosition);

		void SetLocalEulerAngles(float _x, float _y, float _z);
		void SetLocalEulerAngles(Vector3 _eulerAngles);
		void SetLocalRotation(Quaternion _rotation);

		// Sets local scale
		void SetLocalScale(const Vector3& newScale);
		void SetLocalScale(float x, float y, float z);
		void SetLocalScale(const float* newScale);

		const Quaternion FromEulerAngles(glm::vec3 eulerAngles) const;

		const Vector3 ToEulerAngles(glm::quat quaternion) const;

		// Sets local transform
		void SetLocalTransform(Vector3 pos, Vector3 eulerAngles, Vector3 scale);

		void Reset();

		const Vector3& GetLocalPosition() const;
		const Quaternion GetLocalRotation() const;
		const Vector3 GetLocalEulerAngles() const;
		const Vector3& GetLocalScale() const;

		Matrix4 GetInverseParentMat(Matrix4 newTransformMatrix) const;

		void DecomposeGlobalTransform();

		const Vector3& GetPosition() const;
		const Quaternion& GetRotation() const;
		const Vector3& GetScale() const;

		const Matrix4 GetLocalTransformationMatrix() const { return mLocalTransformationMatrix; }
		const Matrix4 GetGlobalTransformationMatrix() const { return mWorldTransformationMatrix; }

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