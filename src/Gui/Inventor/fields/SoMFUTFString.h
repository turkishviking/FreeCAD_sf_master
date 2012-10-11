/***************************************************************************
 *   Copyright (c) 2012 Luke Parry <l.parry@warwick.ac.uk>                 *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#ifndef GUI_INVENTOR_SOMFUTFSTRING_H
#define GUI_INVENTOR_SOMFUTFSTRING_H

#include <Inventor/fields/SoSubField.h>
#include <string>
#include "../base/SbUTFString.h"

using namespace Gui::Inventor;

namespace Gui { namespace Inventor {

class GuiExport SoMFUTFString : public SoMField {

  SO_MFIELD_HEADER(SoMFUTFString, SbUTFString, const SbUTFString &);

public:
  static void initClass(void);

  void setValues(const int start, const int numarg, const char * strings[]);
  void setValues(const int start, const int numarg, const std::string * strings[]);
  void setValues(const int start, const int numarg, const wchar_t * strings[]);

  void setValue(const char * str);
  void setValue(const wchar_t *str);
  void setValue(const std::string str);
  void setValue(const std::string &text, UTFString::Encoding encoding);

//   void deleteText(const int fromline, const int fromchar,
//                   const int toline, const int tochar);
};

}
}

#endif // GUI_INVENTOR_SOMFUTFSTRING_H
