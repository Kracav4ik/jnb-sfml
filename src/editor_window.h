#pragma once

#include <stdio.h>
#include <QMainWindow>
#include <QtCore/QDir>

#include "ui_editor.h"

class EditorWindow : public QMainWindow, private Ui::EditorWindow {
    Q_OBJECT
public:
    EditorWindow() {
        setupUi(this);

        load_anims();

        show(); 
    }

    void load_anims() {
        QString path = QCoreApplication::applicationDirPath();
        QDir dir(path + "/../data/anims");
        QStringList list = dir.entryList();

        for (int i = 2; i < list.size();i++) {
            QString str = list[i];
            animationList->addItem(str.left(str.size() - 4));
        }
    }

public slots:
    void on_startAnimation_clicked(){
        printf("Start\n");
    }

    void on_animationList_currentTextChanged(const QString& string){
        printf("%s\n",string.toUtf8().constData());
    }
};
