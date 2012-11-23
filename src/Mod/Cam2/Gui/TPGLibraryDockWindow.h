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

#ifndef CAMGUI_TPGLIBRARYDOCKWINDOW_H_
#define CAMGUI_TPGLIBRARYDOCKWINDOW_H_

#include <qobject.h>

#include <Gui/DockWindow.h>

#include "../App/TPG/TPGFactory.h"
#include "TPGListModel.h"
#include "ui_TPGLibraryDockWindow.h"

namespace CamGui {

class CamGuiExport TPGLibraryDockWindow : public Gui::DockWindow {

  Q_OBJECT

public:
  TPGLibraryDockWindow(Gui::Document*  pcDocument, QWidget *parent=0);
  virtual ~TPGLibraryDockWindow();

public Q_SLOTS:
  void addBtnClick();
  void updatedTPGList(TPGListModel* tpgs);
  void selectionChanged(const QItemSelection &newselection, const QItemSelection &oldselection);

Q_SIGNALS:
  void addTPG(Cam::TPGDescriptor *tpg);

protected:
  TPGListModel* tpgs;

private:
  Ui_TPGLibraryDockWindow* ui;
};

} /* namespace CamGui */
#endif /* TPGLIBRARYDOCKWINDOW_H_ */
