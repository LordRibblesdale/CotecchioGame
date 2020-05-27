#include "FloatArray.h"

FloatArray::FloatArray(unsigned short rows, unsigned short columns, const std::initializer_list<float> &data) : rows_(rows), columns_(columns) {
   array_ = std::move(std::unique_ptr<float>(new float[rows*columns]));
   rows_ = rows;
   columns_ = columns;

   auto iterator = data.begin();
   for (int i = 0; i < rows_*columns_; ++i) {
      array_.get()[i] = *iterator;
      ++iterator;
   }
}

FloatArray::FloatArray(unsigned short dimension, const std::initializer_list<float> &data) : FloatArray(dimension, dimension, data) {}

FloatArray::FloatArray(unsigned short rows, unsigned short columns, float*& data) {
   array_ = std::move(std::unique_ptr<float>(data));
   rows_ = rows;
   columns_ = columns;
}

FloatArray::FloatArray(const FloatArray &floatArray) {
   array_ = std::unique_ptr<float>(new float[floatArray.getRows() * floatArray.getColumns()]);
   FloatArray::rows_ = floatArray.getRows();
   FloatArray::columns_ = floatArray.getColumns();

   for (int i = 0; i < rows_ * columns_; ++i) {
      array_.get()[i] = floatArray.getArray().get()[i];
   }
}

FloatArray::FloatArray(FloatArray&& floatArray) {
   //TODO test const_cast here
   array_ = move(std::unique_ptr<float>((const_cast<std::unique_ptr<float>&>(floatArray.getArray())).release()));
   rows_ = floatArray.getRows();
   columns_ = floatArray.getColumns();

   //TODO check implicit destructor
}

FloatArray &FloatArray::operator=(const FloatArray &floatArray) {
   array_ = std::unique_ptr<float>(new float[floatArray.getRows() * floatArray.getColumns()]);
   FloatArray::rows_ = floatArray.getRows();
   FloatArray::columns_ = floatArray.getColumns();

   for (int i = 0; i < rows_ * columns_; ++i) {
      array_.get()[i] = floatArray.getArray().get()[i];
   }

   return *this;
}

FloatArray &FloatArray::operator=(FloatArray &&floatArray) {
   //TODO test const_cast here
   array_ = move(std::unique_ptr<float>((const_cast<std::unique_ptr<float>&>(floatArray.getArray())).release()));
   rows_ = floatArray.getRows();
   columns_ = floatArray.getColumns();

   //TODO check implicit destructor

   return *this;
}
