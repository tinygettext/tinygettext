//  $Id$
//
//  tinygettext - A gettext replacement that works directly on .po files
//  Copyright (C) 2009 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_PO_PARSER_HPP
#define HEADER_PO_PARSER_HPP

#include <iosfwd>
#include "iconv.hpp"

namespace tinygettext {

class Dictionary;

class POParser
{
private:
  std::istream& in;
  Dictionary& dict;
  IConv conv;

  bool running;
  bool eof;
  bool big5;

  int line_number;
  std::string current_line;

  POParser(std::istream& in_, Dictionary& dict_);
  ~POParser();

  void parse_header(const std::string& header);
  void parse();
  void next_line();
  std::string get_string(int skip);
  void get_string(std::ostringstream& str, int skip);
  bool is_empty_line();
  bool prefix(const char* );
  void error(const std::string& msg);
  void warning(const std::string& msg);

public:
  static void parse(std::istream& in, Dictionary& dict);

private:
  POParser (const POParser&);
  POParser& operator= (const POParser&);
};

} // namespace tinygettext

#endif

/* EOF */
