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

#include <stdlib.h>
#include <fstream>
#include <algorithm>

#include "tinygettext.hpp"
#include "dictionary_manager.hpp"
#include "log.hpp"

namespace tinygettext {

static bool has_prefix(const std::string& lhs, const std::string rhs)
{
  if (lhs.length() < rhs.length())
    return false;
  else
    return lhs.compare(0, rhs.length(), rhs) == 0;
}

static bool has_suffix(const std::string& lhs, const std::string rhs)
{
  if (lhs.length() < rhs.length())
    return false;
  else
    return lhs.compare(lhs.length() - rhs.length(), rhs.length(), rhs) == 0;
}

DictionaryManager::DictionaryManager()
  : current_dict(&empty_dict)
{
  dir_op.enumerate_files = unix_enumerate_files;
  dir_op.free_list       = unix_free_list;
  dir_op.open_file       = unix_open_file;
}

Dictionary&
DictionaryManager::get_dictionary(Language language)
{
  //log_debug << "Dictionary for language \"" << spec << "\" requested" << std::endl;

  //log_debug << "...normalized as \"" << lang << "\"" << std::endl;

  Dictionaries::iterator i = dictionaries.find(language); 
  if (i != dictionaries.end())
    {
      return i->second;
    }
  else // Dictionary for languages lang isn't loaded, so we load it
    {
      //log_debug << "get_dictionary: " << lang << std::endl;
      Dictionary& dict = dictionaries[language];

      dict.set_language(language);

      if (charset != "")
        dict.set_charset(charset);

      for (SearchPath::iterator p = search_path.begin(); p != search_path.end(); ++p)
        {
          char** files = dir_op.enumerate_files(p->c_str());
          if(!files)
            {
              log_warning << "Error: enumerateFiles() failed on " << *p << std::endl;
            }
          else
            {
              for(const char* const* filename = files;
                  *filename != 0; filename++) 
                {
                  // check if filename matches requested language
                  if (has_suffix(*filename, ".po"))
                    { // ignore anything that isn't a .po file
                      
                      // 
                      if (has_prefix(*filename, language->code))
                        {
                          //log_debug << "Loading dictionary for language \"" << lang << "\" from \"" << filename << "\"" << std::endl;
                          std::string pofile = *p + "/" + *filename;
                          try 
                            {
                              std::istream* in = dir_op.open_file(pofile.c_str());
                              POFileReader::read(*in, dict);
                              delete in;
                            } 
                          catch(std::exception& e) 
                            {
                              log_warning << "Error: Failure file opening: " << pofile << std::endl;
                              log_warning << e.what() << "" << std::endl;
                            }
                        }
                    }
                }
              dir_op.free_list(files);
            }
        }

      return dict;
    }
}

std::set<std::string>
DictionaryManager::get_languages()
{
  std::set<std::string> languages;

  for (SearchPath::iterator p = search_path.begin(); p != search_path.end(); ++p)
    {
      char** files = dir_op.enumerate_files(p->c_str());
      if (!files)
        {
          log_warning << "Error: opendir() failed on " << *p << std::endl;
        }
      else
        {
          for(const char* const* file = files; *file != 0; file++) {
            if(has_suffix(*file, ".po")) {
              std::string filename = *file;
              languages.insert(filename.substr(0, filename.length()-3));
            }
          }
          dir_op.free_list(files);
        }
    }
  return languages;
}

void
DictionaryManager::set_language(Language language)
{
  //log_debug << "set_language \"" << lang << "\"" << std::endl;
  //log_debug << "==> \"" << language << "\"" << std::endl;
  current_dict = &(get_dictionary(language));
}

Language
DictionaryManager::get_language() const
{
  return current_dict->get_language();
}

void
DictionaryManager::set_charset(const std::string& charset_)
{
  Language current_language = current_dict->get_language();
  dictionaries.clear(); // changing charset invalidates cache
  charset = charset_;
  set_language(current_language);
}

void
DictionaryManager::add_directory(const std::string& pathname)
{
  Language current_language = current_dict->get_language();
  dictionaries.clear(); // adding directories invalidates cache
  search_path.push_back(pathname);
  set_language(current_language); // FIXME: Seems very stupid, since it triggers a re-read of the .po files
}

void
DictionaryManager::set_dir_op(const DirOp& dir_op_)
{
  dir_op = dir_op_;
}

} // namespace tinygettext

/* EOF */
