/*
 *  Copyright (C) 2020 KeePassXC Team <team@keepassxc.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEEPASSXC_REPORTSWIDGETHIBP_H
#define KEEPASSXC_REPORTSWIDGETHIBP_H

#include "config-keepassx.h"
#include "gui/entry/EntryModel.h"

#include <QMap>
#include <QPair>
#include <QPointer>
#include <QWidget>

#ifdef WITH_XC_NETWORKING
#include "core/HibpDownloader.h"
#endif

class Database;
class Entry;
class Group;
class QStandardItemModel;

namespace Ui
{
    class ReportsWidgetHibp;
}

class ReportsWidgetHibp : public QWidget
{
    Q_OBJECT
public:
    explicit ReportsWidgetHibp(QWidget* parent = nullptr);
    ~ReportsWidgetHibp();

    void loadSettings(QSharedPointer<Database> db);
    void saveSettings();
    void refreshAfterEdit();

signals:
    void entryActivated(Entry*);

public slots:
    void emitEntryActivated(const QModelIndex&);
    void addHibpResult(const QString&, int);
    void fetchFailed(const QString& error);
    void makeHibpTable();
    void customMenuRequested(QPoint);
    void editFromContextmenu();
    void toggleKnownBad(bool);

private:
    void startValidation();
    static QString countToText(int count);

    QScopedPointer<Ui::ReportsWidgetHibp> m_ui;
    QScopedPointer<QStandardItemModel> m_referencesModel;
    QSharedPointer<Database> m_db;

    QMap<QString, int> m_pwndPasswords; // Passwords we found to have been pwned (value is pwn count)
    QString m_error; // Error message if download failed, else empty
    QList<const Entry*> m_rowToEntry; // List index is table row
    QPointer<const Entry> m_editedEntry; // The entry we're currently editing
    QString m_editedPassword; // The old password of the entry we're editing
    bool m_editedKnownBad; // The old "known bad" flag of the entry we're editing
    Entry* m_contextmenuEntry = nullptr; // The entry that was right-clicked

#ifdef WITH_XC_NETWORKING
    HibpDownloader m_downloader; // This performs the actual HIBP online query
#endif
};

#endif // KEEPASSXC_REPORTSWIDGETHIBP_H
