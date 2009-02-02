# -*- python -*-

env = Environment(CXXFLAGS=['-g', '-O2', '-ansi', '-pedantic', '-Wall', '-Werror'], 
                  CPPPATH=['src'])

# env.ParseConfig("sdl-config --cflags --libs")
# env['CPPDEFINES'] += HAVE_SDL

libtinygettext = env.StaticLibrary('src/tinygettext', 
                                   ['src/tinygettext.cpp',
                                    'src/language.cpp',
                                    'src/plural_forms.cpp',
                                    'src/dictionary.cpp',
                                    'src/dictionary_manager.cpp',
                                    'src/directory.cpp',
                                    'src/po_parser.cpp',
                                    'src/iconv.cpp',
                                    'src/log.cpp'])

env.Program('test/tinygettext', ['test/tinygettext.cpp', libtinygettext])
env.Program('test/po_parser_test', ['test/po_parser_test.cpp', libtinygettext])


# EOF #
