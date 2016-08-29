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

class EditorWindow : public QMainWindow, private Ui::EditorWindow {
Q_OBJECT
private:
    QString anims_root;
    QGraphicsScene scene;
    QGraphicsItem* item;
    AnimInfo animInfo;
    QTimer animTimer;
    bool animRuning;

public:
    EditorWindow() : anims_root(QCoreApplication::applicationDirPath() + "/../data/anims/"), item(NULL), animRuning(false) {
        setupUi(this);

        animTimer.setInterval(200);

        load_anims();

//        item = scene.addText("Just do it!");

        graphicsView->setScene(&scene);
//        graphicsView->scale(14, 14);

        connect(&animTimer, SIGNAL(timeout()), this, SLOT(anim_step()));

        show();
    }

    void refresh_anim() {
        frameNumber->setMaximum(animInfo._frames.size() - 1);
        if(item){
            scene.removeItem(item);
            delete item;
        }
        Frame& frame = animInfo._frames[frameNumber->value()];
        int w = frame._w;
        int h = frame._h;
        QImage qimage(w, h, QImage::Format_ARGB32);

        qimage.fill(Qt::cyan);

        TextureManager& manager = TextureManager::inst();
        Image& image = manager.get_image(animInfo._tex_name.c_str());

        for (unsigned int x = 0; x < w; x++) {
            for (unsigned int y = 0; y < h; y++) {
                const Color& c = image.getPixel(frame._x + x, frame._y + y);
                qimage.setPixelColor(x, y, QColor(c.r, c.g, c.b, 255));
            }
        }

        item = scene.addPixmap(QPixmap::fromImage(qimage));
        item->moveBy(-frame._dx,-frame._dy);
        item->setScale(14);
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
        if(checked){
            startAnimation->setText("Stop");
            animTimer.start();
            log("Check\n");
        } else{
            startAnimation->setText("Start");
            animTimer.stop();
            log("&Mate\n");
        }
    }

    void anim_step() {
        frameNumber->setValue((frameNumber->value()+1)%animInfo._frames.size());
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

    void on_speed_valueChanged(double value){
        animTimer.setInterval(int(200/value));
    }

    void on_frameNumber_valueChanged(int){
        refresh_anim();
    }
};
