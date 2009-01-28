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

#include <string.h>
#include <fstream>
#include <iostream>
#include "tinygettext/tinygettext.hpp"

#define _(String) gettext(String)
#define gettext(String) dict.translate(String)
#define ngettext(msgid, msgid2, num) dict.translate_plural(msgid, msgid2, num)

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      std::cout << "Usage: tinygettext [FILE] ..." << std::endl;
    }
/*
  else if (argc == 2 && strcmp(argv[1], "--mgr") == 0)
    {
      std::cout << "Using DictionaryManager" << std::endl;
      tinygettext::DictionaryManager dictmgr;

      dictmgr.set_language("de");
      dictmgr.add_directory("game");
      dictmgr.add_directory("level");
      
      std::cout << dictmgr.get_dictionary().translate("disabled") << std::endl;
      std::cout << dictmgr.get_dictionary().translate("...walking...") << std::endl;
      int num = 3;
      printf(dictmgr.get_dictionary().translate("You got %d error.\n", 
                                                "You got %d errors.\n", num).c_str(), num);
    }
*/
  else
    {
      tinygettext::Dictionary dict;

      for (int i = 1; i < argc; ++i)
        {
          std::ifstream in(argv[i]);
          tinygettext::POFileReader::read(in, dict);
        }

      std::cout << _("disabled") << std::endl;
      std::cout << _("...walking...") << std::endl;
      int num = 3;
      printf(ngettext("You got %d error.\n", 
                      "You got %d errors.\n", num), num);
      num = 1;
      printf(ngettext("You got %d error.\n", 
                      "You got %d errors.\n", num), num);
    }
}

/* EOF */

