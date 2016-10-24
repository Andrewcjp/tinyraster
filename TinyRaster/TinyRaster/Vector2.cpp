/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include "Vector2.h"

Vector2::Vector2()
{
	m_element[0] = 0.0f;
	m_element[1] = 0.0f;
}

Vector2::Vector2(const Vector2& rhs)
{
	m_element[0] = rhs[0];
	m_element[1] = rhs[1];
}

Vector2::Vector2(float x, float y)
{
	m_element[0] = x; m_element[1] = y;
}

float Vector2::operator [] (const int i) const
{
	return m_element[i];
}

float& Vector2::operator [] (const int i)
{
	return m_element[i];
}

Vector2 Vector2::operator + (const Vector2& rhs) const
{
	return Vector2(
		(*this)[0] + rhs[0],
		(*this)[1] + rhs[1]
		);
}

Vector2 Vector2::operator - (const Vector2& rhs) const
{
	return Vector2(
		(*this)[0] - rhs[0],
		(*this)[1] - rhs[1]
		);
}

Vector2& Vector2::operator = (const Vector2& rhs)
{
	m_element[0] = rhs[0];
	m_element[1] = rhs[1];
	return *this;
}

Vector2 Vector2::operator * (const Vector2& rhs) const
{
	return Vector2(
		m_element[0] * rhs[0],
		m_element[1] * rhs[1]
		);
}

Vector2 Vector2::operator * (float scale) const
{
	return Vector2(
		m_element[0] * scale,
		m_element[1] * scale
		);
}
//Length
float Vector2::Norm() const
{
	return sqrt(m_element[0] * m_element[0] + m_element[1] * m_element[1]);
}
//Length sqr
float Vector2::Norm_Sqr() const
{
	return m_element[0] * m_element[0] + m_element[1] * m_element[1];
}

float Vector2::DotProduct(const Vector2& rhs) const
{
	return m_element[0] * rhs[0] + m_element[1] * rhs[1];
}

Vector2 Vector2::Normalise()
{
	float length = this->Norm();

	if (length > 1.0e-8f)
	{
		float invLen = 1.0f / length;

		m_element[0] *= invLen;
		m_element[1] *= invLen;
	}

	return *this;
}

float Vector2::CrossProduct(const Vector2& rhs) const
{
	return m_element[0] * rhs[1] - m_element[1] * rhs[0];
}

void Vector2::SetZero()
{
	m_element[0] = m_element[1] = 0.0f;
}