#include "widget.h"
#include "ui_widget.h"
#include <graph.cpp>
#include <vertex.cpp>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->setWindowState(Qt::WindowFullScreen);
    W = this->width();
    H = this->height();
    SHIFT = W / 30;
    BG = QColor("#EFEFFE");
    this->setStyleSheet("Widget {background-color: #EFEFFE}");
    this->setFixedSize(W, H);
    FONT = this->font();

    OPTIMIZE = 0;

    FONT.setFamily("courier");
    FONT.setPixelSize(25);

    QFont button_font = FONT;
    button_font.setPixelSize(18);

    QFont name_f = FONT;
    name_f.setPixelSize(55);

    NAME.setParent(this);
    NAME.setFont(name_f);
    NAME.setAlignment(Qt::AlignCenter);
    NAME.setStyleSheet("font-weight: bold");
    NAME.setText("Kuhn algorithm visualization.");
    NAME.setGeometry(SHIFT, SHIFT, W / 3 - SHIFT, SHIFT / 2);

    RULE.setParent(this);
    RULE.setFont(FONT);
    RULE.setAlignment(Qt::AlignLeft);
    RULE.setText("Firstly, enter the number of vertices\nN. Secondly, enter the number of edges\nof the bipartite graph M.\n\nNext, enter M pairs of numbers:\nindexes of vertices that are the ends\nof the edges of the graph.");
    RULE.setGeometry(SHIFT, NAME.y() + NAME.height() + SHIFT / 2, W / 3 - SHIFT, H / 6);

    int button = (RULE.width() - 3 * (SHIFT / 2)) / 4;

    go_original.setParent(this);
    go_original.setGeometry(SHIFT, RULE.y() + RULE.height() + SHIFT, button, SHIFT);
    go_original.setText("original");
    go_original.setFont(button_font);

    go_optimize.setParent(this);
    go_optimize.setGeometry(go_original.x() + go_original.width() + SHIFT / 2, go_original.y(), button, SHIFT);
    go_optimize.setText("optimized");
    go_optimize.setFont(button_font);

    itr.setParent(this);
    itr.setGeometry(go_optimize.x() + go_optimize.width() + SHIFT / 2, go_original.y(), button, SHIFT);
    itr.setText("iterate");
    itr.setFont(button_font);

    reset.setParent(this);
    reset.setGeometry(itr.x() + itr.width() + SHIFT / 2, go_original.y(), button, SHIFT);
    reset.setText("clear");
    reset.setFont(button_font);

    input.setParent(this);
    input.setGeometry(SHIFT, go_original.y() + go_original.height() + SHIFT, RULE.width(), H - SHIFT - (go_original.y() + go_original.height() + SHIFT));
    input.setFont(FONT);

    X0 = NAME.x() + NAME.width() + SHIFT;
    Y0 = NAME.y();
    W0 = W - X0 - SHIFT;
    H0 = H - Y0 - SHIFT;

    PM = QPixmap(W0, H0);

    need_generate = 0;
    need_redraw = 0;
    need_clear = 1;

    ITER_IND = -1;

    QObject::connect(&input, SIGNAL(textChanged()), this, SLOT(get_text()));
    QObject::connect(&go_original, SIGNAL(released()), this, SLOT(set_original()));
    QObject::connect(&go_optimize, SIGNAL(released()), this, SLOT(set_optimize()));
    QObject::connect(&go_original, SIGNAL(released()), this, SLOT(start()));
    QObject::connect(&go_optimize, SIGNAL(released()), this, SLOT(start()));
    QObject::connect(&reset, SIGNAL(released()), this, SLOT(clear()));
    QObject::connect(&itr, SIGNAL(released()), this, SLOT(new_iterate()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::get_vertex_pos(int sz, QVector <v> &vert) {
    for (int y = 0; y + 2 * sz <= H0 - SHIFT; y += 2 * sz) {
        int cur_y1 = y + sz;
        int cur_y2 = y + 2 * sz;
        int cur_x1 = SHIFT;
        int cur_x2 = W0 - SHIFT;
        vert.push_back(v(cur_x1, cur_x2, cur_y1, cur_y2));
    }
}

void Widget::paintEvent(QPaintEvent *event) {
    if (need_clear) {
        QPainter p(&PM);
        p.setPen(BG);
        for (int i = 0; i < H0; ++i) {
            for (int j = 0; j < W0; ++j) {
                p.drawPoint(j, i);
            }
        }

        QPainter p0(this);
        p0.drawPixmap(X0, Y0, W0, H0, PM);
        need_clear = 0;
        return;
    }
    if (!need_redraw) {
        QPainter p(this);
        p.drawPixmap(X0, Y0, W0, H0, PM);
        return;
    }

    QPainter p(&PM);
    QColor cur_color = QColor(6, 24, 65);
    p.setPen(cur_color);

    std::cerr << "ITERATION: " << ITER_IND << "\n";

    QVector <int> cur_ans;
    if (ITER_IND == -1) {
        cur_ans = ANS;
    }
    else {
        cur_ans = ITER[ITER_IND];
    }

    print_match(N, cur_ans, COLOR);

    QVector <v> vert;
    QVector <QVector <bool>> to_color;

    POS.clear();
    POS.resize(N);

    int len = get_num_len(N + 1);
    int sz_x = (W0 / 4) / len;
    int sz_y = (H0 - SHIFT) / (2 * ((N + 1) / 2));
    int sz = std::min(sz_x, sz_y);

    get_vertex_pos(sz, vert);

    int ind = 0;
    QVector <bool> used(N, 0);

    int sz_x_mx = sz * len;

    QVector <bool> in_match(N, 0);

    for (int i = 0; i < N; ++i) {
        if (COLOR[i] == 2 && cur_ans[i] != -1) {
            used[i] = 1;
            used[cur_ans[i]] = 1;

            in_match[i] = 1;
            in_match[cur_ans[i]] = 1;

            auto elem = vert[ind];
            int cur_sz = sz * get_num_len(cur_ans[i] + 1);
            int sh = (sz_x_mx - cur_sz) / 2;
            int cur_x = elem.x1 + sh;

            draw_num(cur_ans[i] + 1, sz, to_color);
            for (int y = 0; y < to_color.size(); ++y) {
                for (int x = 0; x < to_color[y].size(); ++x) {
                    if (to_color[y][x]) {
                        p.drawPoint(x + cur_x, y + elem.y1);
                    }
                }
            }

            draw_frame(sz_x_mx, sz, to_color);
            for (int y = 0; y < to_color.size(); ++y) {
                for (int x = 0; x < to_color[y].size(); ++x) {
                    if (to_color[y][x]) {
                        p.drawPoint(x + elem.x1, y + elem.y1);
                    }
                }
            }
            int y_mid = (elem.y1 + elem.y2) / 2;
            POS[cur_ans[i]] = sq(elem.x1 - 1, y_mid, elem.x1 + sz_x_mx, y_mid);

            cur_sz = sz * get_num_len(i + 1);
            sh = (sz_x_mx - cur_sz) / 2;
            cur_x = elem.x2 - sz_x_mx + sh;

            draw_num(i + 1, sz, to_color);
            for (int y = 0; y < to_color.size(); ++y) {
                for (int x = 0; x < to_color[y].size(); ++x) {
                    if (to_color[y][x]) {
                        p.drawPoint(x + cur_x, y + elem.y1);
                    }
                }
             }

             draw_frame(sz_x_mx, sz, to_color);
             for (int y = 0; y < to_color.size(); ++y) {
                for (int x = 0; x < to_color[y].size(); ++x) {
                    if (to_color[y][x]) {
                        p.drawPoint(x + elem.x2 - sz_x_mx, y + elem.y1);
                    }
                 }
             }
             POS[i] = sq(elem.x2 - sz_x_mx - 1, y_mid, elem.x2, y_mid);

            ++ind;
        }
    }

    p.setPen(QColor(180, 180, 180));

    //draw vertex not in match
    int last_ind = -1;
    for (int i = 0; i < N; ++i) {
        if (used[i]) continue;
        if (last_ind == -1) {
            last_ind = i;
            continue;
        }

        used[i] = 1;
        used[last_ind] = 1;

        auto elem = vert[ind];
        int cur_sz = sz * get_num_len(last_ind + 1);
        int sh = (sz_x_mx - cur_sz) / 2;
        int cur_x = elem.x1 + sh;

        draw_num(last_ind + 1, sz, to_color);
        for (int y = 0; y < to_color.size(); ++y) {
            for (int x = 0; x < to_color[y].size(); ++x) {
                if (to_color[y][x]) {
                    p.drawPoint(x + cur_x, y + elem.y1);
                }
            }
        }

        draw_frame(sz_x_mx, sz, to_color);
        for (int y = 0; y < to_color.size(); ++y) {
            for (int x = 0; x < to_color[y].size(); ++x) {
                if (to_color[y][x]) {
                    p.drawPoint(x + elem.x1, y + elem.y1);
                }
            }
        }
        int y_mid = (elem.y1 + elem.y2) / 2;
        POS[last_ind] = sq(elem.x1 - 1, y_mid, elem.x1 + sz_x_mx, y_mid);

        cur_sz = sz * get_num_len(i + 1);
        sh = (sz_x_mx - cur_sz) / 2;
        cur_x = elem.x2 - sz_x_mx + sh;

        draw_num(i + 1, sz, to_color);
        for (int y = 0; y < to_color.size(); ++y) {
            for (int x = 0; x < to_color[y].size(); ++x) {
                if (to_color[y][x]) {
                    p.drawPoint(x + cur_x, y + elem.y1);
                }
            }
         }

         draw_frame(sz_x_mx, sz, to_color);
         for (int y = 0; y < to_color.size(); ++y) {
            for (int x = 0; x < to_color[y].size(); ++x) {
                if (to_color[y][x]) {
                    p.drawPoint(x + elem.x2 - sz_x_mx, y + elem.y1);
                }
             }
         }
         POS[i] = sq(elem.x2 - sz_x_mx - 1, y_mid, elem.x2, y_mid);
         last_ind = -1;
         ++ind;
    }
    //draw really last vertex in match
    for (int i = 0; i < N; ++i) {
        if (used[i]) continue;
        used[i] = 1;

        auto elem = vert[ind];
        int cur_sz = sz * get_num_len(i + 1);
        int sh = (sz_x_mx - cur_sz) / 2;
        int cur_x = elem.x1 + sh;

        draw_num(i + 1, sz, to_color);
        for (int y = 0; y < to_color.size(); ++y) {
            for (int x = 0; x < to_color[y].size(); ++x) {
                if (to_color[y][x]) {
                    p.drawPoint(x + cur_x, y + elem.y1);
                }
            }
        }

        draw_frame(sz_x_mx, sz, to_color);
        for (int y = 0; y < to_color.size(); ++y) {
            for (int x = 0; x < to_color[y].size(); ++x) {
                if (to_color[y][x]) {
                    p.drawPoint(x + elem.x1, y + elem.y1);
                }
            }
        }
        int y_mid = (elem.y1 + elem.y2) / 2;
        POS[i] = sq(elem.x1 - 1, y_mid, elem.x1 + sz_x_mx, y_mid);
        break;
    }

    //draw edges
    for (int i = 0; i < N; ++i) {
        for (auto j : G[i]) {
            if (j <= i) continue;
            bool good = 0;
            if (COLOR[i] == 2 && cur_ans[i] == j) {
                good = 1;
            }
            if (COLOR[j] == 2 && cur_ans[j] == i) {
                good = 1;
            }

            if (good) {
                p.setPen(QPen(QColor(190, 130, 252), 4, Qt::SolidLine));
            }
            else {
                if (in_match[i] && in_match[j]) {
                    p.setPen(QPen(cur_color, 2, Qt::DotLine));
                }
                else {
                    p.setPen(QPen(QColor(180, 180, 180), 2, Qt::DotLine));
                }
            }

            if (POS[i].Lx < POS[j].Lx) {
                p.drawLine(POS[i].Rx, POS[i].Ry, POS[j].Lx, POS[j].Ly);
            }
            else if (POS[i].Lx > POS[j].Lx) {
                p.drawLine(POS[j].Rx, POS[j].Ry, POS[i].Lx, POS[i].Ly);
            }
            else {
                bool left = (W0 / 2 > POS[i].Rx);
                if (std::abs(POS[i].Ly - POS[j].Ly) == 2 * sz) {
                    int start_x = (POS[i].Lx + POS[i].Rx) / 2;
                    int start_y1 = std::min(POS[i].Ly, POS[j].Ly) + sz / 2;
                    int start_y2 = std::max(POS[i].Ly, POS[j].Ly) - sz / 2;

                    p.drawLine(start_x, start_y1, start_x, start_y2);

                    continue;
                }

                int W_mx = SHIFT * 2 / 3, H_mx = H0 - SHIFT;
                int start_x, start_y;
                if (left) {
                    start_x = POS[i].Lx - SHIFT / 2;
                    start_y = std::min(POS[i].Ly, POS[j].Ly);
                }
                else {
                    start_x = POS[i].Rx;
                    start_y = std::min(POS[i].Ry, POS[j].Ry);
                }

                QVector <sq> ed_line;
                draw_ellips(SHIFT / 2, std::abs(POS[i].Ly - POS[j].Ly), W_mx, H_mx, ed_line, left);

                for (auto cur_line : ed_line) {
                    p.drawLine(cur_line.Lx + start_x, cur_line.Ly + start_y,
                               cur_line.Rx + start_x, cur_line.Ry + start_y);
                }
            }
        }
    }

    QPainter p0(this);
    p0.drawPixmap(X0, Y0, W0, H0, PM);

    need_redraw = 0;
}

void Widget::set_optimize() {
    OPTIMIZE = 1;
}

void Widget::set_original() {
    OPTIMIZE = 0;
}

void Widget::new_iterate() {
    if (ITER_IND + 1 < ITER.size()) {
        ++ITER_IND;
        screen_clear();
        need_redraw = 1;
        repaint();
    }
}

void Widget::start() {
    need_generate = 1;
    get_text();
    need_generate = 0;
    need_redraw = 1;
    ITER_IND = -1;
    repaint();
}

void Widget::screen_clear() {
    need_clear = 1;

    repaint();
}

void Widget::clear() {
    need_clear = 1;

    G.clear();
    ANS.clear();
    COLOR.clear();
    POS.clear();
    ITER.clear();

    N = M = 0;
    ITER_IND = -1;

    repaint();
}

void Widget::get_text() {
    if (!need_generate) {
        return;
    }
    clear();
    QString s = input.toPlainText();
    get_graph(s, N, M, G);
    match(N, G, ANS, COLOR, ITER, OPTIMIZE);
}

void Widget::keyPressEvent(QKeyEvent *event) {
    if (event->key() == 16777216) {
        close();
    }
}
