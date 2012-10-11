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

#ifndef GUI_INVENTOR_SOSFUTFSTRING_H
#define GUI_INVENTOR_SOSFUTFSTRING_H

#include "../base/SbUTFString.cpp"

#include <Inventor/fields/SoSubField.h>

namespace Gui { namespace Inventor {

class UTFString;

class GuiExport SoSFUTFString : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFUTFString, SbUTFString, const SbUTFString &);

public:
  static void initClass(void);

  void setValue(const char * str);
  void setValue(const wchar_t *text);
  void setValue(const std::string);
  void setValue(const std::string &text, UTFString::Encoding encoding);

};

}
}

#endif // GUI_INVENTOR_SOSFUTFSTRING_H
