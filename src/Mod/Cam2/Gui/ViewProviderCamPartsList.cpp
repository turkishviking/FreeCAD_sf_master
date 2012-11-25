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
#include <QMenu>
#endif

#include <Gui/Application.h>
#include <Gui/Command.h>
#include <Gui/Control.h>
#include <Gui/Document.h>
#include <Gui/SoFCSelection.h>
#include <Gui/Selection.h>
#include <Gui/SoTextLabel.h>
#include <Gui/MainWindow.h>
#include <Gui/SoFCUnifiedSelection.h>
#include <Gui/SoFCBoundingBox.h>
#include <Gui/View3DInventor.h>

#include "../App/CamPartsList.h"

#include "TaskDialog/TaskDlgEditCamPartsList.h"
#include "ViewProviderCamPartsList.h"

using namespace CamGui;

PROPERTY_SOURCE(CamGui::ViewProviderCamPartsList, Gui::ViewProviderDocumentObject)

ViewProviderCamPartsList::ViewProviderCamPartsList()
{
}

ViewProviderCamPartsList::~ViewProviderCamPartsList()
{
}


void ViewProviderCamPartsList::setupContextMenu(QMenu *menu, QObject *receiver, const char *member)
{
    menu->addAction(QObject::tr("Edit Cam Geometry"), receiver, member);
}

bool ViewProviderCamPartsList::setEdit(int ModNum)
{
   // When double-clicking on the item for this sketch the
    // object unsets and sets its edit mode without closing
    // the task panel
    Gui::TaskView::TaskDialog *dlg = Gui::Control().activeDialog();
    TaskDlgEditCamPartsList *taskDlgCamPartsList = qobject_cast<TaskDlgEditCamPartsList *>(dlg);
//     if (sketchDlg && sketchDlg->getSketchView() != this)
//         sketchDlg = 0; // another sketch left open its task panel
    if (dlg && !taskDlgCamPartsList) {
        QMessageBox msgBox;
        msgBox.setText(QObject::tr("A dialog is already open in the task panel"));
        msgBox.setInformativeText(QObject::tr("Do you want to close this dialog?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes)
            Gui::Control().closeDialog();
        else
            return false;
    }
    Gui::Selection().clearSelection();


    //start the edit dialog
    if (taskDlgCamPartsList)
        Gui::Control().showDialog(taskDlgCamPartsList);
    else
        Gui::Control().showDialog(new TaskDlgEditCamPartsList(this));

    return true;
}

void ViewProviderCamPartsList::setEditViewer(Gui::View3DInventorViewer* viewer, int ModNum)
{
    viewer->setEditing(TRUE);
//     SoNode* root = viewer->getSceneGraph();
//     //static_cast<Gui::SoFCUnifiedSelection*>(root)->selectionRole.setValue(FALSE);
}

bool ViewProviderCamPartsList::doubleClicked(void)
{
    Gui::Application::Instance->activeDocument()->setEdit(this);
    return true;
}

void ViewProviderCamPartsList::unsetEditViewer(Gui::View3DInventorViewer* viewer)
{
    viewer->setEditing(FALSE);
}

void ViewProviderCamPartsList::unsetEdit(int ModNum)
{
    // clear the selection and set the new/edited sketch(convenience)
    Gui::Selection().clearSelection();
}

Cam::CamPartsList* ViewProviderCamPartsList::getObject() const
{
    return dynamic_cast<Cam::CamPartsList*>(pcObject);
}
