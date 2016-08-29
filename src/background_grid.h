#pragma once

#include <QGraphicsItem>
#include <QPainter>

class BackgroundGrid : public QGraphicsItem {
private:
    int _cellSize;
    int _cellCountHor;
    int _cellCountVert;
public:
    explicit BackgroundGrid(int cellSize, int cellCountGor, int cellCountVert, QGraphicsItem* parent = NULL)
            : QGraphicsItem(parent), _cellSize(cellSize), _cellCountHor(cellCountGor), _cellCountVert(cellCountVert) {

    }

    QRectF boundingRect() const override {
        return QRectF(0, 0, _cellSize * _cellCountHor + 1, _cellSize * _cellCountVert + 1);
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::lightGray);
        painter->drawRect(boundingRect());
        painter->setPen(Qt::blue);
        for (int i = 0; i <= _cellCountVert; i++) {
            int offset_y = _cellSize * i;
            painter->drawLine(0, offset_y, _cellSize * _cellCountHor, offset_y);
        }
        for (int i = 0; i <= _cellCountHor; i++) {
            int offset_x = _cellSize * i;
            painter->drawLine(offset_x, 0, offset_x, _cellSize * _cellCountVert);
        }
    }
};
