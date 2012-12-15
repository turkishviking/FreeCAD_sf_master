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

#include "../PreCompiled.h"
#ifndef _PreComp_
#endif

#include "CppTPGWrapper.h"

namespace Cam {

CppTPGWrapper::CppTPGWrapper(TPG *impl, CppTPGPlugin *plugin)
: TPG() {
    this->impl = impl;
    this->plugin = plugin;
}

CppTPGWrapper::~CppTPGWrapper() {
    // cleanup through the library that created this implementation
    if (plugin != NULL && impl != NULL)
        plugin->delTPG(impl);
}

/**
 * Returns a list of action names supported by this TPG.
 *
 * Note: if this TPG only does one thing then just return a list containing 'default'
 */
std::vector<QString> &CppTPGWrapper::getActions()
{
    return impl->getActions();
}

/**
 * Get the settings for a given action
 */
TPGSettings *CppTPGWrapper::getSettings(QString &action)
{
    return impl->getSettings(action);
}

/**
 * Run the TPG to generate the ToolPath code.
 *
 * Note: the return will change once the TP Language has been set in store
 */
void CppTPGWrapper::run(TPGSettings *settings, QString action)
{
    impl->run(settings, action);
}

} /* namespace Cam */
