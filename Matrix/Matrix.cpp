#include "Matrix.h"

Matrix::Matrix(unsigned int rows, unsigned int columns, const std::initializer_list<float>& data) {
   data_ = std::move(std::make_unique<FloatArray>(rows, columns, data));
}

Matrix::Matrix(unsigned int rows, unsigned int columns, float* data) {
   data_ = std::move(std::make_unique<FloatArray>(rows, columns, data));
}

Matrix::Matrix(FloatArray&& data) {
   data_ = std::move(std::make_unique<FloatArray>(std::move(data)));
}

Matrix::Matrix(const Matrix &matrix) {
   data_ = std::move(std::make_unique<FloatArray>(*matrix.data_));
}

Matrix::Matrix(Matrix &&matrix) {
   data_ = std::move(matrix.data_);
}

Matrix::~Matrix() {
   data_.reset();
}

Matrix &Matrix::operator=(const Matrix &matrix) {
   data_ = std::move(std::make_unique<FloatArray>(*matrix.data_));

   return *this;
}

Matrix &Matrix::operator=(Matrix && matrix) {
   data_ = std::move(matrix.data_);

   return *this;
}

Matrix Matrix::operator+(const Matrix &matrix) const {
   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      FloatArray newData(getRows(), getColumns(), {});

      for (int i = 0; i < getRows() * getColumns(); ++i) {
         newData.getArray().get()[i] = data_->operator[](i) + matrix.getData()->operator[](i);
      }

      return Matrix(std::move(newData));
   } else {
      std::string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
         .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }
}

void Matrix::operator+=(const Matrix &matrix) {
   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      for (int i = 0; i < getRows() * getColumns(); ++i) {
         data_->operator[](i) += matrix.getData()->operator[](i);
      }
   } else {
      std::string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }
}

Matrix Matrix::operator-(const Matrix &matrix) const {
   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      FloatArray newData(getRows(), getColumns(), {});

      for (int i = 0; i < getRows() * getColumns(); ++i) {
         newData.getArray().get()[i] = data_->operator[](i) - matrix.getData()->operator[](i);
      }

      return Matrix(std::move(newData));
   } else {
      std::string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }
}

void Matrix::operator-=(const Matrix &matrix) {
   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      for (int i = 0; i < getRows() * getColumns(); ++i) {
         data_->operator[](i) -= matrix.getData()->operator[](i);
      }
   } else {
      std::string s = "Exception COPY_CONSTRUCTOR: dimensions do not correspond. ";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }
}

Matrix Matrix::operator*(const float& scalar) const {
   FloatArray newData(getRows(), getColumns(), {});

   for (int i = 0; i < getRows() * getColumns(); ++i) {
      newData.getArray().get()[i] = data_->operator[](i) * scalar;
   }

   return Matrix(std::move(newData));
}

void Matrix::operator*=(const float& scalar) {
   for (int i = 0; i < getRows() * getColumns(); ++i) {
      data_->operator[](i) *= scalar;
   }
}

Matrix Matrix::operator*(const Matrix &matrix) const {
   if (getColumns() == matrix.getRows()) {
      FloatArray newData(getRows(), matrix.getColumns(), {});

      for (unsigned int r = 0; r < getRows(); ++r) {
         for (unsigned int c = 0; c < matrix.getColumns(); ++c) {
            for (unsigned int i = 0; i < getColumns(); ++i) {
               newData.getArray().get()[r * matrix.getColumns() + c] += data_->operator[](r * getColumns() + i) * matrix.getData()->operator[](i * matrix.getColumns() + c);
            }
         }
      }

      return Matrix(std::move(newData));
   } else {
      std::string s = "Exception MATRIX_PRODUCT: dimensions do not correspond. ";
      s.append("(").append(std::to_string(getColumns())).append(", ").append(std::to_string(matrix.getRows())).append(")\n");

      throw ExceptionNotifier(s.c_str());
   }
}

Matrix Matrix::transpose(const Matrix &matrix) {
   FloatArray newData(matrix.getColumns(), matrix.getRows(), {});

   for (int i = 0; i < matrix.getColumns(); ++i) {
      for (int j = 0; j < matrix.getRows(); ++j) {
         newData.getArray().get()[i* matrix.getRows() + j] = matrix.getData()->operator[](j * matrix.getColumns() + i);
      }
   }

   return Matrix(std::move(newData));
}

Matrix Matrix::createSubmatrix(const Matrix &matrix, const unsigned int& rowIndex, const unsigned int& columnIndex) {
   FloatArray newData(matrix.getRows()-1, matrix.getColumns()-1, {});

   unsigned int index = 0;
   for (int i = 0; i < matrix.getRows(); ++i) {
      if (i != rowIndex) {
         for (int j = 0; j < matrix.getColumns(); ++j) {
            if (j != columnIndex) {
               newData.getArray().get()[index++] = matrix.getArray()[i * matrix.getColumns() + j];
            }
         }
      }
   }

   return Matrix(std::move(newData));
}

FloatVector Matrix::multiplyVector(const FloatVector& vector) const {
   if (getColumns() == vector.getSize()) {
      FloatVector newData(getRows(), {});

      for (int i = 0; i < getRows(); ++i) {
         float value = 0;

         for (int j = 0; j < getColumns(); ++j) {
            value += getArray()[i * getColumns() + j] * vector.getVector().get()[j];
         }

         newData.getVector().get()[i] = value;
      }

      return std::move(newData);
   } else {
      std::string s = "Exception NO_MATCH_LENGTH: matrix and vector_ do not have correct size_. ";
      s.append("Matrix columns_: ").append(std::to_string(getColumns())).append("!= Vector size_: ")
         .append(std::to_string(vector.getSize())).append("\n");

      throw ExceptionNotifier(s.c_str());
   }
}

std::string Matrix::toString() const {
   std::string s;

   for (auto i = 0; i < getRows(); ++i) {
      s.append("[");

      for (auto j = 0; j < getColumns(); ++j) {
         s.append(std::to_string(getArray()[i * getColumns() + j])).append("\t");
      }

      s.append("]").append("\n");
   }

   return s;
}
