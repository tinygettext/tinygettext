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

#include "log.hpp"
#include "dictionary.hpp"

namespace tinygettext {

Dictionary::Dictionary(const Language& language_, const std::string& charset_)
  : language(language_), charset(charset_)
{
}

std::string
Dictionary::get_charset() const
{
  return charset;
}

const char*
Dictionary::translate_plural(const char* msgid, const char* msgidplural, int num)
{
  return translate_plural(std::string(msgid), std::string(msgidplural), num).c_str();
}

std::string
Dictionary::translate_plural(const std::string& msgid, const std::string& msgid_plural, int num)
{
  return translate_plural(plural_entries, msgid, msgid_plural, num);
}

std::string
Dictionary::translate_plural(const PluralEntries& dict, const std::string& msgid, const std::string& msgid2, int num)
{
  PluralEntries::const_iterator i = dict.find(msgid);
  const std::map<int, std::string>& msgstrs = i->second;

  if (i != dict.end() && !msgstrs.empty())
    {
      int g = language.plural(num);
      std::map<int, std::string>::const_iterator j = msgstrs.find(g);
      if (j != msgstrs.end())
        {
          return j->second;
        }
      else
        {
          // Return the first translation, in case we can't translate the specific number
          return msgstrs.begin()->second;
        }
    }
  else
    {
      log_info << "Couldn't translate: " << msgid << std::endl;
      log_info << "Candidates: " << std::endl;
      for (PluralEntries::const_iterator i = dict.begin(); i != dict.end(); ++i)
        log_info << "'" << i->first << "'" << std::endl;

      if (num != 1) // default to english rules
        return msgid2;
      else
        return msgid;
    }
}

const char*
Dictionary::translate(const char* msgid)
{
  return translate(std::string(msgid)).c_str();
}

std::string
Dictionary::translate(const std::string& msgid)
{
  return translate(entries, msgid);
}

std::string
Dictionary::translate(const Entries& dict, const std::string& msgid)
{
  Entries::const_iterator i = dict.find(msgid);
  if (i != dict.end() && !i->second.empty())
    {
      return i->second;
    }
  else
    {
      log_info << "Couldn't translate: " << msgid << std::endl;
      return msgid;
    } 
}

std::string
Dictionary::translate_ctxt(const std::string& msgctxt, const std::string& msgid)
{
  CtxtEntries::iterator i = ctxt_entries.find(msgctxt);
  if (i != ctxt_entries.end())
    {
      return translate(i->second, msgid);
    }
  else
    {
      log_info << "Couldn't translate: " << msgid << std::endl;
      return msgid;
    }
}

const char* 
Dictionary::translate_ctxt(const char* msgctx, const char* msgid)
{
  return translate_ctxt(std::string(msgctx), std::string(msgid)).c_str();
}

std::string
Dictionary::translate_ctxt_plural(const std::string& msgctxt, 
                                  const std::string& msgid, const std::string& msgidplural, int num)
{
  CtxtPluralEntries::iterator i = ctxt_plural_entries.find(msgctxt);
  if (i != ctxt_plural_entries.end())
    {
      return translate_plural(i->second, msgid, msgidplural, num);
    }
  else
    {
      log_info << "Couldn't translate: " << msgid << std::endl;
      if (num != 1) // default to english
        return msgidplural;
      else
        return msgid;
    }
}

const char*
Dictionary::translate_ctxt_plural(const char* msgctxt, const char* msgid, const char* msgidplural, int num)
{
  return translate_ctxt_plural(std::string(msgctxt), std::string(msgid), std::string(msgidplural), num).c_str();
}

void
Dictionary::add_translation(const std::string& msgid, const std::string& ,
                            const std::map<int, std::string>& msgstrs)
{
  // Do we need msgid2 for anything? its after all supplied to the
  // translate call, so we just throw it away here
  plural_entries[msgid] = msgstrs;
}

void
Dictionary::add_translation(const std::string& msgid, const std::string& msgstr)
{
  entries[msgid] = msgstr;
}

void
Dictionary::add_translation(const std::string& msgctxt, 
                            const std::string& msgid, const std::string& msgid2,
                            const std::map<int, std::string>& msgstrs)
{
  ctxt_plural_entries[msgctxt][msgid] = msgstrs;
}

void
Dictionary::add_translation(const std::string& msgctxt, const std::string& msgid, const std::string& msgstr)
{
  ctxt_entries[msgctxt][msgid] = msgstr;
}

Language
Dictionary::get_language() const
{
  return language;
}

} // namespace tinygettext

/* EOF */
