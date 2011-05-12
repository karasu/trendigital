/**************************************************************************
    This file is part of Tren Digital.

    Tren Digital is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tren Digital is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tren Digital.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QObject>
#include <QTextStream>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QMap>
#include <QIcon>

class Cell;

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View(QWidget *parent=0);

    bool m_editMode; // bDesignRoute in MFC version

    QSize m_gridSize;

    Cell *getCell(int x, int y);

    // Holds the current centerpoint for the view, used for panning and zooming
    QPointF m_currentCenterPoint;

    // From panning the view
    QPoint m_lastPanPoint;

    // Last id and icon that has been selected on edit toolbar
    QIcon m_currentElementIcon;
    int m_currentElementId;

    // Set the current centerpoint in the
    void setCenter(const QPointF& centerPoint);
    QPointF getCenter() { return m_currentCenterPoint; }

    inline void setCurrentElementIcon(QIcon icon) { m_currentElementIcon = icon; }
    inline void setCurrentElementId(QString id) { m_currentElementId = id.toInt(); }
    inline void setCurrentElementId(int id) { m_currentElementId = id; }

    // Take over the interaction
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

    void setEditMode(bool status);

    inline bool editMode() { return m_editMode; }
};

#endif // VIEW_H
