#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

#include "conversion.hpp"
#include "parse.hpp"
#include "logger.hpp"
#include "error.hpp"

DEFINE_ERROR(interface_error);

struct mod {
  int component;
  char op{0};
  double value{std::nan("")};

  void apply(double* target) const;
};

struct term_eater;

class interface {
public:
  interface();

  double data[5];
  int data_count{0};
  colorspace from{colorspaces::rgb};
  colorspace to{colorspaces::rgb};
  colorspace inter{colorspaces::jzazbz};
  std::string separator{" "};
  std::vector<mod> modifications{};
  bool quit{false};
  bool stay{false};
  bool comma{false};
  bool alpha_first{true};
  bool alpha{false};
  std::stringstream output_stream;


  std::string add_term(std::string&&);
  std::string get_state();
  void clear();
  void makesure_empty();
  bool use_hex();
  bool use_hex(bool);
  void print_help();
  void add_term_eater(const term_eater*);
  
protected:
  const term_eater* eater{nullptr};
  
  void process_short_switches(const std::string& switches);
  bool process_long_switch(const std::string& name);
  template<bool, bool> bool switches(const std::string&);

  void feed_term_eater(std::string&& data);
  void unexpected_comma(const std::string& term);
  std::string pop_data(colorspace, colorspace);

};

