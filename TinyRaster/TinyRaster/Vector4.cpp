#include <math.h>

#include "Vector4.h"

Vector4::Vector4(const Vector4& rhs)
{
	m_element[0] = rhs[0];
	m_element[1] = rhs[1];
	m_element[2] = rhs[2];
	m_element[3] = rhs[3];
}

float Vector4::operator [] (const int i) const
{
	return m_element[i];
}

float& Vector4::operator [] (const int i)
{
	return m_element[i];
}

Vector4 Vector4::operator + (const Vector4& rhs) const
{
	return Vector4(
		(*this)[0] + rhs[0],
		(*this)[1] + rhs[1],
		(*this)[2] + rhs[2],
		(*this)[3] + rhs[3]);
}

Vector4 Vector4::operator - (const Vector4& rhs) const
{
	return Vector4(
		(*this)[0] - rhs[0],
		(*this)[1] - rhs[1],
		(*this)[2] - rhs[2],
		(*this)[3] - rhs[3]);
}

Vector4& Vector4::operator = (const Vector4& rhs)
{
	m_element[0] = rhs[0];
	m_element[1] = rhs[1];
	m_element[2] = rhs[2];
	m_element[3] = rhs[3];
	return *this;
}

Vector4 Vector4::operator * (const Vector4& rhs) const
{
	return Vector4(
		m_element[0] * rhs[0],
		m_element[1] * rhs[1],
		m_element[2] * rhs[2],
		m_element[3] * rhs[3]);
}

Vector4 Vector4::operator * (float scale) const
{
	return Vector4(
		m_element[0] * scale,
		m_element[1] * scale,
		m_element[2] * scale,
		m_element[3] * scale
	);
}

float Vector4::Length() const
{
	return sqrtf(m_element[0] * m_element[0]
		+ m_element[1] * m_element[1]
		+ m_element[2] * m_element[2]
		+ m_element[3] * m_element[3]);
}

float Vector4::LengthSqr() const
{
	return m_element[0] * m_element[0]
		+ m_element[1] * m_element[1]
		+ m_element[2] * m_element[2]
		+ m_element[3] * m_element[3];
}

float Vector4::DotProduct(const Vector4& rhs) const
{
	return m_element[0] * rhs[0] + m_element[1] * rhs[1] + m_element[2] * rhs[2] + m_element[3] * rhs[3];
}

void Vector4::Normalise()
{
	float length = this->Length();

	if (length > 1.0e-8f)
	{
		float invLen = 1.0f / length;

		m_element[0] *= invLen;
		m_element[1] *= invLen;
		m_element[2] *= invLen;
		m_element[3] *= invLen;
	}
}

Vector4 Vector4::CrossProduct(const Vector4& rhs) const
{
	return Vector4(
		(m_element[1] * rhs[2] - m_element[2] * rhs[1]),
		(m_element[2] * rhs[0] - m_element[0] * rhs[2]),
		(m_element[0] * rhs[1] - m_element[1] * rhs[0]),
		1.0f
	);
}

void Vector4::SetZero()
{
	m_element[0] = m_element[1] = m_element[2] = m_element[3] = 0.0f;
}
