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

#include "TinyRasterTypes.h"
#include "Rasterizer.h"
namespace AssignmentTests {
	//Test 01: solid lines one pixel thickness
	void AssignmentTest01(Rasterizer *rasterizer);

	//Test 02: lines with gradient colours and thickness > 1
	void AssignmentTest02(Rasterizer *rasterizer);

	//Test 03: unfilled polygons
	void AssignmentTest03(Rasterizer *rasterizer);

	//Test 04: simple solid filled polygons
	void AssignmentTest04(Rasterizer *rasterizer);

	//Test 05: complex solid filled polygons
	void AssignmentTest05(Rasterizer *rasterizer);

	//Test 06: overlapping filled polygons alpha blending
	void AssignmentTest06(Rasterizer *rasterizer);

	//Test 07: interpolated filled polygons
	void AssignmentTest07(Rasterizer *rasterizer);

	//Test 08: A mix of filled and unfilled circle
	void AssignmentTest08(Rasterizer *rasterizer);


	//begin custom tests;
	//textured 2d shapes;
	void AssignmentTest09(Rasterizer *rasterizer);
}