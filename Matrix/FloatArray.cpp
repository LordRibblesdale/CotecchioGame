#include <iostream>
#include "FloatArray.h"

FloatArray::FloatArray(unsigned int rows, unsigned int columns, const std::initializer_list<float> &data) : rows_(rows), columns_(columns) {
   array_ = std::move(std::unique_ptr<float>(new float[rows*columns] {0}));
   rows_ = rows;
   columns_ = columns;

   auto iterator = data.begin();
   for (int i = 0; i < rows_*columns_ && i < data.size(); ++i) {
      array_.get()[i] = *iterator;
      ++iterator;
   }
}

FloatArray::FloatArray(unsigned int dimension, const std::initializer_list<float> &data) : FloatArray(dimension, dimension, data) {}

FloatArray::FloatArray(unsigned int rows, unsigned int columns, float*& data) {
   array_ = std::move(std::unique_ptr<float>(data));
   rows_ = rows;
   columns_ = columns;
}

FloatArray::FloatArray(const FloatArray &floatArray) {
   FloatArray::rows_ = floatArray.getRows();
   FloatArray::columns_ = floatArray.getColumns();
   array_.reset(new float[rows_ * columns_] {0});

   std::copy(floatArray.getArray().get(), floatArray.getArray().get() + rows_*columns_, array_.get());
}

FloatArray::FloatArray(FloatArray&& floatArray) {
   // Accesso a variabile protetta permesso alle classi/sottoclassi
   // https://en.cppreference.com/w/cpp/language/access#Protected_member_access
   array_ = std::move(floatArray.array_);
   rows_ = floatArray.getRows();
   columns_ = floatArray.getColumns();
}

FloatArray &FloatArray::operator=(const FloatArray &floatArray) {
   FloatArray::rows_ = floatArray.getRows();
   FloatArray::columns_ = floatArray.getColumns();
   array_.reset(new float[rows_ * columns_] {0});

   std::copy(floatArray.getArray().get(), floatArray.getArray().get() + rows_*columns_, array_.get());

   return *this;
}

FloatArray &FloatArray::operator=(FloatArray &&floatArray) {
   // Accesso a variabile protetta permesso alle classi/sottoclassi
   // https://en.cppreference.com/w/cpp/language/access#Protected_member_access
   rows_ = floatArray.getRows();
   columns_ = floatArray.getColumns();
   array_ = std::move(floatArray.array_);

   return *this;
}
