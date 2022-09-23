#include <bits/stdc++.h>
#define taskname "test"
#define fi first
#define se second
#define pb push_back
#define faster ios_base::sync_with_stdio(0); cin.tie(0);
using namespace std;
using ll = long long;
using pii = pair <int, int>;
using pil = pair <int, ll>;
using pli = pair <ll, int>;
using pll = pair <ll, ll>;
using ull = unsigned ll;
mt19937 Rand(chrono::steady_clock::now().time_since_epoch().count());
 
ll min(const ll &a, const ll &b){
    return (a < b) ? a : b;
}
 
ll max(const ll &a, const ll &b){
    return (a > b) ? a : b;
}
 
//const ll Mod = 1000000009;
//const ll Mod2 = 999999999989;
//only use when required
const int maxN = 1e6 + 1;
const int inf = 0x3f3f3f3f;
int n, k;
bool visited[maxN];
int par[maxN];
int subtree_size[maxN];
int edge[maxN];
vector <pii> adj[maxN];
 
int cntt = 0, cnttt = 0, total = 0;
void centroid_dfs(int u, int p){
    ++cntt;
    subtree_size[u] = 1;
    for (const auto &v: adj[u]){
        int i = v.fi;
        if (visited[i] || i == p) continue;
        centroid_dfs(i, u);
        subtree_size[u] += subtree_size[i];
    }
}
 
int find_centroid(int u, int p, int sz){
    ++cntt;
    for (const auto &v: adj[u]){
        int i = v.fi;
        if (visited[i] || i == p) continue;
        if (subtree_size[i] > sz / 2){
            return find_centroid(i, u, sz);
        }
    }
    return u;
}
 
//check if there is any "best path" traversing through the ROOT of the subtree(this case: centroid)
int find_path(int u, int p, int d, int cnt){    ++cntt;
  if (k < d) return inf;
    int tem = min(edge[k - d] + cnt, inf);
    for (const auto &i: adj[u]){
        if (visited[i.fi] || i.fi == p || i.se + d > k) continue;
        tem = min(tem, find_path(i.fi, u, i.se + d, cnt + 1));
    }
    return tem;
}
 
void add_path(int u, int p, int d, int cnt){ ++cntt;
    edge[d] = min(edge[d], cnt);
    for (const auto &i: adj[u]){
        if (visited[i.fi] || i.fi == p || i.se + d > k) continue;
        add_path(i.fi, u, i.se + d, cnt + 1);
    }
}
 
void delete_path(int u, int p, int d){
    edge[d] = inf;
    for (const auto &i: adj[u]){
        if (visited[i.fi] || i.fi == p || i.se + d > k) continue;
        delete_path(i.fi, u, i.se + d);
    }
}
 
int ans = inf;
int build_centroid(int u){
    cntt = cnttt = 0;
    centroid_dfs(u, -1);
    int centroid = find_centroid(u, -1, subtree_size[u]);
    //cerr << cntt << " " << total << "\n";
    visited[centroid] = 1;
    edge[0] = 0;
    for (const auto &i: adj[centroid]){
        if (visited[i.fi]) continue;
        ans = min(ans, find_path(i.fi, centroid, i.se, 1));
        add_path(i.fi, centroid, i.se, 1);
    }
    delete_path(centroid, -1, 0);
    for (const auto &i: adj[centroid]){
        if (visited[i.fi]) continue;
        par[build_centroid(i.fi)] = centroid;
    }
    return centroid;
}
 
 
int best_path(int N, int K, int H[][2], int L[]){
    n = N; k = K;
    for (int i = 0; i < n - 1; ++i){
        int u = H[i][0], v = H[i][1];
        adj[u + 1].pb({v + 1, L[i]});
        adj[v + 1].pb({u + 1, L[i]});
    }
    memset(edge, 0x3f, sizeof(edge));
    par[build_centroid(1)] = 0;
    if (ans == inf) ans = -1;
    return ans;
}
 
