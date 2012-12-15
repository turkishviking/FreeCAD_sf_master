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

#ifndef CPPTPGPLUGIN_H_
#define CPPTPGPLUGIN_H_

#include <vector>

#include "TPGFactory.h"
#include "CppTPG.h"

namespace Cam {

/**
 * This class manages the interface to a shared library containing TPG's.  There
 * is one instance of this class per library file.  The library is only loaded
 * when it is needed.
 */
class CppTPGPlugin {
protected:
    QString filename; ///< the filename of the shared object file
    void* library; ///< a pointer to the library when its open
    getDescriptors_t* getDescriptorsPtr; ///< pointer to lib function
    delDescriptors_t* delDescriptorsPtr; ///< pointer to lib function
    getTPG_t* getTPGPtr; ///< pointer to lib function
    delTPG_t* delTPGPtr; ///< pointer to lib function

    std::vector<TPGDescriptor*> *descriptors; ///< a cache for the descriptors from the library.
    /**
     * Makes sure library is open, attempts to open it if it isn't.
     */
    bool isOpen();

    /**
     * Close the library and cleanup pointers
     */
    void close();

public:
    CppTPGPlugin(QString filename);
    virtual ~CppTPGPlugin();

    QString error; ///< if there is a library error the result of dlerror() is stored here.

    /**
     * Returns a list of TPG's that this plugin provides
     * Note: the list is owned by the caller and must be deleted when finished with it.
     */
    std::vector<TPGDescriptor*>* getDescriptors();

    /**
     * Gets an instance of the TPG.
     * Note: the TPG returned is a wrapper around the implementation.  Delete
     * it once you are finished with it and it will automatically delete the
     * implementation using the delTPG(tpg) method below
     */
    TPG* getTPG(QString id);

    /**
     * Used by the CppTPGWrapper to delete its implementation.
     */
    void delTPG(TPG* tpg);
};

} /* namespace CamGui */
#endif /* CPPTPGPLUGIN_H_ */
