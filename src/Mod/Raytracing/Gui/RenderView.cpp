/***************************************************************************
 *   Copyright (c) 2007 J�rgen Riegel <juergen.riegel@web.de>              *
 *                                                                         *
 *   This file is Drawing of the FreeCAD CAx development system.           *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A DrawingICULAR PURPOSE.  See the      *
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

#include "RenderView.h"

#include <Base/PyObjectBase.h>
#include <Gui/FileDialog.h>
#include <Gui/WaitCursor.h>
#include <QGraphicsObject>
#include <QGLWidget>
using namespace RaytracingGui;

// ----------------------------------------------------------------------------

/* TRANSLATOR DrawingGui::DrawingView */

RenderView::RenderView(Gui::Document* doc, QWidget* parent)
  : Gui::MDIView(doc, parent)
{
    QDeclarativeView *view = new QDeclarativeView (this);
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->setSource(QUrl(QString::fromAscii("qrc:/qml/renderPreviewUi.qml")));

    // Needed to improve performance by using Opengl Backend which should be guaranteed within Freecad
    QGLFormat format = QGLFormat::defaultFormat();
    // You can comment the next line if the graphical results are not acceptable
    format.setSampleBuffers(false);
    QGLWidget *glWidget = new QGLWidget(format);
    // Comment the following line if you get display problems
    // (usually when the top-level element is an Item and not a Rectangle)
    glWidget->setAutoFillBackground(false);
    view->setViewport(glWidget);

    view->setAttribute(Qt::WA_OpaquePaintEvent);
    view->setAttribute(Qt::WA_NoSystemBackground);
    view->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view->viewport()->setAttribute(Qt::WA_NoSystemBackground);

    QObject *rootObject = qobject_cast<QObject *>(view->rootObject());
    QObject::connect(this, SIGNAL(updatePreview(QVariant)), rootObject , SLOT(updatePreview(QVariant)));
    setCentralWidget(view);
    view->show();
    //setWindowTitle(tr("SVG Viewer"));
}

bool RenderView::onMsg(const char* pMsg, const char** ppReturn)
{
    return false;
}

void RenderView::attachRender(Renderer *attachedRender)
{
    render = attachedRender;
    QObject::connect(qobject_cast<QObject *>(render->getRenderProcess()), SIGNAL(updateOutput()), this , SLOT(updateOutput()));
}

bool RenderView::onHasMsg(const char* pMsg) const
{
    return false;
}

void RenderView::updateOutput()
{
    // Need to prepend 'file:/'
    QUrl url(QString::fromAscii(render->getOutputPath()).prepend(QString::fromAscii("file:/")));
    Q_EMIT updatePreview(QVariant(url));
}

PyObject* RenderView::getPyObject()
{
    Py_Return;
}

#include "moc_RenderView.cpp"
