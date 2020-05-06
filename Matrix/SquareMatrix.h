#ifndef SQUAREMATRIX_H
#define SQUAREMATRIX_H

#include "Matrix.h"
#include "FloatArray.h"

class SquareMatrix : public Matrix {
public:
   SquareMatrix(unsigned int dimension, const std::initializer_list<float>& data);
   SquareMatrix(unsigned int dimension, float* data);
   SquareMatrix(const SquareMatrix& matrix);
   SquareMatrix(SquareMatrix&& matrix);
   ~SquareMatrix();

   void transpose();
   static SquareMatrix transpose(const SquareMatrix& matrix);

   static SquareMatrix createSubmatrix(const SquareMatrix& matrix, unsigned int rowIndex, unsigned int columnIndex);

   float calculateDeterminant() const;
   static float calculateDeterminant(const SquareMatrix& matrix);
   static float calculateCofactor(const SquareMatrix& matrix, unsigned int rowIndex, unsigned int columnIndex);

   void invert();

   void scaleMatrix(float scaleX, float scaleY, float scaleZ);

   static SquareMatrix calculateInverse(const SquareMatrix& matrix);

   inline unsigned int getDimension() const {
      return data_.getRows();
   }
};

#endif //SQUAREMATRIX_H
