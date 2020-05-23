#include "Matrix.h"

Matrix::Matrix(unsigned int rows, unsigned int columns, const initializer_list<float>& data) {
   data_ = FloatArray(rows, columns, data);
}

Matrix::Matrix(unsigned int rows, unsigned int columns, float* data) {
   data_ = FloatArray(rows, columns, data);
}

Matrix::Matrix(const Matrix &matrix) {
   data_ = FloatArray(matrix.getData());
}

Matrix::Matrix(Matrix &&matrix) {
   data_ = FloatArray(move(matrix.getData()));

   matrix.deleteMatrix();
}

Matrix::~Matrix() {}

Matrix &Matrix::operator=(const Matrix &matrix) {
   data_ = FloatArray(matrix.getData());

   return *this;
}

Matrix &Matrix::operator=(Matrix && matrix) {
   data_ = FloatArray(matrix.getData());

   matrix.deleteMatrix();

   return *this;
}

//TODO fix repeated functions
Matrix Matrix::operator+(const Matrix &matrix) noexcept(false) {
   std::unique_ptr<float> newData(new float[getRows() * getColumns()]);

   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      for (int i = 0; i < getRows() * getColumns(); ++i) {
         newData.get()[i] = data_[i] + matrix.getData()[i];
      }
   } else {
      string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
         .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }

   return Matrix(getRows(), getColumns(), newData.release());
}

Matrix &Matrix::operator+=(const Matrix &matrix) noexcept(false) {
   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      for (int i = 0; i < getRows() * getColumns(); ++i) {
         data_[i] += matrix.getData()[i];
      }
   } else {
      string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }

   return *this;
}

Matrix Matrix::operator-(const Matrix &matrix) noexcept(false) {
   std::unique_ptr<float> newData(new float[getRows() * getColumns()]);

   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      for (int i = 0; i < getRows() * getColumns(); ++i) {
         newData.get()[i] = data_[i] - matrix.getData()[i];
      }
   } else {
      string s = "Exception: dimensions do not correspond.";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }

   return Matrix(getRows(), getColumns(), newData.release());
}

Matrix &Matrix::operator-=(const Matrix &matrix) noexcept(false) {
   if (getRows() == matrix.getRows() && getColumns() == matrix.getColumns()) {
      for (int i = 0; i < getRows() * getColumns(); ++i) {
         data_[i] -= matrix.getData()[i];
      }
   } else {
      string s = "Exception COPY_CONSTRUCTOR: dimensions do not correspond. ";
      s.append("(").append(std::to_string(getRows())).append(", ").append(std::to_string(getColumns())).append(") ")
              .append(" != (").append(std::to_string(matrix.getRows())).append(", ").append(std::to_string(
                      matrix.getColumns())).append(")\n");
      // (row, columns_) != (matrix.getRows(), matrix.getColumns()

      throw ExceptionNotifier(s.c_str());
   }

   return *this;
}

Matrix Matrix::operator*(float scalar) {
   unique_ptr<float> newData(new float[getRows() * getColumns()]);

   for (int i = 0; i < getRows() * getColumns(); ++i) {
      newData.get()[i] = data_[i] * scalar;
   }

   return Matrix(getRows(), getColumns(), newData.release());
}

Matrix& Matrix::operator*=(float scalar) {
   for (int i = 0; i < getRows() * getColumns(); ++i) {
      data_[i] *= scalar;
   }

   return *this;
}

Matrix Matrix::operator*(Matrix &matrix) noexcept(false) {
   std::unique_ptr<float> newData(new float[getRows() * matrix.getColumns()]);

   if (getColumns() == matrix.getRows()) {
      for (unsigned int r = 0; r < getRows(); ++r) {
         for (unsigned int c = 0; c < matrix.getColumns(); ++c) {
            for (unsigned int i = 0; i < getColumns(); ++i) {
               newData.get()[r * matrix.getColumns() + c] += data_[r * getColumns() + i] * matrix.getData()[i * matrix.getColumns() + c];
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
   unique_ptr<float> newData(new float[matrix.getRows() * matrix.getColumns()]);

   for (int i = 0; i < matrix.getColumns(); ++i) {
      for (int j = 0; j < matrix.getRows(); ++j) {
         //TODO check here
         newData.get()[i* matrix.getRows() + j] = matrix.getData()[j * matrix.getColumns() + i];
      }
   }

   return Matrix(matrix.getColumns(), matrix.getRows(), newData.release());
}

Matrix Matrix::createSubmatrix(const Matrix &matrix, unsigned int rowIndex, unsigned int columnIndex) {
   std::unique_ptr<float> newData(new float[(matrix.getRows() - 1) * (matrix.getColumns() - 1)]);

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

      return move(newData);
   } else {
      string s = "Exception NO_MATCH_LENGTH: matrix and vector_ do not have correct size_. ";
      s.append("Matrix columns_: ").append(std::to_string(getColumns())).append("!= Vector size_: ")
         .append(std::to_string(vector.getSize())).append("\n");

      throw ExceptionNotifier(s.c_str());
   }
}

void Matrix::deleteMatrix() {
   for (int i = 0; i < getRows() * getColumns(); ++i) {
      data_[i] = 0;
   }
}

std::string Matrix::toString() const {
   string s;

   for (auto i = 0; i < getRows(); ++i) {
      s.append("[");

      for (auto j = 0; j < getColumns(); ++j) {
         s.append(std::to_string(getArray()[i * getColumns() + j])).append("\t");
      }

      s.append("]").append("\n");
   }

   return s;
}
