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

#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include "tinygettext/tinygettext.hpp"
#include "tinygettext/dictionary.hpp"
#include "tinygettext/dictionary_manager.hpp"
#include "tinygettext/po_file_reader.hpp"

using namespace tinygettext;

void print_usage(int argc, char** argv)
{
  std::cout << "Usage: " << argv[0] << " translate FILE MESSAGE" << std::endl;
  std::cout << "       " << argv[0] << " translate FILE MESSAGE_S MESSAGE_P NUM" << std::endl;
  std::cout << "       " << argv[0] << " directory DIRECTORY MESSAGE [LANG]" << std::endl;
}

void read_dictionary(const std::string& filename, Dictionary& dict)
{
  std::ifstream in(filename.c_str());
  
  if (!in)
    {
      throw std::runtime_error("Couldn't open " + filename);
    }
  else
    {
      POFileReader(in, dict);
      in.close();
    }
}

int main(int argc, char** argv)
{
  try 
    {
      if (argc == 4 && strcmp(argv[1], "translate") == 0)
        {
          const char* filename = argv[2];
          const char* message  = argv[3];

          Dictionary dict;
          read_dictionary(filename, dict);
          std::cout << dict.translate(message) << std::endl;
        }
      else if (argc == 6 && strcmp(argv[1], "translate") == 0)
        {
          const char* filename = argv[2];
          const char* message_singular = argv[3];
          const char* message_plural   = argv[4];
          int num = atoi(argv[5]);

          Dictionary dict;
          read_dictionary(filename, dict);
          std::cout << dict.translate(message_singular, message_plural, num) << std::endl;
        }
      else if ((argc == 4 || argc == 5) && strcmp(argv[1], "directory") == 0)
        {
          const char* directory = argv[2];
          const char* message   = argv[3];
          const char* language  = (argc == 5) ? argv[4] : NULL;
          
          DictionaryManager manager;
          manager.add_directory(directory);

          if (language)
            manager.set_language(language);

          std::cout << "Directory:   '" << directory << "'"  << std::endl;
          std::cout << "Message:     '" << message << "'" << std::endl;
          std::cout << "Language:    '" << language << "'" << std::endl;
          std::cout << "Translation: '" << manager.get_dictionary().translate(message) << std::endl;
        }
      else
        {
          print_usage(argc, argv);
        }
    } 
  catch(std::exception& err) 
    {
      std::cout << "Exception: " << err.what() << std::endl;
    }
  
  return 0;
}

/* EOF */
