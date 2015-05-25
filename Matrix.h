#ifndef Matrix_h
#define Matrix_h

#include "Vector.h"


class Matrix {

	private:
		double Matrix::data[4][4];

	public:
		Matrix::Matrix();
		Matrix::Matrix(double data[4][4]);
		Matrix::Matrix(Vector* center);
		Matrix::Matrix(Vector* row1, Vector* row2, Vector* row3);

		Vector* Matrix::multiply(Vector* vec);
		Matrix* Matrix::combine(Matrix* mat);
};
#endif