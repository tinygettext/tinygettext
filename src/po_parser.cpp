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

#include <iostream>
#include <ctype.h>
#include <string>
#include <istream>
#include <string.h>
#include <map>
#include <stdlib.h>
#include "log.hpp"

#include "iconv.hpp"
#include "dictionary.hpp"
#include "po_parser.hpp"

namespace tinygettext {

void
POParser::parse(const std::string& filename, std::istream& in, Dictionary& dict)
{
  POParser parser(filename, in, dict);
  parser.parse();
}

class POParserError {};

POParser::POParser(const std::string& filename_, std::istream& in_, Dictionary& dict_)
  : filename(filename_), in(in_), dict(dict_),
    running(false), eof(false), big5(false),
    line_number(0)
{
}

POParser::~POParser()
{
}

void
POParser::warning(const std::string& msg)
{
  log_warning << filename << ":" << line_number << ": warning: " << msg << ": " << current_line << std::endl;
  //log_warning << "Line: " << current_line << std::endl;
}

void
POParser::error(const std::string& msg)
{
  log_warning << filename << ":" << line_number << ": error: " << msg  << ": " << current_line << std::endl;

  // Try to recover from an error by searching for start of another entry
  do
    next_line();
  while(!eof && is_empty_line());

  throw POParserError();
}

void
POParser::next_line()
{
  line_number += 1;
  if (!std::getline(in, current_line))
    eof = true;
}

void
POParser::get_string_line(std::ostringstream& out, int skip)
{
  if (skip+1 >= (int)current_line.size())
    error("unexpected end of line5");

  if (current_line[skip] != '"')
    error("expected start of string '\"'");
  
  std::string::size_type i;
  for(i = skip+1; current_line[i] != '\"'; ++i)
    {
      if (big5 && (unsigned char)current_line[i] >= 0x81 && (unsigned char)current_line[i] <= 0xfe)
        {
          out << current_line[i];

          i += 1;
          
          if (i >= current_line.size())
            error("invalid big5 encoding");
          
          out << current_line[i];
        }
      else if (i >= current_line.size())
        {
          error("unexpected end of string");
        }
      else if (current_line[i] == '\\')
        {
          i += 1;

          if (i >= current_line.size())
            error("unexpected end of string in handling '\\'");

          switch (current_line[i])
            {
              case 'a':  out << '\a'; break;
              case 'b':  out << '\b'; break;
              case 'v':  out << '\v'; break;
              case 'n':  out << '\n'; break;
              case 't':  out << '\t'; break;
              case 'r':  out << '\r'; break;
              case '"':  out << '"'; break;
              case '\\': out << '\\'; break;
              default: 
                std::ostringstream err;
                err << "unhandled escape '\\" << current_line[i] << "'";
                warning(err.str());

                out << current_line[i-1] << current_line[i];
                break;
            }
        }
      else
        {
          out << current_line[i];
        }
    }

  // process trailing garbage in line and warn if there is any
  for(i = i+1; i < current_line.size(); ++i)
    if (!isspace(current_line[i]))
      {
        warning("unexpected garbage after string ignoren");
        break;
      }
}

std::string
POParser::get_string(int skip)
{
  std::ostringstream out;

  if (skip+1 >= (int)current_line.size())
    error("unexpected end of line+");

  if (current_line[skip] == ' ' && current_line[skip+1] == '"')
    {
      get_string_line(out, skip+1);
    }
  else
    {
      warning("keyword and string must be seperated by a single space");

      for(;;)
        {
          if (skip >= (int)current_line.size())
            error("unexpected end of line");
          else if (current_line[skip] == '\"')
            {
              get_string_line(out, skip);
              break;
            }
          else if (!isspace(current_line[skip]))
            error("unexpected character");
          else
            ; // skip space

          skip += 1;
        }
    }
  
 next:
  next_line();
  for(std::string::size_type i = 0; i < current_line.size(); ++i)
    {
      if (current_line[i] == '"')
        {
          if (i == 1)
            warning("leading whitespace before string");

          get_string_line(out, i);
          goto next;
        }
      else if (isspace(current_line[i]))
        {
          // skip
        }
      else
        {
          break;
        }
    }

  return out.str();
}

void
POParser::parse_header(const std::string& header)
{
  std::string from_charset;
  std::string::size_type start = 0;
  for(std::string::size_type i = 0; i < header.length(); ++i)
    {
      if (header[i] == '\n')
        {
          std::string line = header.substr(start, i - start);
          int len = strlen("Content-Type: text/plain; charset=");
          if (line.compare(0, len, "Content-Type: text/plain; charset=") == 0)
            {
              from_charset = line.substr(len);
              break;
            }

          start = i+1;
        }
    }

  for(std::string::iterator i = from_charset.begin(); i != from_charset.end(); ++i)
    *i = tolower(*i);

  if (from_charset.empty() || from_charset == "charset")
    {
      log_warning << "Error: Charset not specified for .po, fallback to UTF-8" << std::endl;
      from_charset = "utf-8";
    }
  else if (from_charset == "big5")
    {
      big5 = true;
    }

  std::cout << "From Charset: '" << from_charset << "'" << std::endl;
  conv.set_charsets(from_charset, dict.get_charset());
}

bool
POParser::is_empty_line()
{
  if (current_line.empty())
    {
      return true;
    }
  else if (current_line[0] == '#')
    { // handle comments as empty lines
      if (current_line.size() == 1 || (current_line.size() >= 2 && isspace(current_line[1])))
        return true;
      else
        return false;
    }
  else
    {
      for(std::string::iterator i = current_line.begin(); i != current_line.end(); ++i)
        {
          if (!isspace(*i))
            return false;
        }
    }
  return true;
}

bool
POParser::prefix(const char* prefix)
{
  return current_line.compare(0, strlen(prefix), prefix) == 0;
}

void
POParser::parse()
{
  next_line();

  // skip UTF-8 intro that some text editors produce
  // see http://en.wikipedia.org/wiki/Byte-order_mark
  if (current_line.size() >= 3 &&
      current_line[0] == 0xef &&
      current_line[1] == 0xbb &&
      current_line[2] == 0xbf)
    {
      current_line = current_line.substr(3);
    }

  // Parser structure
  while(!eof)
    {
      try 
        {
          uint32_t flags = 0;
          bool has_msgctxt = false;
          std::string msgctxt;
          std::string msgid;

          while(prefix("#"))
            {
              if (current_line.size() >= 2 && current_line[1] == ',')
                {
                  flags = 0;
                  std::cout << "flags: " << current_line << std::endl;
                }
              //parse_comments(&flags);

              next_line();
            }

          if (!is_empty_line())
            {
              if (prefix("msgctxt"))
                {
                  has_msgctxt = true;
                  msgctxt = get_string(7);
                }

              if (prefix("msgid"))
                msgid = get_string(5);
              else
                error("expected 'msgid'");

              if (prefix("msgid_plural"))
                {
                  std::string msgid_plural = get_string(12);
                  std::map<int, std::string> msgstr_num;

                next:
                  if (is_empty_line())
                    {
                      if (msgstr_num.empty())
                        error("expected 'msgstr[N]'");
                    }
                  else if (prefix("msgstr[") &&
                           current_line.size() > 8 && 
                           isdigit(current_line[7]) && current_line[8] == ']')
                    {
                      int number = current_line[7] - '0';
                      msgstr_num[number] = get_string(9);
                      goto next;
                    }
                  else 
                    {
                      error("expected 'msgstr[N]'");
                    }

                  if (!is_empty_line())
                    error("expected 'msgstr[N]' or empty line");

                  std::cout << "msgid \"" << msgid << "\"" << std::endl;
                  std::cout << "msgid_plural \"" << msgid_plural << "\"" << std::endl;
                  for(std::map<int, std::string>::iterator i = msgstr_num.begin(); i != msgstr_num.end(); ++i)
                    std::cout << "msgstr[" << i->first << "] \"" << conv.convert(i->second) << "\"" << std::endl;
                  std::cout << std::endl;
                }
              else if (prefix("msgstr"))
                {
                  std::string msgstr = get_string(6);

                  if (msgid.empty())
                    {
                      parse_header(msgstr);
                    }
                  else
                    {
                      std::cout << "msgid \"" << msgid << "\"" << std::endl;
                      std::cout << "msgstr \"" << conv.convert(msgstr) << "\"" << std::endl;
                      std::cout << std::endl;
                    }
                }
              else
                {
                  error("expected 'msgstr' or 'msgid_plural'");
                }
            }
      
          if (!is_empty_line())
            error("expected empty line");

          next_line();
        }
      catch(POParserError&)
        {          
        }
    }
}

} // namespace tinygettext

/* EOF */
