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

typedef int (*PluralFunc)(int n);

struct LanguageDef {
  /** Language code: "de", "pt_BR", ... */
  const char* code;

  /** Language name: "German", "English", "French", ... */
  const char* name;

  int         nplural;
  PluralFunc  plural;

  LanguageDef(const char* code_, const char* name_,  int nplural_, PluralFunc plural_)
    : code(code_), name(name_), nplural(nplural_), plural(plural_)
  {}
};

typedef LanguageDef* Language;

Language get_language_def(const std::string& name);

// Englishe Language defaults
extern LanguageDef lang_en;
int plural2_1(int n);

} // namespace tinygettext

#endif

/* EOF */
