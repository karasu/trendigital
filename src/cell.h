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
#ifndef CELL_H
#define CELL_H

#include <QDebug>
#include <QPainter>
#include <QGraphicsItem>
#include <QIcon>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
//#include <QMouseEvent>

#define CELL_TYPE QGraphicsItem::UserType+1

class Element;
class View;

class Cell : public QGraphicsItem
{
public:
    Cell(QGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    void clear();

    void addElement(int id, QIcon *pIcon);
    void addOrEditFBModule(void);
    void drawGrid(QPainter *painter);
    void drawElements(QPainter *painter, bool showElementNumber);
    void drawSelected(QPainter *painter, QPen pen);
    void drawMarked(QPainter *painter, QPen pen);
    void editElement(void);

    bool load(QDataStream & ar, int fileVersion);
    bool save(QDataStream & ar);

    /*
     // fixme
    inline int getElementBitmapNumber() { return m_elements[0]->getBitmapNumber(); }
    inline int getElementNumberOfBitmaps() { return m_elements[0]->getNumberOfBitmaps(); }
    inline int getElementInitialPosition() { return m_elements[0]->getInitialPosition(); }
    inline int getElementId() { return m_elements[0]->getId(); }
    inline int getElementNumber() { return m_elements[0]->getElementNumber(); }
    inline int getElementType() { return m_elements[0]->getElementType(); }
    inline int getNumElements() { return m_numElements; }
    inline int getAddress(int pos) { return m_elements[0]->getAddress(pos); }
    inline int getFBModuleNumber() { return m_elements[1].getFBModuleNumber(); }
    */

    inline QString getBitmapFileNameBase() { return m_elements[0]->getBitmapFileNameBase(); }
    inline QRect getZone() { return m_zone; }
    inline bool isSelected() { return m_selected; }
    inline bool isMarked() { return m_markRoute; }

    bool isPtInCell(QPoint point);

    inline void setSelected(bool selected) { m_selected = selected; }

    void markRoute(bool status);

    void setZone(QPoint topLeft, QPoint bottomRight);

    inline void setElementBitmapNumber(int bitmapNumber) { m_elements[0]->setBitmapNumber(bitmapNumber); }
    inline void setElementInitialPosition(int initialPosition) { m_elements[0]->setInitialPosition(initialPosition); }
    inline void setElementNumber(int elementNumber) { m_elements[0]->setElementNumber(elementNumber); }

    void deleteElements(void);
    void deleteFBModule(void);

    inline bool isElementNumberUsed(int elementNumber)	{ return m_elements[0]->isElementNumberUsed(elementNumber); }

    inline void setPos(int column, int row)
    {
        m_column = column;
        m_row = row;

        QGraphicsItem::setPos(CELL_WIDTH + (column * CELL_WIDTH),
                              CELL_HEIGHT + (row * CELL_HEIGHT));
    }

    enum { m_type = CELL_TYPE };

    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return m_type;
    }

    inline QPoint getPos() { return QPoint(m_column, m_row); }

    // Take over the interaction
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    // virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    // virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    // virtual void wheelEvent(QWheelEvent* event);
    // virtual void resizeEvent(QResizeEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);

    inline void setView(View *pView) { m_pView = pView; }

    View *m_pView;

protected:

    int m_column;
    int m_row;

    // Zona que delimita la casella
    QRect m_zone;

    // Indica si la casella està sel·leccionada o no
    bool m_selected;

    // Ens indica si la casella forma part de una ruta o no.
    bool m_markRoute;

    // Indica si el mouse està per sobre la casella o no
    bool m_hovered;

    // En una casella poden haver-hi a la posicio 0: Pista, pista+senyal i a la posició 1: FBmodule
    QList <Element *> m_elements;
};

#endif // CELL_H
