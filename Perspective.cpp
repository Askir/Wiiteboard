#include "stdafx.h"
#include "Perspective.h"

#include <cmath>
#define PI 3.14159265
#define FOV 30

Perspective::Perspective() 
: fov(FOV*PI / 360)
, view(new Vector(.5, .5, 0.5 / tan(fov), true))
{

}

Perspective::Perspective(Coord ca, Coord cb, Coord cc, Coord cd)
	: fov(FOV*PI/360)
	, view(new Vector(.5, .5, 0.5/tan(fov), true))
{
	Vector* a = new Vector(ca);
	Vector* b = new Vector(cb);
	Vector* c = new Vector(cc);
	Vector* d = new Vector(cd);

	calibrate(a, b, c, d);
}


void Perspective::calibrate(Coord ca, Coord cb, Coord cc, Coord cd)
{
	Vector* a = new Vector(ca);
	Vector* b = new Vector(cb);
	Vector* c = new Vector(cc);
	Vector* d = new Vector(cd);

	calibrate(a, b, c, d);

	delete a, b, c, d;
}

void Perspective::calibrate(Vector* a, Vector* b, Vector* c, Vector* d)
{
	//Calculate middle Coords
	Vector* center;
	center = calcCenter(a, b, c, d);

	Vector* va = view->construct(a);
	Vector* vb = view->construct(b);
	Vector* vc = view->construct(c);
	Vector* vd = view->construct(d);
	Vector* vz = view->construct(center);

	double cosA, cosB, cosC, cosD;
	cosA = vz->cos(va);
	cosB = vz->cos(vb);
	cosC = vz->cos(vc);
	cosD = vz->cos(vd);

	double sinA, sinB, sinC, sinD;
	sinA = sin(acos(cosA));
	sinB = sin(acos(cosB));
	sinC = sin(-1 * acos(cosC));
	sinD = sin(-1 * acos(cosD));

	double distA, distB, distC, distD;
	distA = -2 * sinC / (cosC*sinA - cosA*sinC);
	distC = 2 * sinA / (cosC*sinA - cosA*sinC);
	distB = -2 * sinD / (cosD*sinB - cosB*sinD);
	distD = 2 * sinB / (cosD*sinB - cosB*sinD);

	va->normalize();
	vb->normalize();
	vc->normalize();
	vd->normalize();

	va->scale(distA);
	vb->scale(distB);
	vc->scale(distC);
	vd->scale(distD);

	Vector* pa = view->sum(va);
	Vector* pb = view->sum(vb);
	Vector* pc = view->sum(vc);
	Vector* pd = view->sum(vd);

	//Construct transformation Matrix
	Matrix* transform = new Matrix(pa);

	//Construct row Vectors
	Vector *ax, *ay, *az, *helper;
	ax = pa->construct(pd);
	helper = pa->construct(pb);
	az = ax->cross(helper);
	ay = az->cross(ax);
	ax->normalize();
	ay->normalize();
	az->normalize();
	//Construct rotation Matrix
	Matrix* rotate = new Matrix(ax, ay, az);

	Matrix* transRotate = rotate->combine(transform);
	Vector* tb = transRotate->multiply(pb);

	double displace = -1*tb->v[0]/tb->v[1];
	double shearData[][4] = { { 1, displace, 0, 0 },
							{ 0, 1, 0, 0 },
							{ 0, 0, 1, 0 },
							{ 0, 0, 0, 1 } };
	Matrix* shear = new Matrix(shearData);

	Matrix* rectangify = shear->combine(transRotate);
	Vector* rectC = rectangify->multiply(pc);
	double resizeData[][4] = { { 1/rectC->v[0], 0, 0, 0 },
							{ 0, 1/rectC->v[1], 0, 0 },
							{ 0, 0, 1, 0 },
							{ 0, 0, 0, 1 } };
	Matrix* resize = new Matrix(resizeData);

	morph = resize->combine(rectangify);


	delete va, vb, vc, vd, vz, pa, pb, pc, pd, ax, ay, az, helper, tb, rectC;
	delete transform, rotate, shear, transRotate, rectangify, resize;
	
}

Coord* Perspective::transform(Coord pen)
{
	Vector* p = new Vector(pen);
	Vector* t = view->construct(p);

	Vector* support = morph->multiply(view);
	Vector* trace = morph->multiply(t);

	double zero = -1 * support->v[2] / trace->v[2];
	trace->scale(zero);
	Vector* morphedPen = support->sum(trace);

	double x, y;
	x = morphedPen->v[0];
	if (x < 0) { x = 0; }
	else if (x > 1) { x = 1; }
	y = morphedPen->v[1];
	if (y < 0) { y = 0; }
	else if (y > 1) { y = 1; }

	delete p, t, support, trace, morphedPen;
	return new Coord(x, y);
}


Vector* Perspective::calcCenter(Vector* a, Vector* b, Vector* c, Vector* d)
{
	Vector* dtnA = a->construct(c);
	Vector* dtnB = b->construct(d);

	double meetAt;
	meetAt = dtnB->v[0] * (a->v[1] - b->v[1]);
	meetAt -= dtnB->v[1] * (a->v[0] - b->v[0]);
	meetAt /= dtnB->v[1] * dtnA->v[0] - dtnB->v[0] * dtnA->v[1];

	dtnA->scale(meetAt);
	Vector* center = a->sum(dtnA);

	delete dtnA, dtnB;
	return center;
}

Perspective::~Perspective(){

}