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

template<typename T>
class UndoRedo {
private:
    std::vector<T> diff;
    int currentDiff;
    int maxCurrentDiff;
    int savedAtDiff;
    static const int OUTSIDE = -1;

public:
    UndoRedo() {
        initial_set();
    }

    void initial_set() {
        diff.clear();
        currentDiff = OUTSIDE;
        maxCurrentDiff = OUTSIDE;
        savedAtDiff = OUTSIDE;
    }

    bool can_undo() const { return !diff.empty() && currentDiff > 0; }

    bool can_redo() const { return !diff.empty() && currentDiff < maxCurrentDiff; }

    bool has_changes() const {
        if (currentDiff == OUTSIDE || savedAtDiff == OUTSIDE) {
            return currentDiff != savedAtDiff;
        }
        return diff[currentDiff] != diff[savedAtDiff];
    }

    void push_value(const T& value) {
        currentDiff++;

        if (currentDiff >= diff.size()) {
            diff.resize(2 * diff.size() + 10);
        }

        diff[currentDiff] = value;
        if (savedAtDiff >= currentDiff) {
            savedAtDiff = OUTSIDE;
        }
        maxCurrentDiff = currentDiff;
    }

    const T& do_undo() {
        if (!can_undo()) {
            log("** error at undo\n");
        }
        currentDiff--;

        return peek_value();
    }

    const T& do_redo() {
        if (!can_redo()) {
            log("** error at redo\n");
        }
        currentDiff++;

        return peek_value();
    }

    const T& peek_value() {
        return diff[currentDiff];
    }

    void notify_saved() {
        savedAtDiff = currentDiff;
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
    QStringList animNames;

    UndoRedo<AnimInfo::Frames> undoRedo;

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

        grid._showBg = showBg->isChecked();

        scene.addItem(&grid);
        graphicsView->setScene(&scene);

        connect(&animTimer, SIGNAL(timeout()), this, SLOT(anim_step()));

        show();

        on_filterAnim_textChanged("");
    }

    void refresh_anim() {
        if (animInfo.empty()) {
            grid.setImageFrame(NULL, NULL);
            return;
        }
        frameNumber->setMaximum(animInfo._frames.size() - 1);
        Frame& frame = animInfo._frames[frameNumber->value()];

        TextureManager& manager = TextureManager::inst();
        Image& image = manager.get_image(animInfo._tex_name.c_str());
        grid.setImageFrame(&image, &frame);

        frameDx->setValue(frame.dx());
        frameDy->setValue(frame.dy());

        unDo->setEnabled(undoRedo.can_undo());
        reDo->setEnabled(undoRedo.can_redo());
        saveFrame->setEnabled(undoRedo.has_changes());
    }

    void load_anims() {
        animNames.clear();
        QStringList fileNames = QDir(anims_root).entryList();

        for (QString file : fileNames) {
            if (file.endsWith(".txt")) {
                animNames.push_back(file.left(file.size() - 4));
            }
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
        } else {
            startAnimation->setText("Start");
            animTimer.stop();
        }
    }

    void anim_step() {
        frameNumber->setValue((frameNumber->value() + 1) % animInfo._frames.size());
    }

    void on_saveFrame_clicked() {
        animInfo.save(FilePath(path.toUtf8().constData()));
        undoRedo.notify_saved();
        refresh_anim();
    }

    void on_filterAnim_textChanged(QString filter) {
        filter = filter.trimmed();

        QStringList filterWords = filter.split(" ", QString::SkipEmptyParts);
        QStringList filteredNames;

        for (QString name : animNames) {
            bool addName = true;
            for (QString word : filterWords) {
                if (!name.contains(word)) {
                    addName = false;
                    break;
                }
            }
            if (addName) {
                filteredNames.append(name);
            }
        }

        animationList->clear();
        animationList->addItems(filteredNames);

        refresh_anim();
    }

    void on_clearFilter_clicked() {
        filterAnim->clear();
    }

    void on_animationList_currentTextChanged(const QString& string) {
        bool haveFile = !string.isEmpty();
        frameEditors->setEnabled(haveFile);
        
        undoRedo.initial_set();
        if (haveFile) {
            path = anims_root + string + ".txt";
            animInfo.load(FilePath(path.toUtf8().constData()));

            bool has_animation = animInfo._frames.size() != 1;
            startAnimation->setChecked(has_animation && animRuning);
            startAnimation->setEnabled(has_animation);

            undoRedo.push_value(animInfo._frames);
            undoRedo.notify_saved();
        } else {
            path = "";
            animInfo.clear();
        }

        refresh_anim();
    }

    void on_speed_valueChanged(double value) {
        animTimer.setInterval(int(DEFAULT_ANIM_INTERVAL / value));
    }

    void on_frameNumber_valueChanged(int) {
        refresh_anim();
    }

    void on_frameDx_valueChanged(int i) {
        if (i == grid._frame->dx()) {
            return;
        }
        grid._frame->dx() = i;
        undoRedo.push_value(animInfo._frames);
        refresh_anim();
    }

    void on_frameDy_valueChanged(int i) {
        if (i == grid._frame->dy()) {
            return;
        }
        grid._frame->dy() = i;
        undoRedo.push_value(animInfo._frames);
        refresh_anim();
    }

    void on_unDo_clicked() {
        animInfo._frames = undoRedo.do_undo();
        grid.setFrame(&animInfo._frames[frameNumber->value()]);

        refresh_anim();
    }

    void on_reDo_clicked() {
        animInfo._frames = undoRedo.do_redo();
        grid.setFrame(&animInfo._frames[frameNumber->value()]);

        refresh_anim();
    }

    void on_showBg_stateChanged(int) {
        grid._showBg = showBg->isChecked();
        grid.update();
    }
};
