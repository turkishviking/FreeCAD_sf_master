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

#ifndef CAMGUI__TaskDlgEditStockGeometry_H
#define CAMGUI__TaskDlgEditStockGeometry_H

#include <Gui/TaskView/TaskDialog.h>
#include <Gui/Selection.h>

namespace CamGui {

class  ViewProviderStockGeometry;

/// simulation dialog for the TaskView
class CamGuiExport TaskDlgEditStockGeometry : public Gui::TaskView::TaskDialog, public Gui::SelectionObserver
{
    Q_OBJECT

public:
    TaskDlgEditStockGeometry(ViewProviderStockGeometry *stockGeomView);
    ~TaskDlgEditStockGeometry();
    ViewProviderStockGeometry* getStockGeometryView() const { return stockGeomView; }

public:
    /// is called the TaskView when the dialog is opened
    virtual void open();
    /// is called by the framework if an button is clicked which has no accept or reject role
    virtual void clicked(int);
    /// is called by the framework if the dialog is accepted (Ok)
    virtual bool accept();
    /// is called by the framework if the dialog is rejected (Cancel)
    virtual bool reject();
    /// is called by the framework if the user presses the help button 
    virtual void helpRequested();
    virtual bool isAllowedAlterDocument(void) const
    { return false; }

    void onSelectionChanged(const Gui::SelectionChanges& msg);
    
    /// returns for Close and Help button 
    virtual QDialogButtonBox::StandardButtons getStandardButtons(void) const
    { return QDialogButtonBox::Save|QDialogButtonBox::Close; }

protected:
    ViewProviderStockGeometry   *stockGeomView;
};

} //namespace CamGui

#endif // CAMGUI_TaskDlgEditStockGeometry_H