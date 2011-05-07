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
#ifndef GRID_H
#define GRID_H

#include <QPainter>
#include <QMessageBox>

class Cell;

class Grid
{
public:
    Grid();
    virtual ~Grid();

    void clear();

    void deleteCell(Cell cell);
    void draw(QPainter *painter, bool design, bool showElementNumber);

    void drawThumbnail(QPainter *painter);

    Cell * getCellSelected();
    Cell * getCellByAddress(int address);
    Cell * getCellByElementNumber(int elementNumber);
    inline Cell * getCell(int i, int j) { return &m_map[i][j]; }

    bool load(QDataStream & ar, int fileVersion);
    bool save(QDataStream & ar);

    void selectCell(QPoint p);

    void updateGrid(void);

    void setZoom(int zoom);
    inline int getZoom() { return m_zoom; }

    inline int getColumns() { return m_columns; }
    inline int getRows() { return m_rows; }
    inline int getCellSize() { return m_cellSize; }

    QSize getTotalSize(void); // Returns total size (including zoom!)

    inline QPoint getStartDrawingPos() { return m_startDrawingPos; }

    void setStartDrawingPos(QPoint p);

    void markRoute(QPoint p);
    void markRoute(int row, int column);
    void unmarkAllRoutes(void);

    void deleteAllInterfaceSwitches(void);  // Special for new TCP/IP stations

    bool isElementNumberUsed(int elementNumber);

protected:
    Cell m_map[DEFAULT_GRID_WIDTH][DEFAULT_GRID_HEIGHT];

    int m_zoom;
    int m_cellSize;
    int m_rows;
    int m_columns;

    QPoint m_startDrawingPos;
};

#endif // GRID_H
