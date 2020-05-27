#include "SquareMatrix.h"
#include "memory"
#include <iostream>

SquareMatrix::SquareMatrix(unsigned int dimension, const std::initializer_list<float>& data) : Matrix(dimension, dimension, data) {}

SquareMatrix::SquareMatrix(unsigned int dimension, float* array) : Matrix(dimension, dimension, array) {}

SquareMatrix::SquareMatrix(const SquareMatrix &matrix) : Matrix(matrix) {}

SquareMatrix::SquareMatrix(SquareMatrix &&matrix) : Matrix(std::move(matrix)) {}

SquareMatrix::SquareMatrix(const Matrix &matrix) : Matrix(matrix) {
   if (getRows() != getColumns()) {
      throw (std::exception());
   }
}

SquareMatrix::SquareMatrix(Matrix &&matrix) : Matrix(std::move(matrix)) {
   if (getRows() != getColumns()) {
      throw (std::exception());
   }
}

SquareMatrix::~SquareMatrix() {}

SquareMatrix &SquareMatrix::operator=(const SquareMatrix &matrix) {
   data_ = std::move(std::make_unique<FloatArray>(*matrix.getData()));

   return *this;
}

SquareMatrix &SquareMatrix::operator=(SquareMatrix && matrix) {
   data_ = std::move(std::make_unique<FloatArray>(*const_cast<std::unique_ptr<FloatArray>&>(matrix.getData()).release()));

   matrix.deleteMatrix();

   return *this;
}

//TODO fix repeated functions
//TODO return Matrix results and cast them
SquareMatrix SquareMatrix::operator+(const SquareMatrix &matrix) noexcept(false) {
   return SquareMatrix(std::move((Matrix) *this + matrix));

   /*
   unsigned int dim = getDimension();
   std::unique_ptr<float> newData(new float[dim * dim]);

   if (dim == matrix.getDimension()) {
      for (int i = 0; i < dim*dim; ++i) {
         newData.get()[i] = data_[i] + matrix.getData()[i];
      }
   } else {
      std::string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }

   return SquareMatrix(dim, newData.release());
    */
}

//TODO fix repeated functions
void SquareMatrix::operator+=(const SquareMatrix &matrix) noexcept(false) {
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

SquareMatrix SquareMatrix::operator-(const SquareMatrix &matrix) noexcept(false) {
   return SquareMatrix(std::move((Matrix) *this - matrix));
   /*
   std::unique_ptr<float> newData(new float[getRows() * getColumns()]);

   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      for (int i = 0; i < getRows() * getColumns(); ++i) {
         newData.get()[i] = data_->operator[](i) - matrix.getData()->operator[](i);
      }
   } else {
      string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());

   }

   return SquareMatrix(getDimension(), newData.release());
    */
}

void SquareMatrix::operator-=(const SquareMatrix &matrix) noexcept(false) {
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

SquareMatrix SquareMatrix::operator*(float scalar) {
   //TODO check if optimal (instead of rewriting the whole function)
   return SquareMatrix(std::move((Matrix) *this * scalar));
}

void SquareMatrix::operator*=(float scalar) {
   for (int i = 0; i < getRows() * getColumns(); ++i) {
      data_->operator[](i) *= scalar;
   }
}

//TODO fix repeated functions
SquareMatrix SquareMatrix::operator*(const SquareMatrix& matrix) noexcept(false) {
   return SquareMatrix(std::move((Matrix) *this * matrix));
   /*
   unsigned int dim = getDimension();

   std::unique_ptr<float> newData(new float[getRows() * matrix.getColumns()]);

   if (getDimension() == matrix.getDimension()) {
      for (unsigned int r = 0; r < dim; ++r) {
         for (unsigned int c = 0; c < dim; ++c) {
            for (unsigned int i = 0; i < dim; ++i) {
               newData.get()[r * dim + c] += data_->operator[](r * dim + i) * matrix.getData()->operator[](i * dim + c);
            }
         }
      }
   } else {
      string s = "Exception MATRIX_PRODUCT: dimensions do not correspond. ";
      s.append("(").append(std::to_string(getColumns())).append(", ").append(std::to_string(matrix.getRows())).append(")\n");

      throw ExceptionNotifier(s.c_str());
   }

   return SquareMatrix(dim, newData.release());
    */
}


void SquareMatrix::transpose() {
   SquareMatrix transposed(std::move(Matrix::transpose(*this)));

   //TODO fix "for" calling (more security in modifying matrix values)
   for (int i = 0; i < getDimension() * getDimension(); ++i) {
      getArray()[i] = transposed.getArray()[i];
   }
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

   //TODO optimise here
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
      default: ;
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

         //TODO change calculation through first row
         //TODO improve performance for callings and power calculation
         for (int i = 0; i < matrix.getColumns(); ++i) {
            determinant += matrix.getArray()[i] * (i % 2 == 0 ? 1 : -1) * calculateDeterminant(
                    createSubmatrix(matrix, 0, i));
         }
   }

   return determinant;
}

float SquareMatrix::calculateCofactor(const SquareMatrix& matrix, unsigned int rowIndex, unsigned int columnIndex) {
   return calculateDeterminant(createSubmatrix(matrix, rowIndex, columnIndex));
}

void SquareMatrix::invert() {
   SquareMatrix inverse(std::move(SquareMatrix::calculateInverse(*this)));

   //TODO fix "for" calling (more security in modifying matrix values)
   for (int i = 0; i < getRows() * getColumns(); ++i) {
      getArray()[i] = inverse.getArray()[i];
   }
}

SquareMatrix SquareMatrix::calculateInverse(const SquareMatrix &matrix) {
   unsigned int dimension = matrix.getDimension();
   float determinant = matrix.calculateDeterminant();
   std::unique_ptr<float> newData(new float[dimension*dimension]);

   if (determinant != 0) {
      float scalar = std::abs(1 / determinant);

      for (unsigned int i = 0; i < dimension; ++i) {
         for (unsigned int j = 0; j < dimension; ++j) {
            newData.get()[i*dimension + j] = scalar * ((i+j) % 2 == 0 ? 1 : -1) * calculateCofactor(matrix, j, i);
         }
      }
   }

   return SquareMatrix(dimension, newData.release());
}

//TODO create new classes Matrix2, Matrix3 and Matrix4
void SquareMatrix::scaleMatrix(float scaleX, float scaleY, float scaleZ) {
   //TODO automise here
   data_->getArray().get()[0] *= scaleX;
   data_->getArray().get()[getDimension()] *= scaleY;
   data_->getArray().get()[getDimension() * 2] *= scaleZ;
}
