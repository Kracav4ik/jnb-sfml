#pragma once

#include <stdio.h>
#include <QMainWindow>
#include <QtCore/QDir>
#include <QGraphicsTextItem>

#include "ui_editor.h"
#include "frame_animation.h"
#include "texture_manager.h"

class EditorWindow : public QMainWindow, private Ui::EditorWindow {
Q_OBJECT
private:
    QString anims_root;
    QGraphicsScene scene;
    QGraphicsItem* item;
    AnimInfo animInfo;
public:
    EditorWindow() : anims_root(QCoreApplication::applicationDirPath() + "/../data/anims/") {
        setupUi(this);

        load_anims();

        item = scene.addText("Just do it!");

        graphicsView->setScene(&scene);
        graphicsView->scale(4, 4);

        show();
    }

    void refresh_anim() {
        frameNumber->setMaximum(animInfo._frames.size() - 1);
        scene.removeItem(item);
        delete item;
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
    }

    void load_anims() {
        QStringList list = QDir(anims_root).entryList();

        for (int i = 2; i < list.size(); i++) {
            QString str = list[i];
            animationList->addItem(str.left(str.size() - 4));
        }
    }

public slots:
    void on_startAnimation_clicked() {
        printf("Start\n");
    }

    void on_animationList_currentTextChanged(const QString& string) {
        printf("%s\n", string.toUtf8().constData());
        QString path = anims_root + string + ".txt";
        animInfo.load(FilePath(path.toUtf8().constData()));

        refresh_anim();
    }

    void on_frameNumber_valueChanged(int){
        refresh_anim();
    }
};
