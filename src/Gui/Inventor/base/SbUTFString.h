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

#ifndef GUI_INVENTOR_SBUTFSTRING_H
#define GUI_INVENTOR_SBUTFSTRING_H

#include <stdarg.h>
#include <string>

#include "../base/UTFString.h"
#include <Inventor/system/inttypes.h>

#include <Inventor/SbBasic.h>

namespace Gui { namespace Inventor {


class GuiExport SbUTFString {
public:
  SbUTFString() {}
  SbUTFString(const char * s) { this->str.set(s); }
  SbUTFString(const SbUTFString &text) { this->str.set(text.getUTFString()); }
  SbUTFString(const std::string &text) { this->str.set(text); }
  SbUTFString(const wchar_t *text) { this->str.set(text); }
  SbUTFString(const std::string &text, UTFString::Encoding encoding) { this->str.set(text, encoding); this->str.setEncoding(encoding); }

  ~SbUTFString(){}

  int getLength(void) const { return this->str.size(); }

  void makeEmpty() { this->str.clear(); }

  void setEncoding(UTFString::Encoding encoding) { this->str.setEncoding(encoding); }
  UTFString::Encoding getEncoding() const { return this->str.getEncoding(); }

  const UTFString getUTFString() const { return this->str; }

//   std::string getString(void) const { return this->str.createUTF8EncodedString() }
//   std::string getSubString(int startidx, int endidx = -1) const;
//   void deleteSubString(int startidx, int endidx = -1);

//   uint32_t hash(void) const { return cc_string_hash(&this->str); }
//   static uint32_t hash(const char * s) { return cc_string_hash_text(s); }
//   void addIntString(const int value) { cc_string_append_integer(&this->str, value); }

  char operator[](int index) const { return this->str[index]; }

  SbUTFString & operator=(const char * s)
  {
      this->str.set(s);
      return *this;
  }

  SbUTFString & operator=(const wchar_t* s)
  {
      this->str.set(s);
      return *this;
  }

  SbUTFString & operator=(const std::string &s)
  {
      this->str.set(s);
      return *this;
  }

  SbUTFString & operator=(const SbUTFString & s)
  {
      this->str = s.getUTFString();
      return *this;
  }

  SbUTFString & operator+=(const wchar_t *s)
  {
      this->str.append(s);
      return *this;
  }

  SbUTFString & operator+=(const std::string &s)
  {
      this->str.append(s);
      return *this;
  }
  SbUTFString & operator+=(const SbUTFString & s)
  {
      this->str.append(s.getUTFString().createUTF8EncodedString());
      return *this;
  }

  int operator!(void) const { return !(this->str.size() > 0); }

  int compareSubString(const char * text, int offset = 0) const
  {/* return cc_string_compare_subtext(&this->str, text, offset);*/ }

  SbUTFString & sprintf(const char * formatstr, ...)
  {
//     va_list args; va_start(args, formatstr);
//     cc_string_vsprintf(&this->str, formatstr, args);
//     va_end(args); return *this;
  }

  SbUTFString & vsprintf(const char * formatstr, va_list args)
  { /*cc_string_vsprintf(&this->str, formatstr, args); return *this;*/ }

  void apply(char (*func)(char input)) {/* cc_string_apply(&this->str, (cc_apply_f)func);*/ }

  int find(const SbUTFString & s) const {  return 0;}
//   SbBool findAll(const SbUTFString & s, SbIntList & found) const {}

//   friend int operator==(const SbUTFString & sbstr, const char * s);
//   friend int operator==(const char * s, const SbUTFString & sbstr);
  friend int operator==(const SbUTFString & str1, const SbUTFString & str2);
//   friend int operator!=(const SbUTFString & sbstr, const char * s);
//   friend int operator!=(const char * s, const SbUTFString & sbstr);
//   friend int operator!=(const SbUTFString & str1, const SbUTFString & str2);
//
//   friend const SbUTFString operator+(const SbUTFString & str1, const SbUTFString & str2);
//   friend const SbUTFString operator+(const SbUTFString & sbstr, const char * s);
//   friend const SbUTFString operator+(const char * s, const SbUTFString & sbstr);


private:
    UTFString str;
};

// inline int operator==(const SbUTFString & str1, const char * s)
// {
//     return str1.getUTFString().compare(s);
// }
//
// inline int operator==(const char * s, const SbUTFString & str1)
// {
//     return str1.getUTFString().compare(s);
// }

inline int operator==(const SbUTFString & str1, const SbUTFString & str2)
{
    return str1.getUTFString().compare(str2.getUTFString());
}
/*
inline int operator!=(const SbUTFString & str1, const char * s)
{
    return !str1.getUTFString().compare(s);
}

inline int operator!=(const char * s, const SbUTFString & str1)
{
    return !str1.getUTFString().compare(s.getUTFString());
}*/

inline int operator!=(const SbUTFString & str1, const SbUTFString & str2)
{
    return !str1.getUTFString().compare(str2.getUTFString());

}


} // namespace Inventor

} // namespace Gui

#endif // GUI_INVENTOR_SBUTFSTRING_H
