#include "Matrix.h"

Matrix::Matrix(unsigned int rows, unsigned int columns, const std::initializer_list<float>& data) {
   data_ = std::move(std::make_unique<FloatArray>(rows, columns, data));
}

Matrix::Matrix(unsigned int rows, unsigned int columns, float* data) {
   data_ = std::move(std::make_unique<FloatArray>(rows, columns, data));
}

Matrix::Matrix(const Matrix &matrix) {
   data_ = std::move(std::make_unique<FloatArray>(*matrix.data_));
}

Matrix::Matrix(Matrix &&matrix) {
   data_ = std::move(std::make_unique<FloatArray>(*matrix.data_.release()));
}

Matrix::~Matrix() {
   data_.reset(nullptr);
}

Matrix &Matrix::operator=(const Matrix &matrix) {
   data_ = std::move(std::make_unique<FloatArray>(*matrix.data_));

   return *this;
}

Matrix &Matrix::operator=(Matrix && matrix) {
   data_ = std::move(std::make_unique<FloatArray>(*matrix.data_.release()));

   matrix.deleteMatrix();

   return *this;
}

//TODO fix repeated functions
Matrix Matrix::operator+(const Matrix &matrix) noexcept(false) {
   std::unique_ptr<float> newData(new float[getRows() * getColumns()] {0});

   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      for (int i = 0; i < getRows() * getColumns(); ++i) {
         newData.get()[i] = data_->operator[](i) + matrix.getData()->operator[](i);
      }
   } else {
      std::string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
         .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }

   return Matrix(getRows(), getColumns(), newData.release());
}

void Matrix::operator+=(const Matrix &matrix) noexcept(false) {
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

Matrix Matrix::operator-(const Matrix &matrix) noexcept(false) {
   std::unique_ptr<float> newData(new float[getRows() * getColumns()] {0});

   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      for (int i = 0; i < getRows() * getColumns(); ++i) {
         newData.get()[i] = data_->operator[](i) - matrix.getData()->operator[](i);
      }
   } else {
      std::string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }

   return Matrix(getRows(), getColumns(), newData.release());
}

void Matrix::operator-=(const Matrix &matrix) noexcept(false) {
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

Matrix Matrix::operator*(const float& scalar) {
   std::unique_ptr<float> newData(new float[getRows() * getColumns()] {0});

   for (int i = 0; i < getRows() * getColumns(); ++i) {
      newData.get()[i] = data_->operator[](i) * scalar;
   }

   return Matrix(getRows(), getColumns(), newData.release());
}

void Matrix::operator*=(const float& scalar) {
   for (int i = 0; i < getRows() * getColumns(); ++i) {
      data_->operator[](i) *= scalar;
   }
}

Matrix Matrix::operator*(const Matrix &matrix) noexcept(false) {
   std::unique_ptr<float> newData(new float[getRows() * matrix.getColumns()] {0});

   if (getColumns() == matrix.getRows()) {
      for (unsigned int r = 0; r < getRows(); ++r) {
         for (unsigned int c = 0; c < matrix.getColumns(); ++c) {
            for (unsigned int i = 0; i < getColumns(); ++i) {
               newData.get()[r * matrix.getColumns() + c] += data_->operator[](r * getColumns() + i) * matrix.getData()->operator[](i * matrix.getColumns() + c);
            }
         }
      }
   } else {
      std::string s = "Exception MATRIX_PRODUCT: dimensions do not correspond. ";
      s.append("(").append(std::to_string(getColumns())).append(", ").append(std::to_string(matrix.getRows())).append(")\n");

      throw ExceptionNotifier(s.c_str());
   }

   return Matrix(getRows(), matrix.getColumns(), newData.release());
}

Matrix Matrix::transpose(const Matrix &matrix) {
   std::unique_ptr<float> newData(new float[matrix.getRows() * matrix.getColumns()] {0});

   for (int i = 0; i < matrix.getColumns(); ++i) {
      for (int j = 0; j < matrix.getRows(); ++j) {
         //TODO check here
         newData.get()[i* matrix.getRows() + j] = matrix.getData()->operator[](j * matrix.getColumns() + i);
      }
   }

   return Matrix(matrix.getColumns(), matrix.getRows(), newData.release());
}

Matrix Matrix::createSubmatrix(const Matrix &matrix, const unsigned int& rowIndex, const unsigned int& columnIndex) {
   std::unique_ptr<float> newData(new float[(matrix.getRows() - 1) * (matrix.getColumns() - 1)] {0});

   unsigned int index = 0;
   for (int i = 0; i < matrix.getRows(); ++i) {
      if (i != rowIndex) {
         for (int j = 0; j < matrix.getColumns(); ++j) {
            if (j != columnIndex) {
               newData.get()[index++] = matrix.getArray()[i * matrix.getColumns() + j];
            }
         }
      }
   }

   return Matrix(matrix.getRows() - 1, matrix.getColumns() - 1, newData.release());
}

FloatVector Matrix::multiplyVector(const FloatVector& vector) const noexcept(false) {
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

void Matrix::deleteMatrix() {
   for (int i = 0; i < getRows() * getColumns(); ++i) {
      data_->operator[](i) = 0;
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
