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
#include "iconview.h"
#include "ui_iconview.h"

IconView::IconView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IconView)
{
    selectedImage.clear();

    ui->setupUi(this);

    // QString path("/home/karasu/TrenDigital/images/lokos/Fleischmann/electric");
    QString path(QDir::currentPath());

    FileIconProvider *fileIconProvider = new FileIconProvider;

    QStringList myFilter;
    myFilter << "*.png";
    myFilter << "*.jpg";
    myFilter << "*.bmp";
    myFilter << "*.svg";
    myFilter << "*.xpm";

    QFileSystemModel *model = new QFileSystemModel;
    model->setReadOnly(true); // just in case
    model->setNameFilterDisables(false);
    model->setNameFilters(myFilter);
    model->setRootPath(path);
    model->setIconProvider(fileIconProvider);

    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(path));

    ui->treeView->setHeaderHidden(true);
    ui->treeView->header()->hideSection(1);
    ui->treeView->header()->hideSection(2);
    ui->treeView->header()->hideSection(3);

    ui->listView->setModel(model);
    ui->listView->setRootIndex(model->index(path));

    ui->listView->setViewMode(QListView::IconMode);
    ui->listView->setFlow(QListView::LeftToRight);
    // ui->listView->setWrapping(false);
    ui->listView->setIconSize(QSize(MEDIABROWSER_LIST_ICON_SIZE));
    ui->listView->setResizeMode(QListView::Adjust);
    ui->listView->setUniformItemSizes(true);

    // fer que el treeview no pugui crèixer i aprofitar tot l'espai per la listview
    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);

    // connectar el click de la treeview per actualitzar la listview
    // i també connectar el doble click de la listview per actualitzar la treeview
    QItemSelectionModel *selectionModel = ui->treeView->selectionModel();
    connect(selectionModel, SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(onTreeViewCurrentChanged(QModelIndex)));
    connect(ui->listView,SIGNAL(clicked(QModelIndex)), this, SLOT(onListViewClicked(QModelIndex)));
    connect(ui->listView,SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onListViewDoubleClicked(QModelIndex)));
}

IconView::~IconView()
{
    delete ui;
}

void IconView::onTreeViewCurrentChanged(QModelIndex index)
{
    ui->listView->setRootIndex(index);

    ui->listView->selectionModel()->clearSelection();

    QFileSystemModel *model = (QFileSystemModel *)ui->treeView->model();

    if (!model->isDir(index))
    {
        selectedImage = model->filePath(index);
    }
    else
    {
        selectedImage.clear();
    }
}

void IconView::onListViewDoubleClicked(QModelIndex index)
{
    QFileSystemModel *model = (QFileSystemModel *)ui->listView->model();

    if (!model->isDir(index))
    {
        selectedImage = model->filePath(index);

        // QMessageBox::warning(this, tr("Tren Digital"), selectedImage);
        QDialog::accept();
    }
    else
    {
        // Hem clicat sobre un directori
        ui->treeView->setCurrentIndex(index);
        ui->listView->setRootIndex(index);

        selectedImage.clear();
    }
}

void IconView::onListViewClicked(QModelIndex index)
{
    QFileSystemModel *model = (QFileSystemModel *)ui->listView->model();

    if (!model->isDir(index))
    {
        selectedImage = model->filePath(index);
    }
    else
    {
        selectedImage.clear();
    }
}

void IconView::accept()
{
    // selectedImage = ((QFileSystemModel *)ui->listView->model())->filePath(ui->listView->currentIndex());

    // QMessageBox::warning(this, tr("Tren Digital"), selectedImage);
    QDialog::accept();
}

