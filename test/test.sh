#!/bin/sh

./tinygettext_test translate po/fr.po  "invalid"
./tinygettext_test directory po/ umlaut Deutsch
./tinygettext_test directory po/ umlaut deutsch
./tinygettext_test directory po/ umlaut de

# Plural-Forms headers as written by msginit and translation tools,
# each line should print the msgstr[N] name for that count
./tinygettext_test translate po/sl.po apple apples 102  # two
./tinygettext_test translate po/sl.po apple apples 103  # few
./tinygettext_test translate po/sl.po apple apples 5    # other
./tinygettext_test translate po/lv.po apple apples 21   # one
./tinygettext_test translate po/lv.po apple apples 11   # other
./tinygettext_test translate po/lv.po apple apples 0    # zero
./tinygettext_test translate po/ar.po apple apples 2    # two
./tinygettext_test translate po/ar.po apple apples 110  # few
./tinygettext_test translate po/ar.po apple apples 11   # many
./tinygettext_test translate po/ar.po apple apples 100  # other

# EOF #
