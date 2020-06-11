#ifndef COLOR_H
#define COLOR_H

#include "../Vector/FloatVector.h"

class Color : public FloatVector {
public:
   Color();
   Color(float g);
   Color(float r, float g, float b);
   Color(float r, float g, float b, float a);
   Color(const FloatVector& vector);
   Color(const Color& color);
   Color(Color&& color);
   ~Color();

   Color& operator=(const Color& color);
   Color& operator=(Color&& color);

   const float& getRed() const;
   const float& getGreen() const;
   const float& getBlue() const;
   const float& getAlpha() const;

   void setRed(const float& red);
   void setGreen(const float& green);
   void setBlue(const float& blue);
   void setAlpha(const float& alpha);

   void clamp();

   static int toInt8Bit(const float& f);

   static float toFloat8Bit(const int& i);
};

#endif //COLOR_H
