/*---------------------------------------------------------------------
*
* Copyright © 2016  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include "AssignmentTests.h"
#include "TestData.h"

namespace AssignmentTests {

	void AssignmentTest01(Rasterizer * rasterizer)
	{
		int vertex_count = sizeof(lines) / sizeof(Vertex2d);

		rasterizer->SetGeometryMode(Rasterizer::LINE);
		rasterizer->SetFillMode(Rasterizer::SOLID_FILLED);

		for (int i = 0; i < vertex_count; i += 2)
		{
			rasterizer->DrawLine2D(*(lines + i), *(lines + i + 1));
		}
	}

	void AssignmentTest02(Rasterizer * rasterizer)
	{
		int vertex_count = sizeof(lines_interp) / sizeof(Vertex2d);

		rasterizer->SetGeometryMode(Rasterizer::LINE);
		rasterizer->SetFillMode(Rasterizer::INTERPOLATED_FILLED);

		for (int i = 0; i < vertex_count; i += 2)
		{
			rasterizer->DrawLine2D(*(lines_interp + i), *(lines_interp + i + 1), i + 1);
		}
	}

	void AssignmentTest03(Rasterizer * rasterizer)
	{
		rasterizer->SetGeometryMode(Rasterizer::POLYGON);
		rasterizer->SetFillMode(Rasterizer::UNFILLED);

		rasterizer->DrawUnfilledPolygon2D(rectangle1, 4);
		rasterizer->DrawUnfilledPolygon2D(triangle, 3);
		rasterizer->DrawUnfilledPolygon2D(square, 4);
		rasterizer->DrawUnfilledPolygon2D(pentagon, 5);
	}

	void AssignmentTest04(Rasterizer * rasterizer)
	{
		rasterizer->SetGeometryMode(Rasterizer::POLYGON);
		rasterizer->SetFillMode(Rasterizer::SOLID_FILLED);

		rasterizer->ScanlineFillPolygon2D(rectangle1, 4);
		rasterizer->ScanlineFillPolygon2D(triangle, 3);
		rasterizer->ScanlineFillPolygon2D(square, 4);
		rasterizer->ScanlineFillPolygon2D(pentagon, 5);
	}

	void AssignmentTest05(Rasterizer * rasterizer)
	{
		rasterizer->SetGeometryMode(Rasterizer::POLYGON);
		rasterizer->SetFillMode(Rasterizer::SOLID_FILLED);

		int vertex_count = sizeof(comb) / sizeof(Vertex2d);
		rasterizer->ScanlineFillPolygon2D(comb, vertex_count);
	}

	void AssignmentTest06(Rasterizer * rasterizer)
	{
		rasterizer->SetGeometryMode(Rasterizer::POLYGON);
		rasterizer->SetFillMode(Rasterizer::SOLID_FILLED);
		rasterizer->SetBlendMode(Rasterizer::ALPHA_BLEND);

		rasterizer->ScanlineFillPolygon2D(rectangle1, 4);
		rasterizer->ScanlineFillPolygon2D(triangle, 3);
		rasterizer->ScanlineFillPolygon2D(square, 4);
		rasterizer->ScanlineFillPolygon2D(pentagon, 5);

		rasterizer->SetBlendMode(Rasterizer::NO_BLEND);
	}

	void AssignmentTest07(Rasterizer * rasterizer)
	{
		rasterizer->SetGeometryMode(Rasterizer::POLYGON);
		rasterizer->SetFillMode(Rasterizer::INTERPOLATED_FILLED);

		rasterizer->ScanlineInterpolatedFillPolygon2D(grad_rectangle, 4);
		rasterizer->ScanlineInterpolatedFillPolygon2D(grad_triangle, 3);
		rasterizer->ScanlineInterpolatedFillPolygon2D(grad_square, 4);
		rasterizer->ScanlineInterpolatedFillPolygon2D(grad_pentagon, 5);
	}

	void AssignmentTest08(Rasterizer * rasterizer)
	{
		int numCircles = sizeof(circles) / sizeof(Circle2D);
		Circle2D test;
		test.centre = Vector2(1280 / 2, 720 / 2);
		test.colour = Colour4(1, 1, 1, 1);
		test.radius = 1000;
		rasterizer->DrawCircle2D(test, false);
		for (int i = 0; i < numCircles; i++)
		{
			//		rasterizer->DrawCircle2D(*(circles + i), i % 2 ? true : false);
		}
	}
}