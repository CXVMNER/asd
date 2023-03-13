#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

struct AVL {
    string eng;
    vector<string> pl;
    int w = 0;
    AVL* left = NULL;
    AVL* right = NULL;
};
AVL* root;

bool rotateL(AVL* &node) {
    AVL* nodeR = node->right;
    node->right = nodeR->left;
    nodeR->left = node;
    node = nodeR;
}

bool rotateR(AVL* &node) {
    AVL* nodeL = node->left;
    node->left = nodeL->right;
    nodeL->right = node;
    node = nodeL;
}

bool balanceIL(AVL* &node) {
    switch(node->w) {
    case 1:
        node->w = 0;
        return true;
        break;
    case 0:
        node->w = -1;
        break;
    case -1:
        if(node->right->w == -1) {
            rotateL(node);
            node->left->w = 0;
        } else {
            rotateR(node->right); rotateL(node);
            if(node->w == 0) {
                node->left->w = 0; node->right->w = 0;
            } else if(node->w == 1) {
                node->left->w = 0; node->right->w = -1;
            } else if(node->w == -1) {
                node->left->w = 1; node->right->w = 0;
            }
        }
        node->w = 0; return true;
        break;
    }
    return false;
}

bool balanceIR(AVL* &node) {
    switch(node->w) {
    case 1:
        if(node->left->w == 1) {
            rotateR(node);
            node->right->w = 0;
        } else {
            rotateL(node->left); rotateR(node);
            if(node->w == 0) {
                node->left->w = 0; node->right->w = 0;
            } else if(node->w == 1) {
                node->left->w = 0; node->right->w = -1;
            } else if(node->w == -1) {
                node->left->w = 1; node->right->w = 0;
            }
        }
        node->w = 0; return true;
        break;
    case 0:
        node->w = 1;
        break;
    case -1:
        node->w = 0;
        return true;
        break;
    }
    return false;
}

bool balanceDL(AVL* &node) {
    switch(node->w) {
    case 1:
        node->w = 0;
        break;
    case 0:
        node->w = -1;
        return true;
        break;
    case -1:
        switch(node->right->w) {
        case -1:
            rotateL(node); node->w = 0; node->left->w = 0;
            break;
        case 0:
            rotateL(node); node->w = 1; node->left->w = -1;
            return true;
            break;
        case 1:
            rotateR(node->right); rotateL(node);
            if(node->w == 0) {
                node->left->w = 0; node->right->w = 0;
            } else if(node->w == 1) {
                node->left->w = 0; node->right->w = -1;
            } else if(node->w == -1) {
                node->left->w = 1; node->right->w = 0;
            }
            break;
        } break;
    }
    return false;
}

bool balanceDR(AVL* &node) {
    switch(node->w) {
    case 1:
        switch(node->left->w) {
        case 1:
            rotateR(node); node->w = 0; node->right->w = 0;
            break;
        case 0:
            rotateR(node); node->w = -1; node->right->w = 1;
            return true;
            break;
        case -1:
            rotateL(node->left); rotateR(node);
            if(node->w == 0) {
                node->left->w = 0; node->right->w = 0;
            } else if(node->w == 1) {
                node->left->w = 0; node->right->w = -1;
            } else if(node->w == -1) {
                node->left->w = 1; node->right->w = 0;
            }
            node->w = 0;
            break;
        } break;
    case 0:
        node->w = 1;
        return true;
        break;
    case -1:
        node->w = 0;
        break;
    }
    return false;
}

AVL* searchN(AVL* node, string eng) {
    if(!node) return NULL;
    if(eng.compare(node->eng) < 0) return searchN(node->left, eng);
    else if(eng.compare(node->eng) > 0) return searchN(node->right, eng);
    else return node;
}

bool add_(AVL* &node, string eng, vector<string> pl) {
    if(!node) {
        node = new AVL({eng, pl});
        return false;
    }
    if(eng.compare(node->eng) < 0) {
        if(!add_(node->left, eng, pl)) {
            return balanceIR(node);
        }
    } else if(eng.compare(node->eng) > 0) {
        if(!add_(node->right, eng, pl)) {
            return balanceIL(node);
        }
    } else {
        cout << "Podane slowo juz istnieje w slowniku." << endl;
    }
    return true;
}

void add(string eng, vector<string> pl) {
    add_(root, eng, pl);
}

bool delMax(AVL* &p, AVL* node) {
    if(p->right) {
        if(!delMax(p->right, node)) return balanceDR(p);
    }
    else {
        node->eng = p->eng;
        AVL* pp = p;
        p = p->left;
        delete pp;
        return false;
    }
}

bool del_(AVL* &node, string eng) {
    if(!node) {
        cout << "Brak szukanego slowa" << endl;
        return true;
    }

    if(eng.compare(node->eng) < 0) {
        if(!del_(node->left, eng)) return balanceDL(node);
    }
    else if(eng.compare(node->eng) > 0) {
        if(!del_(node->right, eng)) return balanceDR(node);
    }
    else {
        AVL* p = node;
        if(!p->left) {
            node = node->right;
            delete p;
            return false;
        } else if(!p->right) {
            node = node->left;
            delete p;
            return false;
        } else if(!delMax(p->left, node)) return balanceDL(node);
    }
    return true;
}

void del(string eng) {
    del_(root, eng);
}

void save_(AVL* node, ofstream &out) {
    if(!node) return;
    if(node->left) save_(node->left, out);

    string tmp = node->eng + ',';
    for(int i=0; i<node->pl.size(); i++) tmp += node->pl[i] + ',';
    out << tmp << endl;

    if(node->right) save_(node->right, out);
}

void save(string file) {
    ofstream out;
    out.open(file);
    save_(root, out);
    out.close();
}

void load(string file) {
    ifstream in;
    in.open(file);

    string line, eng;
    bool isEng;
    vector<string> pl;
    while(!in.eof()) {
        getline(in, line);
        if(line == "") continue;
        isEng = true; eng=""; pl.clear();
        size_t pos = 0;
        while((pos = line.find(',')) != string::npos) {
            if(isEng) {
                eng = line.substr(0, pos);
                isEng = false;
            }
            else pl.push_back(line.substr(0, pos));
            line.erase(0, pos+1);
        }
        add(eng, pl);
    }

    in.close();
}

void print_(AVL* node) {
    cout << node->eng << " " << node->w << ":";
    if(node->left && node->right) cout << " (lewy - " << node->left->eng << ", prawy - " << node->right->eng << ")\n";
    else if(node->left) cout << " (lewy - " << node->left->eng << ")\n";
    else if(node->right) cout << " (prawy - " << node->right->eng << ")\n";
    else cout << endl;
    for(int i=0; i<node->pl.size(); i++) cout << node->pl[i] << endl;
    cout << endl;
}

void print(AVL* node) {
    if(!node) return;
    print_(node);
    if(node->left) print(node->left);
    if(node->right) print(node->right);
}

void deleteAll(AVL* &node) {
    if(!node) return;
    if(node->left) deleteAll(node->left);
    if(node->right) deleteAll(node->right);
    node->left = node->right = NULL;
    delete node;
}

int main() {
    int choice=-1; bool fin=false;
    AVL* srchN;
    string eng, tmp;
    vector<string> pl;
    while(!fin) {
        cout << "Slownik polsko-angielski." << endl;
        cout << "Wybierz opcje." << endl;
        cout << "1. Znajdz slowo." << endl;
        cout << "2. Dodaj slowo." << endl;
        cout << "3. Usun slowo." << endl;
        cout << "4. Zapisz slownik." << endl;
        cout << "5. Wczytaj slownik." << endl;
        cout << "6. Wypisz slownik." << endl;
        cout << "0. Zakoncz program." << endl;
        cin >> choice;
        cin.ignore();
        cout << endl;

        switch(choice) {
        case 1:
            cout << "Podaj slowo:" << endl;
            getline(cin, eng);
            cout << endl;
            srchN = searchN(root, eng);
            if(!srchN) cout << "Brak podanego slowa." << endl;
            else print_(srchN);
            cout << endl;
            break;
        case 2:
            eng = ""; pl.clear();
            cout << "Podaj slowo angielskie:" << endl;
            getline(cin, eng);
            cout << "Podaj polskie tlumaczenia (znak q zatrzymuje wpisywanie tlumaczen):" << endl;
            while(1) {
                getline(cin, tmp);
                if(tmp == "q") break;
                else pl.push_back(tmp);
            }
            if(!pl.size()) cout << "Brak tlumaczen. Nie dodano slowa." << endl;
            else add(eng, pl);
            cout << endl;
            break;
        case 3:
            cout << "Podaj slowo angielskie:" << endl;
            getline(cin, eng);
            del(eng);
            cout << endl;
            break;
        case 4:
            save("AVL_malaBaza_7_Karolczak.txt");
            cout << "Zapisano slownik." << endl;
            cout << endl;
            break;
        case 5:
            load("AVL_malaBaza_7_Karolczak.txt");
            cout << "Wczytano slownik." << endl;
            cout << endl;
            break;
        case 6:
            print(root);
            break;
        case 0:
            fin=true;
            deleteAll(root);
            break;
        default:
            cout << "Niepoprawna komenda" << endl;
            cout << endl;
            break;
        }
    }
}
