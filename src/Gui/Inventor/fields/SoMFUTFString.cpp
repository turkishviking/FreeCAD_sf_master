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
#include <assert.h>

#include <string>

#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>

#include "SoMFUTFString.h"

SO_MFIELD_SOURCE(SoMFUTFString, SbUTFString, const SbUTFString &);

// Override from parent class.
void SoMFUTFString::initClass(void)
{
    SO_MFIELD_INIT_CLASS(SoMFUTFString, SoMField);
}

SbBool SoMFUTFString::read1Value(SoInput * in, int idx)
{
    assert(idx < this->maxNum);
//     return in->read((const unsigned int) this->values[idx]);
    return true;
}

void SoMFUTFString::write1Value(SoOutput * out, int idx) const
{
//     out->write((const unsigned int) (*this)[idx]);
}

void SoMFUTFString::setValues(const int start, const int numarg, const char * strings[])
{
    if (start+numarg > this->maxNum)
        this->allocValues(start+numarg);
    else if (start+numarg > this->num)
        this->num = start+numarg;

    for (int i=0; i < numarg; i++)
        this->values[i+start] = SbUTFString(strings[i]);

    this->valueChanged();
}

void SoMFUTFString::setValues(const int start, const int numarg, const std::string *strings[])
{
    if (start+numarg > this->maxNum)
        this->allocValues(start+numarg);
    else if (start+numarg > this->num)
        this->num = start+numarg;

    for (int i=0; i < numarg; i++)
        this->values[i+start] = SbUTFString(*strings[i]);

    this->valueChanged();
}

void SoMFUTFString::setValues(const int start, const int numarg, const wchar_t *strings[])
{
    if (start+numarg > this->maxNum)
        this->allocValues(start+numarg);
    else if (start+numarg > this->num)
        this->num = start+numarg;

    for (int i=0; i < numarg; i++)
        this->values[i+start] = SbUTFString(strings[i]);

    this->valueChanged();
}

void SoMFUTFString::setValue(const char * str)
{
    this->setValue(SbUTFString(str));
}

void SoMFUTFString::setValue(const wchar_t *str)
{
    this->setValue(SbUTFString(str));
}

void SoMFUTFString::setValue(const std::string str)
{
    this->setValue(SbUTFString(str));
}

void SoMFUTFString::setValue(const std::string &text, UTFString::Encoding encoding)
{
    this->setValue(SbUTFString(text, encoding));
}


// void SoMFUTFString::deleteText(const int fromline, const int fromchar,
//                                const int toline, const int tochar)
// {
// #if COIN_DEBUG && 1 // debug
//   if (fromline < 0 || toline >= this->getNum() || fromline > toline ||
//       (fromline == toline && fromchar >= tochar) ||
//       fromchar < 0 || fromchar >= (*this)[fromline].getLength() ||
//       tochar < 0 || tochar >= (*this)[toline].getLength()) {
//     SoDebugError::post("SoMFUTFString::deleteText",
//                        "invalid argument(s), [%d, %d - %d, %d]",
//                        fromline, fromchar, toline, tochar);
//     return;
//   }
// #endif // debug
//
//   if (fromline == toline) {
//     this->values[fromline].deleteSubString(fromchar, tochar);
//   }
//   else {
//     this->values[fromline].deleteSubString(fromchar, -1);
//     this->values[toline].deleteSubString(0, tochar);
//     // Merge.
//     this->values[fromline] += (*this)[toline];
//     // Delete intermediate strings + toline string.
//     this->deleteValues(fromline + 1, toline - fromline);
//   }
// }