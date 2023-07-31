#include "tspch.h"
#include "Transform.h"
#include "Utils/Utility.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE
{
	Transform::Transform() :
		m_Pos(0),
		m_EulerAngles(0.0f, 0.0f, 0.0f),
		m_Scale(1.0f, 1.0f, 1.0f),
		m_Right(Vector3(1, 0, 0)),
		m_Up(Vector3(0, 1, 0)),
		m_Forward(Vector3(0, 0, 1)),
		m_TransformationMatrix(Matrix4(1.0f))
	{

	}

	Transform::Transform(const char* createdFrom) :
		m_Pos(0),
		m_EulerAngles(0.0f, 0.0f, 0.0f),
		m_Scale(1.0f, 1.0f, 1.0f),
		m_Right(Vector3(1, 0, 0)),
		m_Up(Vector3(0, 1, 0)),
		m_Forward(Vector3(0, 0, 1)),
		m_TransformationMatrix(Matrix4(1.0f)),
		data(new float[3] {1, 2, 3})
	{

	}

	Transform::~Transform()
	{

	}

	void Transform::LookAt(Ref<Node> parentNode, const Ref<Transform> target)
	{
		mLookAtTarget = target;
		mLookAtEnabled = true;

		Matrix4 modelMatrix = Matrix4(1);

		const Matrix4 translationMatrix = glm::translate(Matrix4(1.0f), m_Pos);
		const Matrix4 scaleMatrix = glm::scale(Matrix4(1.0f), m_Scale);

		if (mLookAtTarget)
		{
			const Matrix4 lookAtRotationMatrix = TS_ENGINE::Utility::GetLookatAtRotationMatrix(m_Pos, mLookAtTarget->GetLocalPosition(), Vector3(0, 1, 0));
			modelMatrix = translationMatrix * lookAtRotationMatrix * scaleMatrix;
		}

		if (parentNode)
			m_TransformationMatrix = parentNode->GetTransform()->GetTransformationMatrix() * modelMatrix;
		else
			m_TransformationMatrix = modelMatrix;

		m_Right = GetRight();
		m_Up = GetUp();
		m_Forward = GetForward();
	}

	Matrix4 Transform::GetLocalModelMatrixForLookAt(TS_ENGINE::Node* parentNode, const Matrix4& parentModelMatrix, const Matrix4& lookAtRotationMatrix)
	{
		Matrix4 modelMatrix = Matrix4(1);

		if (parentNode)
		{
			const Matrix4 translationMatrix = glm::translate(Matrix4(1.0f), m_Pos);
			const Matrix4 scaleMatrix = glm::scale(Matrix4(1.0f), m_Scale);

			if (parentNode->GetParentNode())
				modelMatrix = parentModelMatrix * translationMatrix * lookAtRotationMatrix * scaleMatrix;
			else
				modelMatrix = translationMatrix * lookAtRotationMatrix * scaleMatrix;

			if (parentNode->GetParentNode())
				GetLocalModelMatrixForLookAt(parentNode->GetParentNode().get(), modelMatrix, lookAtRotationMatrix);
		}

		return modelMatrix;
	}

	void Transform::ComputeTransformationMatrix(TS_ENGINE::Node* node, TS_ENGINE::Node* parentNode)
	{
		Matrix4 modelMatrix = Matrix4(1);

		const Matrix4 translationMatrix = glm::translate(Matrix4(1.0f), m_Pos);
		const Matrix4 scaleMatrix = glm::scale(Matrix4(1.0f), m_Scale);

		if (mLookAtTarget)
		{
			const Matrix4 lookAtRotationMatrix = TS_ENGINE::Utility::GetLookatAtRotationMatrix(m_Pos, mLookAtTarget->GetLocalPosition(), Vector3(0, 1, 0));
			modelMatrix = translationMatrix * lookAtRotationMatrix * scaleMatrix;
		}
		else
		{
			const Matrix4 rotationMatrixX = glm::rotate(Matrix4(1.0f), glm::radians(m_EulerAngles.x), Vector3(1.0f, 0.0f, 0.0f));
			const Matrix4 rotationMatrixY = glm::rotate(Matrix4(1.0f), glm::radians(m_EulerAngles.y), Vector3(0.0f, 1.0f, 0.0f));
			const Matrix4 rotationMatrixZ = glm::rotate(Matrix4(1.0f), glm::radians(m_EulerAngles.z), Vector3(0.0f, 0.0f, 1.0f));

			const Matrix4 rotationMatrix = rotationMatrixY * rotationMatrixX * rotationMatrixZ;
			modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		}

		if (parentNode)		
			m_TransformationMatrix = parentNode->GetTransform()->GetTransformationMatrix() * modelMatrix;		
		else
			m_TransformationMatrix = modelMatrix;

		m_Right = GetRight();
		m_Up = GetUp();
		m_Forward = GetForward();
	}

	void Transform::SetTransformationMatrix(Matrix4 transformationMatrix)
	{
		m_TransformationMatrix = transformationMatrix;
	}

	void Transform::SetLocalPosition(const Vector3& newPosition)
	{
		m_Pos = newPosition;		
	}
	void Transform::SetLocalPosition(float x, float y, float z)
	{
		m_Pos = Vector3(x, y, z);		
	}
	void Transform::SetLocalPosition(const float* newPosition)
	{
		m_Pos = Vector3(newPosition[0], newPosition[1], newPosition[2]);		
	}

	void Transform::SetLocalEulerAngles(const Vector3& newLocalEulerAngles)
	{
		m_EulerAngles = newLocalEulerAngles;
		
	}
	void Transform::SetLocalEulerAngles(float x, float y, float z)
	{
		m_EulerAngles = Vector3(x, y, z);
		
	}
	void Transform::SetLocalEulerAngles(const float* newLocalEulerAngles)
	{
		m_EulerAngles = Vector3(newLocalEulerAngles[0], newLocalEulerAngles[1], newLocalEulerAngles[2]);
	}

	void Transform::SetLocalScale(const Vector3& newScale)
	{
		m_Scale = newScale;
		
	}
	void Transform::SetLocalScale(float x, float y, float z)
	{
		m_Scale = Vector3(x, y, z);
		
	}
	void Transform::SetLocalScale(const float* newScale)
	{
		m_Scale = Vector3(newScale[0], newScale[1], newScale[2]);
		
	}

	void Transform::Reset()
	{
		m_Pos = Vector3(0);
		m_EulerAngles = Vector3(0);
		m_Scale = Vector3(1);
	}

	const Vector3& Transform::GetLocalPosition() const
	{
		return m_Pos;
	}
	const Vector3& Transform::GetLocalEulerAngles() const
	{
		return m_EulerAngles;
	}
	const Vector3& Transform::GetLocalScale() const
	{
		return m_Scale;
	}

	Matrix4 Transform::GetInverseParentMat(Matrix4 newTransformMatrix)
	{
		return glm::inverse(newTransformMatrix) * m_TransformationMatrix;
	}

	const Vector3& Transform::GetGlobalPosition() const
	{
		return Vector3(m_TransformationMatrix[3].x, m_TransformationMatrix[3].y, m_TransformationMatrix[3].z);
	}

	const Vector3& Transform::GetGlobalEulerAngles(Vector3 parentEulerAngles) const
	{
		return parentEulerAngles + m_EulerAngles;
	}

	const Vector3& Transform::GetGlobalScale() const
	{
		return { glm::length(GetRight()), glm::length(GetUp()), glm::length(GetBackward()) };
	}

	Vector3 Transform::GetRight() const
	{
		return (Vector3)m_TransformationMatrix[0];
	}
	Vector3 Transform::GetUp() const
	{
		return (Vector3)m_TransformationMatrix[1];
	}
	Vector3 Transform::GetBackward() const
	{
		return -(Vector3)m_TransformationMatrix[2];
	}
	Vector3 Transform::GetForward() const
	{
		return (Vector3)m_TransformationMatrix[2];
	}

	void Transform::MoveFwd(float moveSpeed, float deltaTime)
	{
		m_Pos -= m_Forward * moveSpeed * deltaTime;
		//TS_CORE_INFO("Position of {0}, {1}, {2}", m_Pos.x, m_Pos.y, m_Pos.z);
	}
	void Transform::MoveBack(float moveSpeed, float deltaTime)
	{
		m_Pos += m_Forward * moveSpeed * deltaTime;
	}
	void Transform::MoveLeft(float moveSpeed, float deltaTime)
	{
		m_Pos -= m_Right * moveSpeed * deltaTime;
	}
	void Transform::MoveRight(float moveSpeed, float deltaTime)
	{
		m_Pos += m_Right * moveSpeed * deltaTime;
	}
	void Transform::MoveUp(float moveSpeed, float deltaTime)
	{
		m_Pos += m_Up * moveSpeed * deltaTime;
	}
	void Transform::MoveDown(float moveSpeed, float deltaTime)
	{
		m_Pos -= m_Up * moveSpeed * deltaTime;
	}
	void Transform::Yaw(float yawSpeed, float deltaTime)
	{
		m_EulerAngles.y -= yawSpeed * deltaTime;
	}
	void Transform::Pitch(float pitchSpeed, float deltaTime)
	{
		m_EulerAngles.x += pitchSpeed * deltaTime;
	}
	void Transform::Roll(float rollSpeed, float deltaTime)
	{
		m_EulerAngles.z -= rollSpeed * deltaTime;
	}
}