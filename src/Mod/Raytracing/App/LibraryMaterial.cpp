/***************************************************************************
 *   Copyright (c) Luke Parry          (l.parry@warwick.ac.uk)    2012     *
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
#ifndef _PreComp_
#endif

#include "LibraryMaterial.h"

using namespace Raytracing;


LibraryMaterial::LibraryMaterial()
{
}
LibraryMaterial::~LibraryMaterial()
{
    if(parameters.isEmpty())
        return;

    // Delete all the parameters stored under the material
    QMap<QString, MaterialParameter *>::iterator it = parameters.begin();
    while (it != parameters.end()) {
        switch(it.value()->getType())
        {
          case MaterialParameter::BOOL: {
            MaterialParameterBool *param = static_cast<MaterialParameterBool *>(it.value());
            delete param;
          } break;
          case MaterialParameter::COLOR: {
            MaterialParameterColor *param = static_cast<MaterialParameterColor *>(it.value());
            delete param;
          } break;
          case MaterialParameter::FLOAT: {
            MaterialParameterFloat *param = static_cast<MaterialParameterFloat *>(it.value());
            delete param;
          } break;
          default: break;
        }
        it.value() = 0;
        ++it;
    }
    parameters.clear();
}