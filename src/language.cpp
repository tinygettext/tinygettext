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
#include <vector>
#include "language.hpp"

namespace tinygettext {

typedef unsigned int (*PluralFunc)(int n);

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

struct LanguageSpec {
  /** Language code: "de", "en", ... */
  const char* language;

  /** Country code: "BR", "DE", ..., can be 0 */
  const char* country;

  /** Modifier/Varint: "Latn", "ije", "latin"..., can be 0 */
  const char* modifier;

  /** Language name: "German", "English", "French", ... */
  const char* name;

  int         nplural;
  PluralFunc  plural;
};

/** Language Definitions */
//*{
// FIXME: entries marked with 'unknown' have unknown plural forms
//LanguageSpec lang_en@boldquot("en@boldquot", 0, 0, "Unknown language")
//LanguageSpec lang_en@quot("en@quot", 0, 0, "Unknown language")
//LanguageSpec lang_en_US@piglatin("en_US@piglatin", 0, 0, "Unknown language",   2, plural2_1);    // "nplurals=2; plural=(n != 1);"
LanguageSpec languages[] = {
  { "aa", 0,    0, "Afar",                          2, plural2_1  },   // unknown
  { "af", 0,    0, "Afrikaans",                     2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "af", "ZA", 0, "Afrikaans (South Africa)",      2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "am", 0,    0, "Amharic",                       2, plural2_1  },   // unknown
  { "ar", 0,    0, "Arabic",                        4, plural4_ar },   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
  { "ar", "AR", 0, "Arabic (Argentina)",            4, plural4_ar },   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
  { "ar", "OM", 0, "Arabic (Oman)",                 4, plural4_ar },   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
  { "ar", "SA", 0, "Arabic (Saudi Arabia)",         4, plural4_ar },   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
  { "ar", "SY", 0, "Arabic (Syrian Arab Republic)", 4, plural4_ar },   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
  { "ar", "TN", 0, "Arabic (Tunisia)",              4, plural4_ar },   // "nplurals=4; plural=n==1 ? 0 : n==2 ? 1 : n>=3 && n<=10 ? 2 : 3"
  { "as", 0,    0, "Assamese",                      2, plural2_1  },   // unknown
  { "ast",0,    0, "Asturian",                      2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "ay", 0,    0, "Aymara",                        2, plural2_1  },   // unknown
  { "az", 0,    0, "Azerbaijani",                   1, plural1    },   // "nplurals=1; plural=0;"
  { "az", "IR", 0, "Azerbaijani (Iran)",            1, plural1    },   // "nplurals=1; plural=0;"
  { "be", 0,    0, "Belarusian",                    3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "be", 0, "latin", "Belarusian",                 3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "bg", 0,    0, "Bulgarian",                     2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "bg", "BG", 0, "Bulgarian (Bulgaria)",          2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "bn", 0,    0, "Bengali",                       2, plural2_1  },   // unknown
  { "bn", "BD", 0, "Bengali (Bangladesh)",          2, plural2_1  },   // unknown
  { "bn", "IN", 0, "Bengali (India)",               2, plural2_1  },   // unknown
  { "bo", 0,    0, "Tibetan",                       2, plural2_1  },   // unknown
  { "br", 0,    0, "Breton",                        2, plural2_1  },   // unknown
  { "bs", 0,    0, "Bosnian",                       3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "bs", "BA", 0, "Bosnian (Bosnia/Herzegovina)",  3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "bs", "BS", 0, "Bosnian (Bahamas)",             3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "ca", "ES", "valencia", "Catalan (valencia)",   2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "ca", "ES", 0, "Catalan (Spain)",               2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "ca", 0,    "valencia", "Catalan (valencia)",   2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "ca", 0,    0, "Catalan",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "co", 0,    0, "Corsican",                      2, plural2_1  },   // unknown
  { "cs", 0,    0, "Czech",                         3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "cs", "CZ", 0, "Czech (Czech Republic)",        3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "cy", 0,    0, "Welsh",                         2, plural2_1  },   // unknown
  { "cy", "GB", 0, "Welsh (Great Britain)",         2, plural2_1  },   // unknown
  { "cz", 0,    0, "Unknown language",              2, plural2_1  },   // unknown
  { "da", 0,    0, "Danish",                        2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "da", "DK", 0, "Danish (Denmark)",              2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "de", 0,    0, "German",                        2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "de", "AT", 0, "German (Austria)",              2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "de", "CH", 0, "German (Switzerland)",          2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "de", "DE", 0, "German (Germany)",              2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "dk", 0,    0, "Unknown language",              2, plural2_1  },   // unknown
  { "dz", 0,    0, "Dzongkha",                      2, plural2_1  },   // unknown
  { "el", 0,    0, "Greek",                         2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "el", "GR",  0, "Greek (Greece)",               2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "en", 0,    0, "English",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "en", "AU", 0, "English (Australia)" ,          2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "en", "CA", 0, "English (Canada)",              2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "en", "GB", 0, "English (Great Britain)",       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "en", "US", 0, "English (United States)",       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "en", "ZA", 0, "English (South Africa)",        2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "eo", 0,    0, "Esperanto",                     2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", 0,    0, "Spanish",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "AR", 0, "Spanish (Argentina)",           2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "CL", 0, "Spanish (Chile)",               2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "CO", 0, "Spanish (Colombia)",            2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "CR", 0, "Spanish (Costa Rica)",          2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "DO", 0, "Spanish (Dominican Republic)",  2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "EC", 0, "Spanish (Ecuador)",             2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "ES", 0, "Spanish (Spain)",               2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "GT", 0, "Spanish (Guatemala)",           2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "HN", 0, "Spanish (Honduras)",            2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "LA", 0, "Spanish (Laos)",                2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "MX", 0, "Spanish (Mexico)",              2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "NI", 0, "Spanish (Nicaragua)",           2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "PA", 0, "Spanish (Panama)",              2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "PE", 0, "Spanish (Peru)",                2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "PR", 0, "Spanish (Puerto Rico)",         2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "SV", 0, "Spanish (El Salvador)",         2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "UY", 0, "Spanish (Uruguay)",             2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "es", "VE", 0, "Spanish (Venezuela)",           2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "et", 0,    0, "Estonian",                      2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "et", "EE", 0, "Estonian (Estonia)",            2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "et", "ET", 0, "Estonian (Ethiopia)",           2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "eu", 0,    0, "Basque",                        2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "eu", "ES", 0, "Basque (Spain)",                2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "fa", 0,    0, "Persian",                       1, plural1    },   // "nplurals=1; plural=0;"
  { "fa", "AF", 0, "Persian (Afghanistan)",         1, plural1    },   // "nplurals=1; plural=0;"
  { "fa", "IR", 0, "Persian (Iran)",                1, plural1    },   // "nplurals=1; plural=0;"
  { "fi", 0,    0, "Finnish",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "fi", "FI", 0, "Finnish (Finland)",             2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "fo", 0,    0, "Faroese",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "fo", "FO", 0, "Faeroese (Faroe Islands)",      2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "fr", 0,    0, "French",                        2, plural2_2  },   // "nplurals=2; plural=(n > 1);"
  { "fr", "CA", 0, "French (Canada)",               2, plural2_2  },   // "nplurals=2; plural=(n > 1);"
  { "fr", "CH", 0, "French (Switzerland)",          2, plural2_2  },   // "nplurals=2; plural=(n > 1);"
  { "fr", "FR", 0, "French (France)",               2, plural2_2  },   // "nplurals=2; plural=(n > 1);"
  { "fr", "LU", 0, "French (Luxembourg)",           2, plural2_2  },   // "nplurals=2; plural=(n > 1);"
  { "fy", 0,    0, "Frisian",                       2, plural2_1  },   // unknown
  { "ga", 0,    0, "Irish",                         3, plural3_ga },   // "nplurals=3; plural=n==1 ? 0 : n==2 ? 1 : 2;"
  { "gd", 0,    0, "Gaelic Scots",                  2, plural2_1  },   // unknown
  { "gl", 0,    0, "Galician",                      2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "gl", "ES", 0, "Galician (Spain)",              2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "gn", 0,    0, "Guarani",                       2, plural2_1  },   // unknown
  { "gu", 0,    0, "Gujarati",                      2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "gv", 0,    0, "Manx",                          2, plural2_1  },   // unknown
  { "ha", 0,    0, "Hausa",                         2, plural2_1  },   // unknown
  { "he", 0,    0, "Hebrew",                        2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "he", "IL", 0, "Hebrew (Israel)",               2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "hi", 0,    0, "Hindi",                         2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "hr", 0,    0, "Croatian",                      3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "hr", "HR", 0, "Croatian (Croatia)",            3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "hu", 0,    0, "Hungarian",                     1, plural1    },   // "nplurals=1; plural=0;"
  { "hu", "HU", 0, "Hungarian (Hungary)",           1, plural1    },   // "nplurals=1; plural=0;"
  { "hy", 0,    0, "Armenian",                      2, plural2_2  },   // "nplurals=2; plural=n > 1;"
  { "ia", 0,    0, "Interlingua",                   2, plural2_1  },   // unknown
  { "id", 0,    0, "Indonesian",                    1, plural1    },   // "nplurals=1; plural=0;"
  { "id", "ID", 0, "Indonesian (Indonesia)",        1, plural1    },   // "nplurals=1; plural=0;"
  { "is", 0,    0, "Icelandic",                     2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "is", "IS", 0, "Icelandic (Iceland)",           2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "it", 0,    0, "Italian",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "it", "CH", 0, "Italian (Switzerland)",         2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "it", "IT", 0, "Italian (Italy)",               2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "iu", 0,    0, "Inuktitut",                     2, plural2_1  },   // unknown
  { "ja", 0,    0, "Japanese",                      1, plural1    },   // "nplurals=1; plural=0;"
  { "ja", "JP", 0, "Japanese (Japan)",              1, plural1    },   // "nplurals=1; plural=0;"
  { "ka", 0,    0, "Georgian",                      1, plural1    },   // "nplurals=1; plural=0;"
  { "kk", 0,    0, "Kazakh",                        2, plural2_1  },   // unknown
  { "kl", 0,    0, "Kalaallisut",                   2, plural2_1  },   // unknown
  { "km", 0,    0, "Khmer",                         2, plural2_1  },   // unknown
  { "km", "KH", 0, "Khmer (Cambodia)",              2, plural2_1  },   // unknown
  { "kn", 0,    0, "Kannada",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "ko", 0,    0, "Korean",                        1, plural1    },   // "nplurals=1; plural=0;"
  { "ko", "KR", 0, "Korean (Korea)",                1, plural1    },   // "nplurals=1; plural=0;"
  { "ku", 0,    0, "Kurdish",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "kw", 0,    0, "Cornish",                       2, plural2_1  },   // unknown
  { "ky", 0,    0, "Kirghiz",                       1, plural1    },   // "nplurals=1; plural=0;"
  { "la", 0,    0, "Latin",                         2, plural2_1  },   // unknown
  { "lo", 0,    0, "Lao",                           2, plural2_1  },   // unknown
  { "lt", 0,    0, "Lithuanian",                    3, plural3_lt },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "lt", "LT", 0, "Lithuanian (Lithuania)",        3, plural3_lt },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "lv", 0,    0, "Latvian",                       3, plural3_lv },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n != 0 ? 1 : 2);"
  { "lv", "LV", 0, "Latvian (Latvia)",              3, plural3_lv },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n != 0 ? 1 : 2);"
  { "mg", 0,    0, "Malagasy",                      2, plural2_1  },   // unknown
  { "mi", 0,    0, "Maori",                         2, plural2_1  },   // unknown
  { "mk", 0,    0, "Macedonian",                    2, plural2_mk },   // plural= n==1 || n%10==1 ? 0 : 1
  { "mk", "MK", 0, "Macedonian (Macedonia)",        2, plural2_mk },   // plural= n==1 || n%10==1 ? 0 : 1
  { "ml", 0,    0, "Malayalam",                     2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "mn", 0,    0, "Mongolian",                     2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "mr", 0,    0, "Marathi",                       2, plural2_1  },   // unknown
  { "ms", 0,    0, "Malay",                         1, plural1    },   // "nplurals=1; plural=0;"
  { "ms", "MY", 0, "Malay (Malaysia)",              1, plural1    },   // "nplurals=1; plural=0;"
  { "mt", 0,    0, "Maltese",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "my", 0,    0, "Burmese",                       2, plural2_1  },   // unknown
  { "my", "MM", 0, "Burmese (Myanmar)",             2, plural2_1  },   // unknown
  { "nb", 0,    0, "Norwegian Bokmal",              2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "nb", "NO", 0, "Norwegian Bokmål (Norway)",     2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "ne", 0,    0, "Nepali",                        2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "nl", 0,    0, "Dutch",                         2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "nl", "BE", 0, "Dutch (Belgium)",               2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "nl", "NL", 0, "Dutch (Netherlands)",           2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "nn", 0,    0, "Norwegian Nynorsk",             2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "nn", "NO", 0, "Norwegian Nynorsk (Norway)",    2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "no", 0,    0, "Norwegian",                     2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "no", "NO", 0, "Norwegian (Norway)",            2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "no", "NY", 0, "Norwegian (NY)",                2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "nr", 0,    0, "Ndebele, South",                2, plural2_1  },   // unknown
  { "oc", 0,    0, "Occitan post 1500",             2, plural2_1  },   // unknown
  { "om", 0,    0, "Oromo",                         2, plural2_1  },   // unknown
  { "or", 0,    0, "Oriya",                         2, plural2_1  },   // unknown
  { "pa", 0,    0, "Punjabi",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "pl", 0,    0, "Polish",                        3, plural3_pl },   // "nplurals=3; plural=(n==1 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);
  { "pl", "PL", 0, "Polish (Poland)",               3, plural3_pl },   // "nplurals=3; plural=(n==1 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);
  { "ps", 0,    0, "Pashto",                        2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "pt", 0,    0, "Portuguese",                    2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "pt", "BR", 0, "Brazilian",                     2, plural2_2  },   // "nplurals=2; plural=(n > 1);"
  { "pt", "PT", 0, "Portuguese (Portugal)",         2, plural2_2  },   // "nplurals=2; plural=(n > 1);"
  { "qu", 0,    0, "Quechua",                       2, plural2_1  },   // unknown
  { "rm", 0,    0, "Rhaeto-Romance",                2, plural2_1  },   // unknown
  { "ro", 0,    0, "Romanian",                      2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "ro", "RO", 0, "Romanian (Romania)",            2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "ru", 0,    0, "Russian",                       3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "ru", "RU", 0, "Russian (Russia",               3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "rw", 0,    0, "Kinyarwanda",                   2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "sa", 0,    0, "Sanskrit",                      2, plural2_1  },   // unknown
  { "sd", 0,    0, "Sindhi",                        2, plural2_1  },   // unknown
  { "se", 0,    0, "Sami",                          2, plural2_1  },   // unknown
  { "se", "NO", 0, "Sami (Norway)",                 2, plural2_1  },   // unknown
  { "si", 0,    0, "Sinhalese",                     2, plural2_1  },   // unknown
  { "sk", 0,    0, "Slovak",                        3, plural3_sk },   // "nplurals=3; plural=(n==1) ? 0 : (n>=2 && n<=4) ? 1 : 2;"
  { "sk", "SK", 0, "Slovak (Slovakia)",             3, plural3_sk },   // "nplurals=3; plural=(n==1) ? 0 : (n>=2 && n<=4) ? 1 : 2;"
  { "sl", 0,    0, "Slovenian",                     3, plural3_sl },   // "nplurals=4; plural=(n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3);"
  { "sl", "SI", 0, "Slovenian (Slovenia)",          3, plural3_sl },   // "nplurals=4; plural=(n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3);"
  { "sl", "SL", 0, "Slovenian (Sierra Leone)",      3, plural3_sl },   // "nplurals=4; plural=(n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3);"
  { "sm", 0,    0, "Samoan",                        2, plural2_1  },   // unknown
  { "so", 0,    0, "Somali",                        2, plural2_1  },   // unknown
  { "sp", 0,    0, "Unknown language",              2, plural2_1  },   // unknown
  { "sq", 0,    0, "Albanian",                      2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "sq", "AL", 0, "Albanian (Albania)",            2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "sr", 0,    0, "Serbian",                       3, plural3_1  },   // "nplurals=3; plural=n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2;"
  { "sr", "YU", 0, "Serbian (Yugoslavia)",          3, plural3_1  },   // "nplurals=3; plural=n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2;"
  { "sr", 0,"ije", "Serbian",                       3, plural3_1  },   // "nplurals=3; plural=n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2;"
  { "sr", 0, "latin", "Serbian",                       3, plural3_1  },   // "nplurals=3; plural=n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2;"
  { "sr", 0, "Latn",  "Serbian",                       3, plural3_1  },   // "nplurals=3; plural=n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2;"
  { "ss", 0,    0, "Swati",                         2, plural2_1  },   // unknown
  { "st", 0,    0, "Sotho",                         2, plural2_1  },   // unknown
  { "sv", 0,    0, "Swedish",                       2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "sv", "SE", 0, "Swedish (Sweden)",              2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "sv", "SV", 0, "Swedish (El Salvador)",         2, plural2_1  },   // "nplurals=2; plural=(n != 1);"
  { "sw", 0,    0, "Swahili",                       2, plural2_1  },   // unknown
  { "ta", 0,    0, "Tamil",                         2, plural2_1  },   // unknown
  { "te", 0,    0, "Telugu",                        2, plural2_1  },   // unknown
  { "tg", 0,    0, "Tajik",                         2, plural2_1  },   // unknown
  { "th", 0,    0, "Thai",                          1, plural1    },   // "nplurals=1; plural=0;"
  { "th", "TH", 0, "Thai (Thailand)",               1, plural1    },   // "nplurals=1; plural=0;"
  { "ti", 0,    0, "Tigrinya",                      2, plural2_1  },   // unknown
  { "tk", 0,    0, "Turkmen",                       2, plural2_1  },   // unknown
  { "tl", 0,    0, "Tagalog",                       2, plural2_1  },   // unknown
  { "to", 0,    0, "Tonga",                         2, plural2_1  },   // unknown
  { "tr", 0,    0, "Turkish",                       1, plural1    },   // "nplurals=1; plural=0;"
  { "tr", "TR", 0, "Turkish (Turkey)",              1, plural1    },   // "nplurals=1; plural=0;"
  { "ts", 0,    0, "Tsonga",                        2, plural2_1  },   // unknown
  { "tt", 0,    0, "Tatar",                         2, plural2_1  },   // unknown
  { "ug", 0,    0, "Uighur",                        2, plural2_1  },   // unknown
  { "uk", 0,    0, "Ukrainian",                     3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "uk", "UA", 0, "Ukrainian (Ukraine)",           3, plural3_1  },   // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
  { "ur", 0,    0, "Urdu",                          2, plural2_1  },   // unknown
  { "ur", "PK", 0, "Urdu (Pakistan)",               2, plural2_1  },   // unknown
  { "uz", 0,    0, "Uzbek",                         2, plural2_1  },   // unknown
  { "uz", 0, "cyrillic", "Uzbek",                   2, plural2_1  },   // unknown
  { "vi", 0,    0, "Vietnamese",                    1, plural1    },   // "nplurals=1; plural=0;"
  { "vi", "VN", 0, "Vietnamese (Vietnam)",          1, plural1    },   // "nplurals=1; plural=0;"
  { "wa", 0,    0, "Walloon",                       2, plural2_2  },   // "nplurals=2; plural=n > 1;"
  { "wo", 0,    0, "Wolof",                         2, plural2_1  },   // unknown
  { "xh", 0,    0, "Xhosa",                         1, plural1    },   // "nplurals=2; plural=(n != 1);"
  { "yi", 0,    0, "Yiddish",                       2, plural2_1  },   // unknown
  { "yo", 0,    0, "Yoruba",                        2, plural2_1  },   // unknown
  { "zh", 0,    0, "Chinese",                       1, plural1    },   // "nplurals=1; plural=0;"
  { "zh", "CN", 0, "Chinese (simplified)",          1, plural1    },   // "nplurals=1; plural=0;"
  { "zh", "HK", 0, "Chinese (Hong Kong)",           1, plural1    },   // "nplurals=1; plural=0;"
  { "zh", "TW", 0, "Chinese (traditional)",         1, plural1    },   // "nplurals=1; plural=0;"
  { "zu", 0,    0, "Zulu",                          2, plural2_1  },   // guessed
  { NULL }
};
//*}

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

Language
Language::from_spec(const std::string& language, const std::string& country, const std::string& modifier)
{
  static std::map<std::string, std::vector<LanguageSpec*> > language_map;

  if (language_map.empty())
    { // Init language_map
      for(int i = 0; languages[i].language != NULL; ++i)
        language_map[languages[i].language].push_back(&languages[i]);
    }
  
  std::map<std::string, std::vector<LanguageSpec*> >::iterator i = language_map.find(language);
  if (i != language_map.end())
    {
      // FIXME: Not handling country and modifier here
      return Language(i->second[0]);
    }
  else
    {
      return Language();
    }
}

Language
Language::from_name(const std::string& spec_str_)
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

  return from_spec(language, country);
}

Language::Language(LanguageSpec* language_spec_)
  : language_spec(language_spec_)
{
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
