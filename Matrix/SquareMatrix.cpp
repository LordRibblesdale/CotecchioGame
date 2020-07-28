#include "SquareMatrix.h"
#include "memory"
#include <iostream>

SquareMatrix::SquareMatrix(unsigned int dimension, const std::initializer_list<float>& data) : Matrix(dimension, dimension, data) {}

SquareMatrix::SquareMatrix(unsigned int dimension, float* array) : Matrix(dimension, dimension, array) {}

SquareMatrix::SquareMatrix(const SquareMatrix &matrix) : Matrix(matrix) {}

SquareMatrix::SquareMatrix(SquareMatrix &&matrix) : Matrix(std::move(matrix)) {}

SquareMatrix::SquareMatrix(const Matrix &matrix) : Matrix(matrix) {
   if (getRows() != getColumns()) {
      std::string s("Error SQUAREMATRIX_INITIALIZATION: initializing SM from Matrix with different size:");
      s.append(std::to_string(getRows())).append(" != ").append(std::to_string(matrix.getColumns()));
      throw (ExceptionNotifier(s.c_str()));
   }
}

SquareMatrix::SquareMatrix(Matrix &&matrix) : Matrix(std::move(matrix)) {
   if (getRows() != getColumns()) {
      std::string s("Error SQUAREMATRIX_INITIALIZATION: initializing SM from Matrix with different size:");
      s.append(std::to_string(getRows())).append(" != ").append(std::to_string(matrix.getColumns()));
      throw (ExceptionNotifier(s.c_str()));
   }
}

SquareMatrix::~SquareMatrix() {}

SquareMatrix& SquareMatrix::operator=(const SquareMatrix &matrix) {
   data_ = std::move(std::make_unique<FloatArray>(*matrix.data_));

   return *this;
}

SquareMatrix& SquareMatrix::operator=(SquareMatrix && matrix) {
   data_ = std::move(matrix.data_);

   return *this;
}

SquareMatrix SquareMatrix::operator+(const SquareMatrix &matrix) const {
   if (getDimension() == matrix.getDimension()) {
      return SquareMatrix(std::move(static_cast<Matrix>(*this) + matrix));
   } else {
      std::string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getDimension())).append(") ")
              .append(" != (").append(std::to_string(matrix.getDimension())).append(", ").append(std::to_string(matrix.getDimension())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }
}

void SquareMatrix::operator+=(const SquareMatrix &matrix) {
   unsigned int dim = getDimension();

   if (dim == matrix.getDimension()) {
      for (int i = 0; i < dim * dim; ++i) {
         data_->operator[](i) += matrix.getData()->operator[](i);
      }
   } else {
      std::string s = "Exception SUM_SQUARE: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }
}

SquareMatrix SquareMatrix::operator-(const SquareMatrix &matrix) const {
   if (getDimension() == matrix.getDimension()) {
      return SquareMatrix(std::move(static_cast<Matrix>(*this) - matrix));
   } else {
      std::string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getDimension())).append(") ")
              .append(" != (").append(std::to_string(matrix.getDimension())).append(", ").append(std::to_string(matrix.getDimension())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }
}

void SquareMatrix::operator-=(const SquareMatrix &matrix) {
   unsigned int dim = getDimension();

   if (dim == matrix.getDimension()) {
      for (int i = 0; i < dim * dim; ++i) {
         data_->operator[](i) -= matrix.getData()->operator[](i);
      }
   } else {
      std::string s = "Exception DIFF_SQUARE: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }
}

SquareMatrix SquareMatrix::operator*(float scalar) const {
   return SquareMatrix(std::move(static_cast<Matrix>(*this) * scalar));
}

void SquareMatrix::operator*=(float scalar) {
   for (int i = 0; i < getRows() * getColumns(); ++i) {
      data_->operator[](i) *= scalar;
   }
}

SquareMatrix SquareMatrix::operator*(const SquareMatrix& matrix) const {
   if (getDimension() == matrix.getDimension()) {
      return SquareMatrix(std::move(static_cast<Matrix>(*this) * matrix));
   } else {
      std::string s = "Exception MATRIX_PRODUCT: dimensions do not correspond. ";
      s.append("(").append(std::to_string(getColumns())).append(", ").append(std::to_string(matrix.getRows())).append(")\n");

      throw ExceptionNotifier(s.c_str());
   }
}

void SquareMatrix::operator*=(const SquareMatrix& matrix) {
   if (getDimension() == matrix.getDimension()) {
      SquareMatrix tmp(std::move(static_cast<Matrix>(*this) * matrix));

      data_ = std::move(tmp.data_);
   } else {
      std::string s = "Exception MATRIX_PRODUCT: dimensions do not correspond. ";
      s.append("(").append(std::to_string(getColumns())).append(", ").append(std::to_string(matrix.getRows())).append(")\n");

      throw ExceptionNotifier(s.c_str());
   }
}


void SquareMatrix::transpose() {
   Matrix transposed(std::move(Matrix::transpose(*this)));

   std::move(transposed.getArray(), transposed.getArray() + getDimension()*getDimension(), getArray());
}

SquareMatrix SquareMatrix::transpose(const SquareMatrix &matrix) {
   return SquareMatrix(std::move(Matrix::transpose(matrix)));
}

SquareMatrix SquareMatrix::createSubmatrix(const SquareMatrix &matrix, unsigned int rowIndex, unsigned int columnIndex) {
   return SquareMatrix(std::move(Matrix::createSubmatrix(matrix, rowIndex, columnIndex)));
}

float SquareMatrix::calculateDeterminant() const {
   return calculateDeterminant(*this);
}

float SquareMatrix::calculateDeterminant(const SquareMatrix& matrix) {
   float determinant = 0;
   const float* array = matrix.getArray();

   //TODO add 0 calculateDeterminant check
   switch (matrix.getDimension()) {
      case 1:
         determinant = array[0];
         break;
      case 2:
         determinant = array[0]*array[3] - (array[1]*array[2]);
         break;
      case 3:
         determinant = (array[0]*array[4]*array[8] + array[1]*array[5]*array[6] + array[2]*array[3]*array[7])
                 - (array[2]*array[4]*array[6] + array[0]*array[5]*array[7] + array[8]*array[3]*array[1]);
         break;
      default:
         /*
         bool calculateRowDeterminant = false;

         for (int i = 0; i < matrix.getRows(); ++i) {
            unsigned int zeroCount = 0;

            for (int j = 0; j < matrix.getColumns(); ++j) {
               if (matrix.getArray()[i*matrix.getColumns()] == 0) {
                  ++zeroCount;
               }
            }
         }
          */

         // Change calculation through first row
         // Implement LU decomposition after 5/6 dimension
         // Laplace det-> O(n!) ; LU dec-> O(n^3)
         for (int i = 0; i < matrix.getColumns(); ++i) {
            determinant += matrix.getArray()[i] * (i % 2 == 0 ? 1.0f : -1.0f) * calculateDeterminant(createSubmatrix(matrix, 0, i));
         }
   }

   return determinant;
}

float SquareMatrix::calculateCofactor(const SquareMatrix& matrix, unsigned int rowIndex, unsigned int columnIndex) {
   return calculateDeterminant(createSubmatrix(matrix, rowIndex, columnIndex));
}

void SquareMatrix::invert() {
   SquareMatrix inverse(std::move(SquareMatrix::calculateInverse(*this)));

   std::move(inverse.getArray(), inverse.getArray() + getDimension()*getDimension(), getArray());
}

SquareMatrix SquareMatrix::calculateInverse(const SquareMatrix &matrix) {
   unsigned int dimension = matrix.getDimension();
   float determinant = matrix.calculateDeterminant();
   FloatArray newData(dimension, {});

   if (determinant != 0) {
      float scalar = std::abs(1 / determinant);

      for (unsigned int i = 0; i < dimension; ++i) {
         for (unsigned int j = 0; j < dimension; ++j) {
            newData.getArray().get()[i*dimension + j] = scalar * ((i+j) % 2 == 0 ? 1 : -1) * calculateCofactor(matrix, j, i);
         }
      }
   }

   return SquareMatrix(std::move(newData));
}