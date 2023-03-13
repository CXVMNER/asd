#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
using namespace std;

ifstream in;
ofstream out;
stack<int> vS, pS, pathS;
bool found = false;

void DFSAlg(vector<int>* G, int* price, int w, int k, int s) {
    vS.push(w);
    if(!pS.empty()) pS.push(pS.top() + price[w]);
    else pS.push(price[w]);

    if(vS.top() == k && pS.top() == s) found = true;

    for(int i=0; i<G[w].size(); i++) {
        if(found == true) break;
        if(pS.top() + price[G[w][i]] <= s) DFSAlg(G, price, G[w][i], k, s);
    }

    if(found == true) pathS.push(vS.top());
    vS.pop();
    pS.pop();
}

int main() {
    in.open("KS_in_7_Karolczak.txt");
    out.open("KS_out_7_Karolczak.txt");

    int n, m, w, k, s;
    in >> n >> m >> w >> k >> s;

    int* price = new int[n+1];
    for(int i=1; i<=n; i++) in >> price[i];

    vector<int>* G = new vector<int>[n+1];
    int x, y;
    for(int i=0; i<m; i++) {
        in >> x >> y;
        G[x].push_back(y);
        G[y].push_back(x);
    }

    DFSAlg(G, price, w, k, s);

    while(!pathS.empty()) {
        out << pathS.top() << " ";
        pathS.pop();
    }

    in.close();
    out.close();
}
