#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    if(!(cin>>n>>m)) return 0;
    vector<vector<int>> adj(n+1);
    vector<pair<int,int>> edges;
    edges.reserve(m);
    for(int i=0;i<m;++i){
        int x,y;cin>>x>>y;
        adj[x].push_back(y);
        adj[y].push_back(x);
        edges.emplace_back(x,y);
    }
    vector<int> disc(n+1,0), low(n+1,0);
    int timer=0;
    vector<pair<int,int>> st; // edge stack
    vector<char> good(n+1,0);
    function<void(int,int)> dfs = [&](int u, int parent){
        disc[u]=low[u]=++timer;
        for(int v: adj[u]){
            if(v==parent) continue; // ignore the edge to parent (but handle multiple edges?)
            if(!disc[v]){
                st.emplace_back(u,v);
                dfs(v,u);
                low[u]=min(low[u], low[v]);
                if(low[v]>=disc[u]){
                    // pop component
                    vector<pair<int,int>> compEdges;
                    while(!st.empty()){
                        auto e = st.back(); st.pop_back();
                        compEdges.push_back(e);
                        if(e.first==u && e.second==v) break;
                    }
                    // build adjacency for component
                    unordered_map<int, vector<int>> compAdj;
                    for(auto &e: compEdges){
                        compAdj[e.first].push_back(e.second);
                        compAdj[e.second].push_back(e.first);
                    }
                    // bipartite check
                    unordered_map<int,int> color;
                    bool isBip=true;
                    for(auto &kv: compAdj){
                        int start = kv.first;
                        if(color.find(start)!=color.end()) continue;
                        queue<int> q; q.push(start); color[start]=0;
                        while(!q.empty() && isBip){
                            int cur=q.front(); q.pop();
                            for(int nb: compAdj[cur]){
                                if(!color.count(nb)){
                                    color[nb]=color[cur]^1;
                                    q.push(nb);
                                }else if(color[nb]==color[cur]){
                                    isBip=false; break;
                                }
                            }
                        }
                        if(!isBip) break;
                    }
                    if(!isBip){
                        for(auto &kv: compAdj) good[kv.first]=1;
                    }
                }
            }else if(disc[v] < disc[u]){ // back edge
                st.emplace_back(u,v);
                low[u]=min(low[u], disc[v]);
            }
        }
    };
    for(int i=1;i<=n;++i){
        if(!disc[i]) dfs(i,-1);
    }
    long long ans=0;
    for(int i=1;i<=n;++i) if(!good[i]) ++ans;
    cout<<ans<<"\n";
    return 0;
}
