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

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <algorithm>

#include "po_parser.hpp"
#include "dictionary_manager.hpp"
#include "log.hpp"

namespace tinygettext {

static bool has_suffix(const std::string& lhs, const std::string rhs)
{
  if (lhs.length() < rhs.length())
    return false;
  else
    return lhs.compare(lhs.length() - rhs.length(), rhs.length(), rhs) == 0;
}

DictionaryManager::DictionaryManager(const std::string& charset_)
  : charset(charset_),
    use_fuzzy(true),
    current_dict(0),
    empty_dict()
{
  dir_op.enumerate_files = unix_enumerate_files;
  dir_op.free_list       = unix_free_list;
  dir_op.open_file       = unix_open_file;
}

DictionaryManager::~DictionaryManager()
{
  for(Dictionaries::iterator i = dictionaries.begin(); i != dictionaries.end(); ++i)
    {
      delete i->second;
    }
}

void
DictionaryManager::clear_cache()
{
  for(Dictionaries::iterator i = dictionaries.begin(); i != dictionaries.end(); ++i)
    {
      delete i->second;
    }
  dictionaries.clear();

  current_dict = 0;
}

Dictionary&
DictionaryManager::get_dictionary()
{
  if (current_dict)
    {
      return *current_dict; 
    }
  else
    {
      if (current_language)
        {
          current_dict = &get_dictionary(current_language);
          return *current_dict;
        }
      else
        {
          return empty_dict;
        }
    }
}

Dictionary&
DictionaryManager::get_dictionary(const Language& language)
{
  //log_debug << "Dictionary for language \"" << spec << "\" requested" << std::endl;
  //log_debug << "...normalized as \"" << lang << "\"" << std::endl;
  assert(language);

  Dictionaries::iterator i = dictionaries.find(language); 
  if (i != dictionaries.end())
    {
      return *i->second;
    }
  else // Dictionary for languages lang isn't loaded, so we load it
    {
      //log_debug << "get_dictionary: " << lang << std::endl;
      Dictionary* dict = new Dictionary(charset);

      dictionaries[language] = dict;

      for (SearchPath::reverse_iterator p = search_path.rbegin(); p != search_path.rend(); ++p)
        {
          char** files = dir_op.enumerate_files(p->c_str());
          if(!files)
            {
              log_error << *p << ": error: enumerateFiles() failed on " << std::endl;
            }
          else
            {
              const char* best_filename = 0;
              int best_score = 0;
              for(const char* const* filename = files; *filename != 0; filename++) 
                {
                  // check if filename matches requested language
                  if (has_suffix(*filename, ".po"))
                    { // ignore anything that isn't a .po file
                      Language po_language = Language::from_env(std::string(*filename, strlen(*filename)-3));

                      if (!po_language)
                        {
                          log_warning << *filename << ": warning: ignoring, unknown language" << std::endl;
                        }
                      else
                        {
                          int score = Language::match(language, po_language);
                          
                          if (score > best_score)
                            {
                              best_score = score;
                              best_filename = *filename;                                  
                            }
                        }
                    }
                }
              
              if (best_filename)
                {
                  std::string pofile = *p + "/" + best_filename;
                  try 
                    {
                      std::istream* in = dir_op.open_file(pofile.c_str());
                      if (!*in)
                        {
                          log_error << "error: failure opening: " << pofile << std::endl;
                        }
                      else
                        {
                          POParser::parse(pofile, *in, *dict);
                        }
                      delete in;
                    }
                  catch(std::exception& e) 
                    {
                      log_error << "error: failure parsing: " << pofile << std::endl;
                      log_error << e.what() << "" << std::endl;
                    }
                }
              dir_op.free_list(files);
            }
        }

      return *dict;
    }
}

std::set<Language>
DictionaryManager::get_languages()
{
  std::set<Language> languages;

  for (SearchPath::iterator p = search_path.begin(); p != search_path.end(); ++p)
    {
      char** files = dir_op.enumerate_files(p->c_str());
      if (!files)
        {
          log_error << "error: opendir() failed on " << *p << std::endl;
        }
      else
        {
          for(const char* const* file = files; *file != 0; file++) 
            {
              if(has_suffix(*file, ".po")) 
                {
                  languages.insert(Language::from_env(std::string(*file, strlen(*file)-3)));
                }
            }
          dir_op.free_list(files);
        }
    }
  return languages;
}

void
DictionaryManager::set_language(const Language& language)
{
  if (current_language != language)
    {
      current_language = language;
      current_dict     = 0;
    }
}

Language
DictionaryManager::get_language() const
{
  return current_language;
}

void
DictionaryManager::set_charset(const std::string& charset_)
{
  clear_cache(); // changing charset invalidates cache
  charset = charset_;
}

void
DictionaryManager::set_use_fuzzy(bool t)
{
  clear_cache();
  use_fuzzy = t;
}

bool
DictionaryManager::get_use_fuzzy(bool t) const
{
  return use_fuzzy;
}

void
DictionaryManager::add_directory(const std::string& pathname)
{
  clear_cache(); // adding directories invalidates cache
  search_path.push_back(pathname);
}

void
DictionaryManager::set_dir_op(const DirOp& dir_op_)
{
  dir_op = dir_op_;
}

} // namespace tinygettext

/* EOF */
