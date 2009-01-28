#!/bin/sh

./tinygettext translate po/fr.po  "invalid"
./tinygettext directory po/ umlaut Deutsch
./tinygettext directory po/ umlaut deutsch
./tinygettext directory po/ umlaut de

# EOF #
