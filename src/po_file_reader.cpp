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

#include <errno.h>
#include <istream>
#include <iconv.h>
#include "po_file_reader.hpp"
#include "log.hpp"
#include "dictionary.hpp"

namespace tinygettext {

static std::string iconv_convert(const std::string& text,
                          const std::string& from_charset,
                          const std::string& to_charset)
{
#ifdef HAVE_SDL
  if (from_charset == to_charset)
    {
      return text;
    }
  else
    {
      char *out = SDL_iconv_string(to_charset.c_str(), from_charset.c_str(), text.c_str(), text.length() + 1);

      if(out == 0)
        {
          log_warning << "Error: conversion from '" << from_charset << "' to '" << to_charset << "' failed" << std::endl;
          return text;
        }
      else
        {
          std::string ret(out);
          SDL_free(out);
          return ret;
        }
    }
#else

#ifndef ICONV_CONST
#  define ICONV_CONST
#endif

  iconv_t cd = iconv_open(to_charset.c_str(), from_charset.c_str());

  size_t in_len  = text.length();
  size_t out_len = text.length()*4;

  char*  out_orig = new char[out_len];
  char*  in_orig  = new char[in_len+1];
  strcpy(in_orig, text.c_str());

  char* out = out_orig;
  ICONV_CONST char* in  = in_orig;
  size_t out_len_temp = out_len; // iconv is counting down the bytes it has
                                 // written from this...

  size_t retval = iconv(cd, &in, &in_len, &out, &out_len_temp);
  out_len -= out_len_temp; // see above
  if (retval == (size_t) -1)
    {
      log_warning << strerror(errno) << std::endl;
      log_warning << "Error: conversion from '" << from_charset << "' to '" << to_charset << "' went wrong: " << retval << std::endl;
      return "";
    }
  iconv_close(cd);

  std::string ret(out_orig, out_len);
  delete[] out_orig;
  delete[] in_orig;
  return ret;
#endif
}

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

          std::vector<std::string> msgstr_plural;
          while((token = nextToken()) == TOKEN_KEYWORD && has_prefix(tokenContent, "msgstr["))
            {
              int num;
              if (sscanf(tokenContent.c_str(), "msgstr[%d]", &num) != 1)
                {
                  log_warning << "Error: Couldn't parse: " << tokenContent << std::endl;
                }

              token = nextToken();
              if(!expectToken("msgstr[x] content", TOKEN_CONTENT)) break;

              if (num >= (int)msgstr_plural.size())
                msgstr_plural.resize(num+1);

              msgstr_plural[num] = iconv_convert(tokenContent, from_charset, dict.get_charset());
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
              dict.add_translation(current_msgid, iconv_convert(tokenContent, from_charset, dict.get_charset()));
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

  // FIXME: This is way to tolerant, real gettext is much more picky
  // about the format (only a single space between msgid and the
  // folowing "" and stuff like that). It would likely be best if this
  // worked line by line instead of just eating up tokens no matter
  // what space
  skipSpace();

  if(c == EOF)
    {
      return TOKEN_EOF;
    }
  else if(c != '"')
    {
      // Read a keyword
      do 
        {
          tokenContent += c;
          nextChar();
        } 
      while(c != EOF && !isspace(static_cast<unsigned char>(c)));

      log_info << "Read Keyword: " << tokenContent << std::endl;

      return TOKEN_KEYWORD;
    }
  else
    {
      do {
        nextChar();
        // Read content
        while(c != EOF && c != '"') 
          {
            if (c == '\\') 
              {
                nextChar();
                if      (c == 'n')  c = '\n';
                else if (c == 't')  c = '\t';
                else if (c == 'r')  c = '\r';
                else if (c == '"')  c = '"';
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
        if(c == EOF) 
          {
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
