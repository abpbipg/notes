#include<iostream>
#include<vector>
#include<queue>
using namespace std;

// 构造边的对象
struct Edge
{
    int start;
    int end;
    int weight;
    Edge(int s,int e,int w){
        start=s;
        end=e;
        weight=2;
    }
    // 重写<运算符
    bool operator<(const Edge& a)const{
        return a.weight < weight;
    }
};

class Graph{
private:
    int vertex_num;   //图的顶点个数
    int edge;
    // 如果顶点的表示是离散的应该加一层映射
    vector<vector<int>> arc; //邻接矩阵
public:
    // 测试用的默认构造函数
    Graph();
    //构造函数,从命令行输入
    Graph(int vertex_num_,int edge_);
    //打印所有的边
    void print_edge();
    // 打印整个邻接矩阵
    void print_arc();
    //求最短路径
    void Dijkstra(int begin);//单源最短路
    void Floyd();//多源最短路
    void Prim();//最小生成树
    void Kruskal();//最小生成树
};

Graph::Graph(){
    this->vertex_num = 7;
    this->edge=12;
    this->arc = vector<vector<int>>(vertex_num,vector<int>(vertex_num,INT_MAX));
    // cout<<vertex_num<<endl;
    // 初始化一个邻接矩阵.dijskra算法的图。
    arc[0][1]=12;
    arc[1][0]=12;
    arc[0][6]=14;
    arc[6][0]=14;
    arc[0][5]=16;
    arc[5][0]=16;
    arc[1][2]=10;
    arc[2][1]=10;
    arc[1][5]=7;
    arc[5][1]=7;
    arc[2][3]=3;
    arc[3][2]=3;
    arc[2][4]=5;
    arc[4][2]=5;
    arc[2][5]=6;
    arc[5][2]=6;
    arc[3][4]=4;
    arc[4][3]=4;
    arc[4][5]=2;
    arc[5][4]=2;
    arc[4][6]=8;
    arc[6][4]=8;
    arc[5][6]=9;
    arc[6][5]=9;
    // 对角线自己到自己是0
    for(int i=0;i<vertex_num;i++){
        arc[i][i]=0;
    }
}

// 从命令行输入一个邻接矩阵
Graph::Graph(int vertex_num_,int edge_){
    this->vertex_num=vertex_num_;
    this->edge=edge_;
    this->arc=vector<vector<int>>(vertex_num,vector<int>(vertex_num,INT_MAX));
    int beg=0,end=0,weight=0;
    for(int i=0;i<edge_;i++){
        cin>>beg>>end>>weight;
        arc[beg][end]=weight;
        // 如果是无向图则需要添加反向的路径
        arc[end][beg]=weight;
    }
}

void Graph::print_edge(){
    for(int i=0;i<vertex_num;i++){
        for(int j=0;j<vertex_num;j++){
            if(arc[i][j]<INT_MAX)
            cout<<"begin:"<<i<<"\tend:"<<j<<"\tweight:"<<arc[i][j]<<endl;
        }
    }
}


void Graph::print_arc(){
    for(int i=0;i<vertex_num;i++){
        for(int j=0;j<vertex_num;j++){
            cout<<arc[i][j]<<"\t";
        }
        cout<<endl;
    }
}
void Graph::Dijkstra(int begin){
    // 初始化结果集,到自己是0
    vector<int> result(vertex_num,INT_MAX);
    result[begin]=0;
    // 初始化距离集合，每次从中挑选
    vector<int> distance(vertex_num,INT_MAX);
    for(int i=0;i<vertex_num;i++){
        distance[i]=arc[begin][i];
    }

    for(int k=0;k<vertex_num-1;k++){
        // 从中挑选非零的最小值，使用0表示已经挑选到结果集合中。
        int min_distance=INT_MAX;
        int min_index=0;
        for(int i=0;i<vertex_num;i++){
            if(distance[i]!=0 && distance[i]<min_distance){
                min_distance=distance[i];
                min_index=i;
            }
        }
        // 添加到结果集合中，并更新删除distance集合。
        cout<<min_index<<endl;
        result[min_index]=min_distance;
        distance[min_index]=0;

        // 使用min_index更新距离集合
        for(int i=0;i<vertex_num;i++){
            if(distance[i]!=0 && arc[min_index][i]<INT_MAX){
                distance[i]=min(distance[i],min_distance+arc[min_index][i]);
            }
        }
    }

    for(int i=0;i<vertex_num;i++){
        cout<<"end:"<<i<<"\tweight:"<<result[i]<<endl;
    }

}

void Graph::Floyd(){
    // 初始化结果集合，一个用来记录最短距离，一个用来记录最短路径。
    vector<vector<int>> distance(arc);
    vector<vector<int>> path(vertex_num,vector<int>(vertex_num,0));
    int i,j,k;
    int temp;

    // 三层for循环。k表示中间点。i表示起点，j表示中点。如果经过起始点小于直接点，则更新。
    // 可以进行优化，例如i||j==k的时候，可以不用运算，直接continue。
    for(k=0;k<vertex_num;k++){
        for(i=0;i<vertex_num;i++){
            for(j=0;j<vertex_num;j++){
                temp=INT_MAX;
                if(distance[i][k]<INT_MAX && distance[k][j]<INT_MAX){
                    temp=distance[i][k]+distance[k][j];
                }
                if(temp<distance[i][j]){
                    distance[i][j]=temp;
                    path[i][j]=k;
                }
            }
        }

    }
    for(i=0;i<vertex_num;i++){
        for(j=0;j<vertex_num;j++){
            cout<<distance[i][j]<<"\t";
        }
        cout<<endl;
    }
}
// 使用优先队列，挑选结果集合
void Graph::Prim(){
    // 最小生成树开始的顶点。
    int start = 2;
    // 初始化结果集合,边的集合。
    vector<Edge> result; 
    // 初始化优先队列，用来挑选满足要求的最小的边
    priority_queue<Edge,vector<Edge>> pri_edge;

    for(int k=0;k<vertex_num;k++){
        // 添加边
        for(int i=0;i<vertex_num;i++){
            // 检查i是不是已经被选中
            for(auto e:result){
                if(i==e.start || i==e.end){
                    continue;
                }
            }
            if(i!=start && arc[start][i]<INT_MAX){
                pri_edge.push(Edge(start,i,arc[start][i]));
            }
        }
        // 挑选边.并将end点作为下一个边的扩展。继续添加边
        Edge e = pri_edge.top();
        pri_edge.pop();
        result.push_back(e);
        start = e.end;
    }

    // 显示结果
    for(auto e:result){
        cout<<e.start<<"\t"<<e.end<<"\t"<<e.weight<<endl;
    }
}
int main(){
    Graph g;
    // g.print();
    // g.Dijkstra(3);
    // g.Floyd();
    g.Prim();
    return 0;
}