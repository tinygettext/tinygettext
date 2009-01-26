//  $Id$
//
//  TinyGetText
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

#include "po_file_reader.hpp"
#include "log.hpp"
#include "dictionary.hpp"

namespace tinygettext {

static bool has_prefix(const std::string& lhs, const std::string rhs)
{
  if (lhs.length() < rhs.length())
    return false;
  else
    return lhs.compare(0, rhs.length(), rhs) == 0;
}

POFileReader::POFileReader(std::istream& in_, Dictionary& dict_)
  : in(in_), dict(dict_)
{
  line_num = 0;
  nextChar();

  // skip UTF-8 intro that some text editors produce
  // see http://en.wikipedia.org/wiki/Byte-order_mark
  if(c == 0xef)
    { 
      nextChar();
      nextChar();
      nextChar();
    }
  tokenize_po();
}

void
POFileReader::parse_header(const std::string& header)
{
  // Separate the header in lines
  typedef std::vector<std::string> Lines;
  Lines lines;

  std::string::size_type start = 0;
  for(std::string::size_type i = 0; i < header.length(); ++i)
    {
      if (header[i] == '\n')
        {
          lines.push_back(header.substr(start, i - start));
          start = i+1;
        }
    }

  for(Lines::iterator i = lines.begin(); i != lines.end(); ++i)
    {
      if (has_prefix(*i, "Content-Type: text/plain; charset=")) {
        from_charset = i->substr(strlen("Content-Type: text/plain; charset="));
      }
    }

  if (from_charset.empty() || from_charset == "CHARSET")
    {
      log_warning << "Error: Charset not specified for .po, fallback to ISO-8859-1" << std::endl;
      from_charset = "ISO-8859-1";
    }

  to_charset = dict.get_charset();
  if (to_charset.empty())
    { // No charset requested from the dict, use utf-8
      to_charset = "utf-8";
      dict.set_charset(from_charset);
    }
}

void
POFileReader::nextChar()
{
  c = in.get();
  if (c == '\n')
    line_num++;
}

void
POFileReader::skipSpace()
{
  if(c == EOF)
    return;

  while(c == '#' || isspace(static_cast<unsigned char>(c))) {
    if(c == '#') {
      while(c != '\n' && c != EOF) nextChar();
    }
    nextChar();
  }
}

bool
POFileReader::expectToken(std::string type, Token wanted) {
  if(token != wanted) {
    log_warning << "Expected " << type << ", got ";
    if(token == TOKEN_EOF)
      log_warning << "EOF";
    else if(token == TOKEN_KEYWORD)
      log_warning << "keyword '" << tokenContent << "'";
    else
      log_warning << "string \"" << tokenContent << '"';

    log_warning << " at line " << line_num << std::endl;
    return false;
  }
  return true;
}

bool
POFileReader::expectContent(std::string type, std::string wanted) {
  if(tokenContent != wanted) {
    log_warning << "Expected " << type << ", got ";
    if(token == TOKEN_EOF)
      log_warning << "EOF";
    else if(token == TOKEN_KEYWORD)
      log_warning << "keyword '" << tokenContent << "'";
    else
      log_warning << "string \"" << tokenContent << '"';

    log_warning << " at line " << line_num << std::endl;
    return false;
  }
  return true;
}

void
POFileReader::tokenize_po()
{
  token = nextToken();
  while(token != TOKEN_EOF)
    {
      if(!expectToken("'msgid' keyword", TOKEN_KEYWORD) || !expectContent("'msgid' keyword", "msgid")) break;

      token = nextToken();
      if(!expectToken("name after msgid", TOKEN_CONTENT)) break;
      std::string current_msgid = tokenContent;

      token = nextToken();
      if(!expectToken("msgstr or msgid_plural", TOKEN_KEYWORD)) break;
      if(tokenContent == "msgid_plural")
        {
          //Plural form
          token = nextToken();
          if(!expectToken("msgid_plural content", TOKEN_CONTENT)) break;
          std::string current_msgid_plural = tokenContent;

          std::map<int, std::string> msgstr_plural;
          while((token = nextToken()) == TOKEN_KEYWORD && has_prefix(tokenContent, "msgstr["))
            {
              int num;
              if (sscanf(tokenContent.c_str(), "msgstr[%d]", &num) != 1)
                {
                  log_warning << "Error: Couldn't parse: " << tokenContent << std::endl;
                }

              token = nextToken();
              if(!expectToken("msgstr[x] content", TOKEN_CONTENT)) break;
              msgstr_plural[num] = convert(tokenContent, from_charset, to_charset);
            }
          dict.add_translation(current_msgid, current_msgid_plural, msgstr_plural);
          // No nextToken()
        }
      else
        {
          // "Ordinary" translation
          if(!expectContent("'msgstr' keyword", "msgstr")) break;

          token = nextToken();
          if(!expectToken("translation in msgstr", TOKEN_CONTENT)) break;

          if (current_msgid == "")
            { // .po Header is hidden in the msgid with the empty string
              parse_header(tokenContent);
            }
          else
            {
              dict.add_translation(current_msgid, convert(tokenContent, from_charset, to_charset));
            }
          token = nextToken();
        }
    }
}

POFileReader::Token
POFileReader::nextToken()
{
  //Clear token contents
  tokenContent = "";

  skipSpace();

  if(c == EOF)
    return TOKEN_EOF;
  else if(c != '"')
    {
      // Read a keyword
      do {
        tokenContent += c;
        nextChar();
      } while(c != EOF && !isspace(static_cast<unsigned char>(c)));
      log_info << "Read Keyword: " << tokenContent << std::endl;
      return TOKEN_KEYWORD;
    }
  else
    {
      do {
        nextChar();
        // Read content
        while(c != EOF && c != '"') {
          if (c == '\\') {
            nextChar();
            if (c == 'n') c = '\n';
            else if (c == 't') c = '\t';
            else if (c == 'r') c = '\r';
            else if (c == '"') c = '"';
            else if (c == '\\') c = '\\';
            else
              {
                log_warning << "Unhandled escape character: " << char(c) << std::endl;
                c = ' ';
              }
          }
          tokenContent += c;
          nextChar();
        }
        if(c == EOF) {
          log_warning << "Unclosed string literal: " << tokenContent << std::endl;
          return TOKEN_CONTENT;
        }

        // Read more strings?
        nextChar();
        skipSpace();
      } while(c == '"');
      log_info << "Read String: " << tokenContent << std::endl;
      return TOKEN_CONTENT;
    }
}

void
POFileReader::read(std::istream& in, Dictionary& dict)
{
  POFileReader reader(in, dict);
}

} // namespace tinygettext

/* EOF */
