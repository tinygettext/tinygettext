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

#include <stdlib.h>
#include <fstream>
#include <algorithm>

#include "tinygettext.hpp"
#include "dictionary_manager.hpp"
#include "log.hpp"
#include "findlocale.hpp"

namespace tinygettext {

DictionaryManager::DictionaryManager()
  : current_dict(&empty_dict)
{
  dir_op.enumerate_files = unix_enumerate_files;
  dir_op.free_list       = unix_free_list;
  dir_op.open_file       = unix_open_file;

  parseLocaleAliases();
  // Environment variable SUPERTUX_LANG overrides language settings.
  const char* lang = getenv( "SUPERTUX_LANG" );
  if( lang ){
    set_language( lang );
    return;
  }
  // use findlocale to setup language
  FL_Locale *locale;
  FL_FindLocale( &locale, FL_MESSAGES );
  if(locale->lang) {
    if (locale->country) {
      set_language( std::string(locale->lang)+"_"+std::string(locale->country) );
    } else {
      set_language( std::string(locale->lang) );
    }
  }
  FL_FreeLocale( &locale );
}

void
DictionaryManager::parseLocaleAliases()
{
  // try to parse language alias list
  std::ifstream in("/usr/share/locale/locale.alias");

  char c = ' ';
  while(in.good() && !in.eof()) {
    while(isspace(static_cast<unsigned char>(c)) && !in.eof())
      in.get(c);

    if(c == '#') { // skip comments
      while(c != '\n' && !in.eof())
        in.get(c);
      continue;
    }

    std::string alias;
    while(!isspace(static_cast<unsigned char>(c)) && !in.eof()) {
      alias += c;
      in.get(c);
    }
    while(isspace(static_cast<unsigned char>(c)) && !in.eof())
      in.get(c);
    std::string language;
    while(!isspace(static_cast<unsigned char>(c)) && !in.eof()) {
      language += c;
      in.get(c);
    }

    if(in.eof())
      break;
    set_language_alias(alias, language);
  }
}

Dictionary&
DictionaryManager::get_dictionary(const std::string& spec)
{
  //log_debug << "Dictionary for language \"" << spec << "\" requested" << std::endl;

  std::string lang = get_language_from_spec(spec);

  //log_debug << "...normalized as \"" << lang << "\"" << std::endl;

  Dictionaries::iterator i = dictionaries.find(get_language_from_spec(lang));
  if (i != dictionaries.end())
    {
      return i->second;
    }
  else // Dictionary for languages lang isn't loaded, so we load it
    {
      //log_debug << "get_dictionary: " << lang << std::endl;
      Dictionary& dict = dictionaries[lang];

      dict.set_language(get_language_def(lang));
      if(charset != "")
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
                  *filename != 0; filename++) {

                // check if filename matches requested language
                std::string fname = std::string(*filename);
                std::string load_from_file = "";
                if(fname == lang + ".po") {
                  load_from_file = fname;
                } else {
                  std::string::size_type s = lang.find("_");
                  if(s != std::string::npos) {
                    std::string lang_short = std::string(lang, 0, s);
                    if (fname == lang_short + ".po") {
                      load_from_file = lang_short;
                    }
                  }
                }

                // if it matched, load dictionary
                if (load_from_file != "") {
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
              dir_op.free_list(files);
            }
        }

      return dict;
    }
}

static bool has_suffix(const std::string& lhs, const std::string rhs)
{
  if (lhs.length() < rhs.length())
    return false;
  else
    return lhs.compare(lhs.length() - rhs.length(), rhs.length(), rhs) == 0;
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
DictionaryManager::set_language(const std::string& lang)
{
  //log_debug << "set_language \"" << lang << "\"" << std::endl;
  language = get_language_from_spec(lang);
  //log_debug << "==> \"" << language << "\"" << std::endl;
  current_dict = & (get_dictionary(language));
}

const std::string&
DictionaryManager::get_language() const
{
  return language;
}

void
DictionaryManager::set_charset(const std::string& charset)
{
  dictionaries.clear(); // changing charset invalidates cache
  this->charset = charset;
  set_language(language);
}

void
DictionaryManager::set_language_alias(const std::string& alias,
                                      const std::string& language)
{
  language_aliases.insert(std::make_pair(alias, language));
}

std::string
DictionaryManager::get_language_from_spec(const std::string& spec)
{
  std::string lang = spec;
  Aliases::iterator i = language_aliases.find(lang);
  if(i != language_aliases.end()) {
    lang = i->second;
  }

  std::string::size_type s = lang.find(".");
  if(s != std::string::npos) {
    lang = std::string(lang, 0, s);
  }

  s = lang.find("_");
  if(s == std::string::npos) {
    std::string lang_big = lang;
    std::transform (lang_big.begin(), lang_big.end(), lang_big.begin(), toupper);
    lang += "_" + lang_big;
  }

  return lang;
}

void
DictionaryManager::add_directory(const std::string& pathname)
{
  dictionaries.clear(); // adding directories invalidates cache
  search_path.push_back(pathname);
  set_language(language);
}

void
DictionaryManager::set_dir_op(const DirOp& dir_op_)
{
  dir_op = dir_op_;
}

} // namespace tinygettext

/* EOF */
