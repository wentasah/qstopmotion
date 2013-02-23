/***************************************************************************
 *   Copyright (C) 2010-2013 by Ralf Lange                                 *
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

#ifndef COMPOSITINGTAB_H
#define COMPOSITINGTAB_H

#include "frontends/frontend.h"

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>


/**
 * The view tab on the right hand side of the preview window
 * @author Ralf Lange
 */
class CompositingTab : public QWidget
{
    Q_OBJECT
public:
    /**
     * Sets up the tab.
     * @param parent the parent of the this widget
     */
    CompositingTab(Frontend *f,
                   QWidget *parent = 0);

    /**
     * Applies the settings in the import tab.
     */
    void apply();

    /**
     * Initializes the table and fills it with starting values.
     */
    void initialize();

    /**
     * Retranslate all strings of the tab after changing the language setting
     */
    void retranslateStrings();

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
