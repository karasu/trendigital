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
#include "../general.h"

#include "editlokos.h"
#include "ui_editlokos.h"

#include "../loko.h"
#include "iconview.h"

EditLokos::EditLokos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditLokos)
{
    ui->setupUi(this);

    ui->address->setMaximum(LOKO_MAX_ADDRESS);

    QPushButton *printButton = new QPushButton(tr("&Imprimir"));
    printButton->setCheckable(false);
    printButton->setAutoDefault(false);
    ui->buttonBox->addButton(printButton, QDialogButtonBox::ActionRole);

    // connect(ui->actionNou, SIGNAL(triggered()), this, SLOT(onNew()));

    connect(ui->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onTreeWidgetItemClicked(QTreeWidgetItem*,int)));
    connect(ui->image, SIGNAL(clicked()), this, SLOT(onImageButton()));
    connect(ui->pushButtonNewLoko, SIGNAL(clicked()), this, SLOT(onNewLoko()));
    connect(printButton, SIGNAL(clicked()), this, SLOT(onPrint()));
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));

    ui->imageFileNameLabel->setVisible(false);

    QSettings settings;

    bool advanced = settings.value("editLokos/showAdvancedOptions", false).toBool();

    if (advanced)
    {
        ui->groupBox->show();
        ui->advanced->setChecked(true);
    }
    else
    {
        ui->groupBox->hide();
        ui->advanced->setChecked(false);
    }

    enableAllControls(false);

    ui->name->setText("");
    ui->image->setText("");
    ui->imageFileNameLabel->setText("");

    m_modifiedLokos = false;

    m_uiFunctionButtons.append(ui->pushButtonF1);
    m_uiFunctionButtons.append(ui->pushButtonF2);
    m_uiFunctionButtons.append(ui->pushButtonF3);
    m_uiFunctionButtons.append(ui->pushButtonF4);
    m_uiFunctionButtons.append(ui->pushButtonF5);
    m_uiFunctionButtons.append(ui->pushButtonF6);
    m_uiFunctionButtons.append(ui->pushButtonF7);
    m_uiFunctionButtons.append(ui->pushButtonF8);
    m_uiFunctionButtons.append(ui->pushButtonF9);
    m_uiFunctionButtons.append(ui->pushButtonF10);
    m_uiFunctionButtons.append(ui->pushButtonF11);
    m_uiFunctionButtons.append(ui->pushButtonF12);
    m_uiFunctionButtons.append(ui->pushButtonF13);
    m_uiFunctionButtons.append(ui->pushButtonF14);
    m_uiFunctionButtons.append(ui->pushButtonF15);
    m_uiFunctionButtons.append(ui->pushButtonF16);

    m_uiExtraFunctionButtons.append(ui->pushButtonExtraF1);
    m_uiExtraFunctionButtons.append(ui->pushButtonExtraF2);
    m_uiExtraFunctionButtons.append(ui->pushButtonExtraF3);
    m_uiExtraFunctionButtons.append(ui->pushButtonExtraF4);
    m_uiExtraFunctionButtons.append(ui->pushButtonExtraF5);
    m_uiExtraFunctionButtons.append(ui->pushButtonExtraF6);
    m_uiExtraFunctionButtons.append(ui->pushButtonExtraF7);
    m_uiExtraFunctionButtons.append(ui->pushButtonExtraF8);

    m_pDocLokos = NULL;
}

// OK
void EditLokos::accept()
{
    if (m_modifiedLokos)
    {
        // ask if user wants to save modified settings?
        /*
        int r = QMessageBox::warning(this, tr("Tren Digital"),
        tr("Aquest diàleg es tancarà.\n"
           "Desitges guardar els canvis?"),
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No,
        QMessageBox::Cancel | QMessageBox::Escape);

        if (r == QMessageBox::Yes)
        {
            setLokoInfo();
        }
        else if (r == QMessageBox::Cancel)
        {
            return;
        }
        */
    }

    apply();

    saveSettings();

    saveChanges();

    clearLokos();

    QDialog::accept();
}

void EditLokos::clearLokos()
{
    // Destroy our working copy of all lokos
    foreach(Loko *pLoko, m_lokos)
    {
        delete pLoko;
    }

    m_lokos.clear();
}

void EditLokos::saveChanges()
{
    // First destroy all document lokos (the original ones)
    foreach(Loko *pLoko, *(m_pDocLokos))
    {
        delete pLoko;
    }

    (*m_pDocLokos).clear();

    // Now we make the copy
    foreach(Loko *pLoko, m_lokos)
    {
        Loko *pNewLoko = new Loko();
        pLoko->copy(pNewLoko);
        (*m_pDocLokos).insert(pLoko->m_id, pNewLoko);
    }
}

// HELP
void EditLokos::helpRequest()
{
    QMessageBox::warning(this, tr("Tren Digital"),
                               tr("Ajuda!\n"),
                               QMessageBox::Ok, QMessageBox::Ok);
}

// CANCEL
void EditLokos::reject()
{
    clearLokos();

    QDialog::reject();
}

// APPLY
void EditLokos::apply()
{
    setLokoInfo();

    refreshTreeWidget();

    // Fixme: La treewidget perd el focus i la loko seleccionada!
}

void EditLokos::refreshTreeWidget()
{
    ui->treeWidget->clear();

    fillTreeWidget();
}

void EditLokos::copyLokosFromDocumentLokos(QMap<int,Loko *> *pDocLokos)
{
    m_pDocLokos = pDocLokos;

    // we make a copy of original document lokos to work with

    foreach(Loko *pLoko, *m_pDocLokos)
    {
        Loko *pNewLoko = new Loko();
        pLoko->copy(pNewLoko);
        m_lokos.insert(pLoko->m_id, pNewLoko);
    }

    fillTreeWidget();
}

void EditLokos::fillTreeWidget()
{
    ASSERT(m_pDocLokos != NULL);

    QTreeWidgetItem *motorola = new QTreeWidgetItem(ui->treeWidget);
    motorola->setText(0, "motorola");

    QTreeWidgetItem *mfx = new QTreeWidgetItem(ui->treeWidget);
    mfx->setText(0, "mfx");

    QTreeWidgetItem *dcc = new QTreeWidgetItem(ui->treeWidget);
    dcc->setText(0, "dcc");

    foreach (Loko *pLoko, m_lokos)
    {
        if (pLoko != NULL)
        {
            QTreeWidgetItem *pLokoItem = NULL;

            if (pLoko->m_type.toUpper() == "MOTOROLA")
            {
                pLokoItem = new QTreeWidgetItem(motorola, QTreeWidgetItem::UserType);
            }
            else if (pLoko->m_type.toUpper() == "MFX")
            {
                pLokoItem = new QTreeWidgetItem(mfx, QTreeWidgetItem::UserType);
            }
            else if (pLoko->m_type.toUpper() == "DCC")
            {
                pLokoItem = new QTreeWidgetItem(dcc, QTreeWidgetItem::UserType);
            }

            if (pLokoItem != NULL)
            {
                pLokoItem->setText(0, pLoko->m_name);
                // pLokoItem->setData(0, Qt::ToolTipRole, pLoko->description);
                pLokoItem->setData(0, Qt::UserRole, pLoko->m_id);
            }
        }
    }

    ui->treeWidget->expandAll();
}

void EditLokos::saveSettings()
{
    QSettings settings;
    settings.setValue("editLokos/showAdvancedOptions", ui->advanced->isChecked());
}

EditLokos::~EditLokos()
{
    delete ui;
}

void EditLokos::onTreeWidgetItemClicked(QTreeWidgetItem* item, int column)
{
    if (item->type() == QTreeWidgetItem::UserType)
    {
        // if it's user type it means it's a loko
        // item->setText(0, QString("thisone!"));

        enableAllControls();

        int id = item->data(0, Qt::UserRole).toInt();

        getLokoInfo(id);
    }
    else
    {
        enableAllControls(false);

        ui->image->setIcon(QIcon());
        ui->imageFileNameLabel->setText(tr("<image not found>"));
    }
}

void EditLokos::enableAllControls(bool status)
{
    ui->name->setEnabled(status);
    ui->image->setEnabled(status);
    ui->imageFileNameLabel->setEnabled(status);
    ui->address->setEnabled(status);
    ui->extraAddress->setEnabled(status);
    ui->description->setEnabled(status);
    ui->type->setEnabled(status);
    ui->engine->setEnabled(status);
    ui->marca->setEnabled(status);

    ui->label->setEnabled(status);
    ui->label_2->setEnabled(status);
    ui->label_3->setEnabled(status);
    ui->label_4->setEnabled(status);
    ui->label_5->setEnabled(status);
    ui->label_6->setEnabled(status);
    ui->label_7->setEnabled(status);
    ui->label_8->setEnabled(status);
    ui->label_9->setEnabled(status);
    ui->label_10->setEnabled(status);
    ui->label_11->setEnabled(status);

    ui->extraAddress->setEnabled(status);

    ui->groupBox->setEnabled(status);

    for (int i=0; i<m_uiFunctionButtons.count(); i++)
    {
        m_uiFunctionButtons.at(i)->setEnabled(status);
    }

    for (int i=0; i<m_uiExtraFunctionButtons.count(); i++)
    {
        m_uiExtraFunctionButtons.at(i)->setEnabled(status);
    }

    ui->multitrackLoko->setEnabled(status);
    ui->toolButton_2->setEnabled(status);
}

Loko * EditLokos::getLoko(int id)
{
    return m_lokos.value(id, NULL);
}

Loko * EditLokos::getSelectedLoko()
{
    int id;

    if (ui->treeWidget->selectedItems().count() > 0)
    {
        QTreeWidgetItem *item = ui->treeWidget->selectedItems().first();
        if (item->type() == QTreeWidgetItem::UserType)
        {
            id = item->data(0, Qt::UserRole).toInt();
            return getLoko(id);
        }
    }

    return NULL;
}

void EditLokos::setSelectedLoko(int id)
{
    ui->treeWidget->clearSelection();

    QList<QTreeWidgetItem *> items = ui->treeWidget->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive);

    QTreeWidgetItem *item;

    foreach (item, items)
    {
        if (item->type() == QTreeWidgetItem::UserType && item->data(0, Qt::UserRole).toInt() == id)
        {
            ui->treeWidget->setItemSelected(item, true);
        }
    }
}

void EditLokos::getLokoInfo(int id)
{
    Loko *pLoko = getLoko(id);

    if (pLoko != NULL)
    {
        // image
        QFileInfo imageFileNameInfo(pLoko->m_imageFileName);

        if (imageFileNameInfo.exists())
        {
            ui->image->setIcon(QIcon(pLoko->m_imageFileName));
            ui->imageFileNameLabel->setText(pLoko->m_imageFileName);
        }
        else
        {
            ui->image->setIcon(QIcon());
            ui->imageFileNameLabel->setText(tr("<image not found>"));
        }

        // name
        ui->name->setText(pLoko->m_name);

        // address
        ui->address->setValue(pLoko->m_address);

        // type
        // ui->type->setCurrentText(pLoko->m_type.upper());
        int typeIndex = ui->type->findText(pLoko->m_type.toUpper());
        ui->type->setCurrentIndex(typeIndex);

        // brand
        ui->marca->setText(pLoko->m_brand);

        // motor
        int engineIndex = ui->engine->findText(pLoko->m_engine);
        ui->engine->setCurrentIndex(engineIndex);

        // description
        ui->description->setText(pLoko->m_description);

        // functions
        for (int i=0; i<NUM_FUNCTIONS; i++)
        {
            m_uiFunctionButtons.at(i)->setChecked(pLoko->m_functions[i]);
        }

        // extra functions

        for (int i=0; i<NUM_EXTRA_FUNCTIONS; i++)
        {
            m_uiExtraFunctionButtons.at(i)->setChecked(pLoko->m_extraFunctions[i]);
        }
    }
}

void EditLokos::setLokoInfo(int id)
{
    Loko *pLoko = NULL;

    if (id < 0)
    {
        pLoko = getSelectedLoko();
    }
    else
    {
        pLoko = getLoko(id);
    }

    if (pLoko != NULL)
    {
        // image
        pLoko->m_imageFileName = ui->imageFileNameLabel->text();

        // name
        pLoko->m_name = ui->name->text();

        // address
        pLoko->m_address = ui->address->text().toInt();

        // type
        pLoko->m_type = ui->type->currentText().toUpper();

        // brand
        pLoko->m_brand = ui->marca->text();

        // motor
        pLoko->m_engine = ui->engine->currentText();

        // description
        pLoko->m_description = ui->description->text();

        // functions
        for (int i=0; i<NUM_FUNCTIONS; i++)
        {
            pLoko->m_functions[i] = m_uiFunctionButtons.at(i)->isChecked();
        }

        // extra functions
        for (int i=0; i<NUM_EXTRA_FUNCTIONS; i++)
        {
            pLoko->m_extraFunctions[i] = m_uiExtraFunctionButtons.at(i)->isChecked();
        }
    }
}

void EditLokos::clearLokoInfo()
{
    ui->image->setIcon(QIcon());
    ui->imageFileNameLabel->setText(tr("<no s'ha trobat la imatge>"));

    // name
    ui->name->setText("");

    // address
    ui->address->setValue(0);

    // type
    // per defecte posem MOTOROLA, segurament serà el més usat
    // ui->type->setEditText("MOTOROLA");
    int typeIndex = ui->type->findText("MOTOROLA");
    ui->type->setCurrentIndex(typeIndex);

    // brand
    ui->marca->setText("");

    // motor
    ui->engine->setCurrentIndex(-1);

    // description
    ui->description->setText("");

    // functions
    for (int i=0; i<NUM_FUNCTIONS; i++)
    {
        m_uiFunctionButtons.at(i)->setChecked(false);
    }

    // extra functions
    for (int i=0; i<NUM_EXTRA_FUNCTIONS; i++)
    {
        m_uiExtraFunctionButtons.at(i)->setChecked(false);
    }
}

// Select image for selected loko
void EditLokos::onImageButton()
{
    Loko *pLoko = getSelectedLoko();

    if (pLoko != NULL)
    {
        IconView *iconView = new IconView(this);

        if (iconView->exec() == QDialog::Accepted && !iconView->selectedImage.isEmpty())
        {
            // pLoko->setImageFileName(iconView->selectedImage);
            ui->image->setIcon(QIcon(iconView->selectedImage));
            ui->imageFileNameLabel->setText(iconView->selectedImage);
        }
    }
    else
    {
        int ret = QMessageBox::warning(this, tr("Tren Digital"),
                                       tr("No hi ha cap locomotora seleccionada.\n"
                                          "Vols crear-ne una de nova?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            onNewLoko();
        }
    }
}

QString EditLokos::askLokoName(bool *ok)
{
    bool repeated;
    QString name;

    do
    {
        repeated = false;

        name = QInputDialog::getText(this, tr("Tren Digital - Nova locomotora"), tr("Entra el nom de la nova locomotora:"), QLineEdit::Normal, "", ok);

        if (getLokoByName(name) != NULL) // check if name is already used
        {
            // already used!
            QMessageBox::warning(this, tr("Tren Digital"), tr("Aquest nom ja està usat per una altra locomotora.\n"
                                 "Si us plau, escull un altre nom."), QMessageBox::Ok);
            repeated = true;
        }
    }
    while (*ok && repeated);

    return name;
}

int EditLokos::askLokoAddress(bool *ok)
{
    bool repeated;
    int address;

    do
    {
        repeated = false;

        address = QInputDialog::getInt(this, tr("Tren Digital - Nova locomotora"), tr("Adreça digital de la nova locomotora:"), 0, 0, LOKO_MAX_ADDRESS, 1, ok);

        if (getLokoByAddress(address) != NULL)
        {
            // already used!
            QMessageBox::warning(this, tr("Tren Digital"), tr("Aquesta adreça ja està usada per una altra locomotora.\n"
                                 "Si us plau, escull una altra adreça."), QMessageBox::Ok);
            repeated = true;
        }
    }
    while (*ok && repeated);

    return address;
}

// Adds new loko
void EditLokos::onNewLoko()
{
    bool ok;
    QString name, type;
    int address = 0;

    name.clear();
    type.clear();

    name = askLokoName(&ok);

    if (name.isEmpty())
    {
        ok = false;
    }

    if (ok)
    {
        QStringList items;
        items << tr("MOTOROLA") << tr("MFX") << tr("DCC");

        type = QInputDialog::getItem(this, tr("Tren Digital - Nova locomotora"), tr("Entra el tipus de la nova locomotora:"), items, 0, false, &ok);

        if (type.isEmpty())
        {
            ok = false;
        }

        if (ok)
        {
            address = askLokoAddress(&ok);

            if (ok)
            {
                clearLokoInfo();

                ui->name->setText(name);

                int typeIndex = ui->type->findText(type);
                ui->type->setCurrentIndex(typeIndex);

                ui->address->setValue(address);

                Loko *pNewLoko = new Loko();

                if (pNewLoko != NULL)
                {
                    pNewLoko->m_id = getFirstFreeId();
                    pNewLoko->m_name = name;
                    pNewLoko->m_type = type;
                    pNewLoko->m_address = address;

                    m_lokos.insert(pNewLoko->m_id, pNewLoko);

                    m_modifiedLokos = true;

                    enableAllControls(true);

                    refreshTreeWidget();

                    getLokoInfo(pNewLoko->m_id);

                    // mark new loko as selected
                    setSelectedLoko(pNewLoko->m_id);

                    QMessageBox::warning(this, tr("Tren Digital"),
                                               tr("La locomotora s'ha creat correctament.\n"
                                                  "Ara pots acabar de configurar-la afegint altres detalls.\n"
                                                  "Recorda prémer el botò d'aplicar per guardar-ne els canvis."),
                                               QMessageBox::Ok, QMessageBox::Ok);
                }
            }
        }
    }

    /*
    if (!ok)
    {
        QMessageBox::warning(this, tr("Tren Digital"),
                                   tr("Locomotora no creada."),
                                   QMessageBox::Ok, QMessageBox::Ok);
    }
    */
}

Loko * EditLokos::getLokoByName(QString name)
{
    // Find loko
    if (!name.isEmpty())
    {
        foreach (Loko *pLoko, m_lokos)
        {
            if (pLoko != NULL && pLoko->m_name == name)
            {
                return pLoko;
            }
        }
    }

    return NULL;
}

Loko * EditLokos::getLokoByAddress(int address)
{
    // Find loko

    foreach (Loko *pLoko, m_lokos)
    {
        if (pLoko != NULL && pLoko->m_address == address)
        {
            return pLoko;
        }
    }

    return NULL;
}

// fixme: QList to QMap
int EditLokos::getFirstFreeId()
{
    int id = 0;

    QList<int> positions = m_lokos.keys();

    foreach(int key, positions)
    {
        if (key == id+1)
        {
            id++;
        }
    }

    return id;
}

void EditLokos::onPrint()
{
    QMessageBox::warning(this, tr("Tren Digital - Imprimir locomotores"),
                               tr("Aquesta opció li permet imprimir una llista de les locomotores configurades.\n"
                                  "Comprovi que té la impressora degudament connectada."),
                               QMessageBox::Ok, QMessageBox::Ok);
    QString html;

    html = "<h1>Tren Digital</h1>\n<h2>" + tr("Llista de locomotores") + "</h2><br/>\n";

    QString address;

    // Imprimim la versió del document (m_pDocLokos).
    // Si no s'ha fet "apply" no es correspondrà amb la versió de la treeview
    if (m_pDocLokos != NULL)
    {
        foreach(Loko *pLoko, (*m_pDocLokos))
        {
            if (!pLoko->m_name.isEmpty())
            {
                address.setNum(pLoko->m_address);

                html += "<table width='100%' border=1 cellspacing=0>\n"
                        "<tr><td bgcolor='lightgray'><font size='+1'>"
                        "<b><i>" + pLoko->m_name + "</i></b></font></td></tr></table>\n"
                        "<table border=0>\n"
                        "<tr><td>" + tr("Nom:") + "</td>\n"
                        "<td>" + pLoko->m_name + "</td></tr>\n"
                        "<tr><td>" + tr("Adreça:") + "</td>\n"
                        "<td>" + address + "</td></tr>\n"
                        "<tr><td>" + tr("Marca:") + "</td>\n"
                        "<td>" + pLoko->m_brand + "</td></tr>\n"
                        "<tr><td>" + tr("Tipus:") + "</td>\n"
                        "<td>" + pLoko->m_type + "</td></tr>\n"
                        "<tr><td>" + tr("Motor:") + "</td>\n"
                        "<td>" + pLoko->m_engine + "</td></tr>\n"
                        "<tr><td>" + tr("Descripció:") + "</td>\n"
                        "<td>" + pLoko->m_description + "</td></tr>\n"
                        "\n</table>\n<br />\n";
            }
        }
    }

    QPrinter *printer = new QPrinter( QPrinter::HighResolution );

    QPrintDialog printDialog(printer, this);

    if (printDialog.exec())
    {
        // QPainter painter(printer);
        QTextDocument textDocument;
        textDocument.setHtml(html);
        textDocument.print(printer);
    }

    delete printer;
}
