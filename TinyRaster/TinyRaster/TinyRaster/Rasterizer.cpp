/*---------------------------------------------------------------------
*
* Copyright © 2016  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include <algorithm>
#include <math.h>
#include <vector>
#include "Rasterizer.h"
#include <iostream>

Rasterizer::Rasterizer(void)
{
	mFramebuffer = NULL;
	mScanlineLUT = NULL;
}

void Rasterizer::ClearScanlineLUT()
{
	Scanline *pScanline = mScanlineLUT;

	for (int y = 0; y < mHeight; y++)
	{
		(pScanline + y)->clear();
		(pScanline + y)->shrink_to_fit();
	}
}

unsigned int Rasterizer::ComputeOutCode(const Vector2 & p, const ClipRect& clipRect)
{
	unsigned int CENTRE = 0x0;
	unsigned int LEFT = 0x1;
	unsigned int RIGHT = 0x1 << 1;
	unsigned int BOTTOM = 0x1 << 2;
	unsigned int TOP = 0x1 << 3;
	unsigned int outcode = CENTRE;

	if (p[0] < clipRect.left)
		outcode |= LEFT;
	else if (p[0] >= clipRect.right)
		outcode |= RIGHT;

	if (p[1] < clipRect.bottom)
		outcode |= BOTTOM;
	else if (p[1] >= clipRect.top)
		outcode |= TOP;

	return outcode;
}

bool Rasterizer::ClipLine(const Vertex2d & v1, const Vertex2d & v2, const ClipRect& clipRect, Vector2 & outP1, Vector2 & outP2)
{
	//TODO: EXTRA This is not directly prescribed as an assignment exercise. 
	//However, if you want to create an efficient and robust rasteriser, clipping is a usefull addition.
	//The following code is the starting point of the Cohen-Sutherland clipping algorithm.
	//If you complete its implementation, you can test it by calling prior to calling any DrawLine2D .

	const Vector2 p1 = v1.position;
	const Vector2 p2 = v2.position;
	unsigned int outcode1 = ComputeOutCode(p1, clipRect);
	unsigned int outcode2 = ComputeOutCode(p2, clipRect);

	outP1 = p1;
	outP2 = p2;

	bool draw = false;

	return true;
}

void Rasterizer::WriteRGBAToFramebuffer(int x, int y, const Colour4 & colour)
{
	PixelRGBA *pixel = mFramebuffer->GetBuffer();
	if ((y*mWidth + x) < ((y + 1) * mWidth) && (y*mWidth + x) > (y*mWidth)) {
		if ((y*mWidth + x) < mWidth*mHeight && (y*mWidth + x) > 0) {
			pixel[y*mWidth + x] = colour;
		}
	}
}


Colour4 Rasterizer::GetColourAtPoint(int x, int y) {
	PixelRGBA *pixel = mFramebuffer->GetBuffer();
	if ((y*mWidth + x) < ((y + 1) * mWidth) && (y*mWidth + x) > (y*mWidth)) {
		if ((y*mWidth + x) < mWidth*mHeight && (y*mWidth + x) > 0) {
			return pixel[y*mWidth + x];
		}
	}

}
Rasterizer::Rasterizer(int width, int height)
{
	//Initialise the rasterizer to its initial state
	mFramebuffer = new Framebuffer(width, height);
	mScanlineLUT = new Scanline[height];
	mWidth = width;
	mHeight = height;

	mBGColour.SetVector(0.0, 0.0, 0.0, 1.0);	//default bg colour is black
	mFGColour.SetVector(1.0, 1.0, 1.0, 1.0);    //default fg colour is white

	mGeometryMode = LINE;
	mFillMode = UNFILLED;
	mBlendMode = NO_BLEND;

	SetClipRectangle(0, mWidth, 0, mHeight);
}

Rasterizer::~Rasterizer()
{
	delete mFramebuffer;
	delete[] mScanlineLUT;
}

void Rasterizer::Clear(const Colour4& colour)
{
	PixelRGBA *pixel = mFramebuffer->GetBuffer();

	SetBGColour(colour);

	int size = mWidth*mHeight;

	for (int i = 0; i < size; i++)
	{
		//fill all pixels in the framebuffer with background colour
		*(pixel + i) = mBGColour;
	}
}

void Rasterizer::DrawPoint2D(const Vector2& pt, int size)
{
	int x = (int)pt[0];
	int y = (int)pt[1];
	WriteRGBAToFramebuffer(x, y, mFGColour);
	for (int i = 0; i < size; i++) {
		WriteRGBAToFramebuffer(x + i, y, mFGColour);
		WriteRGBAToFramebuffer(x - i, y, mFGColour);
		WriteRGBAToFramebuffer(x, y + i, mFGColour);
		WriteRGBAToFramebuffer(x, y - i, mFGColour);
		WriteRGBAToFramebuffer(x + i, y + i, mFGColour);
		WriteRGBAToFramebuffer(x - i, y - i, mFGColour);
		WriteRGBAToFramebuffer(x + i, y + i, mFGColour);
		WriteRGBAToFramebuffer(x - i, y - i, mFGColour);
	}
	//WriteRGBAToFramebuffer(x, y, mFGColour);
}
void Rasterizer::DrawPoint2D(int x, int y, int size)
{
	WriteRGBAToFramebuffer(x, y, mFGColour);
	for (int i = 0; i < size; i++) {
		WriteRGBAToFramebuffer(x + i, y, mFGColour);
		WriteRGBAToFramebuffer(x - i, y, mFGColour);
		WriteRGBAToFramebuffer(x, y + i, mFGColour);
		WriteRGBAToFramebuffer(x, y - i, mFGColour);
		WriteRGBAToFramebuffer(x + i, y + i, mFGColour);
		WriteRGBAToFramebuffer(x - i, y - i, mFGColour);
		WriteRGBAToFramebuffer(x + i, y + i, mFGColour);
		WriteRGBAToFramebuffer(x - i, y - i, mFGColour);
	}
	//WriteRGBAToFramebuffer(x, y, mFGColour);
}
float lerp(float v0, float v1, float t) {
	return (1 - t)*v0 + t*v1;
}
void Rasterizer::DrawLine2D(const Vertex2d & v1, const Vertex2d & v2, int thickness)
{
	//The following code is basic Bresenham's line drawing algorithm.
	//The current implementation is only capable of rasterise a line in the first octant, where dy < dx and dy/dx >= 0
	//See if you want to read ahead https://www.cs.helsinki.fi/group/goa/mallinnus/lines/bresenh.html

	//TODO:
	//Ex 1.1 Complete the implementation of Rasterizer::DrawLine2D method. 
	//This method currently consists of a partially implemented Bresenham algorithm.
	//You must extend its implementation so that it is capable of drawing 2D lines with arbitrary gradient(slope).
	//Use Test 1 (Press F1) to test your implementation
	//CompletE1
	//Ex 1.2 Extend the implementation of Rasterizer::DrawLine2D so that it is capable of drawing lines based on a given thickness.
	//Note: The thickness is passed as an argument int thickness.
	//Use Test 2 (Press F2) to test your implementation
	//Need to Refine Algothom i think>

	//Ex 1.3 Extend the implementation of Rasterizer::DrawLine2D so that it is capable of interpolating colour across a line when each end-point has different colours.
	//Note: The variable mFillMode indicates if the fill mode is set to INTERPOLATED_FILL. 
	//The colour of each point should be linearly interpolated using the colours of v1 and v2.
	//Use Test 2 (Press F2) to test your implementation



	Vector2 pt1 = v1.position;
	Vector2 pt2 = v2.position;
	//calculate wether our line is very steep
	const bool VerySteep = (abs(pt2[1] - pt1[1]) > abs(pt2[0] - pt1[0]));
	Colour4	 Vert1col = GetColor(v1.position[0], v1.position[1], v1);
	Colour4	 Vert2col = GetColor(v2.position[0], v2.position[1], v2);


	if (VerySteep)
	{
		std::swap(pt1[0], pt1[1]);
		std::swap(pt2[0], pt2[1]);
	}
	if (pt1[0] > pt2[0])
	{
		std::swap(pt1, pt2);
		//	std::swap(Vert1col, Vert2col);
	}
	else
	{

	}

	int	dx = pt2[0] - pt1[0];
	int dy = abs(pt2[1] - pt1[1]);
	int error = dx;
	int ystep = (pt1[1] < pt2[1]) ? 1 : -1;

	//int x = (int)pt1[0];
	int y = (int)pt1[1];
	int MaxX = (int)pt2[0];
	if (MaxX > 10000) {
		std::cout << "Line TOO big" << std::endl;
		return;
	}

	for (int x = (int)pt1[0]; x < MaxX; x++)
	{
		Vert1col = GetColor(x, y, v1);
		Vert2col = GetColor(x, y, v2);

		Vector2 temp(x, y);
		Vector2 temp2(y, x);
		Colour4 colour;
		if (mFillMode == FillMode::INTERPOLATED_FILLED) {

			float t = (temp.Normalise() - pt2.Normalise()).Norm();
			printf("%f\n", t);
			colour = Vert1col *(1 - t) + (Vert2col * t);

		}
		else {
			colour = Vert1col;
		}
		SetFGColour(colour);
		if (VerySteep)
		{
			DrawPoint2D(y, x, thickness);
		}
		else
		{
			DrawPoint2D(x, y, thickness);
		}
		error -= dy;
		if (2 * error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

void Rasterizer::DrawUnfilledPolygon2D(const Vertex2d * vertices, int count)
{
	//TODO:
	//Ex 2.1 Implement the Rasterizer::DrawUnfilledPolygon2D method so that it is capable of drawing an unfilled polygon, i.e. only the edges of a polygon are rasterised. 
	//Please note, in order to complete this exercise, you must first complete Ex1.1 since DrawLine2D method is reusable here.
	//Note: The edges of a given polygon can be found by conntecting two adjacent vertices in the vertices array.
	//Use Test 3 (Press F3) to test your solution.
	Vertex2d previous = *vertices;
	for (int i = 1; i < count; i++) {
		DrawLine2D(previous, *(vertices + i));
		previous = *(vertices + i);
	}
	DrawLine2D(*vertices, *(vertices + count - 1));
}
Colour4 MultiplyAlpha(Colour4 color, Colour4 old, float alpha) {
	Colour4 out;
	out[0] = alpha * color[0] + (1 - alpha)*old[0];
	out[1] = alpha * color[1] + (1 - alpha)*old[1];
	out[2] = alpha * color[2] + (1 - alpha)*old[2];
	return out;
}
Colour4 Rasterizer::GetColor(int x, int y, const Vertex2d vert) {
	Colour4 out;
	if (mBlendMode == BlendMode::ALPHA_BLEND) {
		Colour4 currentcol = GetColourAtPoint(x, y);
		out = MultiplyAlpha(vert.colour, currentcol, vert.colour[3]);
	}
	else
	{
		out = vert.colour;
	}
	return out;
}
Colour4 GetColourByPixelpos(int x, int y, Vertex2d* verts, int count) {
	Vector4 out;
	for (int i = 0; i < count; i++) {
		//sum the colour from all the verts?

	}

}
typedef struct edge {
	Vector2 pos1;
	Vector2 pos2;
	float m;
	float Curx;
	float ymin;
	float ymax;
}Edge;
void Rasterizer::ScanlineFillPolygon2D(const Vertex2d * vertices, int count)
{
	//TODO:
	//Ex 2.2 Implement the Rasterizer::ScanlineFillPolygon2D method method so that it is capable of drawing a solidly filled polygon.
	//Note: You can implement floodfill for this exercise however scanline fill is considered a more efficient and robust solution.
	//		You should be able to reuse DrawUnfilledPolygon2D here.
	//
	//Use Test 4 (Press F4) to test your solution, this is a simple test case as all polygons are convex.
	//Use Test 5 (Press F5) to test your solution, this is a complex test case with one non-convex polygon.
	std::vector<Edge> GlobalEdge;
	std::vector<Edge> ActiveEdge;
	int ymax = 1000;
	int ymin = 0;
	//Find Ymin and Ymax
	for (int i = 0; i < count - 1; i++) {
		Edge nedge;
		if ((vertices + i)->position[1] < (vertices + i + 1)->position[1]) {
			nedge.pos1 = (vertices + i)->position;
			nedge.ymin = (vertices + i)->position[1];
			nedge.ymax = (vertices + i + 1)->position[1];
			nedge.pos2 = (vertices + i + 1)->position;
		}
		else
		{
			nedge.ymax = (vertices + i)->position[1];
			nedge.ymin = (vertices + i + 1)->position[1];
			nedge.pos2 = (vertices + i)->position;
			nedge.pos1 = (vertices + i + 1)->position;
		}
		nedge.m = (float)((vertices + i + 1)->position[1] - (vertices + i)->position[1]) / (float)((vertices + i + 1)->position[0] - (vertices + i)->position[0]);//DY / DX
		GlobalEdge.push_back(nedge);

	}
	for (int i = 0; i < count; i++) {
		if ((vertices + i)->position[1] > ymax) {
			ymax = (int)(vertices + i)->position[1];
		}
		if ((vertices + i)->position[1] < ymin) {
			ymin = (int)(vertices + i)->position[1];
		}
	}
	//add last one to the first
	int lasti = count - 1;
	Edge nedge;
	if ((vertices + lasti)->position[1] < (vertices)->position[1]) {
		nedge.pos1 = (vertices + lasti)->position;
		nedge.pos2 = (vertices)->position;
	}
	else
	{
		nedge.pos2 = (vertices + lasti)->position;
		nedge.pos1 = (vertices)->position;
	}
	nedge.m = (float)((vertices + lasti + 1)->position[1] - (vertices + lasti)->position[1]) / (float)((vertices + lasti + 1)->position[0] - (vertices + lasti)->position[0]);//DY / DX
	GlobalEdge.push_back(nedge);
	std::vector<int> Cutpointlist;

	//sort all edges by y coord
	Edge temp;
	for (int j = 0; j < GlobalEdge.size() - 1; j++) {
		for (int i = 0; i < GlobalEdge.size() - 1; i++) {
			if (GlobalEdge[i].pos1[1] > GlobalEdge[i + 1].pos1[1]) {
				temp = GlobalEdge[i];
				GlobalEdge[i] = GlobalEdge[i + 1];
				GlobalEdge[i + 1] = temp;
			}
		}
	}

	/*std::cout << "------------" << std::endl;
	for (int i = 0; i < GlobalEdge.size(); i++) {

		std::cout << "X: " << GlobalEdge[i].pos1[0] << " Y: " << GlobalEdge[i].pos1[1] <<
			" --- " << "X: " << GlobalEdge[i].pos2[0] << " Y: " << GlobalEdge[i].pos2[1] << std::endl;

	}
	std::cout << "------------" << std::endl;*/

	//lets get scaning!
	for (int scanline = ymin; scanline <= ymax; scanline++) {
		Cutpointlist.clear();//clear cut points for this run
		//if edge needs to be active.
		for (int i = 0; i < GlobalEdge.size(); i++) {
			if (GlobalEdge[i].ymin == ymin) {
				ActiveEdge.push_back(GlobalEdge[i]);
			}
			if (GlobalEdge[i].ymax == scanline) {

			}
		}
		for (int i = 0; i < GlobalEdge.size(); i++) {
			// here the scanline intersects the smaller position in the edge
			if (scanline == GlobalEdge[i].pos1[1]) {

				if (scanline == GlobalEdge[i].pos2[1]) {
					//set the curx to the small
					//line is horizontal
					GlobalEdge[i].Curx = GlobalEdge[i].pos1[0];
					Cutpointlist.push_back((int)GlobalEdge[i].Curx);
				}
				else
				{//we are the being handled by the sm,aller vert?
					//the edge is not horizontal
					GlobalEdge[i].Curx = GlobalEdge[i].pos1[0];
					Cutpointlist.push_back((int)GlobalEdge[i].Curx);

				}
			}
			//we hit the bigger vertex
			if (scanline == GlobalEdge[i].pos2[1]) {
				//we hit a vertex add a cut point
				GlobalEdge[i].Curx = GlobalEdge[i].pos1[0];
				Cutpointlist.push_back((int)GlobalEdge[i].Curx);
			}
			//we are within a line
			if (scanline > GlobalEdge[i].pos1[1] && scanline < GlobalEdge[i].pos2[1]) {
				//we have intersected with a line so add a cutpoint.
				//increment the Curx
				GlobalEdge[i].Curx += (float)(1.0f / (float)GlobalEdge[i].m);
				Cutpointlist.push_back((int)GlobalEdge[i].Curx);
			}
		}

		//Now we sort the list to ge accending x coords
		//aww simple little bubble sort
		int temp;
		for (int j = 0; j < Cutpointlist.size(); j++) {
			for (int i = 0; i < Cutpointlist.size() - 1; i++) {
				if (Cutpointlist[i] > Cutpointlist[i + 1]) {
					temp = Cutpointlist[i];
					Cutpointlist[i] = Cutpointlist[i + 1];
					Cutpointlist[i + 1] = temp;
				}
			}
		}

		Colour4 color;
		color[0] = 1;
		color[3] = 1;
		Vertex2d vert = *vertices;
		SetFGColour(color);
		///this value Is a Error catch to prevent the cutpoint drawing loop from failing
		int listErrorCover = Cutpointlist.size();
		if (Cutpointlist.size() % 2 != 0) {
			DrawPoint2D(Cutpointlist[Cutpointlist.size() - 1], scanline, 5);
			listErrorCover--;
			//	std::cout << "Major Issue shouldn't happen\n";
		}
		for (int i = 0; i < listErrorCover; i += 2) {
			Vertex2d vert1;
			Vertex2d vert2;
			//need to find out whtat vertex to get colour from
			vert1.position = Vector2(Cutpointlist[i], scanline);
			vert1.colour = vert.colour;

			vert2.position = Vector2(Cutpointlist[i + 1], scanline);
			vert2.colour = vert.colour;
			DrawLine2D(vert1, vert2);
			//phew!
		}

	}
	Colour4 color;
	color[0] = 1;
	color[3] = 1;
	SetFGColour(color);
	DrawUnfilledPolygon2D(vertices, count);

	//Ex 2.3 Extend Rasterizer::ScanlineFillPolygon2D method so that it is capable of alpha blending, i.e. draw translucent polygons.
	//Note: The variable mBlendMode indicates if the blend mode is set to alpha blending.
	//To do alpha blending during filling, the new colour of a point should be combined with the existing colour in the framebuffer using the alpha value.
	//Use Test 6 (Press F6) to test your solution

}

void Rasterizer::ScanlineInterpolatedFillPolygon2D(const Vertex2d * vertices, int count)
{

	ScanlineFillPolygon2D(vertices, count);
	//TODO:
	//Ex 2.4 Implement Rasterizer::ScanlineInterpolatedFillPolygon2D method so that it is capable of performing interpolated filling.
	//Note: mFillMode is set to INTERPOLATED_FILL
	//		This exercise will be more straightfoward if Ex 1.3 has been implemented in DrawLine2D
	//Use Test 7 to test your solution
}

void Rasterizer::DrawCircle2D(const Circle2D & inCircle, bool filled)
{
	//TODO:
	//Ex 2.5 Implement Rasterizer::DrawCircle2D method so that it can draw a filled circle.
	//Note: For a simple solution, you can first attempt to draw an unfilled circle in the same way as drawing an unfilled polygon.
	//Use Test 8 to test your solution
	std::vector<Vertex2d> verts;
	float r = inCircle.radius;
	int nsegment = 100;//todo: tune this value?
	float t = 0;
	float dt = 2 * PI / nsegment;

	while (t < 2 * PI)
	{
		//	drawline(P(t - dt), P(t));
		Colour4 color;
		color[1] = 1;
		color[3] = 1;
		Vertex2d vert1;
		vert1.colour = color;
		vert1.position = Vector2(r*cos(t - dt) + inCircle.centre[0], r*sin(t - dt) + inCircle.centre[1]);
		verts.push_back(vert1);
		t += dt;
	}
	for (int i = 0; i < verts.size(); i++) {
		Vertex2d centrevert;
		centrevert.position = inCircle.centre;
		centrevert.colour[1] = 1;
		centrevert.colour[0] = 1;
		centrevert.colour[2] = 1;
		SetFillMode(FillMode::INTERPOLATED_FILLED);
		DrawLine2D(centrevert, verts[i]);
	}
	return;
	Vertex2d* a = &verts[0];

	if (filled == false) {
		DrawUnfilledPolygon2D(a, verts.size());
	}
	else {
		ScanlineFillPolygon2D(a, verts.size());
	}
}

Framebuffer *Rasterizer::GetFrameBuffer() const
{
	return mFramebuffer;
}
