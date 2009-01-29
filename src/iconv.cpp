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

#include <ctype.h>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <stdexcept>
#include <string.h>
#include <stdlib.h>

#include "iconv.hpp"
#include "log.hpp"

namespace tinygettext {

#ifndef ICONV_CONST
#  define ICONV_CONST 
#endif

IConv::IConv() 
  : m_conv(0)
{}
 
IConv::IConv(const std::string& from_charset_, const std::string& to_charset_)
  : to_charset(to_charset_),
    from_charset(from_charset_)
{
  for(std::string::iterator i = to_charset.begin(); i != to_charset.end(); ++i)
    *i = tolower(*i);

  for(std::string::iterator i = from_charset.begin(); i != from_charset.end(); ++i)
    *i = tolower(*i);

  if (to_charset == from_charset)
    {
      m_conv = 0;
    }
  else
    {
      m_conv = iconv_open(to_charset.c_str(), from_charset.c_str());
      if (m_conv == (iconv_t)-1)
        {
          if(errno == EINVAL)
            {
              std::ostringstream str;
              str << "IConv construction failed: conversion from '" << from_charset
                  << "' to '" << to_charset << "' not available";
              throw std::runtime_error(str.str());
            }
          else
            {
              std::ostringstream str;
              str << "IConv: construction failed: " << strerror(errno);
              throw std::runtime_error(str.str());
            }
        }
    }
}
 
IConv::~IConv()
{
  if (m_conv)
    iconv_close(m_conv);
}
 
/// Convert a string from encoding to another.
std::string
IConv::convert(const std::string& text)
{
  if (!m_conv)
    {
      return text;
    }
  else
    {
      size_t inbytesleft  = text.size();
      size_t outbytesleft = 4*inbytesleft; // Worst case scenario: ASCII -> UTF-32?

      // We try to avoid to much copying around, so we write directly into
      // a std::string
      ICONV_CONST char* inbuf = const_cast<char*>(text.c_str());

      std::string result(outbytesleft, 'X');
      char* outbuf = &result[0];
 
      // Try to convert the text.
      size_t ret = iconv(m_conv, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
      if (ret == (size_t)-1)
        {
          if (errno == EILSEQ || errno == EINVAL)
            { // invalid multibyte sequence
              iconv(m_conv, NULL, NULL, NULL, NULL); // reset state

              // FIXME: Could try to skip the invalid byte and continue
              log_warning << "tinygettext:iconv: invalid multibyte sequence in:  \"" << text << "\"" << std::endl;
            }
          else if (errno == E2BIG)
            { // output buffer to small
              assert(!"tinygettext/iconv.cpp: E2BIG: This should never be reached");
            }
          else if (errno == EBADF)
            {
              assert(!"tinygettext/iconv.cpp: EBADF: This should never be reached");
            }
          else
            {
              assert(!"tinygettext/iconv.cpp: <unknown>: This should never be reached");
            }
        }

      result.resize(text.size() - outbytesleft);

      return result;
    }
}

} // namespace tinygettext

/* EOF */
