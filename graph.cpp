#include <iostream>
#include <QVector>
#include <QString>
#include "widget.h"

void skip_space(QString &s, int &ind) {
    if (ind >= s.size()) {
        return;
    }
    while (ind < s.size() && (s[ind] == ' ' || s[ind] == '\n')) {
        ++ind;
    }
}

int get_num(QString &s, int &ind) {
    int j = ind;
    while (j + 1 < s.size() && '0' <= s[j + 1] && s[j + 1] <= '9') {
        ++j;
    }
    int ans = 0;
    QString tmp = "";
    for (int i = ind; i <= j; ++i) {
        ans *= 10;
        tmp = "" + s[i];
        ans += tmp.toInt();
    }
    ind = j + 1;
    return ans;
}

void print_graph(QVector<QVector<int>> &GRAPH) {
    for (int i = 0; i < GRAPH.size(); ++i) {
        for (auto j : GRAPH[i]) {
            std::cerr << i + 1 << " " << j + 1 << "\n";
        }
    }
    std::cerr << "________\n";
}

void get_graph(QString &s, int &N, int &M, QVector<QVector<int>> &GRAPH) {
    int i = 0;
    skip_space(s, i);
    N = get_num(s, i);
    skip_space(s, i);
    M = get_num(s, i);

    GRAPH.resize(N);
    for (int u = 0; u < M; ++u) {
        int a, b;
        skip_space(s, i);
        a = get_num(s, i);
        skip_space(s, i);
        b = get_num(s, i);
        --a, --b;

        GRAPH[a].push_back(b);
        GRAPH[b].push_back(a);
    }
}

void divide_to_color(int v, int cur, QVector<QVector<int>> &g, QVector <int> &color) {
    color[v] = cur;
    for (auto i : g[v]) {
        if (!color[i]) {
            divide_to_color(i, 3 - cur, g, color);
        }
    }
}

bool try_kuhn(int v, int cur, QVector<QVector<int>> &g, QVector <int> &used, QVector <int> &ans) {
    if (used[v] == cur) {
        return 0;
    }
    used[v] = cur;

    for (auto i : g[v]) {
        if (ans[i] == -1) {
            //new pair
            std::cerr << i << " - " << v << "\n";
            ans[i] = v;
            return 1;
        }
    }
    for (auto i : g[v]) {
        if (try_kuhn(ans[i], cur, g, used, ans)) {
            //changed pair
            ans[i] = v;
            std::cerr << i << " - " << v << "\n";
            return 1;
        }
    }
    return 0;
}

bool try_kuhn_original(int v, int cur, QVector<QVector<int>> &g, QVector <int> &used, QVector <int> &ans) {
    if (used[v] == cur) {
        return 0;
    }
    used[v] = cur;

    for (auto i : g[v]) {
        if (ans[i] == -1 || try_kuhn_original(ans[i], cur, g, used, ans)) {
            ans[i] = v;
            return 1;
        }
    }
    return 0;
}

void print_match(int N, QVector<int> &ans, QVector<int> &color) {
    for (int i = 0; i < N; ++i) {
        if (color[i] == 2 && ans[i] != -1) {
            std::cerr << i + 1 << " " << ans[i] + 1 << "\n";
        }
    }
    std::cerr << "----------\n";
}

void gen_start(int &N, int &cur, QVector<QVector<int>> &GRAPH, QVector <int> &ans,
               QVector <int> &color, QVector <int> &used, QVector <QVector<int>> &iter) {
    QVector <int> tmp;
    for (int i = 0; i < N; ++i) {
        tmp.push_back(i);
    }
    std::random_shuffle(tmp.begin(), tmp.end());

    for (auto i : tmp) {
        if (color[i] == 2) {
            for (auto to : GRAPH[i]) {
                if (used[to] != cur) {
                    ans[i] = to;
                    used[to] = cur;
                    break;
                }
            }
        }
    }
    iter.push_back(ans);
    cur++;
}

void match(int &N, QVector<QVector<int>> &GRAPH, QVector <int> &ans,
             QVector <int> &color, QVector <QVector<int>> &iter, bool optimize) {
    QVector <int> used(N, 0);
    ans.clear();
    ans.resize(N);
    color.clear();
    color.resize(N);
    int cur = 1;

    for (int i = 0; i < N; ++i) {
        ans[i] = -1;
        color[i] = 0;
    }

    for (int i = 0; i < N; ++i) {
        if (!color[i]) {
            divide_to_color(i, 1, GRAPH, color);
        }
    }

    if (optimize == 1) {
        gen_start(N, cur, GRAPH, ans, color, used, iter);
        QVector <int> good;
        for (int i = 0; i < N; ++i) {
            if (used[i] != cur - 1) {
                good.push_back(i);
            }
        }

        std::cerr << "start:\n";
        print_match(N, ans, color);

        for (auto i : good) {
            if (color[i] == 2) {
                continue;
            }
            if (try_kuhn(i, cur, GRAPH, used, ans)) {
                ++cur;
                iter.push_back(ans);
            }
        }
    }
    else {
        for (int i = 0; i < N; ++i) {
            if (color[i] == 2) {
                continue;
            }
            ++cur;
            try_kuhn_original(i, cur, GRAPH, used, ans);
            iter.push_back(ans);
        }
    }


    std::cerr << "got iters:\n";
    for (auto i : iter) {
        print_match(N, i, color);
    }
}
