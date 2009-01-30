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
unsigned int plural2_mk(int n) { return n==1 || n%10==1 ? 0 : 1; }
unsigned int plural3_lv(int n) { return (n%10==1 && n%100!=11 ? 0 : n != 0 ? 1 : 2); }
unsigned int plural3_ga(int n) { return n==1 ? 0 : n==2 ? 1 : 2; }
unsigned int plural3_lt(int n) { return (n%10==1 && n%100!=11 ? 0 : n%10>=2 && (n%100<10 || n%100>=20) ? 1 : 2); }
unsigned int plural3_1(int n)  { return (n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2); }
unsigned int plural3_sk(int n) { return (n==1) ? 0 : (n>=2 && n<=4) ? 1 : 2; }
unsigned int plural3_pl(int n) { return (n==1 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2); }
unsigned int plural3_sl(int n) { return (n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3); }
unsigned int plural4_ar(int n) { return n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3; }

/** Language Definitions */
//*{
LanguageSpec lang_aa   ("aa", 0,    "Afar",                          2, plural2_1);    // unknown
LanguageSpec lang_af   ("af", 0,    "Afrikaans",                     2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_af_ZA("af", "ZA", "Afrikaans (South Africa)",      2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_am   ("am", 0,    "Amharic",                       2, plural2_1);    // unknown
LanguageSpec lang_ar   ("ar", 0,    "Arabic",                        4, plural4_ar);   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
LanguageSpec lang_ar_AR("ar", "AR", "Arabic (Argentina)",            4, plural4_ar);   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
LanguageSpec lang_ar_OM("ar", "OM", "Arabic (Oman)",                 4, plural4_ar);   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
LanguageSpec lang_ar_SA("ar", "SA", "Arabic (Saudi Arabia)",         4, plural4_ar);   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
LanguageSpec lang_ar_SY("ar", "SY", "Arabic (Syrian Arab Republic)", 4, plural4_ar);   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
LanguageSpec lang_ar_TN("ar", "TN", "Arabic (Tunisia)",              4, plural4_ar);   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
LanguageSpec lang_as   ("as", 0,    "Assamese",                      2, plural2_1);    // unknown
LanguageSpec lang_ast  ("ast",0,    "Asturian",                      2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_ay   ("ay", 0,    "Aymara",                        2, plural2_1);    // unknown
LanguageSpec lang_az   ("az", 0,    "Azerbaijani",                   1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_az_IR("az", "IR", "Azerbaijani (Iran)",            1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_be   ("be", 0,    "Belarusian",                    3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
// LanguageSpec lang_be@latin("be@latin", 0, "Belarusian")
LanguageSpec lang_bg   ("bg", 0,    "Bulgarian",                     2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_bg_BG("bg", "BG", "Bulgarian (Bulgaria)",          2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_bn   ("bn", 0,    "Bengali",                       2, plural2_1);    // unknown
LanguageSpec lang_bn_BD("bn", "BD", "Bengali (Bangladesh)",          2, plural2_1);    // unknown
LanguageSpec lang_bn_IN("bn", "IN", "Bengali (India)",               2, plural2_1);    // unknown
LanguageSpec lang_bo   ("bo", 0,    "Tibetan",                       2, plural2_1);    // unknown
LanguageSpec lang_br   ("br", 0,    "Breton",                        2, plural2_1);    // unknown
LanguageSpec lang_bs   ("bs", 0,    "Bosnian",                       3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_bs_BA("bs_BA", 0, "Bosnian (Bosnia/Herzegovina)",  3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_bs_BS("bs_BS", 0, "Bosnian (Bahamas)",             3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_ca   ("ca", 0,    "Catalan",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
//LanguageSpec lang_ca@valencia("ca@valencia", 0, "Unknown language")
LanguageSpec lang_ca_ES("ca", "ES", "Catalan (Spain)",               2, plural2_1);    // "nplurals=2; plural=(n != 1);"
//LanguageSpec lang_ca_ES@valencia("ca_ES@valencia", 0, "Unknown language")
LanguageSpec lang_co   ("co", 0,    "Corsican",                      2, plural2_1);    // unknown
LanguageSpec lang_cs   ("cs", 0,    "Czech",                         3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_cs_CZ("cs", "CZ", "Czech (Czech Republic)",        3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_cy   ("cy", 0,    "Welsh",                         2, plural2_1);    // unknown
LanguageSpec lang_cy_GB("cy", "GB", "Welsh (Great Britain)",         2, plural2_1);    // unknown
LanguageSpec lang_cz   ("cz", 0,    "Unknown language",              2, plural2_1);    // unknown
LanguageSpec lang_da   ("da", 0,    "Danish",                        2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_da_DK("da", "DK", "Danish (Denmark)",              2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_de   ("de", 0,    "German",                        2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_de_AT("de", "AT", "German (Austria)",              2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_de_CH("de", "CH", "German (Switzerland)",          2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_de_DE("de", "DE", "German (Germany)",              2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_dk   ("dk", 0,    "Unknown language",              2, plural2_1);    // unknown
LanguageSpec lang_dz   ("dz", 0,    "Dzongkha",                      2, plural2_1);    // unknown
LanguageSpec lang_el   ("el", 0,    "Greek",                         2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_el_GR("el", "GR",  "Greek (Greece)",               2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_en   ("en", 0,    "English",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
//LanguageSpec lang_en@boldquot("en@boldquot", 0, "Unknown language")
//LanguageSpec lang_en@quot("en@quot", 0, "Unknown language")
LanguageSpec lang_en_AU("en", "AU", "English (Australia)" ,          2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_en_CA("en", "CA", "English (Canada)",              2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_en_GB("en", "GB", "English (Great Britain)",       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_en_US("en", "US", "English (United States)",       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
//LanguageSpec lang_en_US@piglatin("en_US@piglatin", 0, "Unknown language",   2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_en_ZA("en", "ZA", "English (South Africa)",        2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_eo   ("eo", 0,    "Esperanto",                     2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es   ("es", 0,    "Spanish",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_AR("es", "AR", "Spanish (Argentina)",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_CL("es", "CL", "Spanish (Chile)",               2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_CO("es", "CO", "Spanish (Colombia)",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_CR("es", "CR", "Spanish (Costa Rica)",          2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_DO("es", "DO", "Spanish (Dominican Republic)",  2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_EC("es", "EC", "Spanish (Ecuador)",             2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_ES("es", "ES", "Spanish (Spain)",               2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_GT("es", "GT", "Spanish (Guatemala)",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_HN("es", "HN", "Spanish (Honduras)",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_LA("es", "LA", "Spanish (Laos)",                2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_MX("es", "MX", "Spanish (Mexico)",              2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_NI("es", "NI", "Spanish (Nicaragua)",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_PA("es", "PA", "Spanish (Panama)",              2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_PE("es", "PE", "Spanish (Peru)",                2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_PR("es", "PR", "Spanish (Puerto Rico)",         2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_SV("es", "SV", "Spanish (El Salvador)",         2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_UY("es", "UY", "Spanish (Uruguay)",             2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_es_VE("es", "VE", "Spanish (Venezuela)",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_et   ("et", 0,    "Estonian",                      2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_et_EE("et", "EE", "Estonian (Estonia)",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_et_ET("et", "ET", "Estonian (Ethiopia)",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_eu   ("eu", 0,    "Basque",                        2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_eu_ES("eu", "ES", "Basque (Spain)",                2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_fa   ("fa", 0,    "Persian",                       1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_fa_AF("fa_AF", 0, "Persian (Afghanistan)",         1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_fa_IR("fa_IR", 0, "Persian (Iran)",                1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_fi   ("fi", 0,    "Finnish",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_fi_FI("fi", "FI", "Finnish (Finland)",             2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_fo   ("fo", 0,    "Faroese",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_fo_FO("fo", "FO", "Faeroese (Faroe Islands)",      2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_fr   ("fr", 0,    "French",                        2, plural2_2);    // "nplurals=2; plural=(n > 1);"
LanguageSpec lang_fr_CA("fr", "CA", "French (Canada)",               2, plural2_2);    // "nplurals=2; plural=(n > 1);"
LanguageSpec lang_fr_CH("fr", "CH", "French (Switzerland)",          2, plural2_2);    // "nplurals=2; plural=(n > 1);"
LanguageSpec lang_fr_FR("fr", "FR", "French (France)",               2, plural2_2);    // "nplurals=2; plural=(n > 1);"
LanguageSpec lang_fr_LU("fr", "LU", "French (Luxembourg)",           2, plural2_2);    // "nplurals=2; plural=(n > 1);"
LanguageSpec lang_fy   ("fy", 0,    "Frisian",                       2, plural2_1);    // unknown
LanguageSpec lang_ga   ("ga", 0,    "Irish",                         3, plural3_ga);   // "nplurals=3; plural=n==1 ? 0 : n==2 ? 1 : 2;"
LanguageSpec lang_gd   ("gd", 0,    "Gaelic Scots",                  2, plural2_1);    // unknown
LanguageSpec lang_gl   ("gl", 0,    "Galician",                      2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_gl_ES("gl", "ES", "Galician (Spain)",              2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_gn   ("gn", 0,    "Guarani",                       2, plural2_1);    // unknown
LanguageSpec lang_gu   ("gu", 0,    "Gujarati",                      2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_gv   ("gv", 0,    "Manx",                          2, plural2_1);    // unknown
LanguageSpec lang_ha   ("ha", 0,    "Hausa",                         2, plural2_1);    // unknown
LanguageSpec lang_he   ("he", 0,    "Hebrew",                        2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_he_IL("he", "IL", "Hebrew (Israel)",               2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_hi   ("hi", 0,    "Hindi",                         2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_hr   ("hr", 0,    "Croatian",                      3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_hr_HR("hr", "HR", "Croatian (Croatia)",            3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_hu   ("hu", 0,    "Hungarian",                     1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_hu_HU("hu", "HU", "Hungarian (Hungary)",           1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_hy   ("hy", 0,    "Armenian",                      2, plural2_2);    // "nplurals=2; plural=n > 1;"
LanguageSpec lang_ia   ("ia", 0,    "Interlingua",                   2, plural2_1);    // unknown
LanguageSpec lang_id   ("id", 0,    "Indonesian",                    1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_id_ID("id", "ID", "Indonesian (Indonesia)",        1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_is   ("is", 0,    "Icelandic",                     2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_is_IS("is", "IS", "Icelandic (Iceland)",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_it   ("it", 0,    "Italian",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_it_CH("it", "CH", "Italian (Switzerland)",         2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_it_IT("it", "IT", "Italian (Italy)",               2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_iu   ("iu", 0,    "Inuktitut",                     2, plural2_1);    // unknown
LanguageSpec lang_ja   ("ja", 0,    "Japanese",                      1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_ja_JP("ja", "JP", "Japanese (Japan)",              1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_ka   ("ka", 0,    "Georgian",                      1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_kk   ("kk", 0,    "Kazakh",                        2, plural2_1);    // unknown
LanguageSpec lang_kl   ("kl", 0,    "Kalaallisut",                   2, plural2_1);    // unknown
LanguageSpec lang_km   ("km", 0,    "Khmer",                         2, plural2_1);    // unknown
LanguageSpec lang_km_KH("km", "KH", "Khmer (Cambodia)",              2, plural2_1);    // unknown
LanguageSpec lang_kn   ("kn", 0,    "Kannada",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_ko   ("ko", 0,    "Korean",                        1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_ko_KR("ko", "KR", "Korean (Korea)",                1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_ku   ("ku", 0,    "Kurdish",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_kw   ("kw", 0,    "Cornish",                       2, plural2_1);    // unknown
LanguageSpec lang_ky   ("ky", 0,    "Kirghiz",                       1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_la   ("la", 0,    "Latin",                         2, plural2_1);    // unknown
LanguageSpec lang_lo   ("lo", 0,    "Lao",                           2, plural2_1);    // unknown
LanguageSpec lang_lt   ("lt", 0,    "Lithuanian",                    3, plural3_lt);   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_lt_LT("lt", "LT", "Lithuanian (Lithuania)",        3, plural3_lt);   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_lv   ("lv", 0,    "Latvian",                       3, plural3_lv);   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n != 0 ? 1 : 2);"
LanguageSpec lang_lv_LV("lv", "LV", "Latvian (Latvia)",              3, plural3_lv);   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n != 0 ? 1 : 2);"
LanguageSpec lang_mg   ("mg", 0,    "Malagasy",                      2, plural2_1);    // unknown
LanguageSpec lang_mi   ("mi", 0,    "Maori",                         2, plural2_1);    // unknown
LanguageSpec lang_mk   ("mk", 0,    "Macedonian",                    2, plural2_mk);   // plural= n==1 || n%10==1 ? 0 : 1
LanguageSpec lang_mk_MK("mk", "MK", "Macedonian (Macedonia)",        2, plural2_mk);   // plural= n==1 || n%10==1 ? 0 : 1
LanguageSpec lang_ml   ("ml", 0,    "Malayalam",                     2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_mn   ("mn", 0,    "Mongolian",                     2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_mr   ("mr", 0,    "Marathi",                       2, plural2_1);    // unknown
LanguageSpec lang_ms   ("ms", 0,    "Malay",                         1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_ms_MY("ms", "MY", "Malay (Malaysia)",              1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_mt   ("mt", 0,    "Maltese",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_my   ("my", 0,    "Burmese",                       2, plural2_1);    // unknown
LanguageSpec lang_my_MM("my", "MM", "Burmese (Myanmar)",             2, plural2_1);    // unknown
LanguageSpec lang_nb   ("nb", 0,    "Norwegian Bokmal",              2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_nb_NO("nb", "NO", "Norwegian Bokmål (Norway)",     2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_ne   ("ne", 0,    "Nepali",                        2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_nl   ("nl", 0,    "Dutch",                         2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_nl_BE("nl", "BE", "Dutch (Belgium)",               2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_nl_NL("nl", "NL", "Dutch (Netherlands)",           2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_nn   ("nn", 0,    "Norwegian Nynorsk",             2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_nn_NO("nn_NO", 0, "Norwegian Nynorsk (Norway)",    2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_no   ("no", 0,    "Norwegian",                     2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_no_NO("no", "NO", "Norwegian (Norway)",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_no_NY("no", "NY", "Norwegian (NY)",                2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_nr   ("nr", 0,    "Ndebele, South",                2, plural2_1);    // unknown
LanguageSpec lang_oc   ("oc", 0,    "Occitan post 1500",             2, plural2_1);    // unknown
LanguageSpec lang_om   ("om", 0,    "Oromo",                         2, plural2_1);    // unknown
LanguageSpec lang_or   ("or", 0,    "Oriya",                         2, plural2_1);    // unknown
LanguageSpec lang_pa   ("pa", 0,    "Punjabi",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_pl   ("pl", 0,    "Polish",                        3, plural3_pl);   // "nplurals=3; plural=(n==1 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);
LanguageSpec lang_pl_PL("pl", "PL", "Polish (Poland)",               3, plural3_pl);   // "nplurals=3; plural=(n==1 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);
LanguageSpec lang_ps   ("ps", 0,    "Pashto",                        2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_pt   ("pt", 0,    "Portuguese",                    2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_pt_BR("pt", "BR", "Brazilian",                     2, plural2_2);    // "nplurals=2; plural=(n > 1);"
LanguageSpec lang_pt_PT("pt", "PT", "Portuguese (Portugal)",         2, plural2_2);    // "nplurals=2; plural=(n > 1);"
LanguageSpec lang_qu   ("qu", 0,    "Quechua",                       2, plural2_1);    // unknown
LanguageSpec lang_rm   ("rm", 0,    "Rhaeto-Romance",                2, plural2_1);    // unknown
LanguageSpec lang_ro   ("ro", 0,    "Romanian",                      2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_ro_RO("ro", "RO", "Romanian (Romania)",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_ru   ("ru", 0,    "Russian",                       3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_ru_RU("ru", "RU", "Russian (Russia",               3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_rw   ("rw", 0,    "Kinyarwanda",                   2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_sa   ("sa", 0,    "Sanskrit",                      2, plural2_1);    // unknown
LanguageSpec lang_sd   ("sd", 0,    "Sindhi",                        2, plural2_1);    // unknown
LanguageSpec lang_se   ("se", 0,    "Sami",                          2, plural2_1);    // unknown
LanguageSpec lang_se_NO("se", "NO", "Sami (Norway)",                 2, plural2_1);    // unknown
LanguageSpec lang_si   ("si", 0,    "Sinhalese",                     2, plural2_1);    // unknown
LanguageSpec lang_sk   ("sk", 0,    "Slovak",                        3, plural3_sk);   // "nplurals=3; plural=(n==1) ? 0 : (n>=2 && n<=4) ? 1 : 2;"
LanguageSpec lang_sk_SK("sk", "SK", "Slovak (Slovakia)",             3, plural3_sk);   // "nplurals=3; plural=(n==1) ? 0 : (n>=2 && n<=4) ? 1 : 2;"
LanguageSpec lang_sl   ("sl", 0,    "Slovenian",                     3, plural3_sl);   // "nplurals=4; plural=(n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3);"
LanguageSpec lang_sl_SI("sl", "SI", "Slovenian (Slovenia)",          3, plural3_sl);   // "nplurals=4; plural=(n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3);"
LanguageSpec lang_sl_SL("sl", "SL", "Slovenian (Sierra Leone)",      3, plural3_sl);   // "nplurals=4; plural=(n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3);"
LanguageSpec lang_sm   ("sm", 0,    "Samoan",                        2, plural2_1);    // unknown
LanguageSpec lang_so   ("so", 0,    "Somali",                        2, plural2_1);    // unknown
LanguageSpec lang_sp   ("sp", 0,    "Unknown language",              2, plural2_1);    // unknown
LanguageSpec lang_sq   ("sq", 0,    "Albanian",                      2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_sq_AL("sq", "AL", "Albanian (Albania)",            2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_sr   ("sr", 0,    "Serbian",                       3, plural3_1);    // "nplurals=3; plural=n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2;"
LanguageSpec lang_sr_YU("sr", "YU", "Serbian (Yugoslavia)",          3, plural3_1);    // "nplurals=3; plural=n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2;"
//LanguageSpec lang_sr@Latn("sr@Latn", 0, "Unknown language")        
//LanguageSpec lang_sr@ije("sr@ije", 0, "Unknown language")          
//LanguageSpec lang_sr@latin("sr@latin", 0, "Unknown language")       
//LanguageSpec lang_srLatn("sr@Latn", 0, "Serbian (Latn)", 3, plural3_1);    // "nplurals=3; plural=n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2;"
LanguageSpec lang_ss   ("ss", 0,    "Swati",                         2, plural2_1);    // unknown
LanguageSpec lang_st   ("st", 0,    "Sotho",                         2, plural2_1);    // unknown
LanguageSpec lang_sv   ("sv", 0,    "Swedish",                       2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_sv_SE("sv", "SE", "Swedish (Sweden)",              2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_sv_SV("sv", "SV", "Swedish (El Salvador)",         2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_sw   ("sw", 0,    "Swahili",                       2, plural2_1);    // unknown
LanguageSpec lang_ta   ("ta", 0,    "Tamil",                         2, plural2_1);    // unknown
LanguageSpec lang_te   ("te", 0,    "Telugu",                        2, plural2_1);    // unknown
LanguageSpec lang_tg   ("tg", 0,    "Tajik",                         2, plural2_1);    // unknown
LanguageSpec lang_th   ("th", 0,    "Thai",                          1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_th_TH("th", "TH", "Thai (Thailand)",               1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_ti   ("ti", 0,    "Tigrinya",                      2, plural2_1);    // unknown
LanguageSpec lang_tk   ("tk", 0,    "Turkmen",                       2, plural2_1);    // unknown
LanguageSpec lang_tl   ("tl", 0,    "Tagalog",                       2, plural2_1);    // unknown
LanguageSpec lang_to   ("to", 0,    "Tonga",                         2, plural2_1);    // unknown
LanguageSpec lang_tr   ("tr", 0,    "Turkish",                       1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_tr_TR("tr", "TR", "Turkish (Turkey)",              1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_ts   ("ts", 0,    "Tsonga",                        2, plural2_1);    // unknown
LanguageSpec lang_tt   ("tt", 0,    "Tatar",                         2, plural2_1);    // unknown
LanguageSpec lang_ug   ("ug", 0,    "Uighur",                        2, plural2_1);    // unknown
LanguageSpec lang_uk   ("uk", 0,    "Ukrainian",                     3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_uk_UA("uk", "UA", "Ukrainian (Ukraine)",           3, plural3_1);    // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageSpec lang_ur   ("ur", 0,    "Urdu",                          2, plural2_1);    // unknown
LanguageSpec lang_ur_PK("ur_PK", 0, "Urdu (Pakistan)",               2, plural2_1);    // unknown
LanguageSpec lang_uz   ("uz", 0,    "Uzbek",                         2, plural2_1);    // unknown
//LanguageSpec lang_uz@cyrillic("uz@cyrillic", 0, "Unknown language")
LanguageSpec lang_vi   ("vi", 0,    "Vietnamese",                    1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_vi_VN("vi", "VN", "Vietnamese (Vietnam)",          1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_wa   ("wa", 0,    "Walloon",                       2, plural2_2);    // "nplurals=2; plural=n > 1;"
LanguageSpec lang_wo   ("wo", 0,    "Wolof",                         2, plural2_1);    // unknown
LanguageSpec lang_xh   ("xh", 0,    "Xhosa",                         1, plural1);      // "nplurals=2; plural=(n != 1);"
LanguageSpec lang_yi   ("yi", 0,    "Yiddish",                       2, plural2_1);    // unknown
LanguageSpec lang_yo   ("yo", 0,    "Yoruba",                        2, plural2_1);    // unknown
LanguageSpec lang_zh   ("zh", 0,    "Chinese",                       1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_zh_CN("zh", "CN", "Chinese (simplified)",          1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_zh_HK("zh", "HK", "Chinese (Hong Kong)",           1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_zh_TW("zh", "TW", "Chinese (traditional)",         1, plural1);      // "nplurals=1; plural=0;"
LanguageSpec lang_zu   ("zu", 0,    "Zulu",                          2, plural2_1);    // guessed
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
      // Aliases taken from /etc/locale.alias
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
