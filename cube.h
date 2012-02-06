#ifndef CUBE_H
#define CUBE_H

#include"matrix.h"
#include"point.h"

class Cube {
	Matrix* mat;
	public:
		Point* data;

		Cube();

		void print();
		void printMat();

		void apply(Matrix* m);
		void reset();
		void translateX(float x);
		void translateY(float y);
		void translateZ(float z);
		void rotateH(float h);
		void rotateP(float p);
		void rotateR(float r);
		void scale(float s);
		void translateXYZ(float x, float y, float z);
		void translateXYZ(Vector3 v);
		void rotateHPR(float h, float p, float r);
		void transformXYZHPRS(float x, float y, float z, float h, float p, float r, float s);
};

#endif
