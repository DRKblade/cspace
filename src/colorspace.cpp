#include "colorspace.hpp"

#include "log.hpp"
#include "conversion.hpp"

using namespace std;

namespace colorspaces {
  constexpr const char scope[] = "indirect-convert";

  int component_count(colorspace c) {
    if (c == cmyk)
      return 4;
    return 3;
  }

  colorspace get_mask(colorspace c) {
  #define ENTRY(color) if (c == color) return color##_msk;
    ENTRY(xyz)
    ENTRY(rgb)
    ENTRY(jzazbz)
    ENTRY(cielab)
  #undef ENTRY
    throw conversion_error("Color-Mask: Unknown color space: " + to_string(c));
  }

  void direct(double* value, colorspace from, colorspace to) {
    unsigned short combined = from << 8 | to;
    switch (combined) {
  #define CASE_BASE(a, b, method) \
    case a << 8 | b: \
      method(value, value); \
      log::debug<scope>("Direct conversion: "#a" to "#b); \
      break;
  #define CASE(a, b) \
    CASE_BASE(a, b, a##_##b) \
    CASE_BASE(b, a, b##_##a)
  #define AB_CH(_ab, _ch) \
    CASE_BASE(_ab, _ch, ab_ch) \
    CASE_BASE(_ch, _ab, ch_ab)
      CASE(rgb, xyz)
      CASE(rgb, hsl)
      CASE(rgb, hsv)
      CASE(rgb, cmyk)
      CASE(xyz, jzazbz)
      CASE(xyz, cielab)
      AB_CH(jzazbz, jzczhz)
      AB_CH(cielab, cielch)
      default: throw conversion_error("Color-Direct: Unknown color conversion: from " + to_string(from) + " to " + to_string(to)); 
  #undef AB_CH
  #undef CASE
    }
  }

  void convert_up(double* value, colorspace from, colorspace parent) {
    if (from == parent) return;
    auto next_mask = from & get_mask(parent);
    convert_up(value, from, next_mask);
    direct(value, next_mask, parent);
  }

  void convert_down(double* value, colorspace parent, colorspace to) {
    if (parent == to) return;
    auto next_mask = to & get_mask(parent);
    direct(value, parent, next_mask);
    convert_down(value, next_mask, to);
  }
    
  void convert(double* value, colorspace from, colorspace to) {
    log::debug<scope>("Convert from " + to_string(from) + " to " + to_string(to));
    if (from == to) return;
    auto parent = xyz;
    colorspace common;
    while ((common = from & get_mask(parent)) == (to & get_mask(parent))) {
      parent = common;
    }
    convert_up(value, from, parent);
    convert_down(value, parent, to);
  }
}
