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
	//need to deal with diagonals 
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
	outP1 = p1;
	outP2 = p2;
	while (true) {
		unsigned int outcode1 = ComputeOutCode(outP1, clipRect);
		unsigned int outcode2 = ComputeOutCode(outP2, clipRect);
		int x = 0, y = 0;
		int outcode = outcode1 ? outcode1 : outcode2;

		if ((outcode1 | outcode2) == 0) {
			return true;
		}
		else if ((outcode1 & outcode2)) {
			return false;
		}

		if (outcode & 0x8) {
			y = clipRect.bottom;//ymin
		}
		else if (outcode & 0x4) {
			y = clipRect.top;//ymax
		}
		else if (outcode & 0x2) {
			x = clipRect.right;//xmax
		}
		else if (outcode & 0x1) {
			x = clipRect.left;//xmin
		}
		if (outcode == outcode1) {
			outP1[0] = (float)x;
			outP1[1] = (float)y;
		}
		else {
			outP2[0] = (float)x;
			outP2[1] = (float)y;
		}
	}
	return false;
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
}

void Rasterizer::DrawPoint2D(int x, int y)
{
	WriteRGBAToFramebuffer(x, y, mFGColour);
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

	//Ex 1.3 Extend the implementation of Rasterizer::DrawLine2D so that it is capable of interpolating colour across a line when each end-point has different colours.
	//Note: The variable mFillMode indicates if the fill mode is set to INTERPOLATED_FILL. 
	//The colour of each point should be linearly interpolated using the colours of v1 and v2.
	//Use Test 2 (Press F2) to test your implementation
	Vector2 pt1 = v1.position;
	Vector2 pt2 = v2.position;
	//calculate wether our line is very steep
	const bool Swap_xy = (abs(pt2[1] - pt1[1]) > abs(pt2[0] - pt1[0]));//dy > dx
	bool Swap_verts = pt1[0] > pt2[0];
	Colour4	 Vert1col = GetColor((int)v1.position[0], (int)v1.position[1], v1);
	Colour4	 Vert2col = GetColor((int)v2.position[0], (int)v2.position[1], v2);
	if (Swap_xy)
	{
		std::swap(pt1[0], pt1[1]);
		std::swap(pt2[0], pt2[1]);
	}
	//swap Verts
	if (pt1[0] > pt2[0])
	{
		std::swap(pt1, pt2);
	}
	int y = (int)pt1[1];
	int x = (int)pt1[0];
	int	dx = (int)pt2[0] - (int)pt1[0];
	int dy = (int)abs(pt2[1] - pt1[1]);
	int error = dx;
	int ystep = (pt1[1] < pt2[1]) ? 1 : -1;
	int MaxX = (int)pt2[0];
	for (int curx = x; curx < MaxX; curx++)
	{
		Vert1col = GetColor(curx, y, v1);
		Vert2col = GetColor(curx, y, v2);
		//Triple bools selecting each octant that needs colour swapping ( octs 1,2,3 and 8)
		if ((!Swap_verts && !Swap_xy && ystep == 1) || (!Swap_verts && Swap_xy && ystep == 1) || (Swap_verts && Swap_xy && ystep == -1) || (!Swap_verts && !Swap_xy && ystep == -1))
		{
			std::swap(Vert1col, Vert2col);
		}
		Colour4 colour;
		if (mFillMode == FillMode::INTERPOLATED_FILLED) {
			Vector2 curpos((float)curx, (float)y);
			//calculate the position along the line as a percentage (0-1)
			float t = (pt1 - curpos).Norm() / (pt1 - pt2).Norm();
			//Full Accurate lerp
			colour[0] = t * Vert1col[0] + (1 - t) * Vert2col[0];
			colour[1] = t * Vert1col[1] + (1 - t) * Vert2col[1];
			colour[2] = t * Vert1col[2] + (1 - t) * Vert2col[2];
		}
		else
		{
			colour = Vert1col;
		}
		SetFGColour(colour);
		Colour4 current = GetColourAtPoint(curx, y);
		float a = 0;
		//thickness is implemented here as all needed information is at this scope
		if (Swap_xy)
		{
			DrawPoint2D(y, curx);
			int thicknessi = 0;
			bool plus = false;
			bool minus = false;
			for (int i = 0; i < thickness - 1; i++) {
				if (plus == false) {
					plus = true;
					DrawPoint2D(y + thicknessi, curx);
				}
				if (minus == false) {
					minus = true;
					DrawPoint2D(y - thicknessi, curx);
				}
				if (minus == true && plus == true) {
					if (thicknessi < thickness) {
						thicknessi++;
					}
					plus = false;
					minus = false;
				}
			}
		}
		else
		{
			DrawPoint2D(curx, y);
			int thicknessi = 0;
			bool plus = false;
			bool minus = false;
			for (int i = 0; i < thickness - 1; i++) {
				if (plus == false) {
					plus = true;
					DrawPoint2D(curx, y + thicknessi);
				}
				if (minus == false) {
					minus = true;
					DrawPoint2D(curx, y - thicknessi);
				}
				if (minus == true && plus == true) {
					if (thicknessi < thickness) {
						thicknessi++;
					}
					plus = false;
					minus = false;
				}
			}
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

Colour4 Rasterizer::MultiplyAlpha(Colour4 color, Colour4 background, float alpha) {
	Colour4 out;
	out[0] = alpha * color[0] + (1 - alpha)*background[0];
	out[1] = alpha * color[1] + (1 - alpha)*background[1];
	out[2] = alpha * color[2] + (1 - alpha)*background[2];
	return out;
}
Colour4 Rasterizer::GetColor(int x, int y, const Vertex2d vert) {
	Colour4 out;
	if (mBlendMode == BlendMode::ALPHA_BLEND) {
		Colour4 current = GetColourAtPoint(x, y);
		out = MultiplyAlpha(vert.colour, current, vert.colour[3]);
	}
	else
	{
		out = vert.colour;
	}
	return out;
}
//this struct defines an edge
typedef struct edge {
	Vector2 pos1;
	Vector2 pos2;
	float m;
	float Curx;
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
	//List of all the edges in this polygon
	std::vector<Edge> GlobalEdge;
	int ymax = 10;
	int ymin = 0;
	//Find Ymin and Ymax
	//this loop creates our edge table from the vertice list given to the function
	for (int i = 0; i < count - 1; i++) {
		Edge nedge;
		if ((vertices + i)->position[1] < (vertices + i + 1)->position[1]) {
			nedge.pos1 = (vertices + i)->position;
			nedge.pos2 = (vertices + i + 1)->position;
		}
		else
		{
			nedge.pos2 = (vertices + i)->position;
			nedge.pos1 = (vertices + i + 1)->position;
		}
		nedge.m = (float)((vertices + i + 1)->position[1] - (vertices + i)->position[1]) / (float)((vertices + i + 1)->position[0] - (vertices + i)->position[0]);//DY / DX
		GlobalEdge.push_back(nedge);
	}
	//this creates an edge entry from the last vertex to the first vertex.
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
	nedge.m = (float)((vertices + lasti)->position[1] - (vertices)->position[1]) / (float)((vertices + lasti)->position[0] - (vertices)->position[0]);//DY / DX
	GlobalEdge.push_back(nedge);

	//find the ymax and ymin of the geomertry
	for (int i = 0; i < count; i++) {
		if ((vertices + i)->position[1] > ymax) {
			ymax = (int)(vertices + i)->position[1];
		}
		if ((vertices + i)->position[1] < ymin) {
			ymin = (int)(vertices + i)->position[1];
		}
	}

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
	std::vector<int> Cutpointlist;
	//lets get scaning!
	for (int scanline = ymin; scanline <= ymax; scanline++) {
		Cutpointlist.clear();//clear cut points for this run
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
				{
					//we are the being handled by the smaller vert
					//the edge is not horizontal
					GlobalEdge[i].Curx = GlobalEdge[i].pos1[0];
				}
			}
			//we hit the bigger vertex
			if (scanline == GlobalEdge[i].pos2[1]) {
				//we hit a vertex add a cut point
				GlobalEdge[i].Curx = GlobalEdge[i].pos2[0];
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
		//for speed:
		//this loop removes any duplicate points generated 
		for (int l = 0; l < Cutpointlist.size(); l++) {
			for (int k = 0; k < Cutpointlist.size(); k++) {
				if (k != l && Cutpointlist[k] == Cutpointlist[l]) {
					float t = Cutpointlist[k];
					float t2 = Cutpointlist[l];
					Cutpointlist.erase(std::remove(Cutpointlist.begin(), Cutpointlist.end(), Cutpointlist[k]), Cutpointlist.end());
				}
			}
		}

		//Now we sort the list to get accending x coords
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
		int listErrorCover = Cutpointlist.size();
		// the cupoint list only has even pairs in it
		if (Cutpointlist.size() % 2 != 0) {
			listErrorCover--;
		}
		Vertex2d vert = *vertices;
		//we draw out the lines between our cutpoint pairs
		for (int i = 0; i < listErrorCover; i += 2) {
			Vertex2d vert1;
			Vertex2d vert2;
			vert1.colour = vert.colour;
			vert2.colour = vert.colour;
			vert1.position = Vector2((float)Cutpointlist[i], (float)scanline);
			vert2.position = Vector2((float)Cutpointlist[i + 1], (float)scanline);
			DrawLine2D(vert1, vert2);
		}
	}
	//Ex 2.3 Extend Rasterizer::ScanlineFillPolygon2D method so that it is capable of alpha blending, i.e. draw translucent polygons.
	//Note: The variable mBlendMode indicates if the blend mode is set to alpha blending.
	//To do alpha blending during filling, the new colour of a point should be combined with the existing colour in the framebuffer using the alpha value.
	//Use Test 6 (Press F6) to test your solution

}
//this struct defines an edge and its colour
typedef struct coloredge {
	Vector2 pos1;
	Vector2 pos2;
	Vertex2d vert1;
	Vertex2d vert2;
	float m;//Gradent of the line
	float Curx;//current x incremented by 1/m
}ColorEdge;
void Rasterizer::ScanlineInterpolatedFillPolygon2D(const Vertex2d * vertices, int count)
{
	std::vector<ColorEdge> GlobalEdge;
	int ymax = 10;
	int ymin = 0;
	//Find Ymin and Ymax
	for (int i = 0; i < count - 1; i++) {
		ColorEdge nedge;
		if ((vertices + i)->position[1] < (vertices + i + 1)->position[1]) {
			nedge.pos1 = (vertices + i)->position;
			nedge.pos2 = (vertices + i + 1)->position;
			nedge.vert1 = *(vertices + i);
			nedge.vert2 = *(vertices + i + 1);
		}
		else
		{
			nedge.pos2 = (vertices + i)->position;
			nedge.pos1 = (vertices + i + 1)->position;
			nedge.vert2 = *(vertices + i);
			nedge.vert1 = *(vertices + i + 1);
		}
		nedge.m = (float)((vertices + i + 1)->position[1] - (vertices + i)->position[1]) / (float)((vertices + i + 1)->position[0] - (vertices + i)->position[0]);//DY / DX
		GlobalEdge.push_back(nedge);

	}
	//join the last vertex to the first one
	int lasti = count - 1;
	ColorEdge nedge;
	if ((vertices + lasti)->position[1] < (vertices)->position[1]) {
		nedge.pos1 = (vertices + lasti)->position;
		nedge.pos2 = (vertices)->position;
		nedge.vert1 = *(vertices + lasti);
		nedge.vert2 = *(vertices);
	}
	else
	{
		nedge.pos2 = (vertices + lasti)->position;
		nedge.pos1 = (vertices)->position;
		nedge.vert2 = *(vertices + lasti);
		nedge.vert1 = *(vertices);
	}
	nedge.m = (float)((vertices + lasti)->position[1] - (vertices)->position[1]) / (float)((vertices + lasti)->position[0] - (vertices)->position[0]);//DY / DX
	GlobalEdge.push_back(nedge);
	//finds the ymax and ymin of the polygon
	for (int i = 0; i < count; i++) {
		if ((vertices + i)->position[1] > ymax) {
			ymax = (int)(vertices + i)->position[1];
		}
		if ((vertices + i)->position[1] < ymin) {
			ymin = (int)(vertices + i)->position[1];
		}
	}

	//sort all edges by y coord
	ColorEdge temp;
	for (int j = 0; j < GlobalEdge.size() - 1; j++) {
		for (int i = 0; i < GlobalEdge.size() - 1; i++) {
			if (GlobalEdge[i].pos1[1] > GlobalEdge[i + 1].pos1[1]) {
				temp = GlobalEdge[i];
				GlobalEdge[i] = GlobalEdge[i + 1];
				GlobalEdge[i + 1] = temp;
			}
		}
	}

	std::vector<ScanlineLUTItem> Cutpointlist;
	//lets get scaning!
	for (int scanline = ymin; scanline <= ymax; scanline++) {
		Cutpointlist.clear();//clear cut points for this run		
		for (int i = 0; i < GlobalEdge.size(); i++) {
			// here the scanline intersects the smaller position in the edge
			if (scanline == GlobalEdge[i].pos1[1]) {

				if (scanline == GlobalEdge[i].pos2[1]) {
					//set the curx to the small
					//line is horizontal
					GlobalEdge[i].Curx = GlobalEdge[i].pos1[0];
					ScanlineLUTItem lut;
					lut.pos_x = (int)GlobalEdge[i].Curx;
					lut.colour = GlobalEdge[i].vert1.colour;
					Cutpointlist.push_back(lut);
				}
				else
				{
					//we are the being handled by the smaller vert				
					//the edge is not horizontal
					GlobalEdge[i].Curx = GlobalEdge[i].pos1[0];
				}
			}
			//we hit the bigger vertex
			if (scanline == GlobalEdge[i].pos2[1]) {
				//we hit a vertex add a cut point
				GlobalEdge[i].Curx = GlobalEdge[i].pos2[0];
				ScanlineLUTItem lut;
				lut.pos_x = (int)GlobalEdge[i].Curx;
				lut.colour = GlobalEdge[i].vert2.colour;
				Cutpointlist.push_back(lut);
			}
			//we are within a line
			if (scanline > GlobalEdge[i].pos1[1] && scanline < GlobalEdge[i].pos2[1]) {
				//we have intersected with a line so add a cutpoint.
				//increment the Curx
				GlobalEdge[i].Curx += (float)(1.0f / (float)GlobalEdge[i].m);
				ScanlineLUTItem lut;
				lut.pos_x = (int)GlobalEdge[i].Curx;
				Vector2 curpos((float)lut.pos_x, (float)scanline);
				float t = (float)(GlobalEdge[i].pos1 - curpos).Norm() / (float)(GlobalEdge[i].pos1 - GlobalEdge[i].pos2).Norm();
				lut.colour = GlobalEdge[i].vert1.colour * (1 - t) + (GlobalEdge[i].vert2.colour) *t;
				Cutpointlist.push_back(lut);
			}
		}

		//Now we sort the list to ge accending x coords
		//aww simple little bubble sort
		ScanlineLUTItem temp;
		for (int j = 0; j < Cutpointlist.size(); j++) {
			for (int i = 0; i < Cutpointlist.size() - 1; i++) {
				if (Cutpointlist[i].pos_x > Cutpointlist[i + 1].pos_x) {
					temp.pos_x = Cutpointlist[i].pos_x;
					temp.colour = Cutpointlist[i].colour;
					Cutpointlist[i] = Cutpointlist[i + 1];
					Cutpointlist[i + 1] = temp;
				}
			}
		}

		Vertex2d vert = *vertices;
		for (int i = 0; i < Cutpointlist.size(); i += 2) {
			ScanlineLUTItem p0 = Cutpointlist[i];
			ScanlineLUTItem p1 = Cutpointlist[i + 1];
			//draw our hoizontal line LERPing colour across it
			for (int x = p0.pos_x; x <= p1.pos_x; x++) {
				Colour4 color;

				if (mFillMode == FillMode::INTERPOLATED_FILLED) {
					float deltax = (float)p1.pos_x - (float)p0.pos_x;
					float t = (float)(x - p0.pos_x) / (float)deltax;
					color = p0.colour*(1.0f - t) + p1.colour*t;
					SetFGColour(color);
				}
				DrawPoint2D(x, scanline);
			}
		}
	}
	//TODO:
	//Ex 2.4 Implement Rasterizer::ScanlineInterpolatedFillPolygon2D method so that it is capable of performing interpolated filling.
	//Note: mFillMode is set to INTERPOLATED_FILL
	//		This exercise will be more straightfoward if Ex 1.3 has been implemented in DrawLine2D
	//Use Test 7 to test your solution
}
// this function checks if a point is within a circle
int inline inCircleN(int x, int y, float radius, Vector2 origin) {
	int dx = (int)abs(x - origin[0]);
	int dy = (int)abs(y - origin[1]);
	return (dx * dx + dy * dy <= radius*radius);
}
void Rasterizer::BoundaryDrawFilledCircle2D(const Circle2D & inCircle) {
	//find the bounds of our circle
	int ymax = (int)(inCircle.centre[1] + inCircle.radius);
	int ymin = (int)(inCircle.centre[1] - inCircle.radius);
	int xmax = (int)(inCircle.centre[0] + inCircle.radius);
	int xmin = (int)(inCircle.centre[0] - inCircle.radius);
	//loop though each point in our bounds and check if they are within the circle
	for (int y = ymin; y < ymax; y++) {
		for (int x = xmin; x < xmax; x++) {
			//check this point is within circle;
			if (inCircleN(x, y, inCircle.radius, inCircle.centre)) {
				//this point is inside the circle so we should draw
				SetFGColour(inCircle.colour);
				DrawPoint2D(x, y);
			}
		}
	}
}
void Rasterizer::DrawCircle2D(const Circle2D & inCircle, bool filled)
{
	//TODO:
	//Ex 2.5 Implement Rasterizer::DrawCircle2D method so that it can draw a filled circle.
	//Note: For a simple solution, you can first attempt to draw an unfilled circle in the same way as drawing an unfilled polygon.
	//Use Test 8 to test your solution	
	if (filled == false) {
		//generate vertexs for our circle
		std::vector<Vertex2d> verts;//list of generated vertexes
		float r = inCircle.radius;
		int nsegment = (int)(inCircle.radius * 25);
		float t = 0;
		float dt = 2.0f * (float)PI / nsegment;
		while (t < 2 * PI)
		{
			Vertex2d vert1;
			vert1.colour = inCircle.colour;
			vert1.position = Vector2(r*cos(t - dt) + inCircle.centre[0], r*sin(t - dt) + inCircle.centre[1]);
			verts.push_back(vert1);
			t += dt;
		}
		Vertex2d* a = &verts[0];
		//we draw out our polygon
		DrawUnfilledPolygon2D(a, (int)verts.size());
	}
	else
	{
		BoundaryDrawFilledCircle2D(inCircle);
	}
}

Framebuffer *Rasterizer::GetFrameBuffer() const
{
	return mFramebuffer;
}
