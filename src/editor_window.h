#pragma once

#include <stdio.h>
#include <QMainWindow>
#include <QtCore/QDir>

#include "ui_editor.h"
#include "frame_animation.h"

class EditorWindow : public QMainWindow, private Ui::EditorWindow {
    Q_OBJECT
private:
    QString anims_root;
public:
    EditorWindow() :anims_root(QCoreApplication::applicationDirPath() + "/../data/anims/") {
        setupUi(this);

        load_anims();

        show(); 
    }

    void load_anims() {
        QStringList list = QDir(anims_root).entryList();

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
        QString path = anims_root + string + ".txt";
        AnimInfo animInfo(FilePath(path.toUtf8().constData()));
        printf("\"%s\"\n",animInfo._tex_name.c_str());
    }
};
