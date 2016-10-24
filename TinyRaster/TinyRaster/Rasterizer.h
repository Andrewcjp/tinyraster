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
#include <vector>
#include "Framebuffer.h"
#include "Vector2.h"
#include "Texture.h"
#include <cmath>
#define PI 3.14159
//Struct representing an entry to the scanline lookup table
typedef struct _ScanlineLUTItem
{
	Colour4 colour;			//the colour of an edge pixel
	int pos_x;				//the x position of an edge along the scanline
	Vector2 uv;
} ScanlineLUTItem;

//typedef a scanline as a dynamic array of ScanlineLUTItem
typedef std::vector<ScanlineLUTItem> Scanline;

//struct represent a axis-aligned clip rectangle
typedef struct _ClipRect
{
	int left;			//position of the left edge
	int right;			//position of the right edge
	int top;			//position of the top edge
	int bottom;			//position of the bottom edge
} ClipRect;

class Rasterizer
{
public:
	//enum for the rasterisation geometry
	enum GeometryMode {
		LINE = 0,		//line geometry
		POLYGON			//polygon geometry
	};

	//enum for fill mode
	enum FillMode {
		UNFILLED = 0,				//unfilled
		SOLID_FILLED,				//fill with solid colour
		INTERPOLATED_FILLED,			//interpolated(gradient) fill
		TEXTURED					//Textured Filling
	};

	//enum for blending mode
	enum BlendMode {
		NO_BLEND = 0,				//no blending
		ALPHA_BLEND					//alpha blending, e.g. translucency 
	};

private:
	Colour4			mFGColour;		//default foreground colour
	Colour4			mBGColour;		//default background colour
	ClipRect		mClipRect;		//current clip region
	Framebuffer		*mFramebuffer;	//The framebuffer owned by the rasterizer
	Scanline		*mScanlineLUT;	//Lookup table for scanline filling
	int				mWidth;			//Width of the framebuffer
	int				mHeight;		//Height of the framebuffer
	GeometryMode	mGeometryMode;	//current geometry rasterisation mode 
	FillMode		mFillMode;		//current fill mode
	BlendMode		mBlendMode;		//current blend mode
	Texture*			mTexture;
	bool mEnableAA;
	Rasterizer(void);				//prevent default constructor from being directly invoked

	//Clear all entries in mScanlineLUT[]
	void ClearScanlineLUT();		

	
	//Method for computing the outcode of a given point p
	//input: const Vector2 &p the coordinate of 2D point p
	unsigned int ComputeOutCode(const Vector2& p, const ClipRect& clipRect);

	//Method for line clipping
	//inputs:	v1 and v2 are the end points of an input line segment
	//			clipRect is the rectangular clip region 
	//outputs: outP1 and outP2 are the end points of the output clipped line
	bool ClipLine(const Vertex2d &v1, const Vertex2d &v2, const ClipRect& clipRect, Vector2 &outP1, Vector2 &outP2);

	//Method for writing a given colour to the framebuffer
	//inputs:	int x --- x coordinate of the framebuffer location
	//			int y --- y coordinate of the framebuffer location
	//			const Colour4& rgba --- the RGBA colour to be written to the framebuffer;
	void WriteRGBAToFramebuffer(int x, int y, const Colour4& colour);

	Colour4 GetColourAtPoint(int x, int y);

public:
	Rasterizer(int width, int height);

	~Rasterizer(void);

	inline int Width() { return mWidth; }
	inline int Height() { return mHeight; }
	
	//Method for clearing the entire framebuffer with a given colour
	//input:	const Colour4& colour --- the background colour to be used
	void Clear(const Colour4& colour);
	
	//Method for drawing a single point
	//input:	Vector2& the coordinate of the point
	//			int size the size of the point to be rasterisation in pixel
	void DrawPoint2D(const Vector2&, int size = 1);

	void DrawPoint2D(int x, int y, int size);
	
	//Method for drawing a 2D line from two given vertices using Bresenham's algorithm
	//input:	const Vertex2d &v1 --- vertex 1
	//			const Vertex2d &v2 --- vertex 2
	//			int thickness -- thickness of the line in pixel
	void DrawLine2D(const Vertex2d& v1, const Vertex2d& v2, int thickness = 1);
	
	//Method for drawing an unfilled 2D polygon
	//input:	const Vertex2d* vertices --- an array of polygon vertices ordered in counterclock-wise
	//			int count --- the number of vertices in the array
	void DrawUnfilledPolygon2D(const Vertex2d* vertices, int count);

	Colour4 MultiplyAlpha(Colour4 color, Colour4 background, float alpha);

	Colour4 GetColor(int x, int y, const Vertex2d vert);
	Colour4 SetAlpha(Colour4 colour, float);

	
	//Method for drawing solidly filled 2D polygon
	//input:	const Vertex2d* vertices --- an array of polygon vertices ordered in counterclock-wise
	//			int count --- the number of vertices in the array
	void ScanlineFillPolygon2D(const Vertex2d* vertices, int count);
	
	//Method for drawing filled 2D polygon using interpolated filling (aka gradient colours)
	//input:	const Vertex2d* vertices --- an array of polygon vertices ordered in counterclock-wise
	//			int count --- the number of vertices in the array
	void ScanlineInterpolatedFillPolygon2D(const Vertex2d* vertices, int count);


	void BoundaryDrawFilledCircle2D(const Circle2D & inCircle);

	//Method for drawing a 2D circle either filled or unfill based on the value of bool filled
	//input:	const Circle2D& inCircle --- a 2D circle
	//			bool filled --- indicate if the circle is draw as filled or unfilled
	void DrawCircle2D(const Circle2D& inCircle, bool filled = false);
	
	//Getter for getting the Framebuffer attached to the rasterizer
	Framebuffer *GetFrameBuffer() const;

	//Set the forground colour; the rasterizer uses foreground colour for drawing operation
	//input:	const Colour4 &colour --- a 4 component colour RGBA, each component is a float in [0,1]
	inline void SetFGColour(const Colour4& colour)
	{
		mFGColour = colour;
	}

	//Set the background colour; the rasterizer uses background colour for clearing the content of the framebuffer
	//input:	const Colour4 &colour --- a 4 component colour RGBA, each component is a float in [0,1]
	inline void SetBGColour(const Colour4& colour)
	{
		mBGColour = colour;
	}

	//Method for setting the rectangular clip region
	inline void SetClipRectangle(int left, int right, int bottom, int top)
	{
		mClipRect.left = left;
		mClipRect.right = right;
		mClipRect.bottom = bottom;
		mClipRect.top = top;
	}

	//Getter method for current foreground colour
	inline Colour4 GetCurrentFGColour()
	{
		return mFGColour;
	}

	//Getter method for current background colour
	inline Colour4 GetCurrentBGColour()
	{
		return mBGColour;
	}

	//Getter method for current geometry mode
	inline void SetGeometryMode(GeometryMode mode)
	{
		mGeometryMode = mode;
	}

	//Getter method for current fill mode
	inline void SetFillMode(FillMode mode)
	{
		mFillMode = mode;
	}

	//Getter method for current blend mode
	inline void SetBlendMode(BlendMode mode)
	{
		mBlendMode = mode;
	}
};

bool Intersects(int ypos, Vertex2d v1, Vertex2d v2);

Colour4 MultiplyAlpha(Colour4 color, Colour4 background, float alpha);
