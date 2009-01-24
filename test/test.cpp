#include <fstream>
#include <iostream>
#include "tinygettext.hpp"

#define _(String) gettext(String)
#define gettext(String) dict.translate(String).c_str()
#define ngettext(msgid, msgid2, num) dict.translate(msgid, msgid2, num).c_str()

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      std::cout << "Usage: tinygettext [FILE] ..." << std::endl;
    }
  else if (argc == 2 && strcmp(argv[1], "--mgr") == 0)
    {
      std::cout << "Using DictionaryManager" << std::endl;
      TinyGetText::DictionaryManager dictmgr;

      dictmgr.set_language("de");
      dictmgr.add_directory("game");
      dictmgr.add_directory("level");
      
      std::cout << dictmgr.get_dictionary().translate("disabled") << std::endl;
      std::cout << dictmgr.get_dictionary().translate("...walking...") << std::endl;
      int num = 3;
      printf(dictmgr.get_dictionary().translate("You got %d error.\n", 
                                                "You got %d errors.\n", num).c_str(), num);
    }
  else
    {
      TinyGetText::Dictionary dict;

      for (int i = 1; i < argc; ++i)
        {
          std::ifstream in(argv[i]);
          TinyGetText::read_po_file(dict, in);
        }

      std::cout << _("disabled") << std::endl;
      std::cout << _("...walking...") << std::endl;
      int num = 3;
      printf(ngettext("You got %d error.\n", 
                      "You got %d errors.\n", num), num);
    }
}

/* EOF */

