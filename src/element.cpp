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
#include "mainwindow.h"
#include "element.h"
#include "cell.h"
#include "grid.h"
#include "document.h"
#include "lokocommands.h"
#include "route.h"

Element::Element()
{
    qDebug() << ("Entering Element::Element()");

    m_bitmapNumber = 1; // shouldn't be zero?

    // clear();

    qDebug() << ("Element::Element() finished.");
}

Element::~Element()
{
    // clear();
}

bool Element::isElementNumberUsed(int i)
{
    if (m_elementNumber == i)
    {
        return true;
    }

    return false;
}

Document * Element::getDocument()
{
    MainWindow *pMainWindow = MainWindow::getInstance();

    if (pMainWindow != NULL)
    {
        return pMainWindow->getDocument();
    }

    return NULL;
}

MainWindow * Element::getMainWindow()
{
    return MainWindow::getInstance();
}

void Element::clear()
{
    Document *pDoc = getDocument();

    if (pDoc != NULL)
    {
        pDoc->setAddressStatus(m_address[0], FALSE);
        pDoc->setAddressStatus(m_address[1], FALSE);

        // TODO: Si hi ha el mateix contacte repetit, fent això ens carguem la llista
        // que apareix al fer les condicions inicials, és per això que
        // abans de posar-ho a false s'hauria de comprovar que no n'hi ha un
        // altre amb la mateixa adreça. (es allò de posar varis contactes junts amb
        // la mateixa adreça)
        pDoc->setFBModuleStatus(m_FBModuleNumber, FALSE);
    }

    // -------------------------------------------------------------------

    m_zone.setRect(0,0,0,0);

    m_id = INVALID;

    m_FBMDecoder = INVALID;
    m_FBMDecoderPos = INVALID;
    m_FBModuleNumber = INVALID;

    m_signalDecoder = INVALID;
    m_signalDecoderPos = INVALID;

    m_address[0] = INVALID;
    m_address[1] = INVALID;
    m_address[2] = INVALID;

    m_ecosId = INVALID;

    m_decoderType = DECODER_UNKNOWN;
    m_elementType = TYPE_UNKNOWN;

    m_bitmapNumber = 0;
    m_numberOfBitmaps = 0;

    m_initialPosition = 0;

    m_elementNumber = 0;

    m_dibs.clear();

    m_bitmapFileNameBase.clear();
}

void Element::draw(QPainter *painter, bool showElementNumber)
{
    /*
    if (m_pIcon != NULL)
    {
        painter->drawPixmap(m_zone, m_pIcon->pixmap());
    }
    */





    QPoint p(m_zone.left() + 1, m_zone.top() + 1);
    QSize s(m_zone.width() - 2, m_zone.height() - 2);

    if (m_id == 39010) // Plataforma Giratoria
    {
        s.setWidth((m_zone.width() * 3) - 2);
        s.setHeight((m_zone.height() * 3) - 2);

        p.setX((m_zone.left() + 1) - s.width() / 3);
        p.setY((m_zone.top() + 1) - s.height() / 3);

        s.setWidth(s.width() - 2);
        s.setHeight(s.height() - 2);
    }

    QRect r(p.x(), p.y(), p.x()+s.width(), p.y()+s.height());

    if (m_FBModuleNumber > 0)
    {
        // Check if we have to iluminate it!

        Document *pDoc = getDocument();

        if (pDoc != NULL)
        {
            bool isFBModuleActive = pDoc->m_isFBModuleActive[m_FBModuleNumber];

            if (isFBModuleActive)
            {

                painter->fillRect(r, QColor(255,0,0));
                painter->setBackgroundColor(QColor(0,0,0));
            }
        }
    }

    if (m_bitmapNumber >= 0)
    {

        // m_pDibs[m_bitmapNumber]->Stretch(painter, p, s, FALSE);
        painter->drawImage(r, m_dibs.at(m_bitmapNumber).pixmap());
    }

    if (showElementNumber)
    {
        if (m_FBModuleNumber > 0)
        {
            drawFBModuleNumber(painter);
        }

        if (m_elementNumber > 0)
        {
            drawElementNumber(painter);
        }
    }
}

void Element::drawThumbnail(QPainter *painter, QPoint p, QSize s)
{
    if (m_bitmapNumber >= 0 && m_bitmapNumber < m_dibs.size())
    {
        // m_pDibs[m_bitmapNumber]->Stretch(painter, p, s, FALSE);
        QRect r(p.x(), p.y(), p.x() + s.width(), p.y() + s.height());
        painter->drawImage(r, m_dibs.at(m_bitmapNumber).pixmap());
    }
}

void Element::drawFBModuleNumber(QPainter *painter)
{
    QString number;
    number.setNum(m_FBModuleNumber);

    QRect textZone(m_zone.left()+2, m_zone.right()-2, m_zone.top()+2, m_zone.bottom()-2);

    QPen oldPen(painter->pen());
    painter->setPen(QPen(QColor(255,255,255)));

    painter->drawText(textZone, Qt::AlignLeft, number);

    painter->setPen(oldPen);
}

void Element::drawElementNumber(QPainter *painter)
{
    QString number;
    number.setNum(m_elementNumber);

    QRect textZone(m_zone.left()+2, m_zone.right()-2, m_zone.top()+2, m_zone.bottom()-2);

    QPen oldPen(painter->pen());
    painter->setPen(QPen(QColor(255,255,128)));

    painter->drawText(textZone, Qt::AlignLeft, number);

    painter->setPen(oldPen);
}

void Element::setId(int id)
{
    m_id = id;

    calculateElementType();
}

int Element::getElementType()
{
    if (m_elementType == TYPE_UNKNOWN)
    {
        if (m_id > 0)
        {
            calculateElementType();
        }
    }

    return m_elementType;
}

void Element::calculateElementType()
{
    m_elementType = TYPE_UNKNOWN;

    if (m_id < 0)
    {
        // Incorrect id
        m_numberOfBitmaps = 0;
        return;
    }

    if (m_id >= 33004 && m_id <= 33007)
    {
        m_elementType = TYPE_DETACH;
        m_numberOfBitmaps = 1;
    }
    else if (m_id >= 33000 && m_id <= 33027)
    {
        m_elementType = TYPE_COMMON_TRACK;
        m_numberOfBitmaps = 1;
    }
    else if (m_id >= 34000 && m_id <= 34007)
    {
        m_elementType = TYPE_SIGNAL_THREE;
        m_numberOfBitmaps = 3;
    }
    else if (m_id >= 34008 && m_id <= 34015)
    {
        m_elementType = TYPE_SIGNAL_FOUR;
        m_numberOfBitmaps = 4;
    }
    else if (m_id >= 35000 && m_id <= 35007)
    {
        m_elementType = TYPE_SIGNAL_ONE;
        m_numberOfBitmaps = 2;
    }
    else if (m_id >= 35008 && m_id <= 35015)
    {
        m_elementType = TYPE_SIGNAL_TWO;
        m_numberOfBitmaps = 2;
    }
    else if (m_id >= 36000 && m_id <= 36015)
    {
        m_elementType = TYPE_DETOUR;
        m_numberOfBitmaps = 2;
    }
    else if (m_id >= 36016 && m_id <= 36023)
    {
        m_elementType = TYPE_TRIPLE_SWITCH;
        m_numberOfBitmaps = 3;
    }
    else if (m_id >= 36100 && m_id < 36200)
    {
        m_elementType = TYPE_DOUBLE_SLIP;
        m_numberOfBitmaps = 2;
    }
    else if (m_id >= 36200 && m_id < 36300)
    {
        m_elementType = TYPE_DOUBLE_SLIP_TWO_DECODERS;
        m_numberOfBitmaps = 4;
    }
    else if (m_id >= 36300 && m_id < 36400)
    {
        m_elementType = TYPE_SIMPLE_SLIP;
        m_numberOfBitmaps = 2;
    }
    else if (m_id >= 36400 && m_id < 36500)
    {
        m_elementType = TYPE_SIMPLE_SLIP_TWO_DECODERS;
        m_numberOfBitmaps = 4;
    }
    else if (m_id >= 37000 && m_id <= 37007)
    {
        m_elementType = TYPE_END_TRACK;
        m_numberOfBitmaps = 1;
    }
    else if (m_id >= 37008 && m_id <= 37013)
    {
        m_elementType = TYPE_CROSS;
        m_numberOfBitmaps = 1;
    }
    else if (m_id == 39001 || m_id == 39004)
    {
        m_elementType = TYPE_TRANSFER_TABLE;
        m_numberOfBitmaps = 5;
    }
    else if (m_id == 39000 || m_id == 39002 || m_id == 39003 || m_id == 39005)
    {
        m_elementType = TYPE_COMMON_TRACK;
        m_numberOfBitmaps = 1;
    }
    else if (m_id == 39010)
    {
        m_elementType = TYPE_TURN_TABLE;
        m_numberOfBitmaps = 24;
    }

    if (m_elementType != TYPE_UNKNOWN)
    {
        getAllBitmaps();
    }
}

void Element::calculateFBModuleNumber()
{
    Document *pDoc = getDocument();
    ASSERT(pDoc != NULL);

    int FBModuleNumber = ((m_FBMDecoder - 1) * pDoc->m_decoderPositions)
                         + m_FBMDecoderPos;

    setFBModuleNumber(FBModuleNumber);
}

void Element::setFBModuleNumber(int i)
{
    Document *pDoc = getDocument();
    ASSERT(pDoc != NULL);

    if (m_FBModuleNumber != INVALID)
    {
        // TODO: Si hi ha el mateix contacte repetit, fent això ens carguem la llista
        // que apareix al fer les condicions inicials, és per això que
        // abans de posar-ho a false s'hauria de comprovar que no n'hi ha un
        // altre amb la mateix adreça. (es allò de posar varis contactes junts amb
        // la mateixa adreça)
        // Aquí el problema és més greu, perquè si editem el contacte, a la llista de
        // contactes al formulari de condicions inicials ens apareixen els dos, l'antic
        // i el nou. Tanmateix, si activem aquesta línia, pot passar que tenint dos contactes
        // iguals, n'editem un dels dos (pq ens hem equivocat, p.e.) llavors no ens
        // apareixeran tots dos a la llista de contactes de condicions inicials!!

        // pDoc->setFBModuleStatus(m_iFBModuleNumber, FALSE);
    }

    // This tells us that this address is already used
    pDoc->setFBModuleStatus(i, TRUE);

    m_FBModuleNumber = i;
}

void Element::setAddress(int pos, int address)
{
    Document *pDoc = getDocument();
    ASSERT(pDoc != NULL);

    if (m_address[pos] == address)
    {
        return;
    }
    else if (m_address[pos] != -1)
    {
        if (pDoc != NULL)
        {
            pDoc->setAddressStatus(m_address[pos], false);
        }
    }

    m_address[pos] = address;

    if (pDoc != NULL)
    {
        ASSERT(pDoc->isAddressAvailable(address));

        pDoc->setAddressStatus(address, true);
    }
}

bool Element::getAllBitmaps()
{
    // just in case
    m_dibs.clear();

    if (m_numberOfBitmaps < 1)
    {
        return true;
    }

    QString programPath(QApplication::applicationDirPath()); // was g_programPath in MFC
    QString fileNameBase = programPath + "/images/bitmaps/";

    switch(m_elementType)
    {
        case TYPE_COMMON_TRACK:
            fileNameBase += "Common";
            break;
        case TYPE_DETOUR:
            fileNameBase += "Detour";
            break;
        case TYPE_SIGNAL_ONE:
        case TYPE_SIGNAL_TWO:
        case TYPE_SIGNAL_THREE:
        case TYPE_SIGNAL_FOUR:
            fileNameBase += "Signal";
            break;
        case TYPE_SIMPLE_SLIP:
        case TYPE_SIMPLE_SLIP_TWO_DECODERS:
            fileNameBase += "SimpleSlip";
            break;
        case TYPE_DOUBLE_SLIP:
        case TYPE_DOUBLE_SLIP_TWO_DECODERS:
            fileNameBase += "DoubleSlip";
            break;
        case TYPE_TRIPLE_SWITCH:
            fileNameBase += "TripleSwitch";
            break;

        case TYPE_END_TRACK:
            fileNameBase += "EndTrack";
            break;
        case TYPE_CROSS:
            fileNameBase += "Cross";
            break;
        case TYPE_TRANSFER_TABLE:
            fileNameBase += "plataforma";
            break;
        case TYPE_TURN_TABLE:
            fileNameBase += "plataforma";
            break;
        case TYPE_DETACH:
            fileNameBase += "Detach";
            break;
        default:
            fileNameBase += "Common";
            break;
    }

    QString id;

    id.setNum(m_id);

    fileNameBase += QString("_") + id;

    m_bitmapFileNameBase = fileNameBase;

    for (int i=0; i<m_numberOfBitmaps; i++)
    {
        QString index;

        index.setNum(i);

        QString fileName;

        fileName = QString(":/images/bitmaps/") + QString(fileNameBase) + QString("_") + QString(index) + QString(".png");

        m_dibs.append(QIcon(fileName));
    }

    return true;
}

bool Element::save(QDataStream & ar)
{
    for (int i=0; i<POSSIBLE_ADDRESSES_BY_ELEMENT; i++)
    {
        ar << m_address[i];
    }

    ar << m_bitmapNumber;
    ar << m_decoderType;
    ar << m_elementNumber;
    ar << m_elementType;
    ar << m_FBMDecoder;
    ar << m_FBMDecoderPos;
    ar << m_FBModuleNumber;
    ar << m_id;
    ar << m_initialPosition;
    ar << m_numberOfBitmaps;
    ar << m_signalDecoder;
    ar << m_signalDecoderPos;
    ar << m_zone;
    ar << m_bitmapFileNameBase;

    return true;
}

bool Element::load(QDataStream & ar, int fileVersion)
{
    for (int i=0; i<POSSIBLE_ADDRESSES_BY_ELEMENT; i++)
    {
        ar >> m_address[i];
    }

    ar >> m_bitmapNumber;
    ar >> m_decoderType;
    ar >> m_elementNumber;
    ar >> m_elementType;
    ar >> m_FBMDecoder;
    ar >> m_FBMDecoderPos;
    ar >> m_FBModuleNumber;
    ar >> m_id;
    ar >> m_initialPosition;
    ar >> m_numberOfBitmaps;
    ar >> m_signalDecoder;
    ar >> m_signalDecoderPos;
    ar >> m_zone;
    ar >> m_bitmapFileNameBase;

    setId(m_id);

    if (m_address[0] != INVALID && m_address[1] == INVALID)
    {
        // fixme: create the g_interface global value
        // g_interface.storeSwitch(m_address[0], &m_ecosId);
    }

    return true;
}
