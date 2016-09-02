#pragma once

#include <QGraphicsItem>
#include <QPainter>

#include "texture_manager.h"
#include "frame_animation.h"

class BackgroundGrid : public QGraphicsItem {
private:
    int _cellSize;
    int _cellCountHor;
    int _cellCountVert;
    int _globalOffsetX;
    int _globalOffsetY;
public:
    bool _showBg;
    Image* _image;
    Frame* _frame;

    explicit BackgroundGrid(int cellSize, int cellCountGor, int cellCountVert, int globalOffsetX, int globalOffsetY) :
            _cellSize(cellSize),
            _cellCountHor(cellCountGor),
            _cellCountVert(cellCountVert),
            _globalOffsetX(globalOffsetX),
            _globalOffsetY(globalOffsetY),
            _image(NULL),
            _frame(NULL),
            _showBg(true)
    {}

    QRectF boundingRect() const override {
        return QRectF(0, 0, _cellSize * _cellCountHor + 1, _cellSize * _cellCountVert + 1);
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(Qt::darkCyan, Qt::Dense5Pattern));
        painter->drawRect(boundingRect());

        if(_image && _frame){
            for (unsigned int x = 0; x < _frame->w(); x++) {
                for (unsigned int y = 0; y < _frame->h(); y++) {
                    int frameX = x + _globalOffsetY - _frame->dx();
                    int frameY = y + _globalOffsetY - _frame->dy();
                    if (frameX < 0 || frameY < 0 || frameX >= _cellCountHor || frameY >= _cellCountVert) {
                        continue;
                    }
                    const Color& c = _image->getPixel(_frame->x() + x, _frame->y() + y);
                    Uint8 alpha = c.a;
                    if(_showBg){
                        alpha = 255;
                    }
                    painter->setBrush(QColor(c.r, c.g, c.b, alpha));
                    painter->drawRect(_cellSize * frameX, _cellSize * frameY, _cellSize, _cellSize);
                }
            }

        }

        painter->setPen(Qt::black);
        for (int i = 0; i <= _cellCountVert; i++) {
            int offset_y = _cellSize * i;
            painter->drawLine(0, offset_y, _cellSize * _cellCountHor, offset_y);
        }
        for (int i = 0; i <= _cellCountHor; i++) {
            int offset_x = _cellSize * i;
            painter->drawLine(offset_x, 0, offset_x, _cellSize * _cellCountVert);
        }
    }

    void setFrame(Frame* frame) {
        _frame = frame;
        update();
    }

    void setImageFrame(Image* image, Frame* frame) {
        _image = image;
        setFrame(frame);
    }
};
