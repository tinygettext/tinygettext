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

#include <map>
#include "language.hpp"

namespace tinygettext {

typedef unsigned int (*PluralFunc)(int n);

struct LanguageSpec {
  /** Language code: "de", "en", ... */
  const char* language;

  /** Country code: "BR", "DE", ..., can be 0 */
  const char* country;

  /** Language name: "German", "English", "French", ... */
  const char* name;

  int         nplural;

  PluralFunc  plural;

  LanguageSpec(const char* language_, const char* country_, const char* name_,  int nplural_, PluralFunc plural_)
    : language(language_),
      country(country_),
      name(name_), 
      nplural(nplural_), 
      plural(plural_)
  {}
};

/** 
 *  Plural functions are used to select a string that matches a given
 *  count. \a n is the count and the return value is the string index
 *  used in the .po file, for example:
 *   
 *   msgstr[0] = "You got %d error";
 *   msgstr[1] = "You got %d errors";        
 *          ^-- return value of plural function 
 */
unsigned int plural1(int )     { return 0; }
unsigned int plural2_1(int n)  { return (n != 1); }
unsigned int plural2_2(int n)  { return (n > 1); }
unsigned int plural3_lv(int n) { return (n%10==1 && n%100!=11 ? 0 : n != 0 ? 1 : 2); }
unsigned int plural3_ga(int n) { return n==1 ? 0 : n==2 ? 1 : 2; }
unsigned int plural3_lt(int n) { return (n%10==1 && n%100!=11 ? 0 : n%10>=2 && (n%100<10 || n%100>=20) ? 1 : 2); }
unsigned int plural3_1(int n)  { return (n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2); }
unsigned int plural3_sk(int n) { return (n==1) ? 0 : (n>=2 && n<=4) ? 1 : 2; }
unsigned int plural3_pl(int n) { return (n==1 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2); }
unsigned int plural3_sl(int n) { return (n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3); }

/** Language Definitions */
//*{
LanguageSpec lang_af   ("af", 0,    "Afrikaans",         2, plural2_1); // "nplurals=2; plural=(n != 1);"
// LanguageSpec lang_am   ("am", 0,    "Amharic",           );
// LanguageSpec lang_ar   ("ar", 0,    "Arabic",            ); 
// LanguageSpec lang_ast  ("ast",0,    "Asturian"  );
// LanguageSpec lang_az   ("az", 0,    "Azerbaijani");
// LanguageSpec lang_be   ("be", 0,    "Belarusian");
LanguageSpec lang_bg   ("bg", 0,    "Bulgarian",         2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_bs   ("bs", 0,    "Bosnian",           3, plural3_1); // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_ca   ("ca", 0,    "Catalan",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_cs   ("cs", 0,    "Czech",             3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_da   ("da", 0,    "Danish",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_de   ("de", 0,    "German",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_el   ("el", 0,    "Greek",             2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_en   ("en", 0,    "English",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_en_GB("en", "GB", "British English",   2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_eo   ("eo", 0,    "Esperanto",         2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es   ("es", 0,    "Spanish",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_et   ("et", 0,    "Estonian",          2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_eu   ("eu", 0,    "Basque",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
// LanguageSpec lang_fa   ("fa", 0,    "Persian");
LanguageSpec lang_fi   ("fi", 0,    "Finnish",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_fo   ("fo", 0,    "Faroese",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_fr   ("fr", 0,    "French",            2, plural2_2);    // "nplurals=2; plural=(n > 1);"
LanguageSpec lang_ga   ("ga", 0,    "Irish",             3, plural3_ga);   // "nplurals=3; plural=n==1 ? 0 : n==2 ? 1 : 2;"
LanguageSpec lang_gl   ("gl", 0,    "Galician",          2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_gu   ("gu", 0,    "Gujarati",          2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_he   ("he", 0,    "Hebrew",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
// LanguageSpec lang_hi   ("hi", 0,    "Hindi");
LanguageSpec lang_hr   ("hr", 0,    "Croatian",          3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_hu   ("hu", 0,    "Hungarian",         1, plural1);      // "nplurals=1; plural=0;"
// LanguageSpec lang_hy   ("hy", 0,    "Armenian");
LanguageSpec lang_id   ("id", 0,    "Indonesian",        1, plural1); // "nplurals=1; plural=0;"
// LanguageSpec lang_is   ("is", 0,    "Icelandic");
LanguageSpec lang_it   ("it", 0,    "Italian",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_ja   ("ja", 0,    "Japanese",          1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_ka   ("ka", 0,    "Georgian",          1, plural1);      // "nplurals=1; plural=0;"
// LanguageSpec lang_kn   ("kn", 0,    "Kannada");
LanguageSpec lang_ko   ("ko", 0,    "Korean",            1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_ku   ("ku", 0,    "Kurdish",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_ky   ("ky", 0,    "Kirghiz",           1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_lt   ("lt", 0,    "Lithuanian",        3, plural3_lt);   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_lv   ("lv", 0,    "Latvian",           3, plural3_lv);   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n != 0 ? 1 : 2);"
// LanguageSpec lang_mk   ("mk", 0,    "Macedonian");
// LanguageSpec lang_ml   ("ml", 0,    "Malayalam" );
// LanguageSpec lang_mn   ("mn", 0,    "Mongolian");
LanguageSpec lang_ms   ("ms", 0,    "Malay",             1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_mt   ("mt", 0,    "Maltese",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_nb   ("nb", 0,    "Norwegian Bokmal",  2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_ne   ("ne", 0,    "Nepali",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_nl   ("nl", 0,    "Dutch",             2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_nn   ("nn", 0,    "Norwegian Nynorsk", 2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_no   ("no", 0,    "Norwegian",         2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_pa   ("pa", 0,    "Punjabi",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_pl   ("pl", 0,    "Polish",            3, plural3_pl);   // "nplurals=3; plural=(n==1 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);
// LanguageSpec lang_ps   ("ps", 0,    "Pashto");
LanguageSpec lang_pt   ("pt", 0,    "Portuguese",        2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_pt_BR("pt", "BR", "Brazilian",         2, plural2_2);    // "nplurals=2; plural=(n > 1);"
LanguageSpec lang_ro   ("ro", 0,    "Romanian",          2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_ru   ("ru", 0,    "Russian",           3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_rw   ("rw", 0,    "Kinyarwanda",       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_sk   ("sk", 0,    "Slovak",            3, plural3_sk);   // "nplurals=3; plural=(n==1) ? 0 : (n>=2 && n<=4) ? 1 : 2;"
LanguageSpec lang_sl   ("sl", 0,    "Slovenian",         3, plural3_sl);   // "nplurals=4; plural=(n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3);"
LanguageSpec lang_sq   ("sq", 0,    "Albanian",          2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_sr   ("sr", 0,    "Serbian",           2, plural2_2);    // "nplurals=2; plural=n>1;"
LanguageSpec lang_sv   ("sv", 0,    "Swedish",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
// LanguageSpec lang_tg   ("tg", 0,    "Tajik");
// LanguageSpec lang_th   ("th", 0,    "Thai");
LanguageSpec lang_tr   ("tr", 0,    "Turkish",           1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_uk   ("uk", 0,    "Ukrainian",         3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_vi   ("vi", 0,    "Vietnamese",        1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_wa   ("wa", 0,    "Walloon",           2, plural2_2);    // "nplurals=2; plural=n > 1;"
// LanguageSpec lang_xh   ("xh", 0,    "Xhosa");
LanguageSpec lang_zh_CN("zh", "CN", "Chinese (simplified)",  1, plural1);  // "nplurals=1; plural=0;"
LanguageSpec lang_zh_HK("zh", "HK", "Chinese (Hong Kong)",   1, plural1);  // "nplurals=1; plural=0;"
LanguageSpec lang_zh_TW("zh", "TW", "Chinese (traditional)", 1, plural1);  // "nplurals=1; plural=0;"
// LanguageSpec lang_zu   ("zu", 0, "Zulu");
//*}

LanguageSpec*
get_language_spec(const std::string& language, const std::string& country)
{
  // FIXME: Bad
  if      (language == "cs") return &lang_cs;
  else if (language == "da") return &lang_da;
  else if (language == "de") return &lang_de;
  else if (language == "el") return &lang_el;
  else if (language == "en") return &lang_en;
  else if (language == "eo") return &lang_eo;
  else if (language == "es") return &lang_es;
  else if (language == "et") return &lang_et;
  else if (language == "fi") return &lang_fi;
  else if (language == "fo") return &lang_fo;
  else if (language == "fr") return &lang_fr;
  else if (language == "ga") return &lang_ga;
  else if (language == "he") return &lang_he;
  else if (language == "hr") return &lang_hr;
  else if (language == "hu") return &lang_hu;
  else if (language == "it") return &lang_it;
  else if (language == "ja") return &lang_ja;
  else if (language == "ko") return &lang_ko;
  else if (language == "lt") return &lang_lt;
  else if (language == "lv") return &lang_lv;
  else if (language == "nb") return &lang_nb;
  else if (language == "nl") return &lang_nl;
  else if (language == "nn") return &lang_nn;
  else if (language == "no") return &lang_no;
  else if (language == "pl") return &lang_pl;
  else if (language == "pt") return &lang_pt;
  else if (language == "pt") return &lang_pt_BR;
  else if (language == "ru") return &lang_ru;
  else if (language == "sk") return &lang_sk;
  else if (language == "sl") return &lang_sl;
  else if (language == "sr") return &lang_sr;
  else if (language == "sv") return &lang_sv;
  else if (language == "tr") return &lang_tr;
  else if (language == "uk") return &lang_uk;
  else if (language == "zh") return &lang_zh_TW;
  else return 0;
}

void
resolve_language_alias(std::string& name)
{
  typedef std::map<std::string, std::string> Aliases;
  static Aliases language_aliases;
  if (language_aliases.empty())
    {
      // FIXME: Many of those are not useful for us, since we leave
      // encoding to the app, not to the language
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

  std::string name_lowercase;
  name_lowercase.resize(name.size());
  for(std::string::size_type i = 0; i < name.size(); ++i)
    name_lowercase[i] = tolower(name[i]);

  Aliases::iterator i = language_aliases.find(name_lowercase);
  if (i != language_aliases.end()) 
    {
      name = i->second;
    }
}

Language::Language(const std::string& language, const std::string& country)
{
  language_spec = get_language_spec(language, country);
}

Language::Language(const std::string& spec_str_)
{
  std::string spec_str = spec_str_;

  resolve_language_alias(spec_str);
  
  { // Remove encoding from the language variable (i.e. "da_DK.ISO-8859-1")
    std::string::size_type s = spec_str.find(".");
    if (s != std::string::npos) 
      {
        spec_str = std::string(spec_str, 0, s);
      }
  }

  std::string language;
  std::string country;

  { // Bring language into a form of de_DE
    std::string::size_type s = spec_str.find("_");
    if (s != std::string::npos) 
      {
        language = spec_str.substr(0, s);
        country  = spec_str.substr(s+1);
      }
    else
      {
        language = spec_str;
      }
  }

  language_spec = get_language_spec(language, country);
}

Language::Language()
  : language_spec(0)
{
}

std::string
Language::get_language() const
{
  if (language_spec)
    return language_spec->language;
  else
    return "";
}

std::string
Language::get_country()  const
{
  if (language_spec && language_spec->country)
    return language_spec->country;
  else
    return "";
}


std::string
Language::get_name()  const
{
  if (language_spec)
    return language_spec->name;
  else
    return "";
}

unsigned int
Language::plural(int n) const
{
  if (language_spec)
    return language_spec->plural(n);
  else
    return 0;
}

int
Language::plural_count() const
{
  if (language_spec)
    return language_spec->nplural;
  else
    return 0;
}

} // namespace tinygettext

/* EOF */
