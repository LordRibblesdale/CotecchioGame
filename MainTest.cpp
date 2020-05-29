#include <iostream>
#include "Matrix/Matrix.h"
#include "Matrix/SquareMatrix.h"
#include "Vector/Float4.h"
#include "Matrix/StandardMatrix.h"

using namespace std;

int main() {
   //Matrix matrix0(3, 5, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
   //Matrix matrix2(3, 5, {-1, 2, -3, 4, -5, 6, -7, 8, -9, 10, -11, 12, -13, 14, -15});
   //SquareMatrix matrix1(4, {3, -1, 0, -1, -1, 3, -1, 0, 0, -1 , 3, -1, -1, 0, -1, 3});
   //SquareMatrix matrix3(std::move(SquareMatrix::calculateInverse(matrix1)));

   //Matrix vector0(4, 1, {1, 6, 5, 4});
   //Matrix vector1(1, 4, {0, 5, 10, 100});
   //Matrix vector1(1, 4, {0, 5, 10, 100, 96});

   Matrix matrixNew(3, 4, {1});
   Matrix matrixNew2(2, 1, {1, 6, 4});

   cout << matrixNew.toString() << endl;
   cout << matrixNew2.toString() << endl;

   //cout << matrix0.toString() << endl;
   //cout << matrix2.toString() << endl;

   //cout << (matrix1 * matrix3).toString() << endl;

   //cout << vector0.toString() << endl;
   //cout << vector1.toString() << endl;
   //cout << (vector0 * vector1).toString() << endl;

   //cout << (matrix0.multiply_vector(vector1)).to_string() << endl;

   //cout << (matrix0 + matrix2).to_string() << endl << endl;
   //cout << (matrix0 - matrix2).to_string() << endl << endl;

   //cout << matrix1.toString() << endl << endl;
   //cout << matrix3.toString() << endl << endl;

   //cout << (matrix1*matrix3).to_string() << endl << endl;

   //cout << (matrix2.multiply_vector(vector1)).to_string() << endl << endl;

   //cout << (matrix1.multiplyVector(vector0)).to_string() << endl << endl;
   //cout << (Matrix::createSubmatrix(matrix1, 3, 3)).toString() << endl << endl;

   //Float4 point(0.7, 0.5, 0, 1);
   //Float4 direction(0, 0, 1, 1);
   //Float3 vector2(axisZRotateVertex3(point.getProjectionQuotient(), degree2Radiants(30)));
   //Float4 vector(quaternionAxisRotateVertex4(point, direction, degree2Radiants(30)));
   //std::cout << vector2.toString() << std::endl;
   //std::cout << vector.toString();

   return 0;
}