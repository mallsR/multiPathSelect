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
//    重置节点信息
    void f_reset() {
        b_nodeBusy = false;
        for (int i = 0; i < 4; ++i) {
            b_edgeBusy[i] = false;
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
//    展示节点以及其四周的边是否busy，1表示busy,0表示free
    void f_showGraph() {
        cout<<"整个图的节点及其四周边的繁忙情况."<<endl;
        for (int i = 0; i < m_row; ++i) {
            cout<<"第"<<i + 1<<"行节点的信息."<<endl;
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
    void f_setNodeState(const pair<int, int> & pos, bool state) {
        m_linkState[pos.first][pos.second].b_nodeBusy = state;
    }
//    设置链路状态
    void f_setLinkState(const pair<int, int> & pos, int dir, bool state) {
//        此处忘记传入链路状态 state
        m_linkState[pos.first][pos.second].b_edgeBusy[dir] = state;
    }
//    返回图的边节点信息
    vector<vector<Node>> f_getLinkState() {
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
//        cout<<"f_pathBusy pathVec"<<endl;
//        for(int i = 0; i < pathVec.size(); ++i) {
//            cout<<pathVec[i].first<<" "<<pathVec[i].second<<endl;
//        }
//        cout<<endl;
//        根据之前路径寻找算法，pathVec不可能为空
        pair<int, int> curr_pos = pathVec[0];
//        判断当前位置是否busy
        if (m_linkState[curr_pos.first][curr_pos.second].b_nodeBusy) {
            return true;
        }
        for (int i = 1; i < pathVec.size(); ++i) {
            pair<int, int> next_pos = pathVec[i];
//              i_dirNum表示next_pos在curr_pos的方位
            int i_dirNum = f_posDir(curr_pos, next_pos);
//            cout<<"i_dirNum : "<<i_dirNum<<endl;
//            判断边是否为busy
            if (m_linkState[curr_pos.first][curr_pos.second].b_edgeBusy[i_dirNum]) {
//                cout<<"m_linkState[curr_pos.first][curr_pos.second].b_edgeBusy[i_dirNum] busy"<<endl;
                return true;
            }
//            (i_dirNum + 2) % 4表示curr_pos在next_pos的方位
            if (m_linkState[next_pos.first][next_pos.second].b_edgeBusy[(i_dirNum + 2) % 4]) {
//                cout<<"m_linkState[next_pos.first][next_pos.second].b_edgeBusy[(i_dirNum + 2) % 4]  busy"<<endl;
                return true;
            }
//            判断节点是否处于空闲状态
            if (m_linkState[next_pos.first][next_pos.second].b_nodeBusy) {
//                cout<<"m_linkState[next_pos.first][next_pos.second].b_nodeBusy"<<endl;
                return true;
            }
            curr_pos = next_pos;
        }
        return false;
    }
//    下列注释部分为：不考虑起点和终点的空闲/繁忙状态
//    bool f_pathBusy(vector<pair<int, int>> & pathVec) {
////        打印路径
////        cout<<"f_pathBusy pathVec"<<endl;
////        for(int i = 0; i < pathVec.size(); ++i) {
////            cout<<pathVec[i].first<<" "<<pathVec[i].second<<endl;
////        }
////        cout<<endl;
////        根据之前路径寻找算法，pathVec不可能为空
//        pair<int, int> curr_pos = pathVec[0];
////        判断当前位置是否busy
////        但不用判断起点和终点，因为最开始就把起点和终点设置为busy了
////        if (m_linkState[curr_pos.first][curr_pos.second].b_nodeBusy) {
////            return true;
////        }
//        for (int i = 1; i < pathVec.size(); ++i) {
//            pair<int, int> next_pos = pathVec[i];
////              i_dirNum表示next_pos在curr_pos的方位
//            int i_dirNum = f_posDir(curr_pos, next_pos);
////            cout<<"i_dirNum : "<<i_dirNum<<endl;
////            判断边是否为busy
//            if (m_linkState[curr_pos.first][curr_pos.second].b_edgeBusy[i_dirNum]) {
////                cout<<"m_linkState[curr_pos.first][curr_pos.second].b_edgeBusy[i_dirNum] busy"<<endl;
//                return true;
//            }
////            (i_dirNum + 2) % 4表示curr_pos在next_pos的方位
//            if (m_linkState[next_pos.first][next_pos.second].b_edgeBusy[(i_dirNum + 2) % 4]) {
////                cout<<"m_linkState[next_pos.first][next_pos.second].b_edgeBusy[(i_dirNum + 2) % 4]  busy"<<endl;
//                return true;
//            }
////            判断next_pos是否busy，但不判断终点：因为pathVec[pathVec.size() -  1]表示终点
////            注意这里i < pathVec.size() - 1，而不是i < pathVec.size()，因为不应该判断终点
//            if (i < pathVec.size() - 1 && m_linkState[next_pos.first][next_pos.second].b_nodeBusy) {
////                cout<<"m_linkState[next_pos.first][next_pos.second].b_nodeBusy"<<endl;
//                return true;
//            }
//        }
//        return false;
//    }
    
//    设置路径为busy
    void f_setPathBusy(vector<pair<int, int>> & pathVec) {
        //        根据之前路径寻找算法，pathVec不可能为空
        pair<int, int> curr_pos = pathVec[0];
//        设置起点为繁忙
        m_linkState[curr_pos.first][curr_pos.second].b_nodeBusy = true;
//        与f_pathBusy同理，不用设置起点和终点为busy，因为最初已经设置过
        for (int i = 1; i < pathVec.size(); ++i) {
            pair<int, int> next_pos = pathVec[i];
//              i_dirNum表示next_pos在curr_pos的方位
            int i_dirNum = f_posDir(curr_pos, next_pos);
//            cout<<"i_dirNum : "<<i_dirNum<<endl;
//            将边设置为busy
            m_linkState[curr_pos.first][curr_pos.second].b_edgeBusy[i_dirNum] = true;
//            (i_dirNum + 2) % 4表示curr_pos在next_pos的方位
            m_linkState[next_pos.first][next_pos.second].b_edgeBusy[(i_dirNum + 2) % 4] = true;
//            设置节点为繁忙
            m_linkState[next_pos.first][next_pos.second].b_nodeBusy = true;
//            位置移动，curr_pos 移动到 next_pos
            curr_pos = next_pos;
        }
        return ;
    }
//    下列注释部分表示：不考虑起点和终点是否处于空闲/繁忙状态
//    void f_setPathBusy(vector<pair<int, int>> & pathVec) {
//        //        根据之前路径寻找算法，pathVec不可能为空
//        pair<int, int> curr_pos = pathVec[0];
////        与f_pathBusy同理，不用设置起点和终点为busy，因为最初已经设置过
//        for (int i = 1; i < pathVec.size(); ++i) {
//            pair<int, int> next_pos = pathVec[i];
////              i_dirNum表示next_pos在curr_pos的方位
//            int i_dirNum = f_posDir(curr_pos, next_pos);
////            将边设置为busy
//            m_linkState[curr_pos.first][curr_pos.second].b_edgeBusy[i_dirNum] = true;
////            (i_dirNum + 2) % 4表示curr_pos在next_pos的方位
//            m_linkState[next_pos.first][next_pos.second].b_edgeBusy[(i_dirNum + 2) % 4] = true;
//            if (i < pathVec.size() -  1) {
//                m_linkState[next_pos.first][next_pos.second].b_nodeBusy = true;
//            }
//        }
//        return ;
//    }
    
//    清空地图
    void f_clear() {
        for (int i = 0; i < m_row; ++i) {
            for (int j = 0; j < m_col; ++j) {
                m_linkState[i][j].f_reset();
            }
        }
    }
};


//定义任务类:记录任务，以及寻找路径
class Task{
private:
    //    pair存储起点和终点的位置信息，注意此处是以行号、列号的顺序进行存储，不是x和y
    pair<int, int> m_src;
    pair<int, int> m_des;
//    增加任务到达时间、任务传输数据量和任务完成时间
    float m_fArriveTime;
    float m_fFinishTime;    // 用于记录后续次任务会在多久完成，默认初始化为0
    float m_fDataSize;
    float m_fDelay;
//    最终确定的路径
    vector<pair<int, int>> m_selectPath;
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
//    构造函数：提供默认值
    Task(const pair<int, int> & src = make_pair(0, 0), const pair<int, int> & des = make_pair(0, 0), float fArriveTime = 0, float fDataSize = 0, vector<int> mapSizeVec = {4, 4}, float fFinishTime = 0) {
        //        构造函数内确定起点和终点位置
//        cin >> m_src.first >> m_src.second;
//        cin >> m_des.first >> m_des.second;
        m_src = src;
        m_des = des;
        m_fArriveTime = fArriveTime;
        m_fDataSize = fDataSize;
        m_fFinishTime = fFinishTime;
//        初始化路径地图
        for (int i = 0; i < mapSizeVec[0]; ++i) {
            vector<Node> rowState(mapSizeVec[1], Node());
            m_routeMap.emplace_back(rowState);
        }
    }
    
//    拷贝构造函数
    Task(Task & task) {
        m_src = task.f_getSrc();
        m_des = task.f_getDes();
        m_fArriveTime = task.f_getArriveTime();
        m_fDataSize = task.f_getDataSize();
    }
    
    //    每一条路径为一个vector<pair<int, int>>
//    vector<vector<pair<int, int>>> f_findRoute(vector<vector<Node>> m_linkState) {
//        //         第一类：首先判断I型路径
//        //        1、起点与终点位于同一行，遍历行
//        if(m_src.first == m_des.first) {
//            vector<pair<int, int>> pathVec = f_rowTraverse(m_src, m_des, m_linkState);
////            列遍历结果不为空，可以加入路径集合
//            if (!pathVec.empty()) {
//                m_pathSetVec.emplace_back(pathVec);
//                m_IpathSetVec.emplace_back(pathVec);
////                遍历路径，修改m_routeMap
//                f_modifyRouteMap(pathVec);
//            }
//        }
////        2、起点与终点位于同一列，遍历列
//        if (m_src.second == m_des.second) {
//            vector<pair<int, int>> pathVec = f_colTraverse(m_src, m_des, m_linkState);
////            行遍历结果不为空，可以加入路径集合
//            if (!pathVec.empty()) {
//                m_pathSetVec.emplace_back(pathVec);
//                m_IpathSetVec.emplace_back(pathVec);
//    //                遍历路径，修改m_routeMap
//                f_modifyRouteMap(pathVec);
//            }
//        }
////        第二类：判断L/L7形路径 ： 其实就是除了I型路径之外的优先遍历竖直方向的路径（Y轴/遍历列）:结果也许包括多个路径
//        vector<vector<pair<int, int>>> colFirstPathSet = f_colFirstTraverse(m_src, m_des, m_linkState);
//        if (!colFirstPathSet.empty()) {
//            m_pathSetVec.insert(m_pathSetVec.end(), colFirstPathSet.begin(), colFirstPathSet.end());
//            m_colFirstPathSetVec = colFirstPathSet;
////            遍历路径，修改m_routeMap;
//            for (auto pathVec : m_colFirstPathSetVec) {
//                f_modifyRouteMap(pathVec);
//            }
//        }
////        第三类：优先遍历水平方向的路径（X轴/遍历行），结果也许包括多个路径
//        vector<vector<pair<int, int>>> rowFirstPathSet = f_rowFirstTraverse(m_src, m_des, m_linkState);
//        if (!rowFirstPathSet.empty()) {
//            m_pathSetVec.insert(m_pathSetVec.end(), rowFirstPathSet.begin(), rowFirstPathSet.end());
//            m_rowFirstPathSetVec = rowFirstPathSet;
////            遍历路径，修改m_routeMap;
//            for (auto pathVec : m_rowFirstPathSetVec) {
//                f_modifyRouteMap(pathVec);
//            }
//        }
//        return m_pathSetVec;
//    }
    vector<vector<pair<int, int>>> f_findRoute(Graph graph_obj) {
//        寻找路径之前，清空上次寻找路径是的链路状态记录
        for (int i = 0; i < m_routeMap.size(); ++i) {
            for (int j = 0; j < m_routeMap[0].size(); ++j) {
//                通过调用reset函数，将节点和边繁忙状态设置为空闲
                m_routeMap[i][j].f_reset();
            }
        }
        //         第一类：首先判断I型路径
        //        1、起点与终点位于同一行，遍历行
        if(m_src.first == m_des.first) {
            vector<pair<int, int>> pathVec = f_rowTraverse(m_src, m_des, graph_obj.f_getLinkState());
//            列遍历结果不为空，可以加入路径集合
            if (!pathVec.empty()) {
                m_pathSetVec.emplace_back(pathVec);
                m_IpathSetVec.emplace_back(pathVec);
//                遍历路径，修改m_routeMap，但要记得在再次寻找路径之前修改回来
                f_modifyRouteMap(pathVec);
            }
        }
//        2、起点与终点位于同一列，遍历列
        if (m_src.second == m_des.second) {
            vector<pair<int, int>> pathVec = f_colTraverse(m_src, m_des, graph_obj.f_getLinkState());
//            行遍历结果不为空，可以加入路径集合
            if (!pathVec.empty()) {
                m_pathSetVec.emplace_back(pathVec);
                m_IpathSetVec.emplace_back(pathVec);
    //                遍历路径，修改m_routeMap
                f_modifyRouteMap(pathVec);
            }
        }
//        第二类：判断L/L7形路径 ： 其实就是除了I型路径之外的优先遍历竖直方向的路径（Y轴/遍历列）:结果也许包括多个路径
        vector<vector<pair<int, int>>> colFirstPathSet = f_colFirstTraverse(m_src, m_des, graph_obj.f_getLinkState());
        if (!colFirstPathSet.empty()) {
            m_pathSetVec.insert(m_pathSetVec.end(), colFirstPathSet.begin(), colFirstPathSet.end());
            m_colFirstPathSetVec = colFirstPathSet;
//            遍历路径，修改m_routeMap;
            for (auto pathVec : m_colFirstPathSetVec) {
                f_modifyRouteMap(pathVec);
            }
        }
//        第三类：优先遍历水平方向的路径（X轴/遍历行），结果也许包括多个路径
        vector<vector<pair<int, int>>> rowFirstPathSet = f_rowFirstTraverse(m_src, m_des, graph_obj.f_getLinkState());
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
//        cout<<"f_colFirstTraverse ans : "<<endl;
//        for (int i = 0; i < ans.size(); ++i) {
//            for (int j = 0; j < ans[i].size(); ++j) {
//                cout<<ans[i][j].first<<" "<<ans[i][j].second<<endl;
//            }
//            cout<<endl;
//        }
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
//        cout<<"borders : "<<endl;
//        for (auto  it : borders) {
//            cout<<it.first<<" "<<it.second<<endl;
//        }
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
//        cout<<"i_colDirFlag : "<<i_colDirFlag<<" i_rowDirFlag : "<<i_rowDirFlag<<endl;
//        cout<<"i_colLenFlag : "<<i_colLenFlag<<" i_rowLenFlag: "<<i_rowLenFlag<<endl;
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
//        cout<<"f_rowFirstTraverse ans : "<<endl;
//        for (int i = 0; i < ans.size(); ++i) {
//            for (int j = 0; j < ans[i].size(); ++j) {
//                cout<<ans[i][j].first<<" "<<ans[i][j].second<<endl;
//            }
//            cout<<endl;
//        }
        return ans;
    }
    
//    DFS寻找路径,ans可能为空，但ans内部每条路径不可能为空
    void DFS(pair<int, int> src, pair<int, int> des, const vector<vector<Node>> & m_linkState, vector<vector<bool>> & b_visited, unordered_map<string, int> & borders, vector<pair<int, int>> curr_path, vector<vector<pair<int, int>>> & ans) {
//        输出src，des
//        cout<<"src : "<<src.first<<" "<<src.second<<" des : "<<des.first <<" "<<des.second<<endl;
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
//            cout<<"next_pos : "<<next_pos.first<<" "<<next_pos.second<<endl;
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
        cout<<"m_fArriveTime : "<< m_fArriveTime<< " m_fDataSize : "<<m_fDataSize<<endl;
    }
    
    void f_showFinishInfo() {
        cout<<"m_FinishedTime : "<<m_fFinishTime<<endl;
        cout<<"final path : "<<endl;
        for (int i = 0; i < m_selectPath.size(); ++i) {
            cout<<m_selectPath[i].first<<" "<<m_selectPath[i].second<<endl;
        }
    }
//    输出最终选择的路径
    void f_showSelectPath() {
        cout<<"selectPath : "<<endl;
        for (int i = 0; i < m_selectPath.size(); ++i) {
            cout<<m_selectPath[i].first<<" "<<m_selectPath[i].second<<endl;
        }
    }
    
//      展示任务的routeMap
    void f_showRouteMap() {
        for (int i = 0; i < m_routeMap.size(); ++i) {
            cout<<"第"<<i + 1<<"行节点信息!"<<endl;
            for (int j = 0; j < m_routeMap[0].size(); ++j) {
                cout<< m_routeMap[i][j].b_nodeBusy<<endl;
//                从上方开始，以顺时针方向输出边的信息
                for (int dir = 0; dir < 4; ++dir) {
                    cout<<m_routeMap[i][j].b_edgeBusy[dir]<<" ";
                }
                cout<<endl;
            }
        }
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
//    获取任务到达时间
    float f_getArriveTime() {
        return m_fArriveTime;
    }
//    获取任务传输数据量
    float f_getDataSize() {
        return m_fDataSize;
    }
//    计算任务完成时间:注意根据需求进行参数修正
    void f_computeFinishInfo(float currTime) {
//        任务完成时间包括两部分，任务处理时间(L1:fProcessTime）以及数据传输时间（L2:fTransmitTime）
//        以下为计算任务完成时间所需的常数
        float Wctrl = 8 + 1;
//        b单位：bit，  edgeNum(m)
        int LR = 2, b = 32, edgeNum = m_selectPath.size() - 1;
//        注意⚠️：Lse暂不知道多少，此处设定为1.0，记得修改， waitTime就是处理冲突需要的时间
        float Lse = 1.0, waitTime = currTime - m_fArriveTime;
        float fProcessTime = LR * (edgeNum + 1) + (Wctrl - 1) / b + edgeNum * Lse + waitTime;
//        bandwidth : 带宽 12.5Gbit/s 1GB = 1024MB = 1024 * 1024 KB = 1024 * 1024 * 1024 B
        float bandwidth = 12.5 * 1024 * 1024 * 1024 * 8;
//        distance ：路径长度(换算为单位m)，  n : 折射率； c : 光速
        float distance = edgeNum * 0.3 / 1000, n = 3.48, c = 3 * pow(10, 8);
        float fTransmitTime = m_fDataSize / bandwidth + distance * n / c;
        m_fFinishTime = m_fArriveTime + fProcessTime + fTransmitTime;
        m_fDelay = m_fFinishTime - m_fArriveTime;
    }
//    获取任务完成时间
    float f_getFinishTime() {
        return m_fFinishTime;
    }
//    获取任务时延
    float f_getDelay() {
        return m_fDelay;
    }
//    获取最终选择的路径
    vector<pair<int, int>> f_getSelectPath() {
        return m_selectPath;
    }
//    设定最终选择的路径
    void f_setSelctPath(vector<pair<int, int>> selectPath) {
        m_selectPath = selectPath;
    }
//    清空路径所占资源
    void f_recoverResource(Graph & graph_obj) {
        pair<int, int> currNode = m_selectPath[0];
        graph_obj.f_setNodeState(currNode, false);
        for (int i = 1; i < m_selectPath.size(); ++i) {
//            判断下一个点在currNode的方向
            int dir = graph_obj.f_posDir(currNode, m_selectPath[i]);
//            回收链路资源 ：双向边
            graph_obj.f_setLinkState(currNode, dir, false);
            graph_obj.f_setLinkState(m_selectPath[i], (dir + 2) % 4, false);
//            回收节点资源
            currNode = m_selectPath[i];
            graph_obj.f_setNodeState(currNode, false);
        }
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

//    根据pathVec修改m_routeMap，即修改图中点和边的繁忙状态
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
    bool pathSimilarity(Task * task) {
//        比对两个任务的m_routeMap是否有相同的路径段为busy
        for (int i = 0; i < m_routeMap.size(); ++i) {
            for (int j = 0; j < m_routeMap[i].size(); ++j) {
//                遍历b_edgeBusy
                for (int k = 0; k < 4; ++k) {
                    if (m_routeMap[i][j].b_edgeBusy[k] == true && task->m_routeMap[i][j].b_edgeBusy[k] == true) {
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
    
//    清空任务的路径集合及路径地图
    void f_reset() {
//        清空任务路径集合
        m_pathSetVec.clear();
        m_IpathSetVec.clear();
        m_colFirstPathSetVec.clear();
        m_rowFirstPathSetVec.clear();
//        清空任务的路径地图
        for (int i = 0; i < m_routeMap.size(); ++i) {
            for (int j = 0; j < m_routeMap[i].size(); ++j) {
                m_routeMap[i][j].f_reset();
            }
        }
    }
};

//    路径数量从小到大进行排序
bool g_routeNumAscendCompare(Task * task1, Task * task2) {
    return (task1->f_getPathSet()).size() < (task2->f_getPathSet()).size();
}

//    路径数量从大到小进行排序
bool g_routeNumDesendCompare(Task * task1, Task * task2) {
    return (task1->f_getPathSet()).size() > (task2->f_getPathSet()).size();
}

//    比较路径长度
bool g_routeLenCompare(vector<pair<int, int>> & path1, vector<pair<int, int>> & path2) {
    return path1.size() < path2.size();
}

bool g_taskArriveTimeCompare(Task * a, Task * b) {
    if (a->f_getArriveTime() < b->f_getArriveTime()) {
        return true;
    }
    return false;
}

// 二分法寻找插入位置
int g_findInsertPos(vector<Task *> currTaskVec, Task * targetTask) {
    int left = 0, right = currTaskVec.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        float midFInishTime = currTaskVec[mid]->f_getFinishTime();
        float currFinishTIme = targetTask->f_getFinishTime();
        if (midFInishTime == currFinishTIme) {
            left = mid + 1;
            break;
        } else if (midFInishTime > currFinishTIme) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return left;
}

//    计算平均时延
float g_comAverageDalay(vector<Task *> tasks) {
    float delay = 0.0;
    for (int i = 0; i < tasks.size(); ++i) {
        delay += tasks[i]->f_getDelay();
    }
    return delay / tasks.size();
}
//    计算链路利用率
float g_computeLinkUtilization(Graph & graph) {
    //    计算总边数
    vector<int> size = graph.f_getMapSizeVec();
    int i_row = size[0], i_col = size[1];
    int totalEdge = i_row * (i_col - 1) + i_col * (i_row - 1);
    //    统计利用的边数量
    float useEdge = 0;
    vector<vector<Node>> linkState = graph.f_getLinkState();
    for (int i = 0; i < linkState.size(); ++i) {
        for (int  j = 0; j < linkState[i].size(); ++j) {
//            遍历四个方向
            for (int k = 0; k < 4; ++k) {
                if (linkState[i][j].b_edgeBusy[k] == true) {
                    ++useEdge;
                }
            }
        }
    }
//    重复计算，除以2
    useEdge /= 2;
    return useEdge / totalEdge;
}

int main(int argc, const char * argv[]) {
//    第一阶段
//    初始化地图
//    地图默认为4 * 4
    int i_row = 4, i_col = 4;
    cout << "Please input the size of map. input format : map.row map.col"<<endl;
    cin >> i_row >> i_col;
//    存储完成任务过程中的链路信息
    Graph graph_obj(i_row, i_col);
//    用于保存使用过的链路，进而计算链路利用率
    Graph graph_record(i_row, i_col);
////    显示地图
//    graph_obj.f_showGraph();
//    确定任务
    int i_taskNum = 3;
    cout<<"Please input the Number of tasks. input format : task.num"<<endl;
    cin >> i_taskNum;
    vector<Task *> taskListVec;
//    存储最后完成的任务
    vector<Task *> finishedTaskVec;
//    输入任务信息
    for(int i = 0; i < i_taskNum; ++i) {
        pair<int, int> src, des;
        float fArriveTime, fDataSize;
        cout<<"Task"<<i + 1<<" info."<<endl;
        cout<<"please input source. input format : source.row source.col"<<endl;
        cin >> src.first >> src.second;
        cout<<"please input destination. input format : destination.row  destination.col"<<endl;
        cin >> des.first >> des.second;
        cout<<"Please input arrive time and data size. input format : fArriveTime fDataSize（）"<<endl;
        cin >> fArriveTime >> fDataSize;
        taskListVec.emplace_back(new Task(src, des, fArriveTime, fDataSize, graph_obj.f_getMapSizeVec()));
    }
//    对所有任务按任务到达时间排序
    sort(taskListVec.begin(), taskListVec.end(), g_taskArriveTimeCompare);
//    sort(taskListVec.begin(), taskListVec.end());
//    打印输出任务到达时间
//    for (int i = 0; i < taskListVec.size(); ++i) {
//        taskListVec[i]->f_showTasks();
//    }
    
//  finalPathSetVec存储全任务最终的路径集合，在程序末尾进行打印输出
//    finalPahtSetVec数据格式解析：
//    最内层vector表示路径
//    中间层vector表示任务对应的路径
//    最外层vector表示第i个批次，完成了哪些任务
//    vector<vector<vector<pair<int, int>>>> finalPathSetVec;
//  第二阶段
//    处理逻辑：
//    1、随着iCurrTime的推移，清除已经完成的任务占用的资源（包括节点资源和链路资源）
//    2、从taskListVec中确认，早于fCurrTime到达且未完成的任务集合
//    3、针对这些任务，设置节点和边的繁忙状态
//    4、然后寻找任务的路径集合，再选取每个任务的最终路径；
//    5、于是，每有一个任务完成，我们就需要更新链路状态，以及fCurrTime到达的任务集合，根据我们的方法，确定后续执行的任务。
//    6、fCurrTime会逐渐增加，循环执行1-5，直到所有任务都完成
//    当前任务集合，以任务完成时间从小到大进行排序
    float fCurrTime = 0;
    int i_taskSetNum = 0;
    vector<Task *> currTaskVec;
    vector<Task *> waitTaskVec;
//    Task finishedTask;
//    任务索引
    int iTaskIndex = 0;
//    循环执行，直到任务均被完成
    cout<<"任务完成逻辑 : "<<endl;
    while (iTaskIndex < taskListVec.size() || !currTaskVec.empty() || !waitTaskVec.empty()) {
        cout<<endl<<"fCurrTime : "<<fCurrTime<<endl;
//        确定fCurrTime下已经完成的任务，并回收其资源
        cout<<"currTaskVec.size() : "<<currTaskVec.size()<<endl;
//        输出currTaskVec
//        for (int i = 0; i < currTaskVec.size(); ++i) {
//            currTaskVec[i]->f_showTasks();
//        }
        for (int i = 0; i < currTaskVec.size(); ++i) {
            if (currTaskVec[i]->f_getFinishTime() <= fCurrTime) {
                cout<<"任务完成"<<endl;
                finishedTaskVec.emplace_back(currTaskVec[i]);
//                回收资源，设置节点和链路状态为free
                currTaskVec[i]->f_recoverResource(graph_obj);
//                删除节点
                currTaskVec.erase(currTaskVec.begin() + i);
                --i;
            } else {
//                当前任务在fCurrTime时没有完成，则后续的任务此时也没有完成
                break;
            }
        }
//        打印地图信息
//        if (currTaskVec.empty()) {
//            graph_obj.f_showGraph();
//        }
//        确定当前time下，任务集合
        while (iTaskIndex < taskListVec.size() && taskListVec[iTaskIndex]->f_getArriveTime() <= fCurrTime) {
            waitTaskVec.emplace_back(taskListVec[iTaskIndex++]);
        }
//        确定任务的执行顺序
//        寻找当前时刻下路径并输出
//        注意⚠️：此处寻找到任务的路径，则证明此任务在当前状态的资源下是可以被完成的，没有找到路径，则需要继续等待。
        cout<<"waitTaskVec.size() = "<<waitTaskVec.size()<<endl;
//        在重新寻找路径之前，清楚自身之前的链路状态繁忙记录
        for (int i = 0; i < waitTaskVec.size(); ++i) {
//            waitTaskVec[i]->f_showTasks();
            waitTaskVec[i]->f_findRoute(graph_obj);
//            展示任务的routeMap
//            waitTaskVec[i]->f_showRouteMap();
//            cout<<"f_findRoute之后的路径: "<<endl;
//            waitTaskVec[i]->f_showPathSet();
        }
//        小Tip：根据任务的路径数量从多到少进行排序。因为路径越多，越容易选择较优的路径，所以让这种任务优先被执行。
//        路径较少的任务等待后续的机会，其他任务释放资源后，可以为其提供更多更优路径。
        sort(waitTaskVec.begin(), waitTaskVec.end(), g_routeNumDesendCompare);
//        for (int i = 0; i < waitTaskVec.size(); ++i) {
//            waitTaskVec[i]->f_showTasks();
//            waitTaskVec[i]->f_showPathSet();
//        }
//        展示地图
//        graph_obj.f_showGraph();
//        确定路径集合
        i_taskSetNum = 1;
        vector<Task*> waitTaskTempVec;
        while (!waitTaskVec.empty()) {
            Task * selectTask = waitTaskVec[0];
            waitTaskVec.erase(waitTaskVec.begin());
            vector<Task *> taskSetVec = {selectTask};
            //        寻找与selectTask有交集的路径集合
            for (int i = 0; i < waitTaskVec.size(); ++i) {
                if (selectTask->pathSimilarity(waitTaskVec[i])) {
                    //                将路径集合移到当前任务集合（taskSetVec）中
                    taskSetVec.emplace_back(waitTaskVec[i]);
                    waitTaskVec.erase(waitTaskVec.begin() + i);
                    --i;
                }
            }
            //        第三阶段：根据当前任务集合，确定路径，修改graph_obj.m_linkState
            //        根据当前任务集合中的各任务路径数量从小到大排序
    //        可以直接对taskSetVec逆序，因为waitTaskVec是根据任务路径数量从大到小进行排序的
//            reverse(taskSetVec.begin(), taskSetVec.end());
            sort(taskSetVec.begin(), taskSetVec.end(), g_routeNumAscendCompare);
            //        打印当前任务集合
            cout<<"第"<<i_taskSetNum<<"个任务集合, "<<"taskSetVec.size() : "<<taskSetVec.size()<<endl;
            ++i_taskSetNum;
            for (int i = 0; i < taskSetVec.size(); ++i) {
                cout<<"Task "<<i + 1<<" : "<<endl;
                taskSetVec[i]->f_showTasks();
            }
            //        注意⚠️⚠️：每次确定一个任务的seletctPath，且更新graph_obj.m_linkState之后，需要对其他任务重新寻找路径。
            for (int i = 0; i < taskSetVec.size(); ++i) {
                //            根据I型路径、优先遍历Y、优先遍历X的顺序选择路径
                vector<vector<pair<int, int>>> IpathSetVec = taskSetVec[i]->f_getIpathSet();
//                cout<<"IpathSetVec.size() : "<<IpathSetVec.size()<<endl;
                //            找到一条路径，就退出
                bool findPath = false;
                if (!IpathSetVec.empty()) {
                    for (auto pathVec : IpathSetVec) {
                        //                    判断路径中所有点和线段是否都空闲
                        if (!graph_obj.f_pathBusy(pathVec)) {
                            //                        但此处在路径前面加入任务起点和终点，方便后续打印输出，明确任务信息
//                            pathVec.insert(pathVec.begin(), taskSetVec[i].f_getDes());
//                            pathVec.insert(pathVec.begin(), taskSetVec[i].f_getSrc());
    //                        修改任务的m_selectPath属性
                            taskSetVec[i]->f_setSelctPath(pathVec);
    //                        计算任务完成时间
                            taskSetVec[i]->f_computeFinishInfo(fCurrTime);
                            cout<<"Task_Info : "<<endl;
                            taskSetVec[i]->f_showTasks();
                            taskSetVec[i]->f_showSelectPath();
                            cout<<"finishedTime : "<<taskSetVec[i]->f_getFinishTime()<<endl;
                            //                        设置当前路径的点和边为busy
                            graph_obj.f_setPathBusy(pathVec);
                            graph_record.f_setPathBusy(pathVec);
//                            graph_record.f_showGraph();
                            //                        找到当前任务的合适路径，前往下一任务
                            findPath = true;
                            break;
                        }
                    }
                }
                //            找到一条I型路径,前往下一任务
                if (findPath) {
    //                将任务从taskSetVec中取出，放入currTaskVec;
    //                二分法查找应该放入的位置
                    int pos = g_findInsertPos(currTaskVec, taskSetVec[i]);
    //                将taskSetVec[i] 加入 currTaskVec;
                    currTaskVec.insert(currTaskVec.begin() + pos, taskSetVec[i]);
    //                删除taskSetVec[i];
                    taskSetVec.erase(taskSetVec.begin() + i);
                    --i;
                    continue;
                }
                vector<vector<pair<int, int>>> colPathSetVec = taskSetVec[i]->f_getColFirstPath();
//                cout<<"colPathSetVec.size() : "<<colPathSetVec.size()<<endl;
                if (!colPathSetVec.empty()) {
    //                    根据路径长短排序，同等要求下，保证短路径优先
                    sort(colPathSetVec.begin(), colPathSetVec.end(), g_routeLenCompare);
//                    打印排序之后的路径
//                    for (int i = 0; i < colPathSetVec.size(); ++i) {
//                        cout<<"colPathSetVec["<<i<<"].size() : "<<colPathSetVec[i].size()<<endl;
//                        for (auto node :  colPathSetVec[i]) {
//                            cout<<node.first<<" "<<node.second<<endl;
//                        }
//                        cout<<endl;
//                    }
                    for (auto pathVec : colPathSetVec) {
                        //                    判断路径中所有点和线段是否都空闲
                        if (!graph_obj.f_pathBusy(pathVec)) {
                            //                        但此处在路径前面加入任务起点和终点，方便后续打印输出，明确任务信息
//                            pathVec.insert(pathVec.begin(), taskSetVec[i].f_getDes());
//                            pathVec.insert(pathVec.begin(), taskSetVec[i].f_getSrc());
                            //                        修改任务的m_selectPath属性
                            taskSetVec[i]->f_setSelctPath(pathVec);
                            //                        计算任务完成时间
                            taskSetVec[i]->f_computeFinishInfo(fCurrTime);
                            cout<<"Task_Info : "<<endl;
                            taskSetVec[i]->f_showTasks();
                            taskSetVec[i]->f_showSelectPath();
                            cout<<"finishedTime : "<<taskSetVec[i]->f_getFinishTime()<<endl;
                            //                        设置当前路径的点和边为busy
                            graph_obj.f_setPathBusy(pathVec);
                            graph_record.f_setPathBusy(pathVec);
//                            graph_record.f_showGraph();
                            //                        找到当前任务的合适路径，前往下一任务
                            findPath = true;
                            break;
                        }
                    }
                }
                //            找到一条L型路径,前往下一任务
                if (findPath) {
    //                将任务从taskSetVec中取出，放入currTaskVec;
    //                二分法查找应该放入的位置
                    int pos = g_findInsertPos(currTaskVec, taskSetVec[i]);
    //                将taskSetVec[i] 加入 currTaskVec;
                    currTaskVec.insert(currTaskVec.begin() + pos, taskSetVec[i]);
    //                删除taskSetVec[i];
                    taskSetVec.erase(taskSetVec.begin() + i);
                    --i;
                    continue;
                }
                vector<vector<pair<int, int>>> rowPathSetVec = taskSetVec[i]->f_getRowFirstPath();
//                cout<<"rowPathSetVec.size() : "<<rowPathSetVec.size()<<endl;
                if (!rowPathSetVec.empty()) {
                    //                    根据路径长短排序，同等要求下，保证短路径优先
                    sort(colPathSetVec.begin(), colPathSetVec.end(), g_routeLenCompare);
                    for (auto pathVec : rowPathSetVec) {
                        //                    判断路径中所有点和线段是否都空闲
                        if (!graph_obj.f_pathBusy(pathVec)) {
                            //                        但此处在路径前面加入任务起点和终点，方便后续打印输出，明确任务信息
//                            pathVec.insert(pathVec.begin(), taskSetVec[i].f_getDes());
//                            pathVec.insert(pathVec.begin(), taskSetVec[i].f_getSrc());
                            //                        修改任务的m_selectPath属性
                            taskSetVec[i]->f_setSelctPath(pathVec);
                            //                        计算任务完成时间
                            taskSetVec[i]->f_computeFinishInfo(fCurrTime);
                            cout<<"Task_Info : "<<endl;
                            taskSetVec[i]->f_showTasks();
                            taskSetVec[i]->f_showSelectPath();
                            cout<<"finishedTime : "<<taskSetVec[i]->f_getFinishTime()<<endl;
                            //                        设置当前路径的点和边为busy
                            graph_obj.f_setPathBusy(pathVec);
                            graph_record.f_setPathBusy(pathVec);
//                            graph_record.f_showGraph();
                            //                        找到当前任务的合适路径，前往下一任务
                            findPath = true;
                            break;
                        }
                    }
                }
                //            找到一条7型路径,前往下一任务
                if (findPath) {
    //                将任务从taskSetVec中取出，放入currTaskVec;
    //                二分法查找应该放入的位置
                    int pos = g_findInsertPos(currTaskVec, taskSetVec[i]);
    //                将taskSetVec[i] 加入 currTaskVec;
                    currTaskVec.insert(currTaskVec.begin() + pos, taskSetVec[i]);
    //                删除taskSetVec[i];
                    taskSetVec.erase(taskSetVec.begin() + i);
                    --i;
                    continue;
                }
                //            上述路径都没有合理的
                //            记录这些找不到路径的任务，放回waitTaskTempVec，等待下一个时刻
                waitTaskTempVec.emplace_back(taskSetVec[i]);
//                taskSetVec.erase(taskSetVec.begin() + i);
            }
        }
//        仍然在等待态的任务集合
        waitTaskVec =  waitTaskTempVec;
//        时间后移动
        ++fCurrTime;
    }
      
//    show finalPathSetVec
    cout<<"final path set for all tasks."<<endl;
    for (int i = 0; i < finishedTaskVec.size(); ++i) {
        cout<<"Task "<<i + 1<<" : "<<endl;
        finishedTaskVec[i]->f_showTasks();
        finishedTaskVec[i]->f_showFinishInfo();
        cout<<endl;
    }
//    计算平均时延
    cout<<"the average delay ："<<g_comAverageDalay(finishedTaskVec)<<endl;
//    计算链路利用率
//    显示地图信息
    graph_record.f_showGraph();
    cout<<"link utilization : "<<g_computeLinkUtilization(graph_record)<<endl;
//    回收内存资源，回收taskListVec之后，currTaskVec以及waitTaskVec不能再使用
    for (int i = 0; i < taskListVec.size(); ++i) {
        delete taskListVec[i];
    }
    return 0;
}
