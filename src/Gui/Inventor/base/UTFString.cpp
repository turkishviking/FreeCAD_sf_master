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

#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <string>
#include <boost/concept_check.hpp>

#include "UTFString.h"

using namespace Gui::Inventor;

struct look_ahead_iterator
{
    look_ahead_iterator(const std::string& string):
        _string(string),
        _index(0),
        _nullCharacter(0) {}

    bool valid() const { return _index < _string.length(); }

    look_ahead_iterator & operator ++ ()
    {
        if (_index<_string.length())
            ++_index;
        return *this;
    }

    look_ahead_iterator operator ++ (int)
    {
        look_ahead_iterator tmp(*this);
        if (_index < _string.length())
            ++_index;
        return tmp;
    }

    look_ahead_iterator& operator += (int offset)
    {
        if (_index < _string.length())
            _index = std::min((unsigned int)(_index+offset),(unsigned int)_string.length());
        return *this;
    }

    unsigned char operator * () const
    {
        if (_index < _string.length())
            return _string[_index];
        else
            return _nullCharacter;
    }

    unsigned char operator [] (unsigned int offset) const
    {

        if ((_index + offset) <_string.length())
            return _string[_index+offset];
        else
            return _nullCharacter;
    }

    const std::string&      _string;
    unsigned int            _index;
    unsigned char           _nullCharacter;

protected:
    look_ahead_iterator& operator = (const look_ahead_iterator&) { return *this; }
};

UTFString::Encoding findEncoding(look_ahead_iterator& charString, UTFString::Encoding overrideEncoding)
{
    switch (charString[0])
    {
    case 0xEF: // 8-bit encoding
    {
        // 8-bit signature = EF BB BF
        if ((charString[1]==0xBB) && (charString[2]==0xBF))
        {
            charString+=3;
            return UTFString::ENCODING_UTF8;
        }
        break;
    }
    case 0xFE: // big-endian 16-bit
    {
        // 16-bit signature = FE FF
        if (charString[1]==0xFF)
        {
            charString+=2;
            return UTFString::ENCODING_UTF16_BE;
        }
        break;
    }
    case 0xFF: // little-endian
    {
        // 16-bit signature = FF FE
        // 32-bit signature = FF FE 00 00
        if (charString[1]==0xFE)
        {
            // NOTE: There is an a potential problem as a 16-bit empty string
            // is identical to a 32-bit start signature
            if ((charString[2]==0) && (charString[3]==0) && (overrideEncoding != UTFString::ENCODING_UTF16)) //32-bit
            {
                charString+=4;
                return UTFString::ENCODING_UTF32_LE;
            }
            else //16-bit
            {
                charString+=2;
                return UTFString::ENCODING_UTF16_LE;
            }
        }
        break;
    }
    case 0x00: // 32-bit big-endian
    {
        // 32-bit signature = 00 00 FE FF
        if ((charString[1]==0x00) && (charString[2]==0xFE) && (charString[3]==0xFF))
        {
            charString+=4;
            return UTFString::ENCODING_UTF32_BE;
        }
        break;
    }
    }
    return UTFString::ENCODING_ASCII;
}


unsigned int getNextCharacter(look_ahead_iterator &charString, UTFString::Encoding encoding)
{
    // For more info on unicode encodings see:
    // http://www-106.ibm.com/developerworks/unicode/library/u-encode.html
    switch(encoding)
    {
        case UTFString::ENCODING_ASCII:
        {
            return *charString++;
        }
        case UTFString::ENCODING_UTF8:
        {
            int char0 = *charString++;
            if (char0 < 0x80) // 1-byte character
            {
                return char0;
            }
            int char1 = *charString++;
            if (char0<0xe0) // 2-byte character
            {
                return ((char0&0x1f)<<6) | (char1&0x3f);
            }
            int char2 = *charString++;
            if (char0<0xf0) // 3-byte character
            {
                return ((char0&0xf)<<12) | ((char1&0x3f)<<6) | (char2&0x3f);
            }
            int char3 = *charString++;
            if (char0<0xf8) // 4-byte character
            {
                return ((char0&0x7)<<18) | ((char1&0x3f)<<12) | ((char2&0x3f)<<6) | (char3&0x3f);
            }
            break;
        }
        case UTFString::ENCODING_UTF16_BE:
        {
            int char0 = *charString++;
            int char1 = *charString++;
            if ((char0<=0xD7) || (char0>=0xE0)) // simple character
            {
                return (char0<<8) | char1;
            }
            else if ((char0>=0xD8)&&(char0<=0xDB)) //using planes (this should get called very rarely)
            {
                int char2 = *charString++;
                int char3 = *charString++;
                int highSurrogate = (char0<<8) | char1;
                int lowSurrogate = (char2<<8) | char3;
                if ((char2>=0xDC)&&(char2<=0xDF)) //only for the valid range of low surrogate
                {
                    // This covers the range of all 17 unicode planes
                    return ((highSurrogate-0xD800)*0x400) + (lowSurrogate-0xD800) + 0x10000;
                }
            }
            break;
        }
        case UTFString::ENCODING_UTF16_LE:
        {
            int char1 = *charString++;
            int char0 = *charString++;
            if ((char0<=0xD7) || (char0>=0xE0)) // simple character
            {
                return (char0<<8) | char1;
            }
            else if ((char0>=0xD8)&&(char0<=0xDB)) //using planes (this should get called very rarely)
            {
                int char3 = *charString++;
                int char2 = *charString++;
                int highSurrogate = (char0<<8) | char1;
                int lowSurrogate = (char2<<8) | char3;
                if ((char2>=0xDC)&&(char2<=0xDF)) //only for the valid range of low surrogate
                {
                    // This covers the range of all 17 unicode planes
                    return ((highSurrogate-0xD800)*0x400) + (lowSurrogate-0xD800) + 0x10000;
                }
            }
            break;
        }
        case UTFString::ENCODING_UTF32_BE:
        {
            int character = ((((int)charString[0])<<24) | (((int)charString[1])<<16) |
                            (((int)charString[2])<<8) | charString[3]);
            charString+=4;
            if (character<0x110000)
            {
                // Character is constrained to the range set by the unicode standard
                return character;
            }
            break;
        }
        case UTFString::ENCODING_UTF32_LE:
        {
            int character = ((((int)charString[3])<<24) | (((int)charString[2])<<16) |
                            (((int)charString[1])<<8) | charString[0]);
            charString+=4;
            if (character<0x110000)
            {
                // Character is constrained to the range set by the unicode standard
                return character;
            }
            break;
        }
        default:
        {
            // TODO throw coin3d exception
            break;
        }
    }
    return 0;
}

UTFString & UTFString::operator = (const UTFString &str)
{
    if (&str == this)
        return *this;

    clear();
    std::copy(str.begin(),str.end(),std::back_inserter(*this));

    return *this;
}

void UTFString::append(const std::string &text)
{
    //Allocate enough space for concate
    int n = text.size() + size();
    reserve(n);

    for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
    {
        unsigned int charcode = (unsigned char) *it;
        push_back(charcode);
    }
}

void UTFString::append(const wchar_t* text)
{
    while(*text)
    {
        push_back(*text++);
    }
}

bool UTFString::compare(const UTFString &text) const
{
    std::vector<unsigned int>::const_iterator itr = begin();
    std::vector<unsigned int>::const_iterator it = text.begin();
    for (; it != text.end(); ++it)
    {
        unsigned int char1 = (unsigned char) *it;
        unsigned int char2 = (unsigned char) *itr;
        if(*itr != *it)
            return false;
    }
    return true;
}

void UTFString::set(const std::string &text)
{
    clear();

    //Allocate enough space for string
    int n = text.length();
    reserve(n);

    for (std::string::const_iterator it = text.begin(); it != text.end(); ++it)
    {
        unsigned int charcode = (unsigned char) *it;
        push_back(charcode);
    }
}

void UTFString::set(const UTFString &text)
{
    clear();
    std::copy(text.begin(),text.end(),std::back_inserter(*this));
}

void UTFString::set(const wchar_t* text)
{
    clear();
    while(*text)
    {
        push_back(*text++);
    }
}

void UTFString::set(const std::string &text, Encoding encoding)
{
    clear();

    look_ahead_iterator it(text);

    if ((encoding == ENCODING_SIGNATURE) ||
        (encoding == ENCODING_UTF16) ||
        (encoding == ENCODING_UTF32))
    {
        encoding = findEncoding(it,encoding);
    }

    while(it.valid())
    {
        unsigned int character = getNextCharacter(it,encoding);
        if(character)
            push_back(character);
    }
}

std::string UTFString::createUTF8EncodedString() const
{
    std::string utf8string;
    for(const_iterator it = begin(); it != end(); ++it)
    {
        unsigned int currentChar = *it;
        if (currentChar < 0x80)
        {
            utf8string += (char)currentChar;
        }
        else if (currentChar < 0x800)
        {
            utf8string += (char)(0xc0 | (currentChar >> 6));
            utf8string += (char)(0x80 | (currentChar & 0x3f));
        }
        else
        {
            utf8string += (char)(0xe0 | (currentChar >>12));
            utf8string += (char)(0x80 | ((currentChar >> 6) & 0x3f));
            utf8string += (char)(0x80 | (currentChar & 0x3f));
        }
    }
    return utf8string;
}
