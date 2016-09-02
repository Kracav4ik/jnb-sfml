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

class UndoRedo {
private:
    std::vector<IntPoint2D> diff;
    int currentDiff;
    int maxCurrentDiff;

    void get_history_plus_n(int count) {
        for (int _ = 0; _ < count; _++) {
            diff.push_back(IntPoint2D());
        }
    }

public:
    void initial_set() {
        diff.clear();
        get_history_plus_n(20);
        currentDiff = 0;
        maxCurrentDiff = 0;
        diff[0] = IntPoint2D(2, -4);
    }

    bool can_undo() const { return currentDiff > 0; }
    bool can_redo() const { return currentDiff < maxCurrentDiff; }

    void push_offset(const IntPoint2D& offset) {
        currentDiff++;

        if (currentDiff >= diff.size()) {
            get_history_plus_n(2 * diff.size() + 10);
        }

        diff[currentDiff] = offset;
        maxCurrentDiff = currentDiff;
    }

    const IntPoint2D& do_undo() {
        log("undo\n");
        if (!can_undo()) {
            log("** error at undo\n");
        }
        currentDiff--;

        return peek_offset();
    }

    const IntPoint2D& do_redo() {
        log("redo\n");
        if (!can_redo()) {
            log("** error at redo\n");
        }
        currentDiff++;

        return peek_offset();
    }

    const IntPoint2D& peek_offset() {
        return diff[currentDiff];
    }
};

class EditorWindow : public QMainWindow, private Ui::EditorWindow {
Q_OBJECT
private:
    QString anims_root;
    QGraphicsScene scene;
    AnimInfo animInfo;
    QTimer animTimer;
    bool animRuning;
    BackgroundGrid grid;

    UndoRedo undoRedo;

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

        undoRedo.initial_set();
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

        unDo->setEnabled(undoRedo.can_undo());
        reDo->setEnabled(undoRedo.can_redo());
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

        undoRedo.initial_set();

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
        grid._frame->dx() = i;
        undoRedo.push_offset(grid._frame->offset);
        refresh_anim();
    }

    void on_frameDy_valueChanged(int i) {
        if ( i == grid._frame->dy()){
            return;
        }
        grid._frame->dy() = i;
        undoRedo.push_offset(grid._frame->offset);
        refresh_anim();
    }

    void on_unDo_clicked() {
        grid._frame->offset = undoRedo.do_undo();

        refresh_anim();
    }

    void on_reDo_clicked() {
        grid._frame->offset = undoRedo.do_redo();

        refresh_anim();
    }
};
