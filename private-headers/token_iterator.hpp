#pragma once

#include <string>

#include "tstring.hpp"

using namespace std;
struct token_iterator {
  token_iterator(string&& s) : input(move(s)), position(0), mark(0) {}
  token_iterator() : token_iterator("") {}

  string input;
  size_t position;
  size_t mark;

  token_iterator& set_input(string&&, size_t = 0);
  bool have_token();
  bool next_token();
  token_iterator& return_token(size_t from);
  tstring& token();
  template<int (*F)(int)> bool next_token_base();

private:
  tstring m_token;

};

constexpr const char space_chars[] = " \t\r\n\v\f";
constexpr const char quote_chars[] = "'\"";

// Template implementation
#include <cstring>

// Find the next token that contains only the characters accepted by char_func
template<int (*char_func)(int)>
bool token_iterator::next_token_base() {
  while(true) {
    if (input.empty() || position == tstring::npos) {
      return false;
    } else {
      // Find the start of the token, ignoring spaces
      mark = input.find_first_not_of(space_chars, position);
      if (mark == tstring::npos)
        return false;

      if (strchr(quote_chars, input[mark]) != nullptr) {
        // Parse terms enclosed in quotes
        position = input.find(input[mark], mark + 1);
        mark++;
        if (position == tstring::npos) {
          m_token = tstring(input.data(), mark, input.size());
          // WARNING: Unterminated quotes
        } else {
          m_token = tstring(input.data(), mark, position);
          position++;
        }
        return true;
      } else {
        // Extend the length of the term until we hit an unaccepted character
        for(position = mark+1; position < input.size(); position++)
          if (!char_func(input[position]))
            break;
        m_token = tstring(input.data(), mark, position);
        return true;
      }
    }
  }
}