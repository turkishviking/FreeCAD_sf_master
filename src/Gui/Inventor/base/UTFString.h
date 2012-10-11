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

#ifndef GUI_INVENTOR_UTFSTRING_H
#define GUI_INVENTOR_UTFSTRING_H

#include <vector>
#include <set>
#include <string>

namespace Gui { namespace Inventor {

class GuiExport UTFString : public std::vector<unsigned int>
{
public:
    /**
      * Types of string encodings supported
      */
    enum Encoding
    {
        ENCODING_UNDEFINED,                 /// not using Unicode
        ENCODING_ASCII = ENCODING_UNDEFINED,/// unsigned char ASCII
        ENCODING_UTF8,                      /// 8-bit unicode transformation format
        ENCODING_UTF16,                     /// 16-bit signature
        ENCODING_UTF16_BE,                  /// 16-bit big-endian
        ENCODING_UTF16_LE,                  /// 16-bit little-endian
        ENCODING_UTF32,                     /// 32-bit signature
        ENCODING_UTF32_BE,                  /// 32-bit big-endian
        ENCODING_UTF32_LE,                  /// 32-bit little-endian
        ENCODING_SIGNATURE                  /// detect encoding from signature
    };

    UTFString() : format(ENCODING_UNDEFINED) {}
    UTFString(const UTFString &str)  { format = str.getEncoding(); set(str); }
    UTFString(const std::string &str) : format(ENCODING_UNDEFINED) { set(str); }
    UTFString(const wchar_t *text) { set(text); }
    UTFString(const std::string &text, Encoding encoding) { format = encoding; set(text, encoding); }

    UTFString& operator = (const UTFString &str);

    void append(const std::string &str);
    void append(const wchar_t *text);

    // TODO other convenience compare functions - not really needed?
    bool compare(const UTFString &text) const;

    void set(const std::string &str);
    void set(const UTFString &str);

    /** Set the text using a wchar_t string,
    * which is converted to an internal TextString.*/
    void set(const wchar_t* text);

    /** Set the text using a Unicode encoded std::string, which is converted to an internal TextString.
    * The encoding parameter specifies which Unicode encoding is used in the std::string. */
    void set(const std::string &text, Encoding encoding);

    void setEncoding(Encoding encoding) { format = encoding; }

    Encoding getEncoding() const { return format; }

    /** returns a UTF8 encoded version of this UTFString*/
    std::string createUTF8EncodedString() const;

    protected:
      Encoding format;
};

} // namespace Inventor

} // namespace Gui

#endif // GUI_INVENTOR_UTFSTRING_H
