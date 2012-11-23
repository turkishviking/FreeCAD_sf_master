/***************************************************************************
 *   Copyright (c) 2012 Luke Parry    (l.parry@warwick.ac.uk)              *
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

#include <Base/Console.h>

#include "TPGFeature.h"
#include "TPG/TPGFactory.h"

namespace Cam {

PROPERTY_SOURCE(Cam::TPGFeature, App::DocumentObject)

TPGFeature::TPGFeature() : tpg(0)
{
    ADD_PROPERTY_TYPE(ExternalGeometry,(0, 0), "TPG Feature", (App::PropertyType)(App::Prop_None) , "External geometry");
}

TPGFeature::TPGFeature(TPGDescriptor *tpgDescriptor)
{
    loadTPG(tpgDescriptor);
}

bool TPGFeature::loadTPG(TPGDescriptor *tpgDescriptor)
{    
    if(tpgDescriptor == NULL)
        throw new Base::Exception("TPG Plugin Description is null");
    
    // First check if a plugin already exists and quit if already running
    if(this->hasRunningTPG())
        return false;
    
    // Make the plugin from the descriptor
    TPG *myTpg = NULL;
    myTpg = tpgDescriptor->make();
    
    if(myTpg == NULL)
        return false;
    
    // Set the TPGFeatures internal TPG member
    this->tpg = myTpg;
    return true;
}

void TPGFeature::run()
{
//    if(hasTPG() && tpg->isReady()) {
//        //Initialise the TPG for running
//        this->tpg->setInputBBox(inputBBox); // Set the bounding box could be null
//        this->tpg->run();
//    } else
//        Base::Console().Log("The TPG is not ready");
}

void TPGFeature::stop()
{
//    if(hasRunningTPG())
//        this->tpg->stop();
//    else
//        Base::Console().Log("The TPG is not running");
}

TPG::State TPGFeature::getTPGStatus() {
//    return (hasTPG()) ?  this->tpg->getState() : TPG::UNDEFINED;
	return TPG::UNDEFINED;
}

bool TPGFeature::hasRunningTPG()
{
    TPG::State tpgStatus = getTPGStatus();
     return (tpgStatus == TPG::RUNNING || tpgStatus == TPG::STARTED);
}

TPGFeature::~TPGFeature()
{
    // If the TPG is running we must stop this (especially if a (network) process or thread
    if(this->hasRunningTPG())
        stop();
    //TODO should we wait till the tpg has finished
    delete tpg; // Will internally call destructor and safely stop this
}

App::DocumentObjectExecReturn *TPGFeature::execute(void)
{
    Base::Console().Log("Running Feature \n");
    this->run();
    return App::DocumentObject::StdReturn;
}

}
