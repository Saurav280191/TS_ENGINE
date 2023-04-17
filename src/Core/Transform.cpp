#include "tspch.h"
#include "Transform.h"

namespace TS_ENGINE
{
	Transform::Transform():
		data(NULL),
		m_Pos(0),
		m_EulerAngles(0.0f, 0.0f, 0.0f),
		m_Scale(1.0f, 1.0f, 1.0f),
		m_Right(Vector3(1, 0, 0)),
		m_Up(Vector3(0, 1, 0)),
		m_Forward(Vector3(0, 0, 1)),
		mModelMatrix(Matrix4(1.0f)),
		m_isDirty(false)
	{
		
	}

	Transform::Transform(const char* createdFrom):
		m_Pos(0),
		m_EulerAngles(0.0f, 0.0f, 0.0f),
		m_Scale(1.0f, 1.0f, 1.0f),
		m_Right(Vector3(1, 0, 0)),
		m_Up(Vector3(0, 1, 0)),
		m_Forward(Vector3(0, 0, 1)),
		mModelMatrix(Matrix4(1.0f)),
		m_isDirty(false),
		data(new float[3] {1, 2, 3})
	{
		
	}

	Transform::~Transform()
	{
		m_Pos = Vector3(0.0f);
		m_EulerAngles = Vector3(0.0f);
		m_Scale = Vector3(0.0f);
		m_Right = Vector3(0.0f);
		m_Up = Vector3(0.0f);
		m_Forward = Vector3(0.0f);
		mModelMatrix = Matrix4(1.0f);
		m_isDirty = false;
		data = nullptr;
		delete data;
	}
	
	Matrix4 Transform::GetLocalModelMatrix() const 
	{
		const Matrix4 transformX = glm::rotate(Matrix4(1.0f), glm::radians(m_EulerAngles.x), Vector3(1.0f, 0.0f, 0.0f));
		const Matrix4 transformY = glm::rotate(Matrix4(1.0f), glm::radians(m_EulerAngles.y), Vector3(0.0f, 1.0f, 0.0f));
		const Matrix4 transformZ = glm::rotate(Matrix4(1.0f), glm::radians(m_EulerAngles.z), Vector3(0.0f, 0.0f, 1.0f));

		// Y * X * Z
		const Matrix4 rotationMatrix = transformY * transformX * transformZ;

		// translation * rotation * scale (also know as TRS matrix)
		return glm::translate(Matrix4(1.0f), m_Pos) * rotationMatrix * glm::scale(Matrix4(1.0f), m_Scale);
	}

	void Transform::SetIsDirty(bool isDirty)
	{
		m_isDirty = isDirty;
	}
	
	void Transform::ComputeModelMatrix(const Matrix4& parentGlobalModelMatrix)
	{
		mModelMatrix = parentGlobalModelMatrix * GetLocalModelMatrix();
	}
	void Transform::ComputeModelMatrix()
	{
		mModelMatrix = GetLocalModelMatrix();
	}

	void Transform::SetLocalPosition(const Vector3& newPosition)
	{
		m_Pos = newPosition;
		m_isDirty = true;
	}
	void Transform::SetLocalPosition(const float* newPosition)
	{
		m_Pos = Vector3(newPosition[0], newPosition[1], newPosition[2]);
		m_isDirty = true;
	}
	
	void Transform::SetLocalEulerAngles(const Vector3& newLocalEulerAngles)
	{
		m_EulerAngles = newLocalEulerAngles;
		m_isDirty = true;
	}
	void Transform::SetLocalEulerAngles(const float* newLocalEulerAngles)
	{
		m_EulerAngles = Vector3(newLocalEulerAngles[0], newLocalEulerAngles[1], newLocalEulerAngles[2]);
		m_isDirty = true;
	}
	
	void Transform::SetLocalScale(const Vector3& newScale)
	{
		m_Scale = newScale;
		m_isDirty = true;
	}
	void Transform::SetLocalScale(const float* newScale)
	{
		m_Scale = Vector3(newScale[0], newScale[1], newScale[2]);
		m_isDirty = true;
	}

	void Transform::SetPosition(const Vector3& pos)
	{
		m_Pos = pos;
	}
	void Transform::SetPosition(float x, float y, float z)
	{
		m_Pos = Vector3(x, y, z);
	}
	void Transform::SetEulerAngles(const Vector3& eulerAngles)
	{
		m_EulerAngles = eulerAngles;
	}
	void Transform::SetEulerAngles(float x, float y, float z)
	{
		m_EulerAngles = Vector3(x, y, z);
	}
	void Transform::SetScale(const Vector3& scale)
	{
		m_Scale = scale;
	}
	void Transform::SetScale(float x, float y, float z)
	{
		m_Scale = Vector3(x, y, z);
	}

	void Transform::SetScale(float x)
	{
		m_Scale = Vector3(x, x, x);
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

	Matrix4 Transform::GetModelMatrix()
	{
		return mModelMatrix;
	}
	
	void Transform::OverrideModelMatrix(float* mat)
	{
		mModelMatrix[0] = Vector4(mat[0], mat[1], mat[2], mat[3]);
		mModelMatrix[1] = Vector4(mat[4], mat[5], mat[6], mat[7]);
		mModelMatrix[2] = Vector4(mat[8], mat[9], mat[10], mat[11]);
		mModelMatrix[3] = Vector4(mat[12], mat[13], mat[14], mat[15]);
	}

	Matrix4 Transform::GetInverseParentMat(Matrix4 newModelMatrix)
	{
		return glm::inverse(newModelMatrix) * mModelMatrix;
	}

	void Transform::OverrrideModelMatrix(Matrix4 newModelMatrix)
	{
		//mModelMatrix = newModelMatrix;
		glm::vec3 scale;
		glm::quat orientatation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(newModelMatrix, scale, orientatation, translation, skew, perspective);

		glm::vec3 euler = glm::eulerAngles(orientatation);
		euler = euler * glm::vec3(57.2958f);
		
		m_Pos -= translation;
		m_EulerAngles -= euler;
		m_Scale /= scale;
	}

	const Vector3& Transform::GetGlobalPosition() const
	{
		return Vector3(mModelMatrix[3].x, mModelMatrix[3].y, mModelMatrix[3].z);
	}

	const Vector3& Transform::GetGlobalEulerAngles(Vector3 parentEulerAngles) const
	{
		return parentEulerAngles + m_EulerAngles;
	}

	const Vector3& Transform::GetGlobalScale() const
	{
		return { glm::length(GetRight()), glm::length(GetUp()), glm::length(GetBackward()) };
	}

	const Matrix4& Transform::GetModelMatrix() const
	{
		return mModelMatrix;
	}

	Vector3 Transform::GetRight() const
	{
		return (Vector3)mModelMatrix[0];
	}
	Vector3 Transform::GetUp() const
	{
		return (Vector3)mModelMatrix[1];
	}
	Vector3 Transform::GetBackward() const
	{
		return -(Vector3)mModelMatrix[2];
	}
	Vector3 Transform::GetForward() const
	{
		return (Vector3)mModelMatrix[2];
	}

	bool Transform::IsDirty() const
	{
		return m_isDirty;
	}	
}