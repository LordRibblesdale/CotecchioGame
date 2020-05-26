#ifndef MATRIX_H
#define MATRIX_H

#include "FloatArray.h"
#include "../Vector/FloatVector.h"

class Matrix {
protected:
   std::unique_ptr<FloatArray> data_;

public:
   Matrix(unsigned int rows, unsigned int columns, const std::initializer_list<float>& data);
   Matrix(unsigned int rows, unsigned int columns, float* data);
   Matrix(const Matrix& matrix);
   Matrix(Matrix&& matrix);
   ~Matrix();

   Matrix& operator=(const Matrix& matrix);
   Matrix& operator=(Matrix&& matrix);
   Matrix operator+(const Matrix& matrix);
   void operator+=(const Matrix& matrix);
   Matrix operator-(const Matrix& matrix);
   void operator-=(const Matrix& matrix);
   //TODO complete operator functions
   //Matrix& operator+(Matrix&& matrix);
   Matrix operator*(const float& scalar);
   void operator*=(const float& scalar);
   Matrix operator*(const Matrix& matrix);

   static Matrix transpose(const Matrix& matrix);

   static Matrix createSubmatrix(const Matrix& matrix, const unsigned int& rowIndex, const unsigned int& columnIndex);

   FloatVector multiplyVector(const FloatVector& vector) const;

   inline const std::unique_ptr<FloatArray>& getData() const {
      return data_;
   }

   inline float* getArray() const {
      return data_->getArray().get();
   }

   inline unsigned int getRows() const {
      return data_->getRows();
   }

   inline unsigned int getColumns() const {
      return data_->getColumns();
   }

   void deleteMatrix();

   std::string toString() const;
};

#endif //MATRIX_H
