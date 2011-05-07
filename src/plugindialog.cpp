/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include "traininterface.h"
 #include "plugindialog.h"

 #include <QPluginLoader>
 #include <QStringList>
 #include <QDir>

 #include <QLabel>
 #include <QGridLayout>
 #include <QPushButton>
 #include <QTreeWidget>
 #include <QTreeWidgetItem>
 #include <QHeaderView>

 PluginDialog::PluginDialog(const QString &path, const QStringList &fileNames,
                            QWidget *parent) :
     QDialog(parent),
     m_pLabel(new QLabel),
     m_pTreeWidget(new QTreeWidget),
     m_pOkButton(new QPushButton(tr("OK")))
 {
     m_pTreeWidget->setAlternatingRowColors(false);
     m_pTreeWidget->setSelectionMode(QAbstractItemView::NoSelection);
     m_pTreeWidget->setColumnCount(1);
     m_pTreeWidget->header()->hide();

     m_pOkButton->setDefault(true);

     connect(m_pOkButton, SIGNAL(clicked()), this, SLOT(close()));

     QGridLayout *pMainLayout = new QGridLayout;
     pMainLayout->setColumnStretch(0, 1);
     pMainLayout->setColumnStretch(2, 1);
     pMainLayout->addWidget(m_pLabel, 0, 0, 1, 3);
     pMainLayout->addWidget(m_pTreeWidget, 1, 0, 1, 3);
     pMainLayout->addWidget(m_pOkButton, 2, 1);
     setLayout(pMainLayout);

     m_interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
                             QIcon::Normal, QIcon::On);
     m_interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
                             QIcon::Normal, QIcon::Off);
     m_featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

     setWindowTitle(tr("Plugin Information"));

     findPlugins(path, fileNames);
 }

 void PluginDialog::findPlugins(const QString &path,
                                const QStringList &fileNames)
 {
     m_pLabel->setText(tr("Tren Digital found the following plugins\n"
                       "(looked in %1):")
                    .arg(QDir::toNativeSeparators(path)));

     const QDir dir(path);

     foreach (QObject *plugin, QPluginLoader::staticInstances())
     {
         populateTreeWidget(plugin, tr("%1 (Static Plugin)")
                                    .arg(plugin->metaObject()->className()));
     }

     foreach (QString fileName, fileNames)
     {
         QPluginLoader loader(dir.absoluteFilePath(fileName));
         QObject *plugin = loader.instance();
         if (plugin)
         {
             populateTreeWidget(plugin, fileName);
         }
     }
 }

 void PluginDialog::populateTreeWidget(QObject *plugin, const QString &text)
 {
     QTreeWidgetItem *pPluginItem = new QTreeWidgetItem(m_pTreeWidget);
     pPluginItem->setText(0, text);
     m_pTreeWidget->setItemExpanded(pPluginItem, true);

     QFont boldFont = pPluginItem->font(0);
     boldFont.setBold(true);
     pPluginItem->setFont(0, boldFont);

     if (plugin)
     {
         TrainInterface *pTrain = qobject_cast<TrainInterface *>(plugin);

         if (pTrain)
         {
             addItems(pPluginItem, "TrainInterface", pTrain->name(), pTrain->version());

         }
     }
 }

 void PluginDialog::addItems(QTreeWidgetItem *pluginItem,
                             const char *interfaceName,
                             const QString pluginName,
                             const QString pluginVersion)
 {
     QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(pluginItem);
     interfaceItem->setText(0, interfaceName);
     interfaceItem->setIcon(0, m_interfaceIcon);

     QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
     featureItem->setText(0, pluginName);
     featureItem->setIcon(0, m_featureIcon);

     featureItem->setText(1, pluginVersion);
 }

