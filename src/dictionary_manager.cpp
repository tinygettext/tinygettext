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

  init_language_aliases();

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
DictionaryManager::init_language_aliases()
{
  language_aliases["bokmal"]           = "nb_NO.ISO-8859-1";
  language_aliases["bokmål"]           = "nb_NO.ISO-8859-1";
  language_aliases["catalan"]          = "ca_ES.ISO-8859-1";
  language_aliases["croatian"]         = "hr_HR.ISO-8859-2";
  language_aliases["czech"]            = "cs_CZ.ISO-8859-2";
  language_aliases["danish"]           = "da_DK.ISO-8859-1";
  language_aliases["dansk"]            = "da_DK.ISO-8859-1";
  language_aliases["deutsch"]          = "de_DE.ISO-8859-1";
  language_aliases["dutch"]            = "nl_NL.ISO-8859-1";
  language_aliases["eesti"]            = "et_EE.ISO-8859-1";
  language_aliases["estonian"]         = "et_EE.ISO-8859-1";
  language_aliases["finnish"]          = "fi_FI.ISO-8859-1";
  language_aliases["français"]         = "fr_FR.ISO-8859-1";
  language_aliases["french"]           = "fr_FR.ISO-8859-1";
  language_aliases["galego"]           = "gl_ES.ISO-8859-1";
  language_aliases["galician"]         = "gl_ES.ISO-8859-1";
  language_aliases["german"]           = "de_DE.ISO-8859-1";
  language_aliases["greek"]            = "el_GR.ISO-8859-7";
  language_aliases["hebrew"]           = "he_IL.ISO-8859-8";
  language_aliases["hrvatski"]         = "hr_HR.ISO-8859-2";
  language_aliases["hungarian"]        = "hu_HU.ISO-8859-2";
  language_aliases["icelandic"]        = "is_IS.ISO-8859-1";
  language_aliases["italian"]          = "it_IT.ISO-8859-1";
  language_aliases["japanese"]         = "ja_JP.eucJP";
  language_aliases["japanese.euc"]     = "ja_JP.eucJP";
  language_aliases["ja_JP"]            = "ja_JP.eucJP";
  language_aliases["ja_JP.ujis"]       = "ja_JP.eucJP";
  language_aliases["japanese.sjis"]    = "ja_JP.SJIS";
  language_aliases["korean"]           = "ko_KR.eucKR";
  language_aliases["korean.euc"]       = "ko_KR.eucKR";
  language_aliases["ko_KR"]            = "ko_KR.eucKR";
  language_aliases["lithuanian"]       = "lt_LT.ISO-8859-13";
  language_aliases["no_NO"]            = "nb_NO.ISO-8859-1";
  language_aliases["no_NO.ISO-8859-1"] = "nb_NO.ISO-8859-1";
  language_aliases["norwegian"]        = "nb_NO.ISO-8859-1";
  language_aliases["nynorsk"]          = "nn_NO.ISO-8859-1";
  language_aliases["polish"]           = "pl_PL.ISO-8859-2";
  language_aliases["portuguese"]       = "pt_PT.ISO-8859-1";
  language_aliases["romanian"]         = "ro_RO.ISO-8859-2";
  language_aliases["russian"]          = "ru_RU.ISO-8859-5";
  language_aliases["slovak"]           = "sk_SK.ISO-8859-2";
  language_aliases["slovene"]          = "sl_SI.ISO-8859-2";
  language_aliases["slovenian"]        = "sl_SI.ISO-8859-2";
  language_aliases["spanish"]          = "es_ES.ISO-8859-1";
  language_aliases["swedish"]          = "sv_SE.ISO-8859-1";
  language_aliases["thai"]             = "th_TH.TIS-620";
  language_aliases["turkish"]          = "tr_TR.ISO-8859-9";
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
