//
//  main.cpp
//  multiPathSelect
//
//  Created by xiaoR on 2023/4/14.
//

#include <iostream>
#include <vector>
#include <unordered_map>

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

//定义任务类:记录任务，以及寻找路径
class Task{
private:
    //    pair存储起点和终点的位置信息，注意此处是以行号、列号的顺序进行存储，不是x和y
    pair<int, int> m_src;
    pair<int, int> m_des;
    vector<vector<pair<int, int>>> m_pathSetVec;
public:
    Task(const pair<int, int> & src, const pair<int, int> & des) {
        //        构造函数内确定起点和终点位置
//        cin >> m_src.first >> m_src.second;
//        cin >> m_des.first >> m_des.second;
        m_src = src;
        m_des = des;
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
            }
        }
//        2、起点与终点位于同一列，遍历列
        if (m_src.second == m_des.second) {
            vector<pair<int, int>> pathVec = f_colTraverse(m_src, m_des, m_linkState);
//            行遍历结果不为空，可以加入路径集合
            if (!pathVec.empty()) {
                m_pathSetVec.emplace_back(pathVec);
            }
        }
//        第二类：判断L/L7形路径 ： 其实就是除了I型路径之外的优先遍历竖直方向的路径（Y轴/遍历列）:结果也许包括多个路径
        vector<vector<pair<int, int>>> colFirstPathSet = f_colFirstTraverse(m_src, m_des, m_linkState);
        m_pathSetVec.insert(m_pathSetVec.end(), colFirstPathSet.begin(), colFirstPathSet.end());
//        第三类：优先遍历水平方向的路径（X轴/遍历行），结果也许包括多个路径
        vector<vector<pair<int, int>>> rowFirstPathSet = f_rowFirstTraverse(m_src, m_des, m_linkState);
        m_pathSetVec.insert(m_pathSetVec.end(), rowFirstPathSet.begin(), rowFirstPathSet.end());
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
        cout<<"borders : "<<endl;
        for (auto  it : borders) {
            cout<<it.first<<" "<<it.second<<endl;
        }
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
        cout<<"i_colDirFlag : "<<i_colDirFlag<<" i_rowDirFlag : "<<i_rowDirFlag<<endl;
        cout<<"i_colLenFlag : "<<i_colLenFlag<<" i_rowLenFlag: "<<i_rowLenFlag<<endl;
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
    
//    DFS寻找路径
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
};

// 定义图类：记录边的空闲状态
class Graph{
private:
    int m_row, m_col;
//    为了实现方便，边暂时选用邻接矩阵进行存储
    vector<vector<Node>> m_linkState;
public:
//    地图默认初始化为2 * 2
    Graph(int i_row = 2, int i_col = 2) {
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
};

int main(int argc, const char * argv[]) {
//    初始化地图
//    地图默认为4 * 4
    int i_row = 4, i_col = 4;
    cin >> i_row >> i_col;
    Graph graph_obj(i_row, i_col);
////    显示地图
//    graph_obj.f_showGraph();
//    确定任务
    int i_taskNum = 3;
    cin >> i_taskNum;
    vector<Task> taskListVec;
    while (i_taskNum-- > 0) {
        pair<int, int> src, des;
        cout<<"please input source and destination."<<endl;
        cin >> src.first >> src.second;
        cin >> des.first >> des.second;
        Task task(src, des);
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
    return 0;
}
