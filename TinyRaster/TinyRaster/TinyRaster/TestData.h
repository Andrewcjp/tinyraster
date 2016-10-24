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

namespace AssignmentTests
{
	const static Colour4 BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
	const static Colour4 WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
	const static Colour4 RED = { 1.0f, 0.0f, 0.0f, .4f };
	const static Colour4 GREEN = { 0.0f, 1.0f, 0.0f, .7f };
	const static Colour4 BLUE = { 0.0f, 0.0f, 1.0f, .6f };
	const static Colour4 YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };
	const static Colour4 ORANGE = { 1.0f, 0.65f, 0.0f, .8f };
	const static Colour4 CYAN = { 0.0f, 1.0f, 1.0f, 1.0f };
	const static Colour4 PURPLE = { 1.0f, 0.0f, 1.0f, 1.0f};
	const static Colour4 OLIVEGRN = { 0.43f, 0.56f, 0.24f, 0.3f };
	const static Colour4 LIGHTGRAY = { 0.8f, 0.8f, 0.8f, 0.95f };

	static Vertex2d lines[] = {

		{ YELLOW, Vector2(147, 5) },
		{ YELLOW, Vector2(393, 710) },

		{ WHITE, Vector2(403, 3) },
		{ WHITE, Vector2(403, 710) },

		{ GREEN, Vector2(578, 15) },
		{ GREEN, Vector2(456, 700) },

		{ PURPLE, Vector2(681, 701) },
		{ PURPLE, Vector2(605, 22) },
		
		{ BLUE, Vector2(1057, 5) },
		{ BLUE, Vector2(710, 715) },

		{ RED, Vector2(15, 180) },
		{ RED, Vector2(1200, 180) },

		{ CYAN, Vector2(1094, 616) },
		{ CYAN, Vector2(80, 267) },
			
		{ ORANGE, Vector2(100, 597) },
		{ ORANGE, Vector2(1212, 329)}
	};

	static Vertex2d lines_interp[] = {

		{ YELLOW, Vector2(147, 5) },
		{ RED, Vector2(393, 710) },

		{ WHITE, Vector2(403, 3) },
		{ YELLOW, Vector2(403, 710) },

		{ GREEN, Vector2(578, 15) },
		{ BLUE, Vector2(456, 700) },

		{ PURPLE, Vector2(681, 701) },
		{ CYAN, Vector2(605, 22) },

		{ BLUE, Vector2(1057, 5) },
		{ ORANGE, Vector2(710, 715) },

		{ RED, Vector2(15, 180) },
		{ GREEN, Vector2(1200, 180) },

		{ CYAN, Vector2(1094, 616) },
		{ ORANGE, Vector2(80, 267) },

		{ ORANGE, Vector2(100, 597) },
		{ WHITE, Vector2(1212, 329) }
	};

	static Vertex2d rectangle1[] = {
		{ LIGHTGRAY, Vector2(464, 200) },
		{ LIGHTGRAY, Vector2(464, 590) },
		{ LIGHTGRAY, Vector2(1030, 590) },
		{ LIGHTGRAY, Vector2(1030, 200) }
	};

	static Vertex2d quad1[] = {
		{ GREEN, Vector2(380, -50) },
		{ GREEN, Vector2(900, 10) },
		{ GREEN, Vector2(800, 300) },
		{ GREEN, Vector2(400, 350) }
	};

	static Vertex2d square[] = {
		{ ORANGE, Vector2(355, 435) },
		{ ORANGE, Vector2(355, 675) },
		{ ORANGE, Vector2(595, 675) },
		{ ORANGE, Vector2(595, 435) }
	};

	static Vertex2d triangle[] = {
		{ RED, Vector2{550, 535} },
		{ RED, Vector2{263, -10} },
		{ RED, Vector2{ -10, 535 } }
	};

	static Vertex2d pentagon[] = {
		{ OLIVEGRN, Vector2{1097, 290} },
		{ OLIVEGRN, Vector2{ 900, 440 } },
		{ OLIVEGRN, Vector2{ 975, 690 } },
		{ OLIVEGRN, Vector2{ 1220, 690 } },
		{ OLIVEGRN, Vector2{ 1300, 440 } }
	};

	static Vertex2d comb[] = {
		{ GREEN, Vector2(100, 600) },
		{ GREEN, Vector2(60, 80) },
		{ GREEN, Vector2(620, 30) },
		{ GREEN, Vector2(1200, 120) },
		{ GREEN, Vector2(1100, 600) },
		{ GREEN, Vector2(1050, 300) },
		{ GREEN, Vector2(950, 500) },
		{ GREEN, Vector2(700, 350) },
		{ GREEN, Vector2(500, 650) },
		{ GREEN, Vector2(300, 250) },
	};

	static Vertex2d grad_rectangle[] = {
		{ LIGHTGRAY, Vector2(464, 200) },
		{ BLUE, Vector2(464, 590) },
		{ BLUE, Vector2(1030, 590) },
		{ LIGHTGRAY, Vector2(1030, 200) }
	};

	static Vertex2d grad_square[] = {
		{ WHITE, Vector2(355, 435) },
		{ WHITE, Vector2(355, 675) },
		{ ORANGE, Vector2(595, 675) },
		{ ORANGE, Vector2(595, 435) }
	};

	static Vertex2d grad_triangle[] = {
		{ GREEN, Vector2{ 550, 535 } },
		{ RED, Vector2{ 263, -10 } },
		{ BLUE, Vector2{ -10, 535 } }
	};

	static Vertex2d grad_pentagon[] = {
		{ OLIVEGRN, Vector2{ 1097, 290 } },
		{ WHITE, Vector2{ 900, 440 } },
		{ BLUE, Vector2{ 975, 690 } },
		{ CYAN, Vector2{ 1220, 690 } },
		{ PURPLE, Vector2{ 1300, 440 } }
	};
	
	static Circle2D circles[] = { 
		{ BLUE, Vector2(307,210), 140 },
		{ RED, Vector2(491,420), 200 },
		{ GREEN, Vector2(736,250), 170 },
		{ ORANGE, Vector2(910,356), 120 },
		{ WHITE, Vector2(1020,512), 100 },
	};
}