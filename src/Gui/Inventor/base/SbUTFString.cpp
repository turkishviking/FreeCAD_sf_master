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
#include "SbUTFString.h"

using namespace Gui::Inventor;

// SbUTFString getSubString(int startidx, int endidx = -1) const
// {
//     SbUTFString s;
//     cc_string_set_subtext(&s.str, cc_string_get_text(&this->str), startidx, endidx);
//     return s;
// }
// void deleteSubString(int startidx, int endidx = -1)
// {
//     cc_string_remove_substring(&this->str, startidx, endidx);
// }

// static void compute_prefix_function(SbList <int> & pi, const SbString & str)
// {
//   int len = str.getLength();
//   pi.append(0);
//   int k = 0;
//
//   for (int q = 1; q < len; q++){
//     while(k > 0 && (str.operator[](k) != str.operator[](q)))
//       k = pi[k];
//     if (str.operator[](k) == str.operator[](q))
//       k++;
//     pi.append(k);
//   }
// }


// int SbString::find(const SbString & strarg) const
// {
//   int lenthis = this->getLength();
//   int lenstr = strarg.getLength();
//
//   if (!lenthis) return -1;
//   if (lenstr > lenthis) return -1;
//
//   SbList<int> pi;
//   compute_prefix_function(pi, strarg);
//   int q = 0;
//
//   for (int i = 0; i < lenthis; i ++){
//     while (q > 0 && (strarg.operator[](q) != this->operator[](i)))
//       q = pi.operator[](q - 1);
//     if (strarg.operator[](q) == this->operator[](i))
//       q++;
//     if (q == lenstr){
//       return (i - (lenstr - 1));
//     }
//   }
//   return -1;
// }
//
//
// SbBool SbString::findAll(const SbString & strarg, SbIntList & found) const
// {
//   // The KMP string matching algorithm is used for this method
//   int lenthis = this->getLength();
//   int lenstr = strarg.getLength();
//   found.truncate(0);
//
//   if (!lenthis) return FALSE;
//   if (lenstr > lenthis) return FALSE;
//
//   SbList<int> pi;
//   compute_prefix_function(pi, strarg);
//   int q = 0;
//
//   for (int i = 0; i < lenthis; i ++){
//     while (q > 0 && (strarg.operator[](q) != this->operator[](i)))
//       q = pi.operator[](q - 1);
//     if (strarg.operator[](q) == this->operator[](i))
//       q++;
//     if (q == lenstr){
//       found.append(i - (lenstr - 1));
//       q = pi.operator[](q - 1);
//     }
//   }
//   if (!found.getLength()) return FALSE;
//   return TRUE;
// }
