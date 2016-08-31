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
    std::vector<std::vector<int> > diff;
    int currentDiff;
    int maxCurrentDiff;
    QString path;

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

        get_history_plus_n(20);
        currentDiff = 1;
        maxCurrentDiff = 1;
        diff[0][0] = 2;
        diff[0][1] = -4;

        show();
    }

    void refresh_anim() {
        frameNumber->setMaximum(animInfo._frames.size() - 1);
        Frame& frame = animInfo._frames[frameNumber->value()];

        TextureManager& manager = TextureManager::inst();
        Image& image = manager.get_image(animInfo._tex_name.c_str());
        grid.setImageFrame(&image, &frame);

        frameDx->setValue(frame.dx());
        frameDy->setValue(frame.dy());

        unDo->setDisabled(currentDiff == 0);
        if (maxCurrentDiff < currentDiff) {
            maxCurrentDiff = currentDiff;
        }
        if (currentDiff > diff.size() * 0.75) {
            get_history_plus_n(2 * diff.size() + 10);
        }
        if (maxCurrentDiff == currentDiff) {
            reDo->setEnabled(false);
        }
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

    void on_saveFrame_clicked() {
        log("Save\n");
        animInfo.save(FilePath(path.toUtf8().constData()));
    }

    void on_animationList_currentTextChanged(const QString& string) {
        log("%s\n", string.toUtf8().constData());
        path = anims_root + string + ".txt";
        animInfo.load(FilePath(path.toUtf8().constData()));

        bool has_animation = animInfo._frames.size() != 1;
        startAnimation->setChecked(has_animation && animRuning);
        startAnimation->setEnabled(has_animation);

        currentDiff = 0;
        reDo->setEnabled(false);

        refresh_anim();
    }

    void on_speed_valueChanged(double value) {
        animTimer.setInterval(int(DEFAULT_ANIM_INTERVAL / value));
    }

    void on_frameNumber_valueChanged(int) {
        refresh_anim();
    }

    void on_frameDx_valueChanged(int i) {
        if ( i == grid._frame->dx()){
            return;
        }
        currentDiff++;
        grid._frame->dx() = i;
        reDo->setEnabled(false);
        diff[currentDiff][0] = i;
        diff[currentDiff][1] = grid._frame->dy();
        maxCurrentDiff = currentDiff;
        refresh_anim();
    }

    void on_frameDy_valueChanged(int i) {
        if ( i == grid._frame->dy()){
            return;
        }
        currentDiff++;
        grid._frame->dy() = i;
        reDo->setEnabled(false);
        diff[currentDiff][0] = grid._frame->dx();
        diff[currentDiff][1] = i;
        maxCurrentDiff = currentDiff;
        refresh_anim();
    }

    void on_unDo_clicked() {
        log("undo\n");
        currentDiff--;

        grid._frame->dy() = diff[currentDiff][1];
        grid._frame->dx() = diff[currentDiff][0];

        reDo->setEnabled(true);
        refresh_anim();
    }

    void on_reDo_clicked() {
        log("redo\n");
        currentDiff++;

        grid._frame->dy() = diff[currentDiff][1];
        grid._frame->dx() = diff[currentDiff][0];

        refresh_anim();
    }

    void get_history_plus_n(int count) {
        for (int _ = 0; _ < count; _++) {
            std::vector<int> vector1 = std::vector<int>();
            for (int __ = 0; __ < 2; __++) {
                vector1.push_back(0);
            }
            diff.push_back(vector1);
        }
    }
};
