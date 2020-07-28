#ifndef FLOATVECT_H
#define FLOATVECT_H

#include <memory>

struct FloatArray {
private:
   unsigned int rows_;
   unsigned int columns_;
   std::unique_ptr<float> array_;

public:
   FloatArray(unsigned int dimension, const std::initializer_list<float>& data);
   FloatArray(unsigned int rows, unsigned int columns, const std::initializer_list<float>& data);
   FloatArray(unsigned int rows, unsigned int columns, float*& data);
   FloatArray(const FloatArray& floatArray);
   FloatArray(FloatArray&& floatArray);

   ~FloatArray() {
      rows_ = 0;
      columns_ = 0;

      array_.reset();
   }

   FloatArray& operator=(const FloatArray& floatArray);

   FloatArray& operator=(FloatArray&& floatArray);

   inline const std::unique_ptr<float>& getArray() const {
      return array_;
   }

   inline unsigned int getRows() const {
      return rows_;
   }

   inline unsigned int getColumns() const {
      return columns_;
   }

   float& operator[](int i) {
      return array_.get()[i];
   }
};

#endif //FLOATVECT_H
