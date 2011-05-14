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

Cell::Cell(QGraphicsItem *parent) : QGraphicsItem(parent)

{
    // qDebug() << ("Entering Cell::Cell()");

    clear();

    m_pView = NULL;

    setAcceptHoverEvents(true);

    // qDebug() << ("Cell::Cell() finished.");
}

void Cell::clear()
{
    m_zone = QRect(0, 0, CELL_WIDTH, CELL_HEIGHT);
    m_selected = false;
    m_markRoute = false;
    m_hovered = false;

    for (int i=0; i<m_elements.count(); i++)
    {
        delete m_elements[i];
    }

    m_elements.clear();
}

// *********************************************************************************

void Cell::setZone(QPoint topLeft, QPoint bottomRight)
{
    m_zone = QRect(topLeft, bottomRight);

    for (int i=0; i<m_elements.count(); i++)
    {
        m_elements[i]->setZone(m_zone);
    }
}

bool Cell::isPtInCell(QPoint point)
{
    // La m_zone inclou els límits, per veure si un punt està dins o no de la cel·la s'han de restar.

    QRect r(m_zone.left()+1, m_zone.top()+1, m_zone.width()-1, m_zone.height()-1);

    if (r.contains(point))
    {
        return true;
    }

    return false;
}

void Cell::addElement(int id)
{
    if (m_elements.count() > 0)
    {
        qDebug() << ("There is already an element in this cell");
    }
    else
    {
        Element *pNewElement = new Element();
        pNewElement->setZone(m_zone);
        pNewElement->setId(id);
        m_elements.append(pNewElement);

        // fixme
        /*
        CDigitalTrainDoc *pDoc = (CDigitalTrainDoc *)GetCurrentDocument();
        ASSERT(pDoc != NULL);
        pDoc->SetModifiedFlag(TRUE);
        */
    }
}

void Cell::deleteFBModule()
{
    if (m_elements.count() > 1)
    {
        m_elements[1]->setFBModuleDecoder(INVALID);
        m_elements[1]->setFBModuleDecoderPos(INVALID);
        m_elements[1]->setFBModuleNumber(INVALID);
    }
}

void Cell::addOrEditFBModule()
{
    /*
    CEditFBModuleDlg editFBModuleDlg;

    if (m_elements[1].GetFBModuleNumber() > 0)
    {
        editFBModuleDlg.SetFBModuleDecoder(m_elements[1].GetFBModuleDecoder());
        editFBModuleDlg.SetFBModuleDecoderPosition(m_elements[1].GetFBModuleDecoderPos());
        editFBModuleDlg.SetFBModuleNumber(m_elements[1].GetFBModuleNumber());
    }
    else
    {
        m_iNumElements = 2;
    }

    editFBModuleDlg.DoModal();

    m_elements[1].SetFBModuleDecoder(editFBModuleDlg.GetFBModuleDecoder());
    m_elements[1].SetFBModuleDecoderPos(editFBModuleDlg.GetFBModuleDecoderPosition());
    m_elements[1].CalculateFBModuleNumber();

    m_elements[1].SetZone(m_zone);
    m_elements[1].SetID(TYPE_FBMODULE);
    */
}

void Cell::editElement()
{
    /*
    CDigitalTrainApp *pApp = (CDigitalTrainApp *)AfxGetApp();

    ASSERT(m_iNumElements > 0);

    if (m_iNumElements < 1)
    {
        return;
    }

    int iDecoderType = m_elements[0].GetDecoderType();
    int iElementType = m_elements[0].GetElementType();

    if (iDecoderType == DECODER_UNKNOWN && iElementType != TYPE_COMMON_TRACK)
    {
        if (!pApp->IsMarklin())
        {
            if(iElementType == TYPE_TRANSFER_TABLE)
            {
                iDecoderType = DECODER_K84;
            }
            else
            {
                iDecoderType = DECODER_K83;
            }

            m_elements[0].SetDecoderType(iDecoderType);
        }
        else
        {

            CAskDecoderTypeDlg askDecoderTypeDlg;

            askDecoderTypeDlg.SetElementType(iElementType);

            askDecoderTypeDlg.DoModal();


            iDecoderType = askDecoderTypeDlg.GetDecoderType();
            m_elements[0].SetDecoderType(iDecoderType);
        }
    }

    // Segons el tipus mostrar un diàleg o altre per editar l'element!
    int iAddress1=0;

    switch(iDecoderType)
    {
        case DECODER_74460:
        {
            CEditDecoder74460Dlg editDecoder74460;

            editDecoder74460.SetAddress(GetAddress(0));
            editDecoder74460.SetElementType(iElementType);
            editDecoder74460.SetElementNumber(GetElementNumber());

            if (editDecoder74460.DoModal() == IDOK)
            {
                int iAddress = editDecoder74460.GetAddress();
                m_elements[0].SetAddress(0, iAddress);

                int iElementNumber = editDecoder74460.GetElementNumber();
                m_elements[0].SetElementNumber(iElementNumber);
            }

            break;
        }
        case DECODER_K83:
        {
            if (iElementType == TYPE_TRIPLE_SWITCH ||
                iElementType == TYPE_SIGNAL_THREE ||
                iElementType == TYPE_SIGNAL_FOUR ||
                iElementType == TYPE_DOUBLE_SLIP_TWO_DECODERS ||
                iElementType == TYPE_SIMPLE_SLIP_TWO_DECODERS)
            {
                CEditDecoderK83K84TwoAddressDlg editDecoderK83TwoAddress;
                editDecoderK83TwoAddress.SetAddress1(GetAddress(0));
                editDecoderK83TwoAddress.SetAddress2(GetAddress(1));
                editDecoderK83TwoAddress.SetDecoderType(iDecoderType);
                editDecoderK83TwoAddress.SetElementType(iElementType);

                editDecoderK83TwoAddress.SetElementNumber(GetElementNumber());

                if (editDecoderK83TwoAddress.DoModal() == IDOK)
                {
                    int iAddress1 = editDecoderK83TwoAddress.GetAddress1();
                    int iAddress2 = editDecoderK83TwoAddress.GetAddress2();

                    m_elements[0].SetAddress(0, iAddress1);
                    m_elements[0].SetAddress(1, iAddress2);

                    int iElementNumber = editDecoderK83TwoAddress.GetElementNumber();
                    m_elements[0].SetElementNumber(iElementNumber);
                }
            }
            else
            {
                CEditDecoderK83K84Dlg editDecoderK83;
                editDecoderK83.SetAddress(GetAddress(0));
                editDecoderK83.SetDecoderType(iDecoderType);
                editDecoderK83.SetElementType(iElementType);
                editDecoderK83.SetElementNumber(GetElementNumber());

                if (editDecoderK83.DoModal() == IDOK)
                {
                    int iAddress = editDecoderK83.GetAddress();
                    m_elements[0].SetAddress(0, iAddress);

                    int iElementNumber = editDecoderK83.GetElementNumber();
                    m_elements[0].SetElementNumber(iElementNumber);
                }
            }
            break;
        }
        case DECODER_K84:
        {
            if (iElementType == TYPE_SIGNAL_THREE
                || iElementType == TYPE_SIGNAL_FOUR)
            {
                CEditDecoderK83K84TwoAddressDlg editDecoderK83TwoAddress;
                editDecoderK83TwoAddress.SetAddress1(GetAddress(0));
                editDecoderK83TwoAddress.SetAddress2(GetAddress(1));
                editDecoderK83TwoAddress.SetDecoderType(iDecoderType);
                editDecoderK83TwoAddress.SetElementType(iElementType);

                editDecoderK83TwoAddress.SetElementNumber(GetElementNumber());

                if (editDecoderK83TwoAddress.DoModal() == IDOK)
                {
                    int iAddress1 = editDecoderK83TwoAddress.GetAddress1();
                    int iAddress2 = editDecoderK83TwoAddress.GetAddress2();

                    m_elements[0].SetAddress(0, iAddress1);
                    m_elements[0].SetAddress(1, iAddress2);

                    int iElementNumber = editDecoderK83TwoAddress.GetElementNumber();
                    m_elements[0].SetElementNumber(iElementNumber);
                }
            }
            else if(iElementType == TYPE_TRANSFER_TABLE)
            {
                CEditDecoderK83K84ThreeAddressesDlg editDecoderK83K84ThreeAddresses;

                editDecoderK83K84ThreeAddresses.SetAddress1(GetAddress(0));
                editDecoderK83K84ThreeAddresses.SetAddress2(GetAddress(1));
                editDecoderK83K84ThreeAddresses.SetAddress3(GetAddress(2));

                editDecoderK83K84ThreeAddresses.SetDecoderType(iDecoderType);
                editDecoderK83K84ThreeAddresses.SetElementType(iElementType);

                editDecoderK83K84ThreeAddresses.SetElementNumber(GetElementNumber());

                if (editDecoderK83K84ThreeAddresses.DoModal() == IDOK)
                {
                    int iAddress1 = editDecoderK83K84ThreeAddresses.GetAddress1();
                    int iAddress2 = editDecoderK83K84ThreeAddresses.GetAddress2();
                    int iAddress3 = editDecoderK83K84ThreeAddresses.GetAddress3();

                    m_elements[0].SetAddress(0, iAddress1);
                    m_elements[0].SetAddress(1, iAddress2);
                    m_elements[0].SetAddress(2, iAddress3);

                    int iElementNumber = editDecoderK83K84ThreeAddresses.GetElementNumber();
                    m_elements[0].SetElementNumber(iElementNumber);
                }
            }

            else
            {
                CEditDecoderK83K84Dlg editDecoderK84;

                editDecoderK84.SetAddress(GetAddress(0));
                editDecoderK84.SetDecoderType(iDecoderType);
                editDecoderK84.SetElementType(iElementType);
                editDecoderK84.SetElementNumber(GetElementNumber());

                editDecoderK84.DoModal();

                int iAddress = editDecoderK84.GetAddress();
                m_elements[0].SetAddress(0, iAddress);

                int iElementNumber = editDecoderK84.GetElementNumber();
                m_elements[0].SetElementNumber(iElementNumber);
            }

            break;
        }
        case DECODER_DIGITAL:
        {
            CEditDecoderDigitalDlg editDecoderDigital;

            if (iElementType == TYPE_SIGNAL_ONE || iElementType == TYPE_SIGNAL_TWO)
            {
                editDecoderDigital.SetAddress(GetAddress(0));

                editDecoderDigital.SetElementType(iElementType);

                editDecoderDigital.SetElementNumber(GetElementNumber());

                if (editDecoderDigital.DoModal() == IDOK)
                {
                    int iAddress = editDecoderDigital.GetAddress();
                    m_elements[0].SetAddress(0, iAddress);

                    int iElementNumber = editDecoderDigital.GetElementNumber();
                    m_elements[0].SetElementNumber(iElementNumber);
                }
            }
            else // TYPE_SIGNAL_THREE, TYPE_SIGNAL_FOUR, .... ??
            {
                editDecoderDigital.SetTwoAddresses(TRUE);

                editDecoderDigital.SetAddress(GetAddress(0));

                editDecoderDigital.SetAddress2(GetAddress(1));

                editDecoderDigital.SetElementType(iElementType);

                editDecoderDigital.SetElementNumber(GetElementNumber());

                if (editDecoderDigital.DoModal() == IDOK)
                {
                    int iAddress = editDecoderDigital.GetAddress();
                    m_elements[0].SetAddress(0, iAddress);

                    int iAddress2 = editDecoderDigital.GetAddress2();
                    m_elements[0].SetAddress(1, iAddress2);

                    int iElementNumber = editDecoderDigital.GetElementNumber();
                    m_elements[0].SetElementNumber(iElementNumber);
                }
            }

            break;
        }

        case DECODER_UNKNOWN:
            break;
    }
*/
}

bool Cell::save(QDataStream & ar)
{
    ar << m_markRoute;
    ar << m_selected;

    for (int i=0; i<m_elements.count(); i++)
    {
        m_elements[i]->save(ar);
    }

    ar << m_elements.count(); //  fixme : this should be before the for
    ar << m_zone;

    return true;
}

bool Cell::load(QDataStream & ar, int fileVersion)
{
    qint32 num;

    ar >> m_markRoute;
    ar >> m_selected;

    for (int i=0; i<2; i++)
    {
        Element *pNewElement = new Element();
        pNewElement->load(ar, fileVersion);
        m_elements.append(pNewElement);
    }

    ar >> num;
    ar >> m_zone;

    return true;
}


void Cell::markRoute(bool status)
{
    if (m_elements.count() >= 1)
    {
        m_markRoute = status;
    }
}

void Cell::deleteElements()
{
    m_selected = false;

    m_markRoute = false;

    for (int i=0; i<m_elements.count(); i++)
    {
        delete m_elements[i];
    }

    m_elements.clear();
}

// -----------------------------------------------------------------

QRectF Cell::boundingRect() const
{
    return QRectF(0, 0, CELL_WIDTH, CELL_HEIGHT);
}

void Cell::drawElements(QPainter *painter, bool showElementNumber)
{
    // Create and set element numbers font

    QFont oldFont = painter->font();

    painter->setFont(QFont("Arial", 40 + (m_zone.width() * 2)));

    for (int i=0; i<m_elements.count(); i++)
    {
        m_elements[i]->draw(painter, showElementNumber);
    }

    painter->setFont(oldFont);
}

// *********************************************************************************
// Fixme: brush thingie
// also in grid.cpp
void Cell::drawMarked(QPainter *painter, QPen pen)
{
    if (m_markRoute)
    {
        painter->setPen(pen);
        painter->drawRect(m_zone);
    }
}

// Fixme: brush thingie
void Cell::drawSelected(QPainter *painter, QPen pen)
{
    bool editMode = false;

    if (m_pView != NULL)
    {
        editMode = m_pView->editMode();
    }

    if (editMode && m_selected)
    {
        painter->setPen(pen);
        painter->drawRect(m_zone);
    }
}

void Cell::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    //painter->setClipRect( option->exposedRect );


    if (m_elements.count() > 0)
    {
        Element *pElement = m_elements.at(0);

        if (pElement != NULL)
        {
            pElement->draw(painter, false);
        }
        else
        {
            qDebug() << ("cell::paint(): pElement is null!");
        }
    }

    // fixme
    // drawElements(painter, false);

    if (!m_markRoute)
    {
//        painter->setBrush(Qt::black);
        if (m_hovered && m_pView->editMode())
        {
            painter->setPen(Qt::red);
        }
        else
        {
            painter->setPen(Qt::white);
        }

        painter->setBrush(Qt::NoBrush);
        painter->drawRect(m_zone);
        painter->setBrush(Qt::SolidPattern);
    }
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // For panning the view
    QPointF pos = event->pos();

    QGraphicsItem::mousePressEvent(event);

    ASSERT(m_pView != NULL);

    switch(event->button())
    {
        case Qt::LeftButton:
            if (m_pView->editMode())
            {
                addElement(m_pView->m_currentElementId);
            }
            else
            {
                qDebug() << ("Cell:mousePressEvent not in edit mode.");
                m_selected = !m_selected;
            }
            update(boundingRect());
            break;

        case Qt::MiddleButton:
            break;

        case Qt::RightButton:
            break;

        case Qt::NoButton:
            break;
        case Qt::XButton1:
            break;
        case Qt::XButton2:
            break;
        case Qt::MouseButtonMask:
            break;

    }

    // setCursor(Qt::ClosedHandCursor);

}

void Cell::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    m_hovered = true;
    update(boundingRect());
}

void Cell::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    m_hovered = false;
    update(boundingRect());
}
