//
//  TextureMapping.h
//
//  Created by Shenyao Ke on 3/6/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once
#ifndef __TextureMapping__
#define __TextureMapping__
#include "Core/rtdef.h"
#include "Math/CGVector.h"

class TextureMapping
{
protected:
	Point3D c;
	Vector3D nx, ny, nz;
public:

	TextureMapping();
	TextureMapping(const Point3D& center);
	TextureMapping(const Point3D& center, const Vector3D& view, const Vector3D& upVec);
	TextureMapping(const Point3D& center, const Vector3D& xDir, const Vector3D& yDir, const Vector3D& zDir);
	virtual ~TextureMapping();

	virtual void printInfo() const;
	virtual void setPos(const Point3D& pos);
	virtual void setDir(const Vector3D& view, const Vector3D& upVec);
	virtual Point3D posToUV(const Point3D& pos) const;
	virtual void mapToUV(const DifferentialGeometry *queryPoint) const;
	virtual void getUVDir(const DifferentialGeometry *queryPoint) const;
	//virtual Vector2D mapToXY(const double& x, const double& y) const;
};
/************************************************************************/
/* Spherical Mapping                                                    */
/************************************************************************/
class SphericalMapping2D :public TextureMapping
{
public:
	SphericalMapping2D();
	SphericalMapping2D(const Point3D& center);
	SphericalMapping2D(const Point3D& center, const Vector3D& view, const Vector3D& upVec);
	SphericalMapping2D(const Point3D& center, const Vector3D& xDir, const Vector3D& yDir, const Vector3D& zDir);
	~SphericalMapping2D();

	Point3D posToUV(const Point3D& pos) const;
	//void mapToUV(const DifferentialGeometry *queryPoint) const;
	void getUVDir(const DifferentialGeometry *queryPoint) const;
protected:
private:
};
/************************************************************************/
/* Planar Mapping                                                       */
/************************************************************************/
class PlanarMapping2D :public TextureMapping
{
	double uSize = 1, vSize = 1;
public:
	PlanarMapping2D();
	PlanarMapping2D(const double& us, const double& vs);
	PlanarMapping2D(const Point3D& center);
	PlanarMapping2D(const Point3D& center, const double& us, const double& vs);
	PlanarMapping2D(const Point3D& center, const Vector3D& view, const Vector3D& upVec);
	PlanarMapping2D(const Point3D& center, const Vector3D& xDir, const Vector3D& yDir, const Vector3D& zDir);
	~PlanarMapping2D();

	void setSize(const double& us, const double& vs);
	void mapToUV(const DifferentialGeometry *queryPoint) const;
protected:

private:
};
/************************************************************************/
/* Planar Mapping 3D                                                    */
/************************************************************************/
class PlanarMapping3D :public TextureMapping
{
	double uSize = 1, vSize = 1, dist = 1;
public:
	PlanarMapping3D();
	PlanarMapping3D(const double& us, const double& vs, const double& d);
	PlanarMapping3D(const Point3D& center);
	PlanarMapping3D(const Point3D& center, const double& us, const double& vs, const double& d);
	PlanarMapping3D(const Point3D& center, const Vector3D& view, const Vector3D& upVec);
	PlanarMapping3D(const Point3D& center, const Vector3D& xDir, const Vector3D& yDir, const Vector3D& zDir);
	~PlanarMapping3D();

	void setSize(const double& us, const double& vs, const double& d);
	void mapToUV(const DifferentialGeometry *queryPoint) const;
protected:

private:
};
#endif