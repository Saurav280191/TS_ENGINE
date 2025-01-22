#include "tspch.h"
#include "Transform.h"
#include "Utils/Utility.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE
{
	Transform::Transform() :
		mLocalPosition(0),
		mLocalRotation(1.0, 0.0f, 0.0f, 0.0f),
		mLocalScale(1.0f, 1.0f, 1.0f),
		mRight(Vector3(1, 0, 0)),
		mUp(Vector3(0, 1, 0)),
		mForward(Vector3(0, 0, 1)),
		mWorldTransformationMatrix(Matrix4(1.0f))
	{

	}

	Transform::Transform(const char* createdFrom) :
		mLocalPosition(0),
		mLocalRotation(1.0, 0.0f, 0.0f, 0.0f),
		mLocalScale(1.0f, 1.0f, 1.0f),
		mRight(Vector3(1, 0, 0)),
		mUp(Vector3(0, 1, 0)),
		mForward(Vector3(0, 0, 1)),
		mWorldTransformationMatrix(Matrix4(1.0f))
	{

	}

	Transform::~Transform()
	{
		TS_CORE_INFO("Deleted transform!");
	}

	void Transform::Follow(Ref<Node> targetNode)
	{
		mWorldTransformationMatrix = targetNode->GetTransform()->GetWorldTransformationMatrix();
	}

	void Transform::LookAt(Ref<Node> parentNode, const Ref<Transform> target)
	{
		mLookAtTarget = target;
		mLookAtEnabled = true;

		Matrix4 modelMatrix = Matrix4(1);

		if (parentNode)
		{
			mWorldTransformationMatrix = parentNode->GetTransform()->GetWorldTransformationMatrix() * modelMatrix;
			
			Vector3 skew;
			Vector4 perspective;
			glm::decompose(mWorldTransformationMatrix, mScale, mRotation, mPosition, skew, perspective);
		}
		else
		{
			mWorldTransformationMatrix = modelMatrix;

			Vector3 skew;
			Vector4 perspective;
			glm::decompose(mWorldTransformationMatrix, mScale, mRotation, mPosition, skew, perspective);
		}

		if (mLookAtTarget)
		{
			const Matrix4 lookAtRotationMatrix = Utility::GetLookatAtRotationMatrix(mLocalPosition, mLookAtTarget->GetLocalPosition(), Vector3(0, 1, 0));
			mWorldTransformationMatrix = mWorldTransformationMatrix * lookAtRotationMatrix;

			Vector3 skew;
			Vector4 perspective;
			glm::decompose(mWorldTransformationMatrix, mScale, mRotation, mPosition, skew, perspective);
		}
		
		mRight = GetRight();
		mUp = GetUp();
		mForward = GetForward();
	}

	void Transform::ComputeTransformationMatrix(Ref<Node> parentNode)
	{
		// Update local transformation matrix
		const Matrix4 translationMatrix = glm::translate(Matrix4(1.0f), mLocalPosition);
		const Matrix4 rotationMatrix = glm::toMat4(mLocalRotation);// Quaternion to Matrix4x4						
		const Matrix4 scaleMatrix = glm::scale(Matrix4(1.0f), mLocalScale);

		mLocalTransformationMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		// Update world transformation matrix
		if (!mLookAtTarget)
		{
			if (parentNode && parentNode->mTransform)
			{
				// WorldTransform          = parentNode's WorldTransform						* LocalTrasform
				mWorldTransformationMatrix = parentNode->mTransform->mWorldTransformationMatrix * mLocalTransformationMatrix;

				Vector3 skew;
				Vector4 perspective;
				glm::decompose(mWorldTransformationMatrix, mScale, mRotation, mPosition, skew, perspective);
			}
			else
			{
				mWorldTransformationMatrix = mLocalTransformationMatrix;

				Vector3 skew;
				Vector4 perspective;
				glm::decompose(mWorldTransformationMatrix, mScale, mRotation, mPosition, skew, perspective);
			}
		}

		// Update directions
		mRight = glm::normalize(GetRight());
		mUp = glm::normalize(GetUp());
		mForward = glm::normalize(GetForward());
	}

	void Transform::SetLocalTransformationMatrix(const Matrix4& transformationMatrix)
	{
		mLocalTransformationMatrix = transformationMatrix;
		
		Vector3 skew;
		Vector4 perspective;
		glm::decompose(transformationMatrix, mLocalScale, mLocalRotation, mLocalPosition, skew, perspective);

		mRight = glm::normalize(GetRight());
		mUp = glm::normalize(GetUp());
		mForward = glm::normalize(GetForward());
	}

	void Transform::SetWorldTransformationMatrix(const Matrix4& transformationMatrix)
	{
		mWorldTransformationMatrix = transformationMatrix;
		
		Vector3 skew;
		Vector4 perspective;
		glm::decompose(mWorldTransformationMatrix, mScale, mRotation, mPosition, skew, perspective);

		mRight = GetRight();
		mUp = GetUp();
		mForward = GetForward();
	}

	void Transform::SetLocalPosition(const Vector3& newPosition)
	{
		mLocalPosition = newPosition;		
	}
	void Transform::SetLocalPosition(float x, float y, float z)
	{
		mLocalPosition = Vector3(x, y, z);		
	}
	void Transform::SetLocalPosition(const float* newPosition)
	{
		mLocalPosition = Vector3(newPosition[0], newPosition[1], newPosition[2]);		
	}

	void Transform::SetLocalEulerAngles(float _x, float _y, float _z)
	{
		mLocalRotation = FromEulerAngles(Vector3(_x, _y, _z));
	}

	void Transform::SetLocalEulerAngles(Vector3 _eulerAngles)
	{
		mLocalRotation = FromEulerAngles(_eulerAngles);
	}

	void Transform::SetLocalRotation(Quaternion _rotation)
	{
		mLocalRotation = _rotation;
	}

	void Transform::SetLocalScale(const Vector3& newScale)
	{
		mLocalScale = newScale;
		
	}
	void Transform::SetLocalScale(float x, float y, float z)
	{
		mLocalScale = Vector3(x, y, z);
		
	}
	void Transform::SetLocalScale(const float* newScale)
	{
		mLocalScale = Vector3(newScale[0], newScale[1], newScale[2]);
		
	}

	const Quaternion Transform::FromEulerAngles(glm::vec3 eulerAngles) const
	{
		return glm::quat(glm::radians(eulerAngles));
	}

	const Vector3 Transform::ToEulerAngles(glm::quat quaternion) const
	{
		return glm::degrees(glm::eulerAngles(quaternion));
	}

	void Transform::SetLocalTransform(Vector3 _localPosition, Quaternion _localRotation, Vector3 _localScale, Ref<Node> _parentNode = nullptr)
	{
		mLocalPosition = _localPosition;
		mLocalRotation = _localRotation;
		mLocalScale = _localScale;

		ComputeTransformationMatrix(_parentNode);
	}

	void Transform::SetLocalTransform(Vector3 _localPosition, Vector3 _localEulerAngles, Vector3 _localScale, Ref<Node> _parentNode = nullptr)
	{
		mLocalPosition = _localPosition;
		mLocalRotation = FromEulerAngles(_localEulerAngles);
		mLocalScale = _localScale;

		ComputeTransformationMatrix(_parentNode);
	}

	void Transform::Reset()
	{
		mLocalPosition = Vector3(0);
		mLocalRotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
		mLocalScale = Vector3(1);
	}

	const Vector3& Transform::GetLocalPosition() const
	{
		return mLocalPosition;
	}
	const Quaternion Transform::GetLocalRotation() const
	{
		return mLocalRotation;
	}
	const Vector3 Transform::GetLocalEulerAngles() const
	{
		return ToEulerAngles(mLocalRotation);
	}
	const Vector3& Transform::GetLocalScale() const
	{
		return mLocalScale;
	}

	Matrix4 Transform::GetInverseParentMat(Matrix4 newTransformMatrix) const
	{
		return glm::inverse(newTransformMatrix) * mWorldTransformationMatrix;
	}

	void Transform::DecomposeGlobalTransform()
	{
		Vector3 skew;
		Vector4 perspective;
		glm::decompose(mWorldTransformationMatrix, mScale, mRotation, mPosition, skew, perspective);
	}

	const Vector3& Transform::GetPosition() const
	{
		return mPosition;
	}

	const Quaternion& Transform::GetRotation() const
	{
		return mRotation;
	}

	const Vector3& Transform::GetScale() const
	{
		return mScale;
	}

	Vector3 Transform::GetRight() const
	{
		return (Vector3)mWorldTransformationMatrix[0];
	}
	Vector3 Transform::GetUp() const
	{
		return (Vector3)mWorldTransformationMatrix[1];
	}
	Vector3 Transform::GetBackward() const
	{
		return -(Vector3)mWorldTransformationMatrix[2];
	}
	Vector3 Transform::GetForward() const
	{
		return (Vector3)mWorldTransformationMatrix[2];
	}

	void Transform::MoveFwd(float moveSpeed, float deltaTime)
	{
		mLocalPosition -= mForward * moveSpeed * deltaTime;
		//TS_CORE_INFO("Position of {0}, {1}, {2}", m_Pos.x, m_Pos.y, m_Pos.z);
	}
	void Transform::MoveBack(float moveSpeed, float deltaTime)
	{
		mLocalPosition += mForward * moveSpeed * deltaTime;
	}
	void Transform::MoveLeft(float moveSpeed, float deltaTime)
	{
		mLocalPosition -= mRight * moveSpeed * deltaTime;
	}
	void Transform::MoveRight(float moveSpeed, float deltaTime)
	{
		mLocalPosition += mRight * moveSpeed * deltaTime;
	}
	void Transform::MoveUp(float moveSpeed, float deltaTime)
	{
		mLocalPosition += mUp * moveSpeed * deltaTime;
	}
	void Transform::MoveDown(float moveSpeed, float deltaTime)
	{
		mLocalPosition -= mUp * moveSpeed * deltaTime;
	}
	void Transform::Yaw(float yawSpeed, float deltaTime)
	{
		// Calculate the yaw rotation quaternion
		float angle = glm::radians(yawSpeed * deltaTime); // Convert to radians
		glm::quat yawRotation = glm::angleAxis(-angle, glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw rotates around the Y-axis

		// Update the rotation
		mLocalRotation = yawRotation * mLocalRotation;	// Apply yaw
		mLocalRotation = glm::normalize(mLocalRotation);// Normalize to avoid floating-point drift
	}
	void Transform::Pitch(float pitchSpeed, float deltaTime)
	{
		// Calculate the pitch rotation quaternion (around local X-axis)
		float angle = glm::radians(pitchSpeed * deltaTime);
		glm::vec3 localRight = glm::normalize(glm::vec3(mLocalRotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f))); // Local X-axis
		glm::quat pitchRotation = glm::angleAxis(angle, localRight);

		// Apply pitch in local space
		mLocalRotation = pitchRotation * mLocalRotation; // Pre-multiply for local space
		mLocalRotation = glm::normalize(mLocalRotation);
	}
	void Transform::Roll(float rollSpeed, float deltaTime)
	{
		// Calculate the roll rotation quaternion (around local Z-axis)
		float angle = glm::radians(rollSpeed * deltaTime);
		glm::vec3 localForward = glm::normalize(glm::vec3(mLocalRotation * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f))); // Local Z-axis
		glm::quat rollRotation = glm::angleAxis(angle, localForward);

		// Apply roll in local space
		mLocalRotation = rollRotation * mLocalRotation; // Pre-multiply for local space
		mLocalRotation = glm::normalize(mLocalRotation);
	}
}