#pragma once

#include <QGraphicsItem>
#include <QPainter>

#include "texture_manager.h"
#include "frame_animation.h"
#include "rabbit.h"

class BackgroundGrid : public QGraphicsItem {
private:
    int _cellSize;
    int _cellCountHor;
    int _cellCountVert;
    int _globalOffsetX;
    int _globalOffsetY;
    const Image* _image;
    const AnimInfo::Frames* _frames;
    int _frameIdx;
public:
    bool _showBg;
    bool _showTex;
    bool _showBaseline;
    bool _showHitbox;

    explicit BackgroundGrid(int cellSize, int cellCountGor, int cellCountVert, int globalOffsetX, int globalOffsetY) :
            _cellSize(cellSize),
            _cellCountHor(cellCountGor),
            _cellCountVert(cellCountVert),
            _globalOffsetX(globalOffsetX),
            _globalOffsetY(globalOffsetY),
            _image(NULL),
            _frames(NULL),
            _frameIdx(0),
            _showBg(false),
            _showTex(false),
            _showBaseline(false),
            _showHitbox(false)
    {}

    QRectF boundingRect() const override {
        return QRectF(0, 0, _cellSize * _cellCountHor + 1, _cellSize * _cellCountVert + 1);
    }

    void _paintImagePixel(QPainter* painter, unsigned int frameX, unsigned int frameY, unsigned int pixX, unsigned int pixY) const {
        if (pixX < 0 || pixY < 0 || pixX >= _image->getSize().x || pixY >= _image->getSize().y){
            return;
        }
        const Color& c = _image->getPixel(pixX, pixY);
        Uint8 alpha = c.a;
        if(_showBg){
            alpha = 255;
        }
        painter->setBrush(QColor(c.r, c.g, c.b, alpha));
        painter->drawRect(_cellSize * frameX, _cellSize * frameY, _cellSize, _cellSize);
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(Qt::darkCyan, Qt::Dense5Pattern));
        painter->drawRect(boundingRect());

        if(_image && _frames){
            const Frame& _frame = (*_frames)[_frameIdx];
            if (_showTex) {
                for (unsigned int frameX = 0; frameX < _cellCountVert; frameX++) {
                    for (unsigned int frameY = 0; frameY < _cellCountHor; frameY++) {
                        unsigned int pixX = _frame.x() - _globalOffsetX + frameX + _frame.dx();
                        unsigned int pixY = _frame.y() - _globalOffsetY + frameY + _frame.dy();
                        _paintImagePixel(painter, frameX, frameY, pixX, pixY);
                    }
                }
            } else {
                for (unsigned int x = 0; x < _frame.w(); x++) {
                    for (unsigned int y = 0; y < _frame.h(); y++) {
                        unsigned int frameX = x + _globalOffsetX - _frame.dx();
                        unsigned int frameY = y + _globalOffsetY - _frame.dy();
                        if (frameX < 0 || frameY < 0 || frameX >= _cellCountHor || frameY >= _cellCountVert) {
                            continue;
                        }
                        unsigned int pixX = _frame.x() + x;
                        unsigned int pixY = _frame.y() + y;
                        _paintImagePixel(painter, frameX, frameY, pixX, pixY);
                    }
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

        if(_frames && _image){
            const Frame& _frame = (*_frames)[_frameIdx];
            QPen pen(Qt::green, 3);
            pen.setJoinStyle(Qt::MiterJoin);
            painter->setPen(pen);
            painter->setBrush(Qt::NoBrush);
            int rab_x = (_globalOffsetX) * _cellSize;
            int rab_y = (_globalOffsetY) * _cellSize;
            int x = rab_x - _frame.dx() * _cellSize;
            int y = rab_y - _frame.dy() * _cellSize;
            int w = _frame.w() * _cellSize;
            int h = _frame.h() * _cellSize;
            if (_showTex) {
                painter->drawRect(x, y, w, h);
            }
            pen.setColor(Qt::red);
            if (_showBaseline) {
                painter->setPen(pen);
                const Frame& frame = (*_frames)[0];
                int x0 = (_globalOffsetX - frame.dx()) * _cellSize;
                int y0 = (_globalOffsetY - frame.dy()) * _cellSize;
                int w0 = frame.w() * _cellSize;
                int h0 = frame.h() * _cellSize;
                painter->drawLine(0, y0 + h0, _cellSize * _cellCountVert, y0 + h0);
            }
            pen.setColor(Qt::magenta);
            if (_showHitbox) {
                painter->setPen(pen);
                painter->drawRect(rab_x, rab_y, int(RABBIT_SIZE.x/2) * _cellSize, int(RABBIT_SIZE.y/2) * _cellSize);
            }
        }

    }

    void setFrame(AnimInfo::Frames* frames, int index) {
        _frames = frames;
        _frameIdx = index;
        update();
    }

    void setImageFrames(Image* image, AnimInfo::Frames* frames, int index) {
        _image = image;
        setFrame(frames, index);
    }
};
