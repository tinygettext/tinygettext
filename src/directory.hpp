//  $Id$
//
//  TinyGetText
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

#ifndef HEADER_DIRECTORY_HPP
#define HEADER_DIRECTORY_HPP

namespace tinygettext {

struct DirOp
{
  char** (*enumerate_files)(const char*);
  void   (*free_list)(char**);
  std::istream* (*open_file)(const char*);
};

std::istream* unix_open_file(const char*);
char** unix_enumerate_files(const char* directory);
void   unix_free_list(char** lst);

} // namespace tinygettext

#endif

/* EOF */
