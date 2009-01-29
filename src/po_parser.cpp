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
#include "po_parser.hpp"

namespace tinygettext {

void
POParser::parse(std::istream& in, Dictionary& dict)
{
  POParser parser(in, dict);
  parser.parse();
}

POParser::POParser(std::istream& in_, Dictionary& dict_)
  : in(in_), dict(dict_), running(false), eof(false),
    line_number(1)
{
}

void
POParser::warning(const std::string& msg)
{
  log_warning << msg << std::endl;
}

void
POParser::error(const std::string& msg)
{
  log_warning << "/ Error at line " << line_number << ": " << msg << std::endl;
  log_warning << "\\ " << current_line << std::endl;

  exit(1);
  while(!is_empty_line())
    next_line();

}

void
POParser::next_line()
{
  line_number += 1;
  if (!std::getline(in, current_line))
    eof = true;
}

void
POParser::get_string(std::ostringstream& out, int skip)
{
  if (skip < (int)current_line.size() && current_line[skip] != '"')
    error("Expected start of string '\"'");

  std::string::size_type i;
  for(i = skip+1; current_line[i] != '\"'; ++i)
    {
      if (i >= current_line.size())
        {
          error("Unexpected end of string");
        }
      else if (current_line[i] == '\\')
        {
          i += 1;

          if (i >= current_line.size())
            error("Unexpected end of string in handling '\\'");

          switch (current_line[i])
            {
              case 'n':  out << '\n'; break;
              case 't':  out << '\t'; break;
              case 'r':  out << '\r'; break;
              case '"':  out << '"'; break;
              case '\\': out << '\\'; break;
              default: 
                warning("Unhandled escape in string");
                out << current_line[i-1] << current_line[i];
                break;
            }
        }
      else
        {
          out << current_line[i];
        }
    }

  // process trailing garbage in line
}

std::string
POParser::get_string(int skip)
{
  std::ostringstream out;
  get_string(out, skip);
  
 next:
  next_line();
  if (current_line.size() >= 1 && current_line[0] == '"')
    {
      get_string(out, 0);
      goto next;
    }

  return out.str();
}

bool
POParser::is_empty_line()
{
  for(std::string::iterator i = current_line.begin(); i != current_line.end(); ++i)
    {
      if (!isspace(*i))
        return false;
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
  // Parser structure
  while(!eof)
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
          if (prefix("msgctxt "))
            {
              has_msgctxt = true;
              msgctxt = get_string(8);
            }

          if (prefix("msgid "))
            msgid = get_string(6);
          else
            error("Expected msgid");

          if (prefix("msgid_plural "))
            {
              std::string msgid_plural = get_string(13);
              std::map<int, std::string> msgstr_num;

            next:
              if (is_empty_line())
                {
                  if (msgstr_num.empty())
                    error("Expected msgstr[N]");
                }
              else if (prefix("msgstr[") &&
                  current_line.size() > 8 && 
                  isdigit(current_line[7]) && current_line[8] == ']')
                {                 
                  int number = current_line[7] - '0';
                  msgstr_num[number] = get_string(10);
                  goto next;
                }
              else 
                {
                  error("Expected msgstr[N]");
                }

              if (!is_empty_line())
                error("Expected msgstr[N] or empty line");

              std::cout << "msgid \"" << msgid << "\"" << std::endl;
              std::cout << "msgid_plural \"" << msgid_plural << "\"" << std::endl;
              for(std::map<int, std::string>::iterator i = msgstr_num.begin(); i != msgstr_num.end(); ++i)
                std::cout << "msgstr[" << i->first << "] \"" << i->second << "\"" << std::endl;
              std::cout << std::endl;
            }
          else if (prefix("msgstr "))
            {
              std::string msgstr = get_string(7);

              std::cout << "msgid \"" << msgid << "\"" << std::endl;
              std::cout << "msgstr \"" << msgstr << "\"" << std::endl;
              std::cout << std::endl;
            }
          else
            {
              error("Expected msgstr or msgid_plural");
            }
        }
      
      if (!is_empty_line())
        error("Expected empty line");

      next_line();
    }
}

} // namespace tinygettext

/* EOF */
