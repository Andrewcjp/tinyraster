/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#pragma once
#include <string>

class Vector2
{
private:
	float m_element[2];

public:
	Vector2();

	Vector2(const Vector2& rhs);

	Vector2(float x, float y);

	~Vector2() { ; }

	float operator [] (const int i) const;
	float& operator [] (const int i);
	Vector2 operator + (const Vector2& rhs) const;
	Vector2 operator - (const Vector2& rhs) const;
	Vector2& operator = (const Vector2& rhs);
	Vector2 operator * (const Vector2& rhs) const;
	Vector2 operator * (float scale) const;

	float Norm()	const;
	float Norm_Sqr() const;
	Vector2 Normalise();

	float DotProduct(const Vector2& rhs) const;
	float CrossProduct(const Vector2& rhs) const;
	
	void SetZero();

	inline void SetVector(float x, float y)
	{
		m_element[0] = x; m_element[1] = y;
	}	
};


