#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QPixmap>
#include <QLabel>
#include <iostream>
#include <algorithm>

struct v {
    int x1, x2, y1, y2;
    v(int ex1 = 0, int ex2 = 0, int ey1 = 0, int ey2 = 0) {
        x1 = ex1, x2 = ex2, y1 = ey1, y2 = ey2;
    }
};

struct sq {
    int Lx, Ly, Rx, Ry;
    sq(int lx = 0, int ly = 0, int rx = 0, int ry = 0) {
        Lx = lx, Rx = rx, Ly = ly, Ry = ry;
    }
};

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    int W, H, SHIFT;
    int X0, Y0, W0, H0;
    QFont FONT;
    QColor BG;
    QPlainTextEdit input;
    QLabel RULE, NAME;
    QPushButton go_optimize, go_original, reset, itr;
    bool need_generate, need_redraw, need_clear;
    QPixmap PM;

    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);

    int N, M, ITER_IND;
    bool OPTIMIZE;
    QVector <QVector <int>> G, ITER;
    QVector <int> ANS, COLOR;
    QVector <sq> POS;

    void get_vertex_pos(int sz, QVector <v> &vert);

public slots:
    void get_text();
    void start();
    void clear();
    void screen_clear();
    void new_iterate();
    void set_optimize();
    void set_original();


private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
