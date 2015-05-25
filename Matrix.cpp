#include "stdafx.h"
#include "Matrix.h"



Matrix::Matrix()
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			data[r][c] = 0;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		data[i][i] = 1;
	}
}

Matrix::Matrix(Vector* center)
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			data[r][c] = 0;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		data[i][i] = 1;
	}
	for (int i = 0; i < 3; i++)
	{
		data[i][3] -= center->v[i];
	}
}

Matrix::Matrix(double data_[4][4])
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			data[r][c] = 0;
		}
	}

	int row, col;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			this->data[row][col] = data_[row][col];
		}
	}
}

Matrix::Matrix(Vector* row1, Vector* row2, Vector* row3)
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			data[r][c] = 0;
		}
	}

	int col;
	for (col = 0; col < 3; col++)
	{
		data[0][col] = row1->v[col];
	}
	for (col = 0; col < 3; col++)
	{
		data[1][col] = row2->v[col];
	}
	for (col = 0; col < 3; col++)
	{
		data[2][col] = row3->v[col];
	}

	data[3][3] = 1;
}


Vector* Matrix::multiply(Vector* vec)
{
	double x, y, z, w, vx, vy, vz, vw;
	vx = vec->v[0];
	vy = vec->v[1];
	vz = vec->v[2];
	vw = vec->v[3];

	x = vx*data[0][0] + vy*data[0][1] + vz*data[0][2] + vw*data[0][3];
	y = vx*data[1][0] + vy*data[1][1] + vz*data[1][2] + vw*data[1][3];
	z = vx*data[2][0] + vy*data[2][1] + vz*data[2][2] + vw*data[2][3];
	w = vx*data[3][0] + vy*data[3][1] + vz*data[3][2] + vw*data[3][3];

	return new Vector(x, y, z, w);
}

Matrix* Matrix::combine(Matrix* mat)
{
	Matrix* res = new Matrix();

	int row, col, i;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			res->data[row][col] = 0;
			for (i = 0; i < 4; i++)
			{
				res->data[row][col] += data[row][i] * mat->data[i][col];
			}
		}
	}

	return res;
}
