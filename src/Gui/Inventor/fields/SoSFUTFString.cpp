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

#include "PreCompiled.h"
#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/errors/SoReadError.h>

#include "SoSFUTFString.h"

SO_SFIELD_SOURCE(SoSFUTFString, SbUTFString, const SbUTFString &);

// Override from parent class.
void SoSFUTFString::initClass(void)
{
    SO_SFIELD_INIT_CLASS(SoSFUTFString, SoSField);
}

SbBool SoSFUTFString::readValue(SoInput * in)
{
//   in->read(this->value);
    return true;
}

void SoSFUTFString::writeValue(SoOutput * out) const
{
//     out->write(this->getValue());
}

/*!
  Set string field value from \a str.
*/
void SoSFUTFString::setValue(const char * str)
{
    this->setValue(SbUTFString(str));
}

void SoSFUTFString::setValue(const wchar_t *str)
{
    this->setValue(SbUTFString(str));
}

void SoSFUTFString::setValue(const std::string str)
{
    this->setValue(SbUTFString(str));
}

void SoSFUTFString::setValue(const std::string &text, UTFString::Encoding encoding)
{
    this->setValue(SbUTFString(text, encoding));
}
