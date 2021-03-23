#include <iostream>
#include <algorithm>
#include <QVector>
#include <widget.h>

int get_num_len(int n) {
    if (n == 0) {
        return 1;
    }
    int cnt = 0;
    while (n) {
        n /= 10;
        ++cnt;
    }
    return cnt;
}

void color_resize(int W, int H, QVector <QVector <bool>> &color) {
    color.clear();
    color.resize(H);
    for (int y = 0; y < H; ++y) {
        color[y].resize(W);
        for (int x = 0; x < W; ++x) {
            color[y][x] = 0;
        }
    }
}

void draw_frame(int W, int H, QVector <QVector <bool>> &color) { //(y, x)
    color_resize(W, H, color);

    for (int y = 0; y <= H - 1; ++y) {
        color[y][0] = 1;
        color[y][W - 1] = 1;
    }
    for (int x = 0; x <= W - 1; ++x) {
        color[0][x] = 1;
        color[H - 1][x] = 1;
    }
}

void draw0(int W, int H, QVector <QVector <bool>> &color) { //(y, x)
    color_resize(W, H, color);

    int shift = std::min(W, H) / 4;
    for (int y = shift; y <= H - shift; ++y) {
        color[y][shift] = 1;
        color[y][W - shift] = 1;
    }
    for (int x = shift; x <= W - shift; ++x) {
        color[shift][x] = 1;
        color[H - shift][x] = 1;
    }
}

void draw1(int W, int H, QVector <QVector <bool>> &color) {
    color_resize(W, H, color);

    int shift = std::min(W, H) / 4;
    for (int y = shift; y <= H - shift; ++y) {
        color[y][W / 2] = 1;
    }
}

void draw2(int W, int H, QVector <QVector <bool>> &color) {
    color_resize(W, H, color);

    int shift = std::min(W, H) / 4;
    for (int x = shift; x <= W - shift; ++x) {
        color[shift][x] = 1;
        color[H / 2][x] = 1;
        color[H - shift][x] = 1;
    }
    for (int y = shift; y <= H / 2; ++y) {
        color[y][W - shift] = 1;
    }
    for (int y = H / 2; y <= H - shift; ++y) {
        color[y][shift] = 1;
    }
}

void draw3(int W, int H, QVector <QVector <bool>> &color) {
    color_resize(W, H, color);

    int shift = std::min(W, H) / 4;
    for (int x = shift; x <= W - shift; ++x) {
        color[shift][x] = 1;
        color[H / 2][x] = 1;
        color[H - shift][x] = 1;
    }
    for (int y = shift; y <= H - shift; ++y) {
        color[y][W - shift] = 1;
    }
}

void draw4(int W, int H, QVector <QVector <bool>> &color) {
    color_resize(W, H, color);

    int shift = std::min(W, H) / 4;
    for (int x = shift; x <= W - shift; ++x) {
        color[H / 2][x] = 1;
    }
    for (int y = shift; y <= H / 2; ++y) {
        color[y][shift] = 1;
        color[y][W - shift] = 1;
    }
    for (int y = H / 2; y <= H - shift; ++y) {
        color[y][W - shift] = 1;
    }
}

void draw5(int W, int H, QVector <QVector <bool>> &color) {
    color_resize(W, H, color);

    int shift = std::min(W, H) / 4;
    for (int x = shift; x <= W - shift; ++x) {
        color[shift][x] = 1;
        color[H / 2][x] = 1;
        color[H - shift][x] = 1;
    }
    for (int y = shift; y <= H / 2; ++y) {
        color[y][shift] = 1;
    }
    for (int y = H / 2; y <= H - shift; ++y) {
        color[y][W - shift] = 1;
    }
}

void draw6(int W, int H, QVector <QVector <bool>> &color) {
    color_resize(W, H, color);

    int shift = std::min(W, H) / 4;
    for (int x = shift; x <= W - shift; ++x) {
        color[shift][x] = 1;
        color[H / 2][x] = 1;
        color[H - shift][x] = 1;
    }
    for (int y = shift; y <= H - shift; ++y) {
        color[y][shift] = 1;
    }
    for (int y = H / 2; y <= H - shift; ++y) {
        color[y][W - shift] = 1;
    }
}

void draw7(int W, int H, QVector <QVector <bool>> &color) {
    color_resize(W, H, color);

    int shift = std::min(W, H) / 4;
    for (int x = shift; x <= W - shift; ++x) {
        color[shift][x] = 1;
    }
    for (int y = shift; y <= H - shift; ++y) {
        color[y][W - shift] = 1;
    }
}

void draw8(int W, int H, QVector <QVector <bool>> &color) {
    color_resize(W, H, color);

    int shift = std::min(W, H) / 4;
    for (int x = shift; x <= W - shift; ++x) {
        color[shift][x] = 1;
        color[H / 2][x] = 1;
        color[H - shift][x] = 1;
    }
    for (int y = shift; y <= H - shift; ++y) {
        color[y][shift] = 1;
        color[y][W - shift] = 1;
    }
}

void draw9(int W, int H, QVector <QVector <bool>> &color) {
    color_resize(W, H, color);

    int shift = std::min(W, H) / 4;
    for (int x = shift; x <= W - shift; ++x) {
        color[shift][x] = 1;
        color[H / 2][x] = 1;
        color[H - shift][x] = 1;
    }
    for (int y = shift; y <= H / 2; ++y) {
        color[y][shift] = 1;
        color[y][W - shift] = 1;
    }
    for (int y = H / 2; y <= H - shift; ++y) {
        color[y][W - shift] = 1;
    }
}

void get_digits(int n, QVector <int> &a) {
    a.clear();
    if (n == 0) {
        a.push_back(0);
        return;
    }
    while (n) {
        a.push_back(n % 10);
        n /= 10;
    }
    for (int i = 0; i < a.size() / 2; ++i) {
        std::swap(a[i], a[a.size() - 1 - i]);
    }
}

void draw_num(int n, int sz, QVector <QVector <bool>> &color) {
    QVector <int> digits;
    get_digits(n, digits);
    int W = sz * digits.size();
    int H = sz;

    color.clear();
    color.resize(H);
    QVector <QVector <bool>> tmp;
    for (auto d : digits) {
        if (d == 0) draw0(sz, sz, tmp);
        else if (d == 1) draw1(sz, sz, tmp);
        else if (d == 2) draw2(sz, sz, tmp);
        else if (d == 3) draw3(sz, sz, tmp);
        else if (d == 4) draw4(sz, sz, tmp);
        else if (d == 5) draw5(sz, sz, tmp);
        else if (d == 6) draw6(sz, sz, tmp);
        else if (d == 7) draw7(sz, sz, tmp);
        else if (d == 8) draw8(sz, sz, tmp);
        else draw9(sz, sz, tmp);

        for (int y = 0; y < H; ++y) {
            for (auto elem : tmp[y]) {
                color[y].push_back(elem);
            }
        }
    }
}

void draw_ellips(int W, int H, int W_mx, int H_mx, QVector <sq> &a, bool left) {
    int W0 = H * W_mx / H_mx;

    int x1, x2, x3;
    if (left) {
        x1 = W - W0;
        x2 = W - 1;
        x3 = x1;
    }
    else {
        x1 = 0;
        x2 = W0;
        x3 = x2;
    }

    a.push_back(sq(x1, 0, x2, 0));
    a.push_back(sq(x1, H - 1, x2, H - 1));
    a.push_back(sq(x3, 0, x3, H - 1));
}

