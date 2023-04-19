//
//  main.cpp
//  multiPathSelect
//
//  Created by xiaoR on 2023/4/14.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// 阐述：这个代码应用于矩形区域的遍历
// 无法适用于任何类型的图，并且假设R0的坐标为（0，0）；

// 存储节点与其相邻边是否繁忙
struct Node{
    bool b_nodeBusy;
//    从上方的边开始，顺时针存储
    vector<bool> b_edgeBusy;
    Node() {
        b_nodeBusy = false;
        for (int i = 0; i < 4; ++i) {
            b_edgeBusy.emplace_back(false);
        }
    }
};


// 定义图类：记录边的空闲状态
class Graph{
private:
    int m_row, m_col;
//    为了实现方便，边暂时选用邻接矩阵进行存储
    vector<vector<Node>> m_linkState;
public:
//    地图默认初始化为4 * 4
    Graph(int i_row = 4, int i_col = 4) {
        m_row = i_row;
        m_col = i_col;
        for (int i = 0; i < m_row; ++i) {
            vector<Node> row_state(m_col, Node());
            m_linkState.emplace_back(row_state);
        }
    }
    void f_showGraph() {
        for (int i = 0; i < m_row; ++i) {
            for (int j = 0; j < m_col; ++j) {
                Node curr_node = m_linkState[i][j];
                cout<<curr_node.b_nodeBusy<<" ";
                for (int k = 0; k < 4; ++k) {
                    cout<<curr_node.b_edgeBusy[k]<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
    }
//    设置节点状态
    void f_setLinkState(const pair<int, int> & pos, bool state) {
        m_linkState[pos.first][pos.second].b_nodeBusy = state;
    }
//    返回图的边节点信息
    vector<vector<Node>> & f_getLinkState() {
        return m_linkState;
    }
//    获取地图尺寸
    vector<int> f_getMapSizeVec() {
        return {m_row, m_col};
    }
    
//    判断两个点的位置关系
    int f_posDir(pair<int, int> curr_pos, pair<int, int> next_pos) {
//        next_pos在curr_pos上方
        if (next_pos.first ==  curr_pos.first - 1) {
            return 0;
        }
        //        next_pos在curr_pos右方
        if (next_pos.second == curr_pos.second + 1) {
            return 1;
        }
        //        next_pos在curr_pos下方
        if (next_pos.first == curr_pos.first + 1) {
            return 2;
        }
        //        next_pos在curr_pos左方
        if (next_pos.second == curr_pos.second - 1) {
            return 3;
        }
        return 0;
    }
    
//    判断路径是否空闲
    bool f_pathBusy(vector<pair<int, int>> & pathVec) {
//        打印路径
        cout<<"f_pathBusy pathVec"<<endl;
        for(int i = 0; i < pathVec.size(); ++i) {
            cout<<pathVec[i].first<<" "<<pathVec[i].second<<endl;
        }
        cout<<endl;
//        根据之前路径寻找算法，pathVec不可能为空
        pair<int, int> curr_pos = pathVec[0];
//        判断当前位置是否busy
//        但不用判断起点和终点，因为最开始就把起点和终点设置为busy了
//        if (m_linkState[curr_pos.first][curr_pos.second].b_nodeBusy) {
//            return true;
//        }
        for (int i = 1; i < pathVec.size(); ++i) {
            pair<int, int> next_pos = pathVec[i];
//              i_dirNum表示next_pos在curr_pos的方位
            int i_dirNum = f_posDir(curr_pos, next_pos);
            cout<<"i_dirNum : "<<i_dirNum<<endl;
//            判断边是否为busy
            if (m_linkState[curr_pos.first][curr_pos.second].b_edgeBusy[i_dirNum]) {
                cout<<"m_linkState[curr_pos.first][curr_pos.second].b_edgeBusy[i_dirNum] busy"<<endl;
                return true;
            }
//            (i_dirNum + 2) % 4表示curr_pos在next_pos的方位
            if (m_linkState[next_pos.first][next_pos.second].b_edgeBusy[(i_dirNum + 2) % 4]) {
                cout<<"m_linkState[next_pos.first][next_pos.second].b_edgeBusy[(i_dirNum + 2) % 4]  busy"<<endl;
                return true;
            }
//            判断next_pos是否busy，但不判断终点：因为pathVec[pathVec.size() -  1]表示终点
//            注意这里i < pathVec.size() - 1，而不是i < pathVec.size()，因为不应该判断终点
            if (i < pathVec.size() - 1 && m_linkState[next_pos.first][next_pos.second].b_nodeBusy) {
                cout<<"m_linkState[next_pos.first][next_pos.second].b_nodeBusy"<<endl;
                return true;
            }
        }
        return false;
    }
    
//    设置路径为busy
    void f_setPathBusy(vector<pair<int, int>> & pathVec) {
        //        根据之前路径寻找算法，pathVec不可能为空
        pair<int, int> curr_pos = pathVec[0];
//        与f_pathBusy同理，不用设置起点和终点为busy，因为最初已经设置过
        for (int i = 1; i < pathVec.size(); ++i) {
            pair<int, int> next_pos = pathVec[i];
//              i_dirNum表示next_pos在curr_pos的方位
            int i_dirNum = f_posDir(curr_pos, next_pos);
//            将边设置为busy
            m_linkState[curr_pos.first][curr_pos.second].b_edgeBusy[i_dirNum] = true;
//            (i_dirNum + 2) % 4表示curr_pos在next_pos的方位
            m_linkState[next_pos.first][next_pos.second].b_edgeBusy[(i_dirNum + 2) % 4] = true;
            if (i < pathVec.size() -  1) {
                m_linkState[next_pos.first][next_pos.second].b_nodeBusy = true;
            }
        }
        return ;
    }
};


//定义任务类:记录任务，以及寻找路径
class Task{
private:
    //    pair存储起点和终点的位置信息，注意此处是以行号、列号的顺序进行存储，不是x和y
    pair<int, int> m_src;
    pair<int, int> m_des;
//  m_pathSetVec : 所有路径
    vector<vector<pair<int, int>>> m_pathSetVec;
//  m_IpathSetVec : I型路径
    vector<vector<pair<int, int>>> m_IpathSetVec;
//  m_colFirstPathSetVec : 优先进行竖直方向遍历的路径
    vector<vector<pair<int, int>>> m_colFirstPathSetVec;
//  m_rowFirstPathSetVec : 优先进行水平方向遍历的路径
    vector<vector<pair<int, int>>> m_rowFirstPathSetVec;
//    建立一个路径地图，存储这个任务的路径集合占用的边的情况，方便判断两个任务的路径集合是否有交集
    vector<vector<Node>> m_routeMap;
public:
    Task(const pair<int, int> & src, const pair<int, int> & des, vector<int> mapSizeVec = {4, 4}) {
        //        构造函数内确定起点和终点位置
//        cin >> m_src.first >> m_src.second;
//        cin >> m_des.first >> m_des.second;
        m_src = src;
        m_des = des;
//        初始化路径地图
        for (int i = 0; i < mapSizeVec[0]; ++i) {
            vector<Node> rowState(mapSizeVec[1], Node());
            m_routeMap.emplace_back(rowState);
        }
    }
    
    //    每一条路径为一个vector<pair<int, int>>
    vector<vector<pair<int, int>>> f_findRoute(vector<vector<Node>> m_linkState) {
        //         第一类：首先判断I型路径
        //        1、起点与终点位于同一行，遍历行
        if(m_src.first == m_des.first) {
            vector<pair<int, int>> pathVec = f_rowTraverse(m_src, m_des, m_linkState);
//            列遍历结果不为空，可以加入路径集合
            if (!pathVec.empty()) {
                m_pathSetVec.emplace_back(pathVec);
                m_IpathSetVec.emplace_back(pathVec);
//                遍历路径，修改m_routeMap
                f_modifyRouteMap(pathVec);
            }
        }
//        2、起点与终点位于同一列，遍历列
        if (m_src.second == m_des.second) {
            vector<pair<int, int>> pathVec = f_colTraverse(m_src, m_des, m_linkState);
//            行遍历结果不为空，可以加入路径集合
            if (!pathVec.empty()) {
                m_pathSetVec.emplace_back(pathVec);
                m_IpathSetVec.emplace_back(pathVec);
    //                遍历路径，修改m_routeMap
                f_modifyRouteMap(pathVec);
            }
        }
//        第二类：判断L/L7形路径 ： 其实就是除了I型路径之外的优先遍历竖直方向的路径（Y轴/遍历列）:结果也许包括多个路径
        vector<vector<pair<int, int>>> colFirstPathSet = f_colFirstTraverse(m_src, m_des, m_linkState);
        if (!colFirstPathSet.empty()) {
            m_pathSetVec.insert(m_pathSetVec.end(), colFirstPathSet.begin(), colFirstPathSet.end());
            m_colFirstPathSetVec = colFirstPathSet;
//            遍历路径，修改m_routeMap;
            for (auto pathVec : m_colFirstPathSetVec) {
                f_modifyRouteMap(pathVec);
            }
        }
//        第三类：优先遍历水平方向的路径（X轴/遍历行），结果也许包括多个路径
        vector<vector<pair<int, int>>> rowFirstPathSet = f_rowFirstTraverse(m_src, m_des, m_linkState);
        if (!rowFirstPathSet.empty()) {
            m_pathSetVec.insert(m_pathSetVec.end(), rowFirstPathSet.begin(), rowFirstPathSet.end());
            m_rowFirstPathSetVec = rowFirstPathSet;
//            遍历路径，修改m_routeMap;
            for (auto pathVec : m_rowFirstPathSetVec) {
                f_modifyRouteMap(pathVec);
            }
        }
        return m_pathSetVec;
    }
    
//    沿着列进行遍历
    vector<pair<int, int>> f_rowTraverse(pair<int, int> src, pair<int, int> des, vector<vector<Node>> m_linkState) {
//        src在des同row左边，具体见结构体struct Node
//          i_dirFlag保存移动方向，i_lenFlag保存移动的距离，负数表示反方向移动
        int i_dirFlag = 1;
        int i_lenFlag = 1;
        if (src.second < des.second) {
            i_dirFlag = 1;
            i_lenFlag = 1;
        } else {
            i_dirFlag = 3;
            i_lenFlag = -1;
        }
        vector<pair<int, int>> path = {src};
//        路径上边和点均不被占有，这条路径才有效
        while (src != des) {
//            边被占用，返回空路径
            if (m_linkState[src.first][src.second].b_edgeBusy[i_dirFlag] == true) {
                return {};
            }
//            src移动到下一个点
            src.second += i_lenFlag;
//            当前src不为des,且点被占用，返回空路径
            if (m_linkState[src.first][src.second].b_nodeBusy == true) {
                if (src != des) {
                    return  {};
                }
            }
//            加入当前src
            path.emplace_back(src);
        }
        return path;
    }
    
//    沿着行遍历与沿着列遍历类似
    vector<pair<int, int>> f_colTraverse(pair<int, int> src, pair<int, int> des, vector<vector<Node>> m_linkState) {
//        src在des同col上边，具体见结构体struct Node
//          i_dirFlag保存移动方向，i_lenFlag保存移动的距离，负数表示反方向移动
        int i_dirFlag = 2;
        int i_lenFlag = 1;
        if (src.first < des.first) {
            i_dirFlag = 2;
            i_lenFlag = 1;
        } else {
            i_dirFlag = 0;
            i_lenFlag = -1;
        }
        vector<pair<int, int>> path = {src};
//        路径上边和点均不被占有，这条路径才有效
        while (src != des) {
//            边被占用，返回空路径
            if (m_linkState[src.first][src.second].b_edgeBusy[i_dirFlag] == true) {
                return {};
            }
//            src移动到下一个点
            src.first += i_lenFlag;
//            当前src不为des,且点被占用，返回空路径
            if (m_linkState[src.first][src.second].b_nodeBusy == true) {
                if (src != des) {
                    return  {};
                }
            }
//            加入当前src
            path.emplace_back(src);
        }
        return path;
    }
    
//      判断L/L7形路径 ： 优先遍历竖直方向的路径（Y轴/遍历列），结果也许包括多个路径
    vector<vector<pair<int, int>>> f_colFirstTraverse(pair<int, int> src, pair<int, int> des, vector<vector<Node>> m_linkState) {
//        设置遍历区域的border
        unordered_map<string, int> borders = {
            {"up", min(src.first, des.first)},
            {"bottom", max(src.first, des.first)},
            {"left", min(src.second, des.second)},
            {"right", max(src.second, des.second)},
        };
//          打印输出
//        cout<<"borders : "<<endl;
//        for (auto  it : borders) {
//            cout<<it.first<<" "<<it.second<<endl;
//        }
        //        把src的位置设置为visited，然后让src沿着列移动一个位置
        vector<vector<bool>> b_visited(m_linkState.size(), vector<bool>(m_linkState[0].size(), false));
        b_visited[src.first][src.second] = true;
        //        确定沿着列移动方向，
        int i_colDirFlag = 2, i_rowDirFlag = 1;
        int i_colLenFlag = 1, i_rowLenFlag = 1;
        //        确定des在src的方位
        if (src.first < des.first) {
            i_colDirFlag = 2;
            i_colLenFlag = 1;
        } else {
            i_colDirFlag = 0;
            i_colLenFlag = -1;
        }
        if (src.second < des.second) {
            i_rowDirFlag = 1;
            i_rowLenFlag = 1;
        } else {
            i_rowDirFlag = 3;
            i_rowLenFlag = -1;
        }
//        打印输出
//        cout<<"i_colDirFlag : "<<i_colDirFlag<<" i_rowDirFlag : "<<i_rowDirFlag<<endl;
//        cout<<"i_colLenFlag : "<<i_colLenFlag<<" i_rowLenFlag: "<<i_rowLenFlag<<endl;
//        所有路径都从src开始出发，存储当前遍历得到的路径
        vector<pair<int, int>> curr_path = {src};
//        确定竖直方向的下一个节点是否busy，以及这一段路径是否busy。
//        第一个节点或者路径就繁忙，则不可能寻找到路径，直接放回空
        if (m_linkState[src.first][src.second].b_edgeBusy[i_colDirFlag]) {
            return {};
        }
//        此处决定了其至少优先沿着竖直方向移动一个位置
        src.first += i_colLenFlag;
        if (m_linkState[src.first][src.second].b_nodeBusy) {
            return {};
        }
//          从移动后的src开始进行DFS，直至des
        //        ans存储除了I型路径之外，优先遍历列的路径集合
        vector<vector<pair<int, int>>> ans;
        DFS(src, des, m_linkState, b_visited, borders, curr_path, ans);
//        打印ans路径集合
        cout<<"f_colFirstTraverse ans : "<<endl;
        for (int i = 0; i < ans.size(); ++i) {
            for (int j = 0; j < ans[i].size(); ++j) {
                cout<<ans[i][j].first<<" "<<ans[i][j].second<<endl;
            }
            cout<<endl;
        }
        return ans;
    }
    
//    第三类：优先遍历水平方向的路径（X轴/遍历行），结果也许包括多个路径
    vector<vector<pair<int, int>>> f_rowFirstTraverse(pair<int, int> src, pair<int, int> des, vector<vector<Node>> m_linkState) {
//        设置遍历区域的border
        unordered_map<string, int> borders = {
            {"up", min(src.first, des.first)},
            {"bottom", max(src.first, des.first)},
            {"left", min(src.second, des.second)},
            {"right", max(src.second, des.second)},
        };
//          打印输出
        cout<<"borders : "<<endl;
        for (auto  it : borders) {
            cout<<it.first<<" "<<it.second<<endl;
        }
        //        把src的位置设置为visited，然后让src沿着列移动一个位置
        vector<vector<bool>> b_visited(m_linkState.size(), vector<bool>(m_linkState[0].size(), false));
        b_visited[src.first][src.second] = true;
        //        确定沿着行移动方向，
        int i_colDirFlag = 2, i_rowDirFlag = 1;
        int i_colLenFlag = 1, i_rowLenFlag = 1;
        //        确定des在src的方位
        if (src.first < des.first) {
            i_colDirFlag = 2;
            i_colLenFlag = 1;
        } else {
            i_colDirFlag = 0;
            i_colLenFlag = -1;
        }
        if (src.second < des.second) {
            i_rowDirFlag = 1;
            i_rowLenFlag = 1;
        } else {
            i_rowDirFlag = 3;
            i_rowLenFlag = -1;
        }
//        打印输出
        cout<<"i_colDirFlag : "<<i_colDirFlag<<" i_rowDirFlag : "<<i_rowDirFlag<<endl;
        cout<<"i_colLenFlag : "<<i_colLenFlag<<" i_rowLenFlag: "<<i_rowLenFlag<<endl;
//        所有路径都从src开始出发，存储当前遍历得到的路径
        vector<pair<int, int>> curr_path = {src};
//        确定水平方向的下一个节点是否busy，以及这一段路径是否busy。
//        第一个节点或者路径就繁忙，则不可能寻找到路径，直接放回空
        if (m_linkState[src.first][src.second].b_edgeBusy[i_rowDirFlag]) {
            return {};
        }
//        此处确定了优先沿着水平方向至少遍历1个位置
        src.second += i_rowLenFlag;
        if (m_linkState[src.first][src.second].b_nodeBusy) {
            return {};
        }
//          从移动后的src开始进行DFS，直至des
        //        ans存储除了I型路径之外，优先遍历列的路径集合
        vector<vector<pair<int, int>>> ans;
        DFS(src, des, m_linkState, b_visited, borders, curr_path, ans);
//        打印ans路径集合
        cout<<"f_rowFirstTraverse ans : "<<endl;
        for (int i = 0; i < ans.size(); ++i) {
            for (int j = 0; j < ans[i].size(); ++j) {
                cout<<ans[i][j].first<<" "<<ans[i][j].second<<endl;
            }
            cout<<endl;
        }
        return ans;
    }
    
//    DFS寻找路径,ans可能为空，但ans内部每条路径不可能为空
    void DFS(pair<int, int> src, pair<int, int> des, const vector<vector<Node>> & m_linkState, vector<vector<bool>> & b_visited, unordered_map<string, int> & borders, vector<pair<int, int>> curr_path, vector<vector<pair<int, int>>> & ans) {
//        输出src，des
        cout<<"src : "<<src.first<<" "<<src.second<<" des : "<<des.first <<" "<<des.second<<endl;
//        边界条件，src到des
//        注意⚠️：虽然此处的边界判定在这个project中无用，但还是留在这里
//        if (src == des) {
//            curr_path.emplace_back(src);
//            ans.emplace_back(curr_path);
//            return ;
//        }
//        上右下左的顺序遍历
        vector<vector<int>> dirsVec = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
        //            当前位置加入curr_path;
        curr_path.emplace_back(src);
//        当前src设置为visited
        b_visited[src.first][src.second] = true;
//        注意⚠️：这里是允许绕路的方式，即可以走4个方向
//        可以改为不允许绕路的方式，只允许按着i_colDirFlag,i_rowDirFlag方向走
        for (int i = 0; i < 4; ++i) {
            pair<int, int> next_pos = make_pair(src.first + dirsVec[i][0], src.second + dirsVec[i][1]);
//            判断next_pos是否越界
            if (next_pos.first < borders["up"] || next_pos.first > borders["bottom"] || next_pos.second < borders["left"] || next_pos.second > borders["right"]) {
                continue;
            }
            cout<<"next_pos : "<<next_pos.first<<" "<<next_pos.second<<endl;
//            如果新位置visited过，不允许访问
            if (b_visited[next_pos.first][next_pos.second]) {
                continue;
            }
//            如果路径busy不允许往此方向移动
            if (m_linkState[src.first][src.second].b_edgeBusy[i]) {
                continue;
            }
//            由于最初设置了起点和终点的b_nodeBusy为true，所以需要在此判断next_pos是否等于des,相等则找到路径，加入结果集
            if (next_pos == des) {
                curr_path.emplace_back(next_pos);
                ans.emplace_back(curr_path);
                continue;
            }
//            如果节点busy，不允许访问next_pos
            if (m_linkState[next_pos.first][next_pos.second].b_nodeBusy) {
                continue;
            }
//            允许往新位置走，next_pos设置为访问过
            b_visited[next_pos.first][next_pos.second] = true;
            DFS(next_pos, des, m_linkState, b_visited, borders, curr_path, ans);
            //            next_pos恢复访问设置
            b_visited[next_pos.first][next_pos.second] = false;
        }
//        恢复当前位置为未被访问
        b_visited[src.first][src.second] = false;
        return ;
    }
    
    void f_showTasks() {
        cout<<"m_src : ("<<m_src.first<<", "<<m_src.second<<")"<<endl;
        cout<<"m_des : ("<<m_des.first<<", "<<m_des.second<<")"<<endl;
        cout<<endl;
    }
    
//    输出路径集合
    void f_showPathSet() {
        for (int i = 0; i < m_pathSetVec.size(); ++i) {
            cout<<"path "<<i + 1<<endl;
            for (int j = 0; j < m_pathSetVec[i].size(); ++j) {
                cout<<m_pathSetVec[i][j].first<<" "<<m_pathSetVec[i][j].second<<endl;
            }
            cout<<endl;
        }
    }

//    获取任务起点
    pair<int, int> f_getSrc() {
        return m_src;
    }
//    获取任务终点
    pair<int, int> f_getDes() {
        return m_des;
    }
    
//    获取m_pathSetVec
    vector<vector<pair<int, int>>> f_getPathSet() {
        return m_pathSetVec;
    }
    
//    判断两个点的位置关系
    int f_posDir(pair<int, int> curr_pos, pair<int, int> next_pos) {
//        next_pos在curr_pos上方
        if (next_pos.first ==  curr_pos.first - 1) {
            return 0;
        }
        //        next_pos在curr_pos右方
        if (next_pos.second == curr_pos.second + 1) {
            return 1;
        }
        //        next_pos在curr_pos下方
        if (next_pos.first == curr_pos.first + 1) {
            return 2;
        }
        //        next_pos在curr_pos左方
        if (next_pos.second == curr_pos.second - 1) {
            return 3;
        }
        return 0;
    }

//    根据pathVec修改m_routeMap
    void f_modifyRouteMap(vector<pair<int, int>> pathVec) {
    //                遍历路径，修改m_routeMap
        pair<int, int> curr_pos = pathVec[0];
        for (int i = 1; i < pathVec.size(); ++i) {
            pair<int, int> next_pos = pathVec[i];
    //                    判断next_pos在curr_pos的方位；
            int i_dirNum = f_posDir(curr_pos, next_pos);
    //                    将对应的边设置为被占用
            m_routeMap[curr_pos.first][curr_pos.second].b_edgeBusy[i_dirNum] = true;
            m_routeMap[next_pos.first][next_pos.second].b_edgeBusy[(i_dirNum + 2) % 4] = true;
    //                    顺带将节点设置为busy
            m_routeMap[curr_pos.first][curr_pos.second].b_nodeBusy = true;
            m_routeMap[next_pos.first][next_pos.second].b_nodeBusy = true;
    //                    位置移动
            curr_pos = next_pos;
        }
    }
    
//    路径相似判断
    bool pathSimilarity(Task & task) {
//        比对两个任务的m_routeMap是否有相同的路径段为busy
        for (int i = 0; i < m_routeMap.size(); ++i) {
            for (int j = 0; j < m_routeMap[i].size(); ++j) {
//                遍历b_edgeBusy
                for (int k = 0; k < 4; ++k) {
                    if (m_routeMap[i][j].b_edgeBusy[k] == true && task.m_routeMap[i][j].b_edgeBusy[k] == true) {
                        return true;
                    }
                }
            }
        }
        return false;
    }
    
//    获取I型路径集合
    vector<vector<pair<int, int>>> f_getIpathSet() {
        return m_IpathSetVec;
    }
    
//    获取优先遍历Y轴的路径集合
    vector<vector<pair<int, int>>> f_getColFirstPath() {
        return m_colFirstPathSetVec;
    }
    
//    获取优先遍历X轴的路径集合
    vector<vector<pair<int, int>>> f_getRowFirstPath() {
        return m_rowFirstPathSetVec;
    }
};


bool g_routeNumCompare(Task & task1, Task & task2) {
    return task1.f_getPathSet().size() < task2.f_getPathSet().size();
}



int main(int argc, const char * argv[]) {
//    第一阶段
//    初始化地图
//    地图默认为4 * 4
    int i_row = 4, i_col = 4;
    cout << "Please input the size of map. input format : map.row map.col"<<endl;
    cin >> i_row >> i_col;
    Graph graph_obj(i_row, i_col);
////    显示地图
//    graph_obj.f_showGraph();
//    确定任务
    int i_taskNum = 3;
    cout<<"Please input the Number of tasks. input format : task.num"<<endl;
    cin >> i_taskNum;
    vector<Task> taskListVec;
    while (i_taskNum-- > 0) {
        pair<int, int> src, des;
        cout<<"please input source. input format : source.row source.col"<<endl;
        cin >> src.first >> src.second;
        cout<<"please input destination. input format : destination.row  destination.col"<<endl;
        cin >> des.first >> des.second;
        Task task(src, des, graph_obj.f_getMapSizeVec());
        //        标记起点和终点为繁忙状态
        graph_obj.f_setLinkState(src, true);
        graph_obj.f_setLinkState(des, true);
//         寻找路径
        task.f_findRoute(graph_obj.f_getLinkState());
//        测试：打印路径
        task.f_showPathSet();
        taskListVec.emplace_back(task);
    }
//    显示任务
    for(int i = 0; i < taskListVec.size(); ++i) {
        cout<<"task"<<i + 1<<": "<<endl;
        taskListVec[i].f_showTasks();
    }
//    显示地图
    graph_obj.f_showGraph();
//  finalPathSetVec存储全任务最终的路径集合，在程序末尾进行打印输出
    vector<vector<pair<int, int>>> finalPathSetVec;
//  第二阶段
//    选取一个路径集合D1，确定与其有交集的路径集合的集合
    int i_taskSetNum = 0;
    while (!taskListVec.empty()) {
        ++i_taskSetNum;
        Task selectTask = taskListVec[0];
        taskListVec.erase(taskListVec.begin());
        vector<Task> taskSetVec = {selectTask};
//        寻找与selectTask有交集的路径集合
        for (int i = 0; i < taskListVec.size(); ++i) {
            if (selectTask.pathSimilarity(taskListVec[i])) {
//                将路径集合移到当前任务集合（taskSetVec）中
                taskSetVec.emplace_back(taskListVec[i]);
                taskListVec.erase(taskListVec.begin() + i);
                --i;
            }
        }
//        在路径集合的集合内，进行路径选择
//        打印当前任务集合
        cout<<"第"<<i_taskSetNum<<"个任务集合, "<<"taskSetVec.size() : "<<taskSetVec.size()<<endl;
        for (int i = 0; i < taskSetVec.size(); ++i) {
            taskSetVec[i].f_showTasks();
        }
//        第三阶段：根据当前任务集合，确定路径，修改graph_obj.m_linkState
//        根据当前任务集合中的各任务路径数量从小到大排序
        sort(taskSetVec.begin(), taskSetVec.end(), g_routeNumCompare);
//        根据排序结果，确定最终路径，然后修改graph_obj.m_linkState
        for (int i = 0; i < taskSetVec.size(); ++i) {
//            根据I型路径、优先遍历Y、优先遍历X的顺序选择路径
            vector<vector<pair<int, int>>> IpathSetVec = taskSetVec[i].f_getIpathSet();
//            cout<<"IpathSetVec.size() : "<<IpathSetVec.size()<<endl;
//            找到一条路径，就退出
            bool findPath = false;
            if (!IpathSetVec.empty()) {
                for (auto pathVec : IpathSetVec) {
//                    判断路径中所有点和线段是否都空闲
                    if (!graph_obj.f_pathBusy(pathVec)) {
//                        找到路径，将路径加入到finalPathSetVec
//                        但此处在路径前面加入任务起点和终点，方便后续打印输出，明确任务信息
                        pathVec.insert(pathVec.begin(), taskSetVec[i].f_getDes());
                        pathVec.insert(pathVec.begin(), taskSetVec[i].f_getSrc());
                        finalPathSetVec.emplace_back(pathVec);
//                        cout<<"finalPathSetVec.size() : "<<finalPathSetVec.size()<<endl;
//                        设置当前路径的点和边为busy
                        graph_obj.f_setPathBusy(pathVec);
//                        找到当前任务的合适路径，前往下一任务
                        findPath = true;
                        break;
                    }
                }
            }
            //            找到一条路径,前往下一任务
            if (findPath) {
                continue;
            }
            vector<vector<pair<int, int>>> colPathSetVec = taskSetVec[i].f_getColFirstPath();
//            cout<<"colPathSetVec.size() : "<<colPathSetVec.size()<<endl;
            if (!colPathSetVec.empty()) {
                for (auto pathVec : colPathSetVec) {
//                    判断路径中所有点和线段是否都空闲
                    if (!graph_obj.f_pathBusy(pathVec)) {
//                        找到路径，将路径加入到finalPathSetVec
//                        但此处在路径前面加入任务起点和终点，方便后续打印输出，明确任务信息
                        pathVec.insert(pathVec.begin(), taskSetVec[i].f_getDes());
                        pathVec.insert(pathVec.begin(), taskSetVec[i].f_getSrc());
                        finalPathSetVec.emplace_back(pathVec);
//                        cout<<"finalPathSetVec.size() : "<<finalPathSetVec.size()<<endl;
//                        设置当前路径的点和边为busy
                        graph_obj.f_setPathBusy(pathVec);
//                        找到当前任务的合适路径，前往下一任务
                        findPath = true;
                        break;
                    }
                }
            }
            //            找到一条路径,前往下一任务
            if (findPath) {
                continue;
            }
            vector<vector<pair<int, int>>> rowPathSetVec = taskSetVec[i].f_getRowFirstPath();
//            cout<<"rowPathSetVec.size() : "<<rowPathSetVec.size()<<endl;
            if (!rowPathSetVec.empty()) {
                for (auto pathVec : rowPathSetVec) {
//                    判断路径中所有点和线段是否都空闲
                    if (!graph_obj.f_pathBusy(pathVec)) {
//                        找到路径，将路径加入到finalPathSetVec
//                        但此处在路径前面加入任务起点和终点，方便后续打印输出，明确任务信息
                        pathVec.insert(pathVec.begin(), taskSetVec[i].f_getDes());
                        pathVec.insert(pathVec.begin(), taskSetVec[i].f_getSrc());
                        finalPathSetVec.emplace_back(pathVec);
//                        cout<<"finalPathSetVec.size() : "<<finalPathSetVec.size()<<endl;
//                        设置当前路径的点和边为busy
                        graph_obj.f_setPathBusy(pathVec);
//                        找到当前任务的合适路径，前往下一任务
                        findPath = true;
                        break;
                    }
                }
            }
            //            找到一条路径,前往下一任务
            if (findPath) {
                continue;
            }
//            上述路径都没有合理的，则加入空路径
            vector<pair<int, int>> pathVec;
//          但此处在路径前面加入任务起点和终点，方便后续打印输出，明确任务信息
            pathVec.insert(pathVec.begin(), taskSetVec[i].f_getDes());
            pathVec.insert(pathVec.begin(), taskSetVec[i].f_getSrc());
            finalPathSetVec.emplace_back(pathVec);
        }
//        处理完当前任务集合后，m_linkState状态已经修改过，所以需要对后续任务重新寻找pathSetVec
        for (auto task : taskListVec) {
            task.f_findRoute(graph_obj.f_getLinkState());
        }
//        清空当前任务集合
        taskSetVec.clear();
    }
    
//    show finalPathSetVec
    for (int i = 0; i < finalPathSetVec.size(); ++i) {
//        pathVec前两个节点表示起点和终点
        vector<pair<int, int>> pathVec = finalPathSetVec[i];
//        判断路径是否为空
//        show task_id, src, des
        cout<<"task "<<i + 1<<" src : ("<<pathVec[0].first<<" , "<<pathVec[0].second<<")  des : ("<<pathVec[1].first<<" , "<<pathVec[1].second<<") "<<endl;
//      show path
//        此时没有可选路径
        if (pathVec.size() <= 2) {
            cout<<"No path."<<endl;
            continue;
        } else {
            cout<<"Path."<<endl;
        }
        for (int j = 2; j < pathVec.size(); ++j) {
            cout << pathVec[j].first << " " << pathVec[j].second << endl;
        }
        cout << endl;
    }
    return 0;
}
