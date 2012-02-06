#ifndef MATRIX_H
#define MATRIX_H

#include"vector3.h"

class Matrix {
	protected:
		int width, height;
	public:
		float data[4][4];

		Matrix();

		void print();

		void add(Matrix* m);

		void apply(Vector3* v);
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
