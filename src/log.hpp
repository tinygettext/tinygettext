//  $Id$
//
//  tinygettext - A gettext replacement that works directly on .po files
//  Copyright (C) 2009 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_LOG_HPP
#define HEADER_LOG_HPP

#include <sstream>

namespace tinygettext {

extern void (*log_callback)(const std::string&);

class Log
{
private:
  std::ostringstream out;

public:
  Log() {}
  ~Log() {
    log_callback(out.str());
  }
  
  std::ostream& get() { return out; }
};

#define log_warning if (!log_callback); else Log().get()
#define log_info    if (!log_callback); else Log().get()

} // namespace tinygettext

#endif

/* EOF */
