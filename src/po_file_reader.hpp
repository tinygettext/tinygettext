//  $Id$
//
//  tinygettext - A gettext replacement that works directly on .po files
//  Copyright (C) 2006 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PO_FILE_READER_HPP
#define HEADER_PO_FILE_READER_HPP

#include <vector>
#include <string.h>
#include "tinygettext.hpp"

namespace tinygettext {

class Dictionary;

class POFileReader
{
private:
  std::istream& in;
  Dictionary& dict;

  std::string from_charset;
  std::string to_charset;

  int line_num;
  int c; //TODO: char c? unsigned char c?
  enum Token {
      TOKEN_KEYWORD, //msgstr, msgid, etc.
      TOKEN_CONTENT, //string literals, concatenated ("" "foo\n" "bar\n" -> "foo\nbar\n")
      TOKEN_EOF      //ran out of tokens
  };
  Token token;
  std::string tokenContent; //current contents of the keyword or string literal(s)

  inline void nextChar();
  inline void skipSpace();
  inline bool expectToken(std::string type, Token wanted);
  inline bool expectContent(std::string type, std::string wanted);

  void parse_header(const std::string& header);
  void tokenize_po();
  Token nextToken();

  POFileReader(std::istream& in_, Dictionary& dict_);

public:
  /** Read the content of the .po file given as \a in into the
    dictionary given as \a dict */
  static void read(std::istream& in, Dictionary& dict);
};

} // namespace tinygettext

#endif

/* EOF */
