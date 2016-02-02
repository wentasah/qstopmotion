/***************************************************************************
 *   Copyright (C) 2010-2015 by Ralf Lange                                 *
 *   ralf.lange@longsoft.de                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef VIEWTAB_H
#define VIEWTAB_H

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

#include "frontends/frontend.h"


/**
 * The view tab on the right hand side of the preview window
 * @author Ralf Lange
 */
class ViewTab : public QWidget
{
    Q_OBJECT

public:
    /**
     * Sets up the tab.
     * @param parent the parent of the this widget
     */
    ViewTab(Frontend *f,
            QWidget *parent = 0);

    /**
     * Retranslate all strings of the tab after changing the language setting
     */
    void retranslateStrings();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

    /**
     * Applies the settings in the import tab.
     */
    void apply();

protected:
    // void resizeEvent(QResizeEvent *event);

private slots:
    void firstSlot();
    void secondSlot();
    void thirdSlot();

private:
    Frontend    *frontend;
    QGroupBox   *firstGroupBox;
    QLabel      *firstIcon;
    QSlider     *firstSlider;
    QPushButton *firstButton;
    QGroupBox   *secondGroupBox;
    QPushButton *secondButton;
    QGroupBox   *thirdGroupBox;
    QPushButton *thirdButton;

    void makeGUI();
};

#endif
