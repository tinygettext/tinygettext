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

#ifndef HEADER_LANGUAGE_DEF_HPP
#define HEADER_LANGUAGE_DEF_HPP

#include <string>

namespace tinygettext {

struct LanguageSpec;

/** Lightweight wrapper around LanguageSpec */
class Language
{
private:
  LanguageSpec* language_spec;

  Language(LanguageSpec* language_spec);

public:
  /** Create a language from language and country code:
      Example: Languge("de", "DE"); */
  static Language from_spec(const std::string& language, const std::string& country = std::string(), const std::string& modifier = std::string());

  /** Create a language from language and country code:
      Example: Languge("deutsch"); 
      Example: Languge("de_DE"); */
  static Language from_name(const std::string& str);

  /** Create an undefined Language object */
  Language();

  operator bool() const { return language_spec; }

  std::string get_language() const;
  std::string get_country()  const;
  std::string get_name()     const;
  unsigned int plural(int n) const;
  int plural_count() const;
};

} // namespace tinygettext

#endif

/* EOF */
