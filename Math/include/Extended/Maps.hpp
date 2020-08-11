
/// =============================================================================
///	Mapping Functions
///
/// LICENSE: BSD 3-Clause License
/// 
/// Copyright (c) 2017 Sentek Systems, LLC.
///
/// This software is provided 'as-is', without any express or implied
/// warranty.  In no event will the authors be held liable for any damages
/// arising from the use of this software.
/// 
/// Permission is granted to anyone to use this software for any purpose,
/// including commercial applications, and to alter it and redistribute it
/// freely, subject to the following restrictions:
/// 
/// 1. The origin of this software must not be misrepresented; you must not
///    claim that you wrote the original software. If you use this software
///    in a product, an acknowledgment in the product documentation would be
///    appreciated but is not required.
/// 2. Altered source versions must be plainly marked as such, and must not be
///    misrepresented as being the original software.
/// 3. This notice may not be removed or altered from any source distribution.
/// =============================================================================

/// See ../../License.txt for additional license info.

#pragma once

#include "../Core/Vector2d.hpp"
#include "../Core/Vector4d.hpp"

namespace HANDYMATH_NS::Maps
{
/// NM = Normalized Mercator Units X, Y e [-1, 1].
/// T000 = Position on Tile 0 0 0. X, Y e [ 0, 1], Y points down.

	FORCEINLINE Vector2d NMToT000(Vector2d const & pointNM)  { return pointNM * Vector2d(0.5, -0.5) + 0.5; }
	FORCEINLINE Vector2d T000ToNM(Vector2d const & pointT000) { return (pointT000 - 0.5) * Vector2d(2.0, -2.0); }

	// Lat Lon are in RADIANS
	FORCEINLINE Vector2d NMToLatLon(Vector2d const & NMCoords)
	{
		constexpr double PI = Handy::Pi<double>;
		return Vector2d(2.0 * (atan(exp(NMCoords.Y * PI)) - PI / 4.0), PI * NMCoords.X);
	}

	FORCEINLINE Vector2d T000ToLatLon(Vector2d const & pointT000)
	{
		constexpr double pi    = Handy::Pi<double>;
		constexpr double twoPi = Handy::TwoPi<double>;
		return Vector2d(2.0 * (atan(exp(-twoPi * pointT000.Y + pi)) - Handy::PiQuarter<double>), twoPi * pointT000.X - pi);
	}

	// Lat Lon are in RADIANS
	FORCEINLINE Vector2d LatLonToNM(Vector2d const & LatLonRadians)
	{
		return Vector2d(LatLonRadians.Y, log(tan(LatLonRadians.X * 0.5 + Handy::PiQuarter<double>))) * Handy::RPi<double>;
	}

	// Lat Lon are in RADIANS
	FORCEINLINE Vector2d LatLonToT000(Vector2d const & LatLonRadians)
	{
		return Vector2d(LatLonRadians.Y, -log(tan(LatLonRadians.X * 0.5 + Handy::PiQuarter<double>))) * Handy::RTwoPi<double> +0.5;
	}

	// Convert a distance in meters to normalized mercator units at the given point on Earth (only the y-coord matters).
	// Be careful with this - it is not perfect and gets especially bad over long distances.
	FORCEINLINE double MetersToNMUnits(double Meters, double yPos_NM)
	{
		constexpr double PI = Handy::Pi<double>;
		constexpr double C = 40075017.0; // meters (Wikipedia)

		double lat = 2.0 * (atan(exp(yPos_NM * PI)) - PI / 4.0);
		double NMUnitsPerMeter = 2.0 / (C * cos(lat));
		return Meters * NMUnitsPerMeter;
	}

	//Convert a distance in meters to pixels at the given zoom level and point on Earth (only the y-coord matters).
	//Be careful with this - it is not perfect and gets especially bad over long distances.
	FORCEINLINE double MetersToPixels(double Meters, double yPos_NM, double MapZoom)
	{
		constexpr double PI = Handy::Pi<double>;
		constexpr double C = 40075017.0; //meters (Wikipedia)

		double lat = 2.0 * (atan(exp(yPos_NM * PI)) - PI / 4.0);
		double PixelsPerMeter = pow(2.0, MapZoom + 8.0) / (C * cos(lat));
		return Meters * PixelsPerMeter;
	}

	//Convert a distance in pixels at a given zoom level to NM units at the given point on Earth (only the y-coord matters).
	//Be careful with this - it is not perfect and gets especially bad over long distances.
	FORCEINLINE double PixelsToNMUnits(double Pixels, double yPos_NM, double MapZoom)
	{
		constexpr double PI = Handy::Pi<double>;
		constexpr double C = 40075017.0; //meters (Wikipedia)

		double lat = 2.0*(atan(exp(yPos_NM*PI)) - PI / 4.0);
		double NMUnitsPerMeter = 2.0 / (C*cos(lat));
		double PixelsPerMeter = pow(2.0, MapZoom + 8.0) / (C*cos(lat));
		double NMUnitsPerPixel = NMUnitsPerMeter / PixelsPerMeter;
		return Pixels * NMUnitsPerPixel;
	}

	//Get the Normalized-Mercator coordinates of the center of pixel (row, col) in the given tile
	FORCEINLINE Vector2d TilePixelToNM(int32_t TileX, int32_t TileY, int32_t PyramidLevel, int Row, int Col, int32_t tileWidth)
	{
		int32_t tilesOnThisLevel = (int32_t)(1U << PyramidLevel); //In each dimension
		double xNM = (double(TileX) + (double(Col) + 0.5) / double(tileWidth))*2.0 / (double(tilesOnThisLevel)) - 1.0;
		double yNM = 1.0 - (double(TileY) + (double(Row) + 0.5) / double(tileWidth))*2.0 / double(tilesOnThisLevel);
		return Vector2d(xNM, yNM);
	}

	//Get the pixel coordinates of the given Normalized-Mercator position in the given tile. Returned in form <col, row>.
	FORCEINLINE Vector2d NMToTilePixel(int32_t TileX, int32_t TileY, int32_t PyramidLevel, Vector2d const & Position_NM, int32_t tileWidth)
	{
		int32_t tilesOnThisLevel = (int32_t)(1U << PyramidLevel); //In each dimension
		double Col = ((1.0 + Position_NM.X)*double(tilesOnThisLevel) / 2.0 - double(TileX))*double(tileWidth) - 0.5;
		double Row = ((1.0 - Position_NM.Y)*double(tilesOnThisLevel) / 2.0 - double(TileY))*double(tileWidth) - 0.5;
		return Vector2d(Col, Row);
	}

	//Get the pixel containing the given Normalized-Mercator position in the given tile. Returned in form <col, row>. This version
	//saturates each coordinate to [0, tileWidth-1]
	FORCEINLINE std::tuple<int, int> NMToTilePixel_int(int32_t TileX, int32_t TileY, int32_t PyramidLevel, Vector2d const & Position_NM, int32_t tileWidth)
	{
		int32_t tilesOnThisLevel = (int32_t)(1U << PyramidLevel); //In each dimension
		double Col = ((1.0 + Position_NM.X)*double(tilesOnThisLevel) / 2.0 - double(TileX))*double(tileWidth) - 0.5;
		double Row = ((1.0 - Position_NM.Y)*double(tilesOnThisLevel) / 2.0 - double(TileY))*double(tileWidth) - 0.5;
		Col = static_cast<int32_t>(std::floor(Handy::FastMin(Handy::FastMax(Col, 0.0), double(tileWidth - 1))));
		Row = static_cast<int32_t>(std::floor(Handy::FastMin(Handy::FastMax(Row, 0.0), double(tileWidth - 1))));
		return std::make_tuple(int(Col), int(Row));
	}

	//Get <tileCol, tileRow> for the tile on the given pyramid level containing the given point in T000 coordinates
	FORCEINLINE Vector2i GetCoordsOfTileContainingPointT000(Vector2d const & PointT000, int32_t PyramidLevel)
	{
		Vector2d pointNM = T000ToNM(PointT000);

		int32_t tilesOnThisLevel = (int32_t)(1U << PyramidLevel); //In each dimension
		int32_t tileX = static_cast<int32_t>(std::floor((pointNM.X + 1.0)*tilesOnThisLevel / 2.0));
		int32_t tileY = static_cast<int32_t>(std::floor((1.0 - pointNM.Y)*tilesOnThisLevel / 2.0));
		tileX = Handy::FastMax((int32_t)0, Handy::FastMin(tilesOnThisLevel - 1, tileX));
		tileY = Handy::FastMax((int32_t)0, Handy::FastMin(tilesOnThisLevel - 1, tileY));
		return Vector2i(tileX, tileY);
	}


	//Get <tileCol, tileRow> for the tile on the given pyramid level containing the given point in Normalized Mercator coordinates
	FORCEINLINE Vector2i GetCoordsOfTileContainingPointNM(Vector2d const & PointNM, int32_t PyramidLevel)
	{
		int32_t tilesOnThisLevel = (int32_t)(1U << PyramidLevel); //In each dimension
		int32_t tileX = static_cast<int32_t>(std::floor((PointNM.X + 1.0)*tilesOnThisLevel / 2.0));
		int32_t tileY = static_cast<int32_t>(std::floor((1.0 - PointNM.Y)*tilesOnThisLevel / 2.0));
		tileX = Handy::FastMax((int32_t)0, Handy::FastMin(tilesOnThisLevel - 1, tileX));
		tileY = Handy::FastMax((int32_t)0, Handy::FastMin(tilesOnThisLevel - 1, tileY));
		return  Vector2i(tileX, tileY);
	}

	//Get <tileCol, tileRow> for the tile on the given pyramid level containing the given point in T000 coordinates
	FORCEINLINE Vector2d GetCoordsFloatOfTileContainingPointT000(Vector2d const & PointT000, int32_t pyramidLevel)
	{
		Vector2d const & PointNM = T000ToNM(PointT000);

		double tilesOnThisLevel = (double)(1U << pyramidLevel); //In each dimension
		Vector2d tileXY = (PointNM * Vector2d(1.0, -1.0f) + 1.0) * tilesOnThisLevel / 2.0;

		return tileXY;
	}

	//Get <tileCol, tileRow> for the tile on the given pyramid level containing the given point in Normalized Mercator coordinates
	FORCEINLINE Vector2d GetCoordsFloatOfTileContainingPointNM(Vector2d const & PointNM, int32_t pyramidLevel)
	{
		double tilesOnThisLevel = (double)(1U << pyramidLevel); //In each dimension
		Vector2d tileXY = (PointNM * Vector2d(1.0, -1.0f) + 1.0) * tilesOnThisLevel / 2.0;

		return tileXY;
	}

	FORCEINLINE Vector2d T000ToTile(Vector2d const & pointT000, int32_t pyramidLevel)
	{
		double tilesOnThisLevel = (double)(1U << pyramidLevel); //In each dimension
		return pointT000 * tilesOnThisLevel;
	}

	FORCEINLINE Vector2d TileToT000(Vector2i const & tileCoord, int32_t pyramidLevel)
	{
		double tilesOnThisLevel = (double)(1U << pyramidLevel); //In each dimension
		return Vector2d(tileCoord.X, tileCoord.Y) / tilesOnThisLevel;
	}

	FORCEINLINE double ZLevelSize (int32_t z) { return (double)(1 << z); }
	FORCEINLINE double ZLevelSizeR(int32_t z) { return 1.0 / ZLevelSize(z); }


	FORCEINLINE AABB2d XYZToT000BoundingBox(Vector3i tileXYZ)
	{
		double rzls = ZLevelSizeR(tileXYZ.Z);
		Vector2d minV = Vector2d((double)tileXYZ.X, (double)tileXYZ.Y);
		return AABB2d(minV * rzls, (minV + 1.0) * rzls);
	}

	FORCEINLINE static Vector2d T000ToZ01(Vector2d const & pT000, Vector3i const & tileXYZ)
	{ 
		return (pT000 - XYZToT000BoundingBox(tileXYZ).Min) * ZLevelSize(tileXYZ.Z);
	}

	FORCEINLINE static AABB2d T000ToZ01(AABB2d const & bbT000, Vector3i const & tileXYZ)
	{
		return AABB2d(T000ToZ01(bbT000.Min, tileXYZ), T000ToZ01(bbT000.Max, tileXYZ));
	}


	/// Get the XYZ tile coordinates of the upper left corner of tileXYZ one Z level deeper
	FORCEINLINE Vector3i GetCornerXYZp1_TL(Vector3i const & tileXYZ)             { return (tileXYZ.XY() * 2).XYZ(tileXYZ.Z + 1); }
	FORCEINLINE Vector3i GetCornerXYZm1   (Vector3i const & tileXYZ)             { return (tileXYZ.XY() / 2).XYZ(tileXYZ.Z - 1); }
	FORCEINLINE Vector3i GetCornerXYZpN_TL(Vector3i const & tileXYZ, uint32_t n) { return (tileXYZ.XY() * (1 << n)).XYZ(tileXYZ.Z + n); }
	FORCEINLINE Vector3i GetCornerXYZmN   (Vector3i const & tileXYZ, uint32_t n) { return (tileXYZ.XY() / (1 << n)).XYZ(tileXYZ.Z - n); }


	FORCEINLINE bool XYZIntersectsXYZ(Vector3i tileXYZ1, Vector3i tileXYZ2)
	{
		if      (tileXYZ1.Z < tileXYZ2.Z) /* if 1 is a bigger tile than 2 */ tileXYZ2 = GetCornerXYZmN(tileXYZ2, (uint32_t)(tileXYZ2.Z - tileXYZ1.Z));
		else if (tileXYZ2.Z < tileXYZ1.Z) /* if 2 is a bigger tile than 1 */ tileXYZ1 = GetCornerXYZmN(tileXYZ1, (uint32_t)(tileXYZ1.Z - tileXYZ2.Z));

		return tileXYZ1 == tileXYZ2;
	}


	FORCEINLINE double ScreenPixelLengthT000(double Zoom, int32_t tileWidth = 256)
	{
		return 1.0 / (pow(2.0, Zoom) * ((double)tileWidth));
	}
	
	//Get NM coords of upper-left corner of upper-left pixel of a given tile
	FORCEINLINE Vector2d GetNMCoordsOfULCornerOfTile(int32_t TileX, int32_t TileY, int32_t PyramidLevel)
	{
		int32_t tilesOnThisLevel = (int32_t)(1U << PyramidLevel); //In each dimension
		double xNM = ((double)TileX)*2.0 / ((double)tilesOnThisLevel) - 1.0;
		double yNM = 1.0 - ((double)TileY)*2.0 / ((double)tilesOnThisLevel);
		return Vector2d(xNM, yNM);
	}

	FORCEINLINE Vector2d GetT000CoordsOfULCornerOfTile(int32_t TileX, int32_t TileY, int32_t PyramidLevel)
	{
		int32_t tilesOnThisLevel = (int32_t)(1U << PyramidLevel); //In each dimension
		double xNM = ((double)TileX)*2.0 / ((double)tilesOnThisLevel) - 1.0;
		double yNM = 1.0 - ((double)TileY)*2.0 / ((double)tilesOnThisLevel);
		return NMToT000(Vector2d(xNM, yNM));
	}

	//Get NM coords of lower-right corner of lower-right pixel of a given tile
	FORCEINLINE Vector2d GetNMCoordsOfLRCornerOfTile(int32_t TileX, int32_t TileY, int32_t PyramidLevel)
	{
		int32_t tilesOnThisLevel = (int32_t)(1U << PyramidLevel); //In each dimension
		double xNM = ((double)(TileX + 1))*2.0 / ((double)tilesOnThisLevel) - 1.0;
		double yNM = 1.0 - ((double)(TileY + 1))*2.0 / ((double)tilesOnThisLevel);
		return Vector2d(xNM, yNM);
	}

	//Get NM coords of lower-right corner of lower-right pixel of a given tile
	FORCEINLINE Vector2d GetT000CoordsOfLRCornerOfTile(int32_t TileX, int32_t TileY, int32_t PyramidLevel)
	{
		int32_t tilesOnThisLevel = (int32_t)(1U << PyramidLevel); //In each dimension
		double xNM = ((double)(TileX + 1))*2.0 / ((double)tilesOnThisLevel) - 1.0;
		double yNM = 1.0 - ((double)(TileY + 1))*2.0 / ((double)tilesOnThisLevel);
		return NMToT000(Vector2d(xNM, yNM));
	}
}
