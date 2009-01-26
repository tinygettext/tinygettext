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

#include <sys/types.h>
#include <fstream>
#include <dirent.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "directory.hpp"

namespace tinygettext {

char** unix_enumerate_files(const char* directory)
{
  DIR* dir = opendir(directory);
  if (!dir)
    {
      return NULL;
    }
  else
    {
      int len = 32;
      int i = 0;
      char** lst = (char**)malloc(sizeof(lst) * len);
      struct dirent* dp;
      while((dp = readdir(dir)) != 0)
        {
          lst[i] = strdup(dp->d_name);

          i += 1;

          if (i >= len)
            {
              len *= 2;
              lst = (char**)realloc(lst, sizeof(lst) * len);
            }
        }
      lst[i] = 0;

      closedir(dir);
      return lst;
    }
}

void unix_free_list(char** lst)
{
  for(char** i = lst; *i != 0; ++i)
    {
      free(*i);
    }
  free(lst);
}

std::istream* unix_open_file(const char* filename)
{
  return new std::ifstream(filename);
}

} // namespace tinygettext

/* EOF */
