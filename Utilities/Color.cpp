#include "Color.h"

Color::Color() : FloatVector(4, {0, 0, 0, 1}) {}

Color::Color(float& r, float& g, float& b) : FloatVector(4, {r, g, b, 1}) {}

Color::Color(float &r, float& g, float& b, float& a) : FloatVector(4, {r, g, b, a}) {}

Color::Color(const Color &color) : FloatVector(color) {}

Color::Color(Color &&color) : FloatVector(color) {
   color.setRed(0);
   color.setGreen(0);
   color.setBlue(0);
}

Color::~Color() {
   setRed(0);
   setGreen(0);
   setBlue(0);
   setAlpha(0);
}

Color &Color::operator=(const Color &color) {
   setRed(color.getRed());
   setGreen(color.getGreen());
   setBlue(color.getBlue());
   setAlpha(color.getAlpha());

   return *this;
}

Color &Color::operator=(Color &&color) {
   setRed(color.getRed());
   setGreen(color.getGreen());
   setBlue(color.getBlue());
   setAlpha(color.getAlpha());

   color.setRed(0);
   color.setGreen(0);
   color.setBlue(0);
   color.setAlpha(0);

   return *this;
}

const float &Color::getRed() const {
   return vector_.get()[0];
}

const float &Color::getGreen() const {
   return vector_.get()[1];
}

const float &Color::getBlue() const {
   return vector_.get()[2];
}

const float &Color::getAlpha() const {
   return vector_.get()[3];
}

void Color::setRed(const float &red) {
   vector_.get()[0] = red;
}

void Color::setGreen(const float &green) {
   vector_.get()[1] = green;
}

void Color::setBlue(const float &blue) {
   vector_.get()[2] = blue;
}

void Color::setAlpha(const float &alpha) {
   vector_.get()[3] = alpha;
}

void Color::clamp() {

}

int Color::toInt8Bit(const float &f) {
   return 255*f;
}
