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
#include "grid.h"

Grid::Grid()
{
    // clear();
}

Grid::~Grid()
{
}

void Grid::clear()
{
    m_zoom = 4;
    m_cellSize = 6;
    m_columns = DEFAULT_GRID_WIDTH;
    m_rows = DEFAULT_GRID_HEIGHT;

    for (int i=0; i<DEFAULT_GRID_WIDTH; i++)
    {
        for (int j=0; j<DEFAULT_GRID_HEIGHT; j++)
        {
            m_map[i][j].clear();
            m_map[i][j].setPos(i,j);
        }
    }

    m_startDrawingPos = QPoint(0,0);

    // Special for ECOS and Märklín CS1 TCP/IP stations
    deleteAllInterfaceSwitches();
}

// Special for ECOS and Märklín CS1 TCP/IP stations
void Grid::deleteAllInterfaceSwitches()
{
    // fixme: create the g_interface global var or whatever is needed
    /*
    if (!g_interface.deleteAllSwitches())
    {
        qDebug << tr("Grid::deleteAllInterfaceSwitches(): Interface not connected!");
        int ret = QMessageBox::critical(this, tr("Tren Digital - Error!"),
                                       tr("No puc connectar amb la interfície."),
                                       QMessageBox::Ok, QMessageBox::Ok);
    }
    */
}

void Grid::deleteCell(Cell cell)
{
    ASSERT(false);
}

void Grid::drawThumbnail(QPainter *painter)
{
    QRect r(m_columns, m_rows, 0, 0);

    int i=0, j=0;

    for (j=0; j < m_rows; j++)
    {
        for (i=0; i < m_columns; i++)
        {
            if (m_map[i][j].getElementNumberOfBitmaps() > 0)
            {
                if (r.left() > i) r.setLeft(i);
                if (r.right() < i) r.setRight(i);

                if (r.top() > j) r.setTop(j);
                if (r.bottom() < j) r.setBottom(j);
            }
        }
    }

    QSize s(0,0);

    s.setWidth(THUMBNAIL_WIDTH / (r.width() + 1));
    s.setHeight(THUMBNAIL_HEIGHT / (r.height() + 1));

    if (s.width() > s.height())
    {
        s.setWidth(s.height());
    }
    else
    {
        s.setHeight(s.width());
    }

    QSize thumbnailSize(s.width() * (r.width()+1), s.height() * (r.height()+1));

    QPoint p0(0,0);

    if (thumbnailSize.width() < THUMBNAIL_WIDTH)
    {
        p0.setX((THUMBNAIL_WIDTH - thumbnailSize.width()) / 2);
    }

    if (thumbnailSize.height() < THUMBNAIL_HEIGHT)
    {
        p0.setY((THUMBNAIL_HEIGHT - thumbnailSize.height()) / 2);
    }

    QPoint p(p0);

    for (j=r.top(); j<=r.bottom(); j++)
    {
        for (i=r.left(); i<=r.right(); i++)
        {
            m_map[i][j].getElementPtr()->drawThumbnail(painter, p, s);
            p.setX(p.x() + s.width());
        }

        p.setX(p0.x());
        p.setY(p.y() + s.height());
    }
}

// *********************************************************************************
// Fixme: brush thingie
// also in cell.cpp
void Grid::draw(QPainter *painter, bool design, bool showElementNumber)
{
    updateGrid();

    if (design)
    {
        QPen pen(QColor(125,125,125));

        QPen oldPen = painter->pen();
        painter->setPen(pen);

        for (int j=0; j < m_rows; j++)
        {
            for (int i=0; i < m_columns; i++)
            {
                m_map[i][j].drawGrid(painter);
            }
        }

        QRect r = m_map[0][0].getZone();

        QPoint p0(r.left(), r.top());

        r = m_map[0][m_rows-1].getZone();

        QPoint p1(r.left(), r.bottom());

        r = m_map[m_columns-1][m_rows-1].getZone();

        QPoint p2(r.right()+1, r.bottom());

        painter->drawLine(p0, p1);
        painter->drawLine(p1, p2);

        painter->setPen(oldPen);
    }

    // ----------------------------------------------------

    for (int j=0; j<m_rows; j++)
    {
        for (int i=0; i<m_columns; i++)
        {
            m_map[i][j].drawElements(painter, showElementNumber);
        }
    }

    // ----------------------------------------------------

    QBrush oldBrush = painter->brush();

    painter->setBrush(QBrush(QColor(255,255,128)));

    for (int j=0; j < m_rows; j++)
    {
        for (int i=0; i < m_columns; i++)
        {
            m_map[i][j].drawMarked(painter, NULL, design);
        }
    }

    // ----------------------------------------------------

    painter->setBrush(QBrush(QColor(255,0,0)));

    for (int j=0; j < m_rows; j++)
    {
        for (int i=0; i < m_columns; i++)
        {
            m_map[i][j].drawSelected(painter, NULL, design);
        }
    }

    painter->setBrush(oldBrush);
}

Cell * Grid::getCellSelected()
{
    for (int j=0; j < m_rows; j++)
    {
        for (int i=0; i < m_columns; i++)
        {
            if (m_map[i][j].isSelected())
            {
                return &m_map[i][j];
            }
        }
    }
    return NULL;
}

void Grid::selectCell(QPoint p)
{
    for (int j=0; j < m_rows; j++)
    {
        for (int i=0; i < m_columns; i++)
        {
            m_map[i][j].setSelected(false);
        }
    }

    for (int j=0; j < m_rows; j++)
    {
        for (int i=0; i < m_columns; i++)
        {
            if (m_map[i][j].isPtInCell(p))
            {
                m_map[i][j].setSelected(true);
            }
        }
    }
}

void Grid::unmarkAllRoutes()
{
    for (int j=0; j < m_rows; j++)
    {
        for (int i=0; i < m_columns; i++)
        {
            m_map[i][j].markRoute(false);
        }
    }
}

void Grid::markRoute(QPoint p)
{
    for (int j=0; j < m_rows; j++)
    {
        for (int i=0; i < m_columns; i++)
        {
            if (m_map[i][j].isPtInCell(p))
            {
                m_map[i][j].markRoute(true);
            }
        }
    }
}

void Grid::markRoute(int row, int column)
{
    m_map[column][row].markRoute(true);
}

void Grid::updateGrid()
{
    int zoomedCellSize = m_cellSize * m_zoom;

    QPoint tlPoint(0,0);

    QPoint brPoint(zoomedCellSize, zoomedCellSize);

    int startX = m_startDrawingPos.x();

    tlPoint += m_startDrawingPos;
    brPoint += m_startDrawingPos;

    for (int j=0; j < m_rows; j++)
    {
        for (int i=0; i < m_columns; i++)
        {
            m_map[i][j].setZone(tlPoint, brPoint);

            tlPoint.setX(tlPoint.x() + zoomedCellSize);
            brPoint.setX(brPoint.x() + zoomedCellSize);
        }

        tlPoint.setX(startX);
        brPoint.setX(startX + zoomedCellSize);

        tlPoint.setY(tlPoint.y() + zoomedCellSize);
        brPoint.setY(brPoint.y() + zoomedCellSize);
    }
}

void Grid::setZoom(int zoom)
{
    m_zoom = zoom;

    updateGrid();
}

bool Grid::save(QDataStream & ar)
{
    ar << m_cellSize;
    ar << m_columns;
    ar << m_rows;
    ar << m_zoom;
    ar << m_startDrawingPos;

    // Fixme: show some kind of progress bar
    // int numCells = m_columns * m_rows;
    // QString strMsg(tr("Saving..."));
    // CProgressBar bar(strMsg, 40, iNumCells);

    for (int i=0; i<m_columns; i++)
    {
        for (int j=0; j<m_rows; j++)
        {
            m_map[i][j].save(ar);
            // bar.StepIt();
        }
    }

    return true;
}

bool Grid::load(QDataStream & ar, int fileVersion)
{
    clear();

    ar >> m_cellSize;
    ar >> m_columns;
    ar >> m_rows;
    ar >> m_zoom;
    ar >> m_startDrawingPos;

    // Fixme: show some kind of progress bar
    // int numCells = m_columns * m_rows;
    // strMsg = GetTranslatedStringID(IDS_SAVING);
    // CProgressBar bar(strMsg, 40, iNumCells);

    for (int i=0; i<m_columns; i++)
    {
        for (int j=0; j<m_rows; j++)
        {
            m_map[i][j].load(ar, fileVersion);
            // bar.StepIt();
        }
    }

    return true;
}

bool Grid::isElementNumberUsed(int elementNumber)
{
    for (int j=0; j<m_rows; j++)
    {
        for (int i=0; i<m_columns; i++)
        {
            if (m_map[i][j].isElementNumberUsed(elementNumber))
            {
                return true;
            }
        }
    }

return false;
}

Cell * Grid::getCellByAddress(int address)
{
    for (int j=0; j < m_rows; j++)
    {
        for (int i=0; i < m_columns; i++)
        {
            for (int k=0; k<POSSIBLE_ADDRESSES_BY_ELEMENT; k++)
            {
                if (address == m_map[i][j].getAddress(k))
                {
                    return &m_map[i][j];
                }
            }
        }
    }

    return NULL;
}

Cell * Grid::getCellByElementNumber(int elementNumber)
{
    for (int j=0; j < m_rows; j++)
    {
        for (int i=0; i < m_columns; i++)
        {
            if (elementNumber == m_map[i][j].getElementNumber())
            {
                return &m_map[i][j];
            }
        }
    }

    return NULL;
}

QSize Grid::getTotalSize()
{
    QSize s;

    s.setWidth((m_columns * m_cellSize * m_zoom) + m_startDrawingPos.x());
    s.setHeight((m_rows * m_cellSize * m_zoom) + m_startDrawingPos.y());

    return s;
}

/*
void CGrid::SetStartDrawingPos(CPoint p)
{
CMDIFrameWnd *pFrame = (CMDIFrameWnd *)AfxGetApp()->m_pMainWnd;
ASSERT_VALID(pFrame);

CMDIChildWnd *pChild = (CMDIChildWnd *) pFrame->GetActiveFrame();

ASSERT_VALID(pChild);

CWnd *pView = (CWnd *) pChild->GetActiveView();

CRect r;

pView->GetClientRect(&r);

m_startDrawingPos += p;

if (m_startDrawingPos.x < 0)
{
m_startDrawingPos.x = 0;
}

const int iMaxRight = r.right - 30;

if (m_startDrawingPos.x > iMaxRight)
{
m_startDrawingPos.x = iMaxRight;
}

if (m_startDrawingPos.y < 0)
{
m_startDrawingPos.y = 0;
}

const int iMaxBottom = r.bottom - 30;

if (m_startDrawingPos.y > iMaxBottom)
{
m_startDrawingPos.y = iMaxBottom;
}
}
*/
