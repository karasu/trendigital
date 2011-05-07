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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#ifndef ELEMENT_H
#define ELEMENT_H

#include <QList>
#include <QPainter>

#include "general.h"

class MainWindow;
class Document;

class Element
{
public:
    Element();
    virtual ~Element();

    void clear(void);

    // LoadAllBitmaps in mfc version
    // bool getAllBitmaps(void);

    void draw(QPainter *painter, bool showElementNumber);

    void calculateElementType(void);
    void calculateFBModuleNumber(void);

    bool load(QDataStream & ar, int fileVersion);
    bool save(QDataStream & ar);

    int getElementType(void);

    inline QString getBitmapFileNameBase() { return m_bitmapFileNameBase; }

    inline int getId() { return m_id; }
    inline int getFBModuleDecoder() { return m_FBMDecoder; }
    inline int getFBModuleDecoderPos() { return m_FBMDecoderPos; }
    inline int getFBModuleNumber() { return m_FBModuleNumber; }
    inline int getAddress(int iPos) { return m_address[iPos]; }
    inline int getElementNumber() { return m_elementNumber; }
    inline int getBitmapNumber() {return m_bitmapNumber; }
    inline int getNumberOfBitmaps() { return m_numberOfBitmaps; }
    inline int getInitialPosition() { return m_initialPosition; }
    inline int getSignalDecoder() { return m_signalDecoder; }
    inline int getSignalDecoderPos() { return m_signalDecoderPos; }
    inline int getDecoderType() { return m_decoderType; }

    inline void setFBModuleDecoder(int i) { m_FBMDecoder = i; }
    inline void setFBModuleDecoderPos(int i) { m_FBMDecoderPos = i; }
    inline void setZone(QRect r) { m_zone = r; }
    inline void setSignalDecoder(int i) { m_signalDecoder = i; }
    inline void setSignalDecoderPos(int i) { m_signalDecoderPos = i; }
    inline void setDecoderType(int i) { m_decoderType = i; }
    inline void setElementType(int i) { m_elementType = i; }
    inline void setBitmapNumber(int i) { m_bitmapNumber = i; }
    inline void setInitialPosition(int i) { m_initialPosition = i; }
    inline void setElementNumber(int i) { m_elementNumber = i; }

    inline void setIcon(QIcon *pIcon) { m_pIcon = pIcon; }

    void setFBModuleNumber(int i);
    void setId(int id);
    void setAddress(int pos, int address);

    void destroy(void);

    void drawFBModuleNumber(QPainter *painter);
    void drawElementNumber(QPainter *painter);

    bool isElementNumberUsed(int i);

    void drawThumbnail(QPainter *painter, QPoint p, QSize s);

    QIcon *m_pIcon;

protected:

    Document * getDocument(void);
    MainWindow * getMainWindow(void);

    int m_bitmapNumber;
    int m_numberOfBitmaps;

    int m_initialPosition;

    QRect m_zone;

    int m_id;				// ID del botó.
    int m_elementType;      // Tipus d'element (desviament, senyal, ...)
    int m_elementNumber;	// Numero d'element editable per decoder.

    int m_FBMDecoder;		// Decoder on està el feedback module
    int m_FBMDecoderPos;	// Posició del decoder on es troba el fm (1,2,3,...16)
    int m_FBModuleNumber;	// Número total del Modul.

    int m_signalDecoder;
    int m_signalDecoderPos;

    int m_decoderType;

    int m_address[POSSIBLE_ADDRESSES_BY_ELEMENT]; // Adreça interna per l'interface.

    QString m_bitmapFileNameBase;

    int m_ecosId;
};

#endif // ELEMENT_H
