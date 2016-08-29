#pragma once

#include <stdio.h>
#include <QMainWindow>
#include <QDir>
#include <QGraphicsTextItem>
#include <QTimer>

#include "ui_editor.h"
#include "debug.h"
#include "frame_animation.h"
#include "texture_manager.h"
#include "background_grid.h"

const int DEFAULT_ANIM_INTERVAL = 200;

class EditorWindow : public QMainWindow, private Ui::EditorWindow {
Q_OBJECT
private:
    QString anims_root;
    QGraphicsScene scene;
    AnimInfo animInfo;
    QTimer animTimer;
    bool animRuning;
    BackgroundGrid grid;

public:
    EditorWindow() :
            anims_root(QCoreApplication::applicationDirPath() + "/../data/anims/"),
            animRuning(false),
            grid(14, 30, 30, 5, 5)
    {
        setupUi(this);

        animTimer.setInterval(DEFAULT_ANIM_INTERVAL);

        load_anims();

        scene.addItem(&grid);
        graphicsView->setScene(&scene);

        connect(&animTimer, SIGNAL(timeout()), this, SLOT(anim_step()));

        show();
    }

    void refresh_anim() {
        frameNumber->setMaximum(animInfo._frames.size() - 1);
        Frame& frame = animInfo._frames[frameNumber->value()];

        TextureManager& manager = TextureManager::inst();
        Image& image = manager.get_image(animInfo._tex_name.c_str());
        grid.setImageFrame(&image, &frame);

        frameDx->setValue(frame._dx);
        frameDy->setValue(frame._dy);
    }

    void load_anims() {
        QStringList list = QDir(anims_root).entryList();

        for (int i = 2; i < list.size(); i++) {
            QString str = list[i];
            animationList->addItem(str.left(str.size() - 4));
        }
    }

public slots:

    void on_startAnimation_clicked(bool checked) {
        animRuning = checked;
    }

    void on_startAnimation_toggled(bool checked) {
        if (checked) {
            startAnimation->setText("Stop");
            animTimer.start();
            log("Check\n");
        } else {
            startAnimation->setText("Start");
            animTimer.stop();
            log("&Mate\n");
        }
    }

    void anim_step() {
        frameNumber->setValue((frameNumber->value() + 1) % animInfo._frames.size());
        log("Work\n");
    }

    void on_animationList_currentTextChanged(const QString& string) {
        log("%s\n", string.toUtf8().constData());
        QString path = anims_root + string + ".txt";
        animInfo.load(FilePath(path.toUtf8().constData()));

        bool has_animation = animInfo._frames.size() != 1;
        startAnimation->setChecked(has_animation && animRuning);
        startAnimation->setEnabled(has_animation);

        refresh_anim();
    }

    void on_speed_valueChanged(double value) {
        animTimer.setInterval(int(DEFAULT_ANIM_INTERVAL / value));
    }

    void on_frameNumber_valueChanged(int) {
        refresh_anim();
    }
};
