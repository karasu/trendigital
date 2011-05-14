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
#ifndef ICONVIEW_H
#define ICONVIEW_H

#include <QDialog>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include <QRegExp>

#include <QMessageBox>

#define MEDIABROWSER_LIST_ICON_SIZE 98,98

namespace Ui {
    class IconView;
    class FileIconProvider;
}

class FileIconProvider : public QFileIconProvider
{
public:
    FileIconProvider() : QFileIconProvider()  {}

    bool isImage(const QString & extension) const
    {
        return extension.indexOf(QRegExp("(png|jpg|bmp|svg|xpm)",Qt::CaseInsensitive)) == 0;
    }

    QIcon icon(const QFileInfo &info) const
    {
        // QApplication::processEvents();
        if (isImage(info.suffix()))
        {
            QPixmap pix(info.absoluteFilePath());

            return pix.scaled(QSize(MEDIABROWSER_LIST_ICON_SIZE),
                              Qt::KeepAspectRatio,
                              //Qt::IgnoreAspectRatio,
                              Qt::SmoothTransformation);
        }
        else
        {
            return QFileIconProvider::icon(info);
        }
    }
};

class IconView : public QDialog
{
    Q_OBJECT

public:
    explicit IconView(QWidget *parent = 0);
    ~IconView();

    void accept(void);

    QString selectedImage;

private:
    Ui::IconView *ui;

private slots:
    void onTreeViewCurrentChanged(QModelIndex index);
    void onListViewClicked(QModelIndex index);
    void onListViewDoubleClicked(QModelIndex index);
};

#endif // ICONVIEW_H
