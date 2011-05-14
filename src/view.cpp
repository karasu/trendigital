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
#include "general.h"
#include "element.h"
#include "cell.h"
#include "view.h"
#include "debug.h"

// fixme
// check that all Grid MFC class is well imported here, as it will disappear in this version

View::View(QWidget *parent) : QGraphicsView(parent)
{
    // Set-up the scene
    QGraphicsScene *pScene = new QGraphicsScene(this);

    pScene->setBackgroundBrush(Qt::black);

    m_gridSize = QSize(35, 25);

    for (int y=0; y<m_gridSize.height(); y++)
    {
        for (int x=0; x<m_gridSize.width(); x++)
        {
            Cell *pCell = new Cell();
            pCell->setPos(x, y);
            pCell->setView(this);
            pScene->addItem(pCell);
        }
    }

    setScene(pScene);

    setRenderHints(QPainter::Antialiasing); // | QPainter::SmoothPixmapTransform);

    m_editMode = false;

    m_currentElementId = INVALID;

    // Set-up the view
    // setSceneRect(0, 0, 1000, 1000);
    // setCenter(QPointF(500.0, 500.0)); // A modified version of centerOn(), handles special cases
    // setCursor(Qt::OpenHandCursor);
}

Cell * View::getCell(int x, int y)
{
    QList<QGraphicsItem *> itemList = scene()->items();

    QPoint p(x, y);

    foreach (QGraphicsItem *item, itemList)
    {
        if (item->type() == CELL_TYPE)
        {
            Cell *pCell = qgraphicsitem_cast<Cell *>(item);

            if (pCell->getPos() == p)
            {
                return pCell;
            }
        }
    }

    return NULL;
}


/**
  * Sets the current centerpoint.  Also updates the scene's center point.
  * Unlike centerOn, which has no way of getting the floating point center
  * back, SetCenter() stores the center point.  It also handles the special
  * sidebar case.  This function will claim the centerPoint to sceneRec ie.
  * the centerPoint must be within the sceneRec.
  */

// Set the current centerpoint
void View::setCenter(const QPointF& centerPoint)
{
    // Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();

    // Get the scene area
    QRectF sceneBounds = sceneRect();

    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;

    // The max boundary that the centerPoint can be to
    QRectF bounds(boundX, boundY, boundWidth, boundHeight);

    if (bounds.contains(centerPoint))
    {
        // We are within the bounds
        m_currentCenterPoint = centerPoint;
    }
    else
    {
        // We need to clamp or use the center of the screen
        if (visibleArea.contains(sceneBounds))
        {
            // Use the center of scene ie. we can see the whole scene
            m_currentCenterPoint = sceneBounds.center();
        }
        else
        {
            m_currentCenterPoint = centerPoint;

            // We need to clamp the center. The centerPoint is too large
            if (centerPoint.x() > bounds.x() + bounds.width())
            {
                m_currentCenterPoint.setX(bounds.x() + bounds.width());
            }
            else if(centerPoint.x() < bounds.x())
            {
                m_currentCenterPoint.setX(bounds.x());
            }

            if (centerPoint.y() > bounds.y() + bounds.height())
            {
                m_currentCenterPoint.setY(bounds.y() + bounds.height());
            }
            else if(centerPoint.y() < bounds.y())
            {
                m_currentCenterPoint.setY(bounds.y());
            }
        }
    }

    // Update the scrollbars
    centerOn(m_currentCenterPoint);
}

// Handles when the mouse button is pressed
void View::mousePressEvent(QMouseEvent* event)
{
    // For panning the view
    m_lastPanPoint = event->pos();
    // setCursor(Qt::ClosedHandCursor);

    QGraphicsView::mousePressEvent(event);
}

// Handles when the mouse button is released
void View::mouseReleaseEvent(QMouseEvent* event)
{
    // setCursor(Qt::OpenHandCursor);
    m_lastPanPoint = QPoint();

    QGraphicsView::mouseReleaseEvent(event);
}

// Handles the mouse move event

void View::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_lastPanPoint.isNull())
    {
        // Get how much we panned
        QPointF delta = mapToScene(m_lastPanPoint) - mapToScene(event->pos());
        m_lastPanPoint = event->pos();

        // Update the center ie. do the pan
        setCenter(getCenter() + delta);
    }

    QGraphicsView::mouseMoveEvent(event);
}

// Zoom the view in and out.

void View::wheelEvent(QWheelEvent* event)
{
    // Get the position of the mouse before scaling, in scene coords
    QPointF pointBeforeScale(mapToScene(event->pos()));

    // Get the original screen centerpoint
    QPointF screenCenter = getCenter(); //CurrentCenterPoint; //(visRect.center());

    // Scale the view ie. do the zoom
    double scaleFactor = 1.15; // How fast we zoom
    if(event->delta() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    }
    else
    {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    // Get the position after scaling, in scene coords
    QPointF pointAfterScale(mapToScene(event->pos()));

    // Get the offset of how the screen moved
    QPointF offset = pointBeforeScale - pointAfterScale;

    // Adjust to the new center for correct zooming
    QPointF newCenter = screenCenter + offset;
    setCenter(newCenter);
}

// Need to update the center so there is no jolt in the
// interaction after resizing the widget.
void View::resizeEvent(QResizeEvent* event)
{
    // Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();
    setCenter(visibleArea.center());

    // Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}

void View::setEditMode(bool status)
{
    m_editMode = status;
}
