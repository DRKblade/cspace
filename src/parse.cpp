#include "parse.hpp"
#include "log.hpp"
#include <sstream>

using namespace std;

string to_string(const double* arr, colorspace space) {
  stringstream result;
  result << arr[0];
  auto count = colorspaces::component_count(space);
  for(int i = 1; i < count; i++)
    result << ' ' << arr[i];
  log::debug("To-String: Result: " + result.str() + ", first element: " + to_string(arr[0]));
  return result.str();
}

colorspace stospace(const char* value) {
#define CASE(a) if (strcmp(#a, value)) return colorspaces::a;
  CASE(xyz);
  CASE(rgb);
  CASE(jzazbz);
  CASE(cielab);
  CASE(hsl);
  CASE(hsv);
  CASE(jzczhz);
  CASE(cielch);
#undef CASE
  throw parse_error("Unknown colorspace: "s + value);
}

int parse_code(const string& value, component& a, component& r,
                                    component& g, component& b) {
  if (value[0] != '#')
    return 0;
  bool error = false;
  int divider;
  auto str = value.c_str();
	auto hex = [&] (int index)->unsigned int {
		auto& c = str[index];
    if (c <= '9' && c >= '0')
      return c - '0';
    if (c <= 'F' && c >= 'A')
      return c - 'A' + 10;
    if (c <= 'f' && c >= 'a')
      return c - 'a' + 10;
  	error = true;
  	return 0;
	};

	switch(value.length()) {
  	case 5:
    	a = hex(1);
    	str++;
    	if (false) {
		case 4:
  			a = 15;
    	}
  		r = hex(1);
  		g = hex(2);
  		b = hex(3);
  		divider = 15;
			break;
		case 9:
			a = (hex(1) * 16 + hex(2));
			str += 2;
			if (false) {
		case 7:
    		a = 255;
			}
			r = hex(1) * 16 + hex(2);
			g = hex(3) * 16 + hex(4);
			b = hex(5) * 16 + hex(6);
			divider = 255;
			break;
		case 17:
			a = hex(1)*4096 + hex(2)*256 + hex(3)*16 + hex(4);
			str += 4;
			if (false) {
		case 13:
  			a = 65535;
			}
			r = hex(1)*4096 + hex(2)*256 + hex(3)*16 + hex(4);
			g = hex(5)*4096 + hex(6)*256 + hex(7)*16 + hex(8);
			b = hex(9)*4096 + hex(10)*256 + hex(11)*16 + hex(12);
			divider = 65535;
			break;
		default:
  		return 0;
	}
  if(error)
    return 0;
  if (a == 0)
    r = g = b = 0;
  return divider;
}
