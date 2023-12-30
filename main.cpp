#include <bits/stdc++.h>

using namespace std;

struct forest {
    int left = -1;
    int right = -1;
    int parent = -1;
    int val = -1;
    int sz = 0;
};

string archive_name = "output.txt";
string file_name = "input.txt";

void encode() {
    FILE *f;
    vector<int> fr(256);
    f = fopen(file_name.c_str(), "rb");
    unsigned char ch;
    while (fscanf(f, "%c", &ch) != -1) {
        fr[ch]++;
    }
    fclose(f);
    vector<forest> ft;
    int K = 0;
    for (int i = 0; i < 256; i++) {
        if (fr[i] > 0) {
            K++;
            ft.push_back({-1, -1, -1, (unsigned char) i, fr[i]});
        }
    }
    while (true) {
        int mn = 1000000000;
        int mn2 = 1000000000;
        int p1 = 0;
        int p2 = 0;
        for (int i = 0; i < ft.size(); i++) {
            if (ft[i].sz == 0) continue;
            if (ft[i].sz <= mn) {
                mn2 = mn;
                p2 = p1;
                p1 = i;
                mn = ft[i].sz;
            } else if (ft[i].sz <= mn2) {
                mn2 = ft[i].sz;
                p2 = i;
            }
        }
        if (mn2 == 1000000000) {
            break;
        }
        ft.push_back({p1, p2, -1, -1, mn + mn2});
        ft[p1].sz = 0;
        ft[p2].sz = 0;
        ft[p1].parent = ft.size() - 1;
        ft[p2].parent = ft.size() - 1;
    }
    vector<string> code(256);
    for (int i = 0; i < K; i++) {
        string s;
        int v = i;
        while (ft[v].parent != -1) {
            int nv = ft[v].parent;
            if (ft[nv].left == v) {
                s += '0';
            } else {
                s += '1';
            }
            v = nv;
        }
        reverse(s.begin(), s.end());
        if (s.empty()) {
            s = "0";
        }
        code[ft[i].val] = s;
    }
//    FILE *g = fopen(archive_name.c_str(),"wb");
    bitset<9> B = K;
    vector<bool> to_print;
    for (int i = 0; i < 9; i++) {
        to_print.push_back(B[i]);
    }
    for (int i = 0; i < 256; i++) {
        if (!code[i].empty()) {
            bitset<8> B1 = i;
            for (int i1 = 0; i1 < 8; i1++) {
                to_print.push_back(B1[i1]);
            }
            B1 = code[i].size();
            for (int i1 = 0; i1 < 8; i1++) {
                to_print.push_back(B1[i1]);
            }
            for (char i1: code[i]) {
                to_print.push_back(i1 == '1');
            }
        }
    }
    f = fopen(file_name.c_str(), "rb");
    while (fscanf(f, "%c", &ch) != -1) {
        for (char i1: code[ch]) {
            to_print.push_back(i1 == '1');
        }
    }
    to_print.push_back(1);
    while (to_print.size() % 8 != 0) {
        to_print.push_back(0);
    }
    int sz = to_print.size();
    sz /= 8;
    bitset<32> C = sz;

    int i = 0;
    FILE *g = fopen(archive_name.c_str(), "wb");

    while (i < 32) {
        ch = 0;
        for (int j = 0; j < 8; j++) {
            ch <<= 1;
            ch |= C[i + j];
        }
        i += 8;
        fprintf(g, "%c", ch);
    }
    i = 0;
    while (i < to_print.size()) {
        ch = 0;
        for (int j = 0; j < 8; j++) {
            ch <<= 1;
            ch |= to_print[i + j];
        }
        i += 8;
        fprintf(g, "%c", ch);
    }
    fclose(g);
}

struct node {
    node *left = nullptr;
    node *right = nullptr;
    unsigned char val;
};

void decode() {
    int sz = 0;
    unsigned char ch;
    FILE *f = fopen(archive_name.c_str(), "rb");
    for (int i1 = 0; i1 < 4; i1++) {
        fscanf(f, "%c", &ch);
        for (int j = 7; j >= 0; j--) {
            sz |= (ch & 1) << (i1 * 8 + j);
            ch >>= 1;
        }
        sz = (sz | (int(ch) << (i1 * 8)));
    }
    vector<bool> bites(sz * 8);
    for (int i = 0; i < sz; i++) {
        fscanf(f, "%c", &ch);
        for (int j = 7; j >= 0; j--) {
            bites[i * 8 + j] = (ch & 1);
            ch >>= 1;
        }
    }
    fclose(f);
    while (!bites[bites.size() - 1]) {
        bites.pop_back();
    }
    bites.pop_back();
    int n = 0;
    int i = 0;
    for (int j = 0; j < 9; j++) {
        n |= bites[i + j] << j;
    }
    i += 9;
    node *root = new node();
    for (int i1 = 0; i1 < n; i1++) {
        unsigned char val = 0;
        for (int j = 0; j < 8; j++) {
            val |= bites[i + j] << (j);
        }
        i += 8;
        int sz2 = 0;
        for (int i3 = 0; i3 < 8; i3++) {
            sz2 |= (bites[i + i3]) << (i3);
        }
        i += 8;
        node *v = root;
        for (int j = 0; j < sz2; j++) {
            if (bites[i + j]) {
                if (v->right == nullptr) {
                    v->right = new node();
                }
                v = v->right;
            } else {
                if (v->left == nullptr) {
                    v->left = new node();
                }
                v = v->left;
            }
        }
        i += sz2;
        v->val = val;
    }
    node *v = root;
    FILE *g = fopen(file_name.c_str(), "wb");
    while (i < bites.size()) {
        if (bites[i]) {
            v = v->right;
        } else {
            v = v->left;
        }
        if (v->left == nullptr && v->right == nullptr) {
            fprintf(g, "%c", v->val);
            v = root;
        }
        i++;
    }
    fclose(g);
}


int32_t main(int argc, char *argv[]) {
    archive_name = argv[2];
    file_name = argv[3];
    if (!strcmp("encode", argv[1])) {
        encode();
    } else {
        decode();
    }
}
