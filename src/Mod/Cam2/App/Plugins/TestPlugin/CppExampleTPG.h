/***************************************************************************
 *   Copyright (c) 2012 Andrew Robinson <andrewjrobinson@gmail.com>        *
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

#ifndef CPPEXAMPLETPG_H_
#define CPPEXAMPLETPG_H_

#include <vector>

#include <TPG/TPG.h>
#include <TPG/TPGFactory.h>
#include <TPG/CppTPG.h>

namespace Cam {

/**
 * This is an example C++ TPG that is inside TestPlugin.so
 * Note: Not yet implemented
 */
class CppExampleTPG : public TPG {
public:
    CppExampleTPG();
    virtual ~CppExampleTPG();

    /**
     * Run the TPG to generate the ToolPath code.
     *
     * Note: the return will change once the TP Language has been set in stone
     */
    virtual void run(TPGSettings *settings, QString action);
};

} /* namespace Cam */

#endif /* CPPEXAMPLETPG_H_ */
