/*---------------------------------------------------------------------
*
* Copyright © 2016  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

//Various data type used by the rasterizer

typedef Vector3 PixelRGB;				//type define Vector3 as PixelRGB type
typedef Vector4 PixelRGBA;				//type define Vector4 as PixelRGBA type
typedef Vector4 Colour4;				//type define Vector4 as Colour4 type

//struct for a 2D vertex
typedef struct _Vertex2d
{
	Colour4 colour;					//Colour property of the vertex
	Vector2 position;				//Coordinate (position) of the vertex
} Vertex2d;

//struct for a 2D circle
typedef struct _Circle2D
{
	Colour4 colour;					//colour of the circle
	Vector2 centre;					//location of the centre of the circle
	float radius;					//radius of the circle
} Circle2D;

