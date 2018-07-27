#pragma once

#include <string>
#include <assert.h>
#include <vector>

#define DEGREETORADIAN 0.0174532925199432957f
#define RADIANTODEGREE 57.295779513082320876f

#ifndef M_PI
#define M_PI		3.14159265358979323846f
#define M_PI_HALF	1.57079632679489661923f
#define M_PI_QUART	0.785398163397448309616f
#endif // !M_PI

	typedef std::string string;

	typedef int int32;
	typedef unsigned int uint32;

	typedef float float32;
	typedef double float64;

	struct sFloat2
	{
		float32 x, y;

		inline sFloat2() : x(0), y(0) {}
		inline sFloat2(float32 px, float32 py) : x(px), y(py) {}
		inline sFloat2(const sFloat2& p) : x(p.x), y(p.y) {}

		inline void operator() (float32 px, float32 py) { x = px; y = py; }
		inline void operator() (const sFloat2& p) { x = p.x; y = p.y; }

		void operator -= (const sFloat2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
		}

		void operator += (const sFloat2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
		}

		bool operator == (const sFloat2& rhs)
		{
			return (x == rhs.x && y == rhs.y);
		}

		sFloat2 operator + (const sFloat2& rhs)
		{
			return { x + rhs.x, y + rhs.y };
		}

		sFloat2 operator - (const sFloat2& rhs)
		{
			return { x - rhs.x, y - rhs.y };
		}

		bool operator != (const sFloat2& rhs)
		{
			return (x != rhs.x || y != rhs.y);
		}
	};

	struct sFloat3
	{
		float32 x, y, z;

		inline sFloat3() : x(0), y(0), z(0) {}
		inline sFloat3(float32 px, float32 py, float32 pz) : x(px), y(py), z(pz) {}
		inline sFloat3(const sFloat3& p) : x(p.x), y(p.y), z(p.z) {}

		inline void operator() (float32 px, float32 py, float32 pz) { x = px; y = py; z = pz;}
		inline void operator() (const sFloat3& p) { x = p.x; y = p.y; z = p.z; }

		void operator -= (const sFloat3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
		}

		void operator += (const sFloat3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
		}

		bool operator == (const sFloat3& rhs)
		{
			return (
				x == rhs.x &&
				y == rhs.y &&
				z == rhs.z );
		}

		sFloat3 operator + (const sFloat3& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z };
		}

		sFloat3 operator - (const sFloat3& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z };
		}
	};

	struct sFloat4
	{
		float32 x, y, z, w;

		inline sFloat4() : x(0), y(0), z(0), w(0) {}
		inline sFloat4(float32 px, float32 py, float32 pz, float32 pw) : x(px), y(py), z(pz), w(pw) {}
		inline sFloat4(const sFloat4& p) : x(p.x), y(p.y), z(p.z), w(p.w) {}

		inline void operator() (float32 px, float32 py, float32 pz, float32 pw) { x = px; y = py; z = pz; w = pw; }
		inline void operator() (const sFloat4& p) { x = p.x; y = p.y; z = p.z; w = p.w; }

		void operator -= (const sFloat4& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
		}

		void operator += (const sFloat4& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w -= rhs.w;
		}

		bool operator == (const sFloat4& rhs)
		{
			return (
				x == rhs.x &&
				y == rhs.y &&
				z == rhs.z &&
				w == rhs.w);
		}

		sFloat4 operator + (const sFloat4& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}

		sFloat4 operator - (const sFloat4& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
		}
	};

	struct sFloatRect
	{
		float32 x1, y1, x2, y2;

		inline sFloatRect() : x1(0), y1(0), x2(0), y2(0) {}
		inline sFloatRect(float32 px1, float32 py1, float32 px2, float32 py2)
			: x1(px1), y1(py1), x2(px2), y2(py2) {}
		inline sFloatRect(sFloat2 pPoint1, sFloat2 pPoint2)
			: x1(pPoint1.x), y1(pPoint1.y), x2(pPoint2.x), y2(pPoint2.y) {}

		void operator() (float32 px1, float32 py1, float32 px2, float32 py2)
		{
			x1 = px1;
			y1 = py1;
			x2 = px2;
			y2 = py2;
		}

		void operator() (sFloat2 rhs1, sFloat2 rhs2)
		{
			x1 = rhs1.x;
			y1 = rhs1.y;
			x2 = rhs2.x;
			y2 = rhs2.y;
		}
	};

	struct sInt2
	{
		int32 x, y;

		inline sInt2() : x(0), y(0) {}
		inline sInt2(int32 px, int32 py) : x(px), y(py) {}
		inline sInt2(const sInt2& p) : x(p.x), y(p.y) {}

		inline void operator() (int32 px, int32 py) { x = px; y = py; }
		inline void operator() (const sInt2& p) { x = p.x; y = p.y; }

		bool operator == (const sInt2& rhs)
		{
			return (x == rhs.x && y == rhs.y);
		}

		void operator -= (const sInt2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
		}

		void operator += (const sInt2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
		}


		sInt2 operator + (const sInt2& rhs)
		{
			return { x + rhs.x, y + rhs.y };
		}

		sInt2 operator - (const sInt2& rhs)
		{
			return { x - rhs.x, y - rhs.y };
		}
	};

	struct sInt4
	{
		int32 x, y, z, w;

		inline sInt4() : x(0), y(0), z(0), w(0) {}
		inline sInt4(int32 px, int32 py, int32 pz, int32 pw) : x(px), y(py), z(pz), w(pw) {}
		inline sInt4(const sInt4& p) : x(p.x), y(p.y), z(p.z), w(p.w) {}

		inline void operator() (int32 px, int32 py, int32 pz, int32 pw) { x = px; y = py; z = pz; w = pw; }
		inline void operator() (const sInt4& p) { x = p.x; y = p.y; z = p.z; w = p.w; }

		bool operator == (const sInt4& rhs)
		{
			return (x == rhs.x && 
					y == rhs.y &&
					z == rhs.z &&
					w == rhs.w);
		}

		void operator -= (const sInt4& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
		}

		void operator += (const sInt4& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w -= rhs.w;
		}


		sInt4 operator + (const sInt4& rhs)
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}

		sInt4 operator - (const sInt4& rhs)
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
		}
	};

	struct sColour
	{
		float32 r, g, b, a;

		inline sColour() : r(0), g(0), b(0), a(0) {}
		inline sColour(float32 R, float32 G, float32 B, float32 A) : r(R), g(G), b(B), a(A) {}
		inline void operator () (float32 R, float32 G, float32 B, float32 A) { r = R; g = G; b = B; a = A; }

		void operator -= (const sColour& rhs)
		{
			r -= rhs.r;
			g -= rhs.g;
			b -= rhs.b;
			a -= rhs.a;
		}

		void operator += (const sColour& rhs)
		{
			r += rhs.r;
			g += rhs.g;
			b += rhs.b;
			a -= rhs.a;
		}

		bool operator == (const sColour& rhs)
		{
			return (
				r == rhs.r &&
				g == rhs.g &&
				b == rhs.b &&
				a == rhs.a);
		}

		sColour operator + (const sColour& rhs)
		{
			return { r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a };
		}

		sColour operator - (const sColour& rhs)
		{
			return { r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a };
		}

	};