#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
#include <graphics.h>
#include <ctype.h>
#include <string>
#include <queue>
#include <vector>
using namespace std;
#pragma warning(disable : 4996)

#define lens 96
#define lenx 94

IMAGE img11[12];	//界面图
IMAGE img1;	//沙河地图
IMAGE img2;	//本部地图
IMAGE img3;	//背景
IMAGE img4;	//公交车

class Point
{
public:
	int x;
	int y;
};
vector<Point> points;
vector<Point> pointx;
vector<Point>pointl;
void Output();


int ci[3] = {1, 360, 260};		//当前位置
int endnum;		//终点个数
int way;		//物理导航策略——1最短时间、2最短距离、3交通工具
int flag = 0;	//== 0 为输入起点， == 1 为当前位置为起点
string start;					//存储起点字符串——物理、逻辑、多目标
string endpoint;				//存储终点字符串——物理导航
vector<string> zhongdian;		//逻辑导航的终点信息——逻辑导航
vector<string> mubiao;			//存储多个目标地点——多目标导航

vector<string> dingwei;			//定位时输出的信息
vector<string> luojixinxi;		//逻辑导航输出的信息	
vector<string> mubiaoxinxi;		//多目标导航输出的信息

int mouse1[] = { 990, 390, 1280, 430 };//定位框————————1号框
int mouse2[] = { 990, 450, 1280, 490 };//物理导航框————————2号框
int mouse3[] = { 990, 510, 1280, 550 };//逻辑导航框————————3号框
int mouse4[] = { 990, 570, 1280, 610 };//多目标导航框————————4号框
int mouse5[] = { 1095, 295, 1175, 335 };//物理导航暂停框————————5号框
int mouse6[] = { 990, 70, 1280, 110 };	//继续框、选择输入起点框————————6号框
int mouse7[] = { 990, 180, 1280, 220 };	//返回主菜单框、选择当前位置框————————7号框


int button_judge(int x, int y)
{	//判断鼠标点击坐标在哪个框
	if (x > mouse1[0] && x < mouse1[2] && y > mouse1[1] && y < mouse1[3])	return 1;//————————1号框
	if (x > mouse2[0] && x < mouse2[2] && y > mouse2[1] && y < mouse2[3])	return 2;//————————2号框
	if (x > mouse3[0] && x < mouse3[2] && y > mouse3[1] && y < mouse3[3])	return 3;//————————3号框
	if (x > mouse4[0] && x < mouse4[2] && y > mouse4[1] && y < mouse4[3])	return 4;//————————4号框
	if (x > mouse5[0] && x < mouse5[2] && y > mouse5[1] && y < mouse5[3])	return 5;//————————5号框
	if (x > mouse6[0] && x < mouse6[2] && y > mouse6[1] && y < mouse6[3])	return 6;//————————6号框
	if (x > mouse7[0] && x < mouse7[2] && y > mouse7[1] && y < mouse7[3])	return 7;//————————7号框
	return 0;
}

/*
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*/


class Busy
{
public:
    int start;
    int end;
    int level;
};
vector<Busy> busy1;
vector<Busy> busy2;
vector<string>choice;
time_t timep;
struct tm* p;

#define file1 "./loading/study.txt"
#define file2 "./loading/teacher.txt"


void Input();//读入文件
int Atime();//获取当前时间
void decidelevel();

void Input()
{
    Busy bu;
    int size1, size2;
    ifstream mycin1(file1);
    if (!mycin1)
    {
        cout << "文件打开错误！" << endl;
    }
    mycin1 >> size1;
    for (int i = 0; i < size1; i++)
    {
        mycin1 >> bu.start;
        mycin1 >> bu.end;
        mycin1 >> bu.level;
        busy1.push_back(bu);
    }
    mycin1.close();
    busy1.at(size1 - 1).end = 24;

    ifstream mycin2(file2);
    if (!mycin2)
    {
        cout << "文件打开错误！" << endl;
    }
    mycin2 >> size2;
    for (int i = 0; i < size1; i++)
    {
        mycin2 >> bu.start;
        mycin2 >> bu.end;
        mycin2 >> bu.level;
        busy2.push_back(bu);
    }
    mycin2.close();
    busy2.at(size2 - 1).end = 24;
}

int Atime()
{
    p = gmtime(&timep);
    return(p->tm_hour + 8);
}
//负载均衡
void decidelevel()
{
    int now;
    int level1;
    int level2;
    string s;
    time(&timep);
    Input();
    now = Atime();
    for (int i = 0; i < busy1.size(); i++)
    {
        if (i >= busy1.at(i).start && i <= busy1.at(i).end)
        {
            level1 = busy1.at(i).level;
            break;
        }
    }
    for (int i = 0; i < busy2.size(); i++)
    {
        if (i >= busy2.at(i).start && i <= busy2.at(i).end)
        {
            level2 = busy2.at(i).level;
            break;
        }
    }
    choice.clear();
    if (ci[0] == 1)
    {
        if (level2 <= level1)
        {
            s = "学生食堂";
            choice.push_back(s);
        }
        else
        {
            s = "教工食堂";
            choice.push_back(s);
        }
    }
    else
    {
        if (level2 <= level1)
        {
            s = "西土城学生食堂";
            choice.push_back(s);
        }
        else
        {
            s = "教工餐厅";
            choice.push_back(s);
        }
    }
}




/*
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*/


//两个校区的长宽系数
#define shahe 1
#define xitucheng 2
#define map_len_shahe 96
#define map_wid_shahe 59
#define map_len_xitucheng 94
#define map_wid_xitucheng  60
#define shahe_map_len 96
#define shahe_map_wid 59
#define xitucheng_map_len 94
#define xitucheng_map_wid  60
#define cqueue_length 2000
#define inf 65535;

#define shahe_bus_station 2402
#define xitucheng_bus_station 2344
#define shahe_school_bus_station 2271
#define xitucheng_school_bus_station 314
#define man_speed 1
#define bycicle_speed 5

//两个校区信息文件路径
#define path_file_name "./loading/path.txt"
#define map_name_shahe "./loading/shahe_0607.txt"
#define map_cost_shahe "./loading/shahe_cost_0607.txt"
#define map_name_xitucheng "./loading/xitucheng_0607.txt"
#define map_cost_xitucheng "./loading/xitucheng_cost_0607.txt"
#define bus_time "./loading/school_bus.txt"
#define building_campus_position_file "./loading/building_campus_position.txt"
#define class_data_file "./loading/class.txt"
//可选导航策略
#define DISTANCE_SHORTEST 0                //距离最短策略
#define TIME_SHORTEST 1                    //时间最短最短策略
#define TRAFFIC_TOOL_SHORTEST 2            //交通工具时间最短策略

//两个校区信息
char map_shahe[map_wid_shahe][map_len_shahe + 2];
char cost_shahe[map_wid_shahe + 2][map_len_shahe + 2];
char map_xitucheng[map_wid_xitucheng + 2][map_len_xitucheng + 2];
char cost_xitucheng[map_wid_xitucheng + 2][map_len_xitucheng + 2];



int path_pause = -1;
int path_end = -1;
int start_campus = -1;
int end_campus = -1;



//bfs
struct cqueue {
    int queue[cqueue_length];
    int head = 0;
    int tail = 0;
};
bool is_cqueue_empty(struct cqueue* C) {
    if (C->head == C->tail)
        return 1;
    return 0;
}
bool is_cqueue_full(struct cqueue* C) {
    if ((C->tail + 1) % cqueue_length == C->head)
        return 1;
    return 0;
}
void incqueue(int x, struct cqueue* C) {
    if (is_cqueue_full(C) == 1) {
        cout << "循环队列已满，入队失败";
        return;
    }
    C->queue[C->tail] = x;
    C->tail = (C->tail + 1) % cqueue_length;
}
int decqueue(struct cqueue* C) {
    if (is_cqueue_empty(C)) {
        cout << "循环队列为空，出队失败";
        return -1;
    }
    int temp = C->queue[C->head];
    C->head = (C->head + 1) % cqueue_length;
    return temp;
}



struct building_campus_position {
    string building;
    char campus;
    int x;
    int y;
};
struct building_position {
    string building;
    int x;
    int y;
    int range;
};

struct number_path {
    int source;
    int dest;
    int path_length;
    int* path;
};//储存对应起点和终点的路程，指针指向路的数组

//名字对应的坐标。在结构体数组的下标即为序号
struct name_number {
    string name;
    int number;
};

struct T_C_P {
    string time;
    string class_name;
    string position;
};
struct T_C_P_T {
    int day;
    struct T_C_P data;
};





//分支限界核心算法
class TSP_node {
    friend int initial_h(int number, unsigned int** graph, unsigned int** father_graph);
public:
    int lower_bound;//下界
    int n;//阶数
    unsigned int** graph;
    int* row_number;//行邻接表
    int* col_number;
    int* start;
    int* end;
    TSP_node* lchild;
    TSP_node* rchild;
    TSP_node* father;
    //要减的col和row是物理上的col和row。
    int reduced_matrix(TSP_node& father_node, int row, int col);
    //父节点初始化
    TSP_node(unsigned int** graph, int number);
    //活结点下属节点初始化
    //type=1为包含，0为不包含
    TSP_node(int type, int row, int col, TSP_node& father_node, int number);
    //归约结果-优先节点处理
    ~TSP_node() {
        delete start;
        delete end;
        delete row_number;
        delete col_number;
        for (int i = 0; i < n; i++)
            delete graph[i];
        delete graph;
    }
};

void position_building_match(string building, struct building_campus_position* temp) {
    ifstream f(building_campus_position_file);
    while (f) {
        getline(f, temp->building, ' ');
        f >> temp->campus;
        f >> temp->x;
        f >> temp->y;
        string buf;
        getline(f, buf);
        //f.seekg(sizeof(char), ios::cur);
        if (building == temp->building)
            return;
    }
    cout << "未匹配到，请检查文件中是否含有本建筑物名：" << building << ",或重新输入";
}





void map_initial();//将两个校区信息读入程序
int S_num(int x, int y, int campus);//将坐标由(x,y)形式转为序号，1代表沙河，2代表西土城
void initial(int* list, int list_size);//将list中的元素全部置为-1
int reachable(int source, int dest, int campus, int strategy);//判断某校区内相邻两点是否可达
int step_cost(int p1, int p2, int strategy, int campus);//根据导航策略，计算并返回两点之间单步代价
int search_neighbor(int cur, int* neignbor, int campus);//搜索当前节点的邻居，返回邻居个数
int find_min_Fcost_point(int* open_list, int* open_F, int campus);//找到并返回open_list中F值最小的节点
int H_Cost(int cur, int end, int strategy, int campus);//用海明顿距离估计从当前节点到终点的开销
int G_Cost(int cur, int start, int* parent, int strategy, int campus);//计算从起点到当前节点的代价
int AStar_Search(int start, int end, int strategy, int* Path, int* COST, int campus);//A*算法寻路
int get_path(int end, int start, int* parent, int* cost, int* path, int strategy, int campus);//从终结点连接到起点，形成最短路线
void print_path(int* path, int size, int campus);//打印路径
int inlist(int* list, int sign, int size);//从list中查找sign元素

void map_initial() {

    //读入沙河校区地图
    FILE* fp_shahe;
    fopen_s(&fp_shahe, map_name_shahe, "r");
    if (fp_shahe == NULL)
        cout << "<error1>: can not open file map_name_shahe!\n";
    for (int i = 0; !feof(fp_shahe); i++) {
        fgets(map_shahe[i], sizeof(map_shahe[i]), fp_shahe);
    }

    //读入西土城校区地图
    FILE* fp_xitucheng;
    fopen_s(&fp_xitucheng, map_name_xitucheng, "r");
    if (fp_xitucheng == NULL)
        cout << "<error2>: can not open file map_name_xitucheng!\n";
    for (int i = 0; !feof(fp_xitucheng); i++) {
        fgets(map_xitucheng[i], sizeof(map_xitucheng[i]), fp_xitucheng);
    }

    //读入沙河校区拥挤度地图
    FILE* fp_cost_shahe;
    fopen_s(&fp_cost_shahe, map_cost_shahe, "r");
    if (fp_xitucheng == NULL)
        cout << "<error3>: can not open file map_cost_shahe!\n";
    for (int i = 0; !feof(fp_cost_shahe); i++) {
        fgets(cost_shahe[i], sizeof(cost_shahe[i]), fp_cost_shahe);
    }

    //读入西土城校区拥挤度地图
    FILE* fp_cost_xitucheng;
    fopen_s(&fp_cost_xitucheng, map_cost_xitucheng, "r");
    if (fp_cost_xitucheng == NULL)
        cout << "<error4>: can not open file map_cost_xitucheng!\n";
    for (int i = 0; !feof(fp_cost_xitucheng); i++)
        fgets(cost_xitucheng[i], sizeof(cost_xitucheng[i]), fp_cost_xitucheng);

}
//将两个校区地图属性文件中信息读入程序

int S_num(int x, int y, int campus) {
    if (campus == 1)
        return y * map_len_shahe + x;
    if (campus == 2)
        return y * map_len_xitucheng + x;
    cout << "<errror4>: campus can be only 1 or 2!\n";
    return 0;
}
//获取每个节点的序号

void initial(int* list, int list_size) {
    for (int i = 0; i < list_size; i++)
        list[i] = -1;
}
//将list中的元素全部置为-1

int reachable(int source, int dest, int campus, int strategy) {
    if (campus != 1 && campus != 2) {
        cout << "<error5>: campus can bo only 1 or 2!\n";
        return -1;
    }


    //沙河校区、步行
    if (campus == 1 && (strategy == DISTANCE_SHORTEST || strategy == TIME_SHORTEST)) {
        int y = source / map_len_shahe;
        int x = source % map_len_shahe;
        int y1 = dest / map_len_shahe;
        int x1 = dest % map_len_shahe;
        if (x1 == x && y1 == y - 1) {
            if (map_shahe[y - 1][x] != '0')
                return 1;
            else if (x - 1 >= 0 && map_shahe[y - 1][x - 1] != '0')
                return 1;
            return 0;
        }
        if (x1 == x + 1 && y1 == y) {
            if (map_shahe[y][x] != '0')
                return 1;
            else if (y - 1 >= 0 && map_shahe[y - 1][x] != '0')
                return 1;
            return 0;
        }
        if (x1 == x && y1 == y + 1) {
            if (map_shahe[y][x] != '0')
                return 1;
            else if (x - 1 >= 0 && map_shahe[y][x - 1] != '0')
                return 1;
            return 0;
        }
        if (x1 == x - 1 && y1 == y) {
            if (map_shahe[y][x - 1] != '0')
                return 1;
            else if (y - 1 >= 0 && map_shahe[y - 1][x - 1] != '0')
                return 1;
            return 0;
        }
        if (x1 == x - 1 && y1 == y - 1 && map_shahe[y - 1][x - 1] != '0')
            return 1;
        if (x1 == x + 1 && y1 == y - 1 && map_shahe[y - 1][x] != '0')
            return 1;
        if (x1 == x + 1 && y1 == y + 1 && map_shahe[y][x] != '0')
            return 1;
        if (x1 == x - 1 && y1 == y + 1 && map_shahe[y][x - 1] != '0')
            return 1;
        return 0;
    }

    //西土城校区、步行
    if (campus == 2 && (strategy == DISTANCE_SHORTEST || strategy == TIME_SHORTEST)) {
        int y = source / map_len_xitucheng;
        int x = source % map_len_xitucheng;
        int y1 = dest / map_len_xitucheng;
        int x1 = dest % map_len_xitucheng;
        if (x1 == x && y1 == y - 1) {
            if (map_xitucheng[y - 1][x] != '0')
                return 1;
            else if (x - 1 >= 0 && map_xitucheng[y - 1][x - 1] != '0')
                return 1;
            return 0;
        }
        if (x1 == x + 1 && y1 == y) {
            if (map_xitucheng[y][x] != '0')
                return 1;
            else if (y - 1 >= 0 && map_xitucheng[y - 1][x] != '0')
                return 1;
            return 0;
        }
        if (x1 == x && y1 == y + 1) {
            if (map_xitucheng[y][x] != '0')
                return 1;
            else if (x - 1 >= 0 && map_xitucheng[y][x - 1] != '0')
                return 1;
            return 0;
        }
        if (x1 == x - 1 && y1 == y) {
            if (map_xitucheng[y][x - 1] != '0')
                return 1;
            else if (y - 1 >= 0 && map_xitucheng[y - 1][x - 1] != '0')
                return 1;
            return 0;
        }
        if (x1 == x - 1 && y1 == y - 1 && map_xitucheng[y - 1][x - 1] != '0')
            return 1;
        if (x1 == x + 1 && y1 == y - 1 && map_xitucheng[y - 1][x] != '0')
            return 1;
        if (x1 == x + 1 && y1 == y + 1 && map_xitucheng[y][x] != '0')
            return 1;
        if (x1 == x - 1 && y1 == y + 1 && map_xitucheng[y][x - 1] != '0')
            return 1;
        return 0;
    }

    //沙河校区、自行车
    if (campus == 1 && strategy == TRAFFIC_TOOL_SHORTEST) {
        int y = source / map_len_shahe;
        int x = source % map_len_shahe;
        int y1 = dest / map_len_shahe;
        int x1 = dest % map_len_shahe;
        if (x1 == x && y1 == y - 1) {
            if (map_shahe[y - 1][x] >= '2')
                return 1;
            else if (x - 1 >= 0 && map_shahe[y - 1][x - 1] >= '2')
                return 1;
            return 0;
        }
        if (x1 == x + 1 && y1 == y) {
            if (map_shahe[y][x] >= '2')
                return 1;
            else if (y - 1 >= 0 && map_shahe[y - 1][x] >= '2')
                return 1;
            return 0;
        }
        if (x1 == x && y1 == y + 1) {
            if (map_shahe[y][x] >= '2')
                return 1;
            else if (x - 1 >= 0 && map_shahe[y][x - 1] >= '2')
                return 1;
            return 0;
        }
        if (x1 == x - 1 && y1 == y) {
            if (map_shahe[y][x - 1] >= '2')
                return 1;
            else if (y - 1 >= 0 && map_shahe[y - 1][x - 1] >= '2')
                return 1;
            return 0;
        }
        if (x1 == x - 1 && y1 == y - 1 && map_shahe[y - 1][x - 1] >= '2')
            return 1;
        if (x1 == x + 1 && y1 == y - 1 && map_shahe[y - 1][x] >= '2')
            return 1;
        if (x1 == x + 1 && y1 == y + 1 && map_shahe[y][x] >= '2')
            return 1;
        if (x1 == x - 1 && y1 == y + 1 && map_shahe[y][x - 1] >= '2')
            return 1;
        return 0;
    }

    if (campus == 2 && strategy == TRAFFIC_TOOL_SHORTEST) {
        int y = source / map_len_xitucheng;
        int x = source % map_len_xitucheng;
        int y1 = dest / map_len_xitucheng;
        int x1 = dest % map_len_xitucheng;
        if (x1 == x && y1 == y - 1) {
            if (map_xitucheng[y - 1][x] >= '2')
                return 1;
            else if (x - 1 >= 0 && map_xitucheng[y - 1][x - 1] >= '2')
                return 1;
            return 0;
        }
        if (x1 == x + 1 && y1 == y) {
            if (map_xitucheng[y][x] >= '2')
                return 1;
            else if (y - 1 >= 0 && map_xitucheng[y - 1][x] >= '2')
                return 1;
            return 0;
        }
        if (x1 == x && y1 == y + 1) {
            if (map_xitucheng[y][x] >= '2')
                return 1;
            else if (x - 1 >= 0 && map_xitucheng[y][x - 1] >= '2')
                return 1;
            return 0;
        }
        if (x1 == x - 1 && y1 == y) {
            if (map_xitucheng[y][x - 1] >= '2')
                return 1;
            else if (y - 1 >= 0 && map_xitucheng[y - 1][x - 1] >= '2')
                return 1;
            return 0;
        }
        if (x1 == x - 1 && y1 == y - 1 && map_xitucheng[y - 1][x - 1] >= '2')
            return 1;
        if (x1 == x + 1 && y1 == y - 1 && map_xitucheng[y - 1][x] >= '2')
            return 1;
        if (x1 == x + 1 && y1 == y + 1 && map_xitucheng[y][x] >= '2')
            return 1;
        if (x1 == x - 1 && y1 == y + 1 && map_xitucheng[y][x - 1] >= '2')
            return 1;
        return 0;
    }
    return -1;
}
//判断两点之间是否可达

int step_cost(int p1, int p2, int strategy, int campus) {

    if (campus != 1 && campus != 2) {
        cout << "<error8>: campus can only be 1 or 2!\n";
        return -1;
    }

    int map_len = -1;
    if (campus == 1)
        map_len = map_len_shahe;
    else
        map_len = map_len_xitucheng;

    //最短距离策略
    if (strategy == DISTANCE_SHORTEST) {
        if (p1 == p2)
            return 0;
        if (p1 / map_len == p2 / map_len || p1 % map_len == p2 % map_len)
            return 10;
        if (abs(p1 / map_len - p2 / map_len) == 1 && abs(p1 % map_len - p2 % map_len) == 1)
            return 14;
        else {
            cout << "<error7>: p1 and p2 are not adjcent!\n";
            return 0;
        }
    }

    //最短时间策略、交通工具最短时间策略
    if (strategy == TIME_SHORTEST || strategy == TRAFFIC_TOOL_SHORTEST) {
        if (p1 == p2)
            return 0;
        else {
            if (campus == 1) {
                if (p1 / map_len == p2 / map_len || p1 % map_len == p2 % map_len)
                    return 10 * (cost_shahe[p1 / map_len][p1 % map_len] - '0');
                else if (abs(p1 / map_len - p2 / map_len) == 1 && abs(p1 % map_len - p2 % map_len) == 1)
                    return 14 * (cost_shahe[p1 / map_len][p1 % map_len] - '0');
            }
            else if (campus == 2) {
                if (p1 / map_len == p2 / map_len || p1 % map_len == p2 % map_len)
                    return 10 * (cost_xitucheng[p1 / map_len][p1 % map_len] - '0');
                else if (abs(p1 / map_len - p2 / map_len) == 1 && abs(p1 % map_len - p2 % map_len) == 1)
                    return 14 * (cost_xitucheng[p1 / map_len][p1 % map_len] - '0');
            }
        }
    }

    cout << "<error9>: not a step cost!\n";
    return -1;
}
//计算从点P1到点P2的单步代价

int search_neighbor(int cur, int* neighbor, int campus) {
    if (campus != 1 && campus != 2) {
        cout << "<error9>: campus can only be 1 or 2!\n";
        return -1;
    }
    int map_len, map_wid;
    map_len = (campus == 1) ? map_len_shahe : map_len_xitucheng;
    map_wid = (campus == 1) ? map_wid_shahe : map_wid_xitucheng;

    int y = cur / map_len;
    int x = cur % map_len;
    int i = 0;
    if (x - 1 >= 0)
        neighbor[i++] = S_num(x - 1, y, campus);
    if (y - 1 >= 0)
        neighbor[i++] = S_num(x, y - 1, campus);
    if (x + 1 < map_len)
        neighbor[i++] = S_num(x + 1, y, campus);
    if (y + 1 < map_wid)
        neighbor[i++] = S_num(x, y + 1, campus);
    if (x - 1 >= 0 && y - 1 >= 0)
        neighbor[i++] = S_num(x - 1, y - 1, campus);
    if (x + 1 < map_len && y - 1 >= 0)
        neighbor[i++] = S_num(x + 1, y - 1, campus);
    if (x - 1 >= 0 && y + 1 < map_wid)
        neighbor[i++] = S_num(x - 1, y + 1, campus);
    if (x + 1 < map_len && y + 1 < map_wid)
        neighbor[i++] = S_num(x + 1, y + 1, campus);
    return i;
}
//搜索当前节点的邻居,返回邻居个数

int find_min_Fcost_point(int* open_list, int* open_F, int campus) {
    if (campus != 1 && campus != 2) {
        cout << "<error10>：campus can only be 1 or 2!\n";
        return -1;

    }
    int map_len = (campus == 1) ? map_len_shahe : map_len_xitucheng;
    int map_wid = (campus == 1) ? map_wid_shahe : map_wid_xitucheng;
    int F_min_Point;
    int min_F;
    int i;
    for (i = 0; i < map_len * map_wid; i++)
    {
        if (open_list[i] == 1) {
            F_min_Point = i;
            min_F = open_F[i];
        }
    }
    for (i = 0; i < map_len * map_wid; i++) {
        if (open_list[i] == 1) {
            if (open_F[i] < min_F) {
                F_min_Point = i;
                min_F = open_F[i];
            }
        }
    }
    return F_min_Point;
}
//找到并返回open_list中F值最小的节点序号

int H_Cost(int cur, int end, int strategy, int campus) {
    if (campus != 1 && campus != 2) {
        cout << "<error11>: campus can only be 1 or 2!\n";
        return -1;
    }

    int map_len = (campus == 1) ? map_len_shahe : map_len_xitucheng;
    int map_wid = (campus == 1) ? map_wid_shahe : map_wid_xitucheng;

    //距离最短策略估计当前节点到终点距离
    if (strategy == DISTANCE_SHORTEST)
        return 10 * abs(cur / map_len - end / map_len) + 10 * abs(cur % map_len - end % map_len);

    if (strategy == TIME_SHORTEST || strategy == TRAFFIC_TOOL_SHORTEST) {
        int x0 = cur % map_len;
        int y0 = cur / map_len;
        int x1 = end % map_len;
        int y1 = end / map_len;

        int cost_H = (campus == 1) ? (cost_shahe[y0][x0] - '0') : (cost_xitucheng[y0][x0] - '0');//水平代价
        int cost_V = (campus == 1) ? (cost_shahe[y1][x1] - '0') : (cost_xitucheng[y1][x1] - '0');//垂直代价
        return 10 * abs(x1 - x0) * cost_H + 10 * abs(y1 - y0) * cost_V;
    }
    return -1;
}
//用海明顿距离估计当前节点到终点的距离

int G_Cost(int cur, int start, int* parent, int strategy, int campus) {
    if (campus != 1 && campus != 2) {
        cout << "<error12>: campus can only be 1 or 2!\n";
        return -1;
    }
    int map_len = (campus == 1) ? map_len_shahe : map_len_xitucheng;
    int map_wid = (campus == 1) ? map_wid_shahe : map_wid_xitucheng;

    int cost = 0;
    if (strategy == DISTANCE_SHORTEST) {
        while (cur != start) {
            int par = parent[cur];
            if (parent[cur] != -1 && parent[cur] != cur) {
                if (par / map_len == cur / map_len || par % map_len == cur % map_len)
                    cost += 10;
                else
                    cost += 14;
            }
            cur = par;
        }
    }
    else if (strategy == TIME_SHORTEST || strategy == TRAFFIC_TOOL_SHORTEST) {
        //沙河校区
        if (campus == 1) {
            while (cur != start) {
                int par = parent[cur];
                if (parent[cur] != -1 && parent[cur] != cur) {
                    if (par / map_len == cur / map_len || par % map_len == cur % map_len)
                        cost += 10 * (cost_shahe[par / map_len][par % map_len] - '0');
                    else
                        cost += 14 * (cost_shahe[par / map_len][par % map_len] - '0');
                }
                cur = par;
            }
        }

        //西土城校区
        else if (campus == 2) {
            while (cur != start) {
                int par = parent[cur];
                if (parent[cur] != -1 && parent[cur] != cur) {
                    if (par / map_len == cur / map_len || par % map_len == cur % map_len)
                        cost += 10 * cost_xitucheng[par / map_len][par % map_len];
                    else
                        cost += 14 * cost_xitucheng[par / map_len][par % map_len];
                }
                cur = par;
            }
        }
    }
    return cost;
}
//计算从起点到当前节点的代价

int AStar_Search(int start, int end, int strategy, int* Path, int* COST, int campus) {
    if (campus != 1 && campus != 2) {
        cout << "<error13>: campus can only be 1 or 2!\n";
        return -1;
    }
    int map_len = (campus == 1) ? map_len_shahe : map_len_xitucheng;
    int map_wid = (campus == 1) ? map_wid_shahe : map_wid_xitucheng;
    int max_size = map_len_shahe * map_wid_xitucheng;

    int open_list[map_len_shahe * map_wid_xitucheng];//记录将要探索的节点
    int close_list[map_len_shahe * map_wid_xitucheng];//记录已经探索过的节点
    int parent[map_len_shahe * map_wid_xitucheng];//记录每个节点的父节点
    int open_F[map_len_shahe * map_wid_xitucheng], open_G[map_len_shahe * map_wid_xitucheng], open_H[map_len_shahe * map_wid_xitucheng];//记录节点的F值、G值、H值
    int neighbor[8];//保存每个节点的邻居
    int open_size = 0, close_size = 0, open_sign = 0;//open_size、close_size分别记录open_list、close_list中节点个数

    initial(open_list, max_size);
    initial(close_list, max_size);
    initial(parent, max_size);
    initial(open_F, max_size);
    initial(open_G, max_size);
    initial(open_H, max_size);
    initial(neighbor, 8);
    //将各表项元素置为-1

    open_list[start] = 1;
    open_size++;
    //将起点加入open_list

    //open_list不能为空，当终点被加入open_list时停止探索
    while (open_size != 0 && open_list[end] == -1) {
        int Cur = find_min_Fcost_point(open_list, open_F, campus);
        //遍历open_list,找到F最小的节点，并将其设为当前节点
        open_list[Cur] = -1;//将当前节点从open_list移除
        open_size--;//更新open_size
        close_list[Cur] = 1;//将Cur移到close_list
        close_size++;
        int num = search_neighbor(Cur, neighbor, campus);
        for (int i = 0; i < num; i++)
        {
            int temp = neighbor[i];
            if (reachable(Cur, temp, campus, strategy) == 0 || close_list[temp] == 1)
                ;//若该方格不可抵达或该方格在close_list中，则忽略
            else if (open_list[temp] == -1) {
                open_list[temp] = 1;//将该方格加入open_list
                open_size++;
                parent[temp] = Cur;//将该节点的父节点设为Cur
                open_H[temp] = H_Cost(temp, end, strategy, campus);
                open_G[temp] = G_Cost(temp, start, parent, strategy, campus);
                open_F[temp] = open_H[temp] + open_G[temp];
                //计算该方格的F、G、H值
            }
            else if (open_list[temp] == 1) {
                int G_temp = G_Cost(Cur, start, parent, strategy, campus) + step_cost(Cur, temp, strategy, campus);//计算从Cur到该节点的G值
                if (G_temp < open_G[temp]) {
                    //如果小于原来G值，则更新该节点的父节点为Cur
                    parent[temp] = Cur;
                    open_G[temp] = G_temp;
                    open_F[temp] = open_G[temp] + open_H[temp];
                    //更新该节点的G值和F值
                }
            }
        }
    }
    if (open_list[end] == 1) {
        //当终点位于open_list，则返回路线
        //int path[100];
        int cost = 0;
        int num = get_path(end, start, parent, &cost, Path, strategy, campus);//返回路径上节点个数
        cout << "cost:" << cost << "\n";
        *COST = cost;
        //    print_path(Path, num,campus);
        return num;
    }
    else {
        //当终点不在open_list,寻路失败
        cout << "no path available\n";
        return 0;
    }

}
int get_path(int end, int start, int* parent, int* cost, int* path, int strategy, int campus) {
    *cost = 0;
    int size = 0;
    int cur = end;
    int cost_distance = 0;//记录距离代价
    while (cur != start) {
        cur = parent[cur];
        size++;
    }
    int num = size + 1;
    for (cur = end; cur != start; cur = parent[cur]) {
        path[size--] = cur;
        cost_distance += step_cost(parent[cur], cur, DISTANCE_SHORTEST, campus);
        *cost += step_cost(parent[cur], cur, strategy, campus);
    }
    path[0] = start;

    if (strategy == TIME_SHORTEST) {
        double busy_level = (double)(*cost) / cost_distance;//计算路径平均拥挤度
        double time = (double)cost_distance / (double)(man_speed * busy_level);//计算平均时间
        *cost = 10 * (int)time;
    }

    if (strategy == TRAFFIC_TOOL_SHORTEST) {
        double busy_level = (double)(*cost) / cost_distance;//计算拥挤度
        double time = (double)cost_distance / (double)(bycicle_speed * busy_level);
        *cost = 10 * (int)time;
    }

    return num;
}

//从起点到终点连成最短路线
void print_path(int* path, int size, int campus) {
    int map_len = (campus == 1) ? map_len_shahe : map_len_xitucheng;
    int j = 0;
    for (int i = size - 1; i >= 0; i--, j++) {
        cout << "(" << path[i] % map_len << "," << path[i] / map_len << ")\t";
        if ((j + 1) % 8 == 0)
            cout << "\n";
    }
    cout << "\n";
}
//打印路径

int inlist(int* list, int sign, int size) {
    for (int i = 0; i < size; i++)
        if (list[i] == sign)
            return 1;
    return 0;
}
//从list中查找值为sign的元素


int writes_path_into_file(int* path) {
    ofstream fp(path_file_name, ios::out | ios::trunc);
    if (fp.fail()) {
        cout << "<error>：file <data.txt> open failed!\n";
        return -1;
    }

    //若起点在沙河校区
    if (start_campus == shahe) {
        fp << shahe << "\n";//写入起点所在校区
        fp << path_pause << "\n";//写入沙河校区坐标总数

        //若路径在单校区
        if (end_campus == -1) {
            fp << 0 << "\n";//西土城坐标数为0
            for (int i = 0; i < path_pause; i++)
                fp << path[i] << "\n";
        }

        //若路径在两个校区
        else {
            fp << path_end << "\n";
            int i;
            for (i = 0; i < path_pause; i++)
                fp << path[i] << "\n";
            for (int j = 0; j < path_end; j++, i++)
                fp << path[i] << "\n";
        }
    }

    //若起点在西土城校区
    else {
        fp << xitucheng << "\n";

        //若路径仅在西土城校区
        if (end_campus == -1) {
            fp << 0 << "\n";
            fp << path_pause << "\n";
            for (int i = 0; i < path_pause; i++)
                fp << path[i] << "\n";
        }

        //若路径在两个校区
        else {
            fp << path_end << "\n";
            fp << path_pause << "\n";
            int i;
            for (i = 0; i < path_pause; i++)
                fp << path[i] << "\n";

            for (int j = 0; j < path_end; i++, j++)
                fp << path[i] << "\n";

        }
    }
    return 0;
}




string shahe_position_building_match(int tempx, int tempy) {
    struct building_campus_position temp;
    ifstream f(building_campus_position_file);
    while (f) {
        getline(f, temp.building, ' ');
        f >> temp.campus;
        f >> temp.x;
        f >> temp.y;
        string buf;
        getline(f, buf);
       // f.seekg(sizeof(char), ios::cur);
        if (temp.x == tempx && temp.y == tempy && temp.campus == '1')
            return temp.building;
    }
    return "null";

}
string xitucheng_position_building_match(int tempx, int tempy) {
    struct building_campus_position temp;
    ifstream f(building_campus_position_file);
    while (f) {
        getline(f, temp.building, ' ');
        f >> temp.campus;
        f >> temp.x;
        f >> temp.y;
        string buf;
        getline(f, buf);
       // f.seekg(sizeof(char), ios::cur);
        if (temp.x == tempx && temp.y == tempy && temp.campus == '2')
            return temp.building;
    }
    return "null";

}

vector<struct building_position>* shahe_locate(int cur_x, int cur_y, int cell_range) {
    int range = (cell_range / 15 + 1)*5;
    vector<struct building_position>* building_name = new vector<struct building_position>;
    int map_flag[shahe_map_wid][shahe_map_len];
    for (int i = 0; i < shahe_map_wid; i++) {
        for (int j = 0; j < shahe_map_len; j++)
            map_flag[i][j] = 65535;
    }
    struct cqueue C;
    struct cqueue* Cptr = &C;
    incqueue(cur_y * shahe_map_len + cur_x, Cptr);
    map_flag[cur_y][cur_x] = 0;
    while (!is_cqueue_empty(Cptr)) {
        //处理传入参数
        int tempseq = decqueue(Cptr);
        int tempx = tempseq % shahe_map_len;
        int tempy = tempseq / shahe_map_len;
        //处理左侧
        if (tempx - 1 > 0 && map_shahe[tempy][tempx - 1] != '0') {
            int step = map_flag[tempy][tempx] + 10;
            if (step < range && step < map_flag[tempy][tempx - 1]) {
                incqueue(tempy * shahe_map_len + tempx - 1, Cptr);
                map_flag[tempy][tempx - 1] = step;
                if (map_shahe[tempy][tempx - 1] == '3') {
                    string temp_name = shahe_position_building_match(tempx - 1, tempy);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy;
                        p.x = tempx - 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理右侧
        if (tempx + 1 < shahe_map_len && map_shahe[tempy][tempx + 1] != '0') {
            int step = map_flag[tempy][tempx] + 10;
            if (step < range && step < map_flag[tempy][tempx + 1]) {
                incqueue(tempy * shahe_map_len + tempx + 1, Cptr);
                map_flag[tempy][tempx + 1] = step;
                if (map_shahe[tempy][tempx + 1] == '3') {
                    string temp_name = shahe_position_building_match(tempx + 1, tempy);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy;
                        p.x = tempx + 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理上侧
        if (tempy - 1 > 0 && map_shahe[tempy - 1][tempx] != '0') {
            int step = map_flag[tempy][tempx] + 10;
            if (step < range && step < map_flag[tempy - 1][tempx]) {
                incqueue((tempy - 1) * shahe_map_len + tempx, Cptr);
                map_flag[tempy - 1][tempx] = step;
                if (map_shahe[tempy - 1][tempx] == '3') {
                    string temp_name = shahe_position_building_match(tempx, tempy - 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy - 1;
                        p.x = tempx;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理下侧
        if (tempy + 1 < shahe_map_wid && map_shahe[tempy + 1][tempx] != '0') {
            int step = map_flag[tempy][tempx] + 10;
            if (step < range && step < map_flag[tempy + 1][tempx]) {
                incqueue((tempy + 1) * shahe_map_len + tempx, Cptr);
                map_flag[tempy + 1][tempx] = step;
                if (map_shahe[tempy + 1][tempx] == '3') {
                    string temp_name = shahe_position_building_match(tempx, tempy + 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy + 1;
                        p.x = tempx;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理左下侧
        if (tempx - 1 > 0 && tempy + 1 < shahe_map_wid && map_shahe[tempy + 1][tempx - 1] != '0') {
            int step = map_flag[tempy][tempx] + 14;
            if (step < range && step < map_flag[tempy + 1][tempx - 1]) {
                incqueue((tempy + 1) * shahe_map_len + (tempx - 1), Cptr);
                map_flag[tempy + 1][tempx - 1] = step;
                if (map_shahe[tempy + 1][tempx - 1] == '3') {
                    string temp_name = shahe_position_building_match(tempx - 1, tempy + 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy + 1;
                        p.x = tempx - 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理左上侧
        if (tempx - 1 > 0 && tempy - 1 > 0 && map_shahe[tempy - 1][tempx - 1] != '0') {
            int step = map_flag[tempy][tempx] + 14;
            if (step < range && step < map_flag[tempy - 1][tempx - 1]) {
                incqueue((tempy - 1) * shahe_map_len + tempx - 1, Cptr);
                map_flag[tempy - 1][tempx - 1] = step;
                if (map_shahe[tempy - 1][tempx - 1] == '3') {
                    string temp_name = shahe_position_building_match(tempx - 1, tempy - 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy - 1;
                        p.x = tempx - 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理右上侧
        if (tempx + 1 < shahe_map_len && tempy - 1 > 0 && map_shahe[tempy - 1][tempx + 1] != '0') {
            int step = map_flag[tempy][tempx] + 14;
            if (step < range && step < map_flag[tempy - 1][tempx + 1]) {
                incqueue((tempy - 1) * shahe_map_len + tempx + 1, Cptr);
                map_flag[tempy - 1][tempx + 1] = step;
                if (map_shahe[tempy - 1][tempx + 1] == '3') {
                    string temp_name = shahe_position_building_match(tempx + 1, tempy - 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy - 1;
                        p.x = tempx + 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理右下侧
        if (tempx + 1 < shahe_map_len && tempy + 1 < shahe_map_wid && map_shahe[tempy + 1][tempx + 1] != '0') {
            int step = map_flag[tempy][tempx] + 14;
            if (step < range && step < map_flag[tempy + 1][tempx + 1]) {
                incqueue((tempy + 1) * shahe_map_len + tempx + 1, Cptr);
                map_flag[tempy + 1][tempx + 1] = step;
                if (map_shahe[tempy + 1][tempx + 1] == '3') {
                    string temp_name = shahe_position_building_match(tempx + 1, tempy + 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy + 1;
                        p.x = tempx + 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
    }
    return building_name;//注意这个是用new的，没有释放；
}

vector<struct building_position>* xitucheng_locate(int cur_x, int cur_y, int cell_range) {
    int range = (cell_range / 15 + 1)*5;
    vector<struct building_position>* building_name = new vector<struct building_position>;
    int map_flag[xitucheng_map_wid][xitucheng_map_len];
    for (int i = 0; i < xitucheng_map_wid; i++) {
        for (int j = 0; j < xitucheng_map_len; j++)
            map_flag[i][j] = 65535;
    }
    struct cqueue C;
    struct cqueue* Cptr = &C;
    incqueue((cur_y * xitucheng_map_len) + cur_x, Cptr);
    map_flag[cur_y][cur_x] = 0;
    while (!is_cqueue_empty(Cptr)) {
        //处理传入参数
        int tempseq = decqueue(Cptr);
        int tempx = tempseq % xitucheng_map_len;
        int tempy = tempseq / xitucheng_map_len;
        //处理左侧
        if (tempx - 1 > 0 && map_xitucheng[tempy][tempx - 1] != '0') {
            int step = map_flag[tempy][tempx] + 10;
            if (step < range && step < map_flag[tempy][tempx - 1]) {
                incqueue(tempy * xitucheng_map_len + tempx - 1, Cptr);
                map_flag[tempy][tempx - 1] = step;
                if (map_xitucheng[tempy][tempx - 1] == '3') {
                    string temp_name = xitucheng_position_building_match(tempx - 1, tempy);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy;
                        p.x = tempx - 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理右侧
        if (tempx + 1 < xitucheng_map_len && map_xitucheng[tempy][tempx + 1] != '0') {
            int step = map_flag[tempy][tempx] + 10;
            if (step < range && step < map_flag[tempy][tempx + 1]) {
                incqueue(tempy * xitucheng_map_len + tempx + 1, Cptr);
                map_flag[tempy][tempx + 1] = step;
                if (map_xitucheng[tempy][tempx + 1] == '3') {
                    string temp_name = xitucheng_position_building_match(tempx + 1, tempy);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy;
                        p.x = tempx + 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理上侧
        if (tempy - 1 > 0 && map_xitucheng[tempy - 1][tempx] != '0') {
            int step = map_flag[tempy][tempx] + 10;
            if (step < range && step < map_flag[tempy - 1][tempx]) {
                incqueue((tempy - 1) * xitucheng_map_len + tempx, Cptr);
                map_flag[tempy - 1][tempx] = step;
                if (map_xitucheng[tempy - 1][tempx] == '3') {
                    string temp_name = xitucheng_position_building_match(tempx, tempy - 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy - 1;
                        p.x = tempx;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理下侧
        if (tempy + 1 < xitucheng_map_wid && map_xitucheng[tempy + 1][tempx] != '0') {
            int step = map_flag[tempy][tempx] + 10;
            if (step < range && step < map_flag[tempy + 1][tempx]) {
                incqueue((tempy + 1) * xitucheng_map_len + tempx, Cptr);
                map_flag[tempy + 1][tempx] = step;
                if (map_xitucheng[tempy + 1][tempx] == '3') {
                    string temp_name = xitucheng_position_building_match(tempx, tempy + 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy + 1;
                        p.x = tempx;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理左下侧
        if (tempx - 1 > 0 && tempy + 1 < xitucheng_map_wid && map_xitucheng[tempy + 1][tempx - 1] != '0') {
            int step = map_flag[tempy][tempx] + 14;
            if (step < range && step < map_flag[tempy + 1][tempx - 1]) {
                incqueue((tempy + 1) * xitucheng_map_len + tempx - 1, Cptr);
                map_flag[tempy + 1][tempx - 1] = step;
                if (map_xitucheng[tempy + 1][tempx - 1] == '3') {
                    string temp_name = xitucheng_position_building_match(tempx - 1, tempy + 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy + 1;
                        p.x = tempx - 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理左上侧
        if (tempx - 1 > 0 && tempy - 1 > 0 && map_xitucheng[tempy - 1][tempx - 1] != '0') {
            int step = map_flag[tempy][tempx] + 14;
            if (step < range && step < map_flag[tempy - 1][tempx - 1]) {
                incqueue((tempy - 1) * xitucheng_map_len + tempx - 1, Cptr);
                map_flag[tempy - 1][tempx - 1] = step;
                if (map_xitucheng[tempy - 1][tempx - 1] == '3') {
                    string temp_name = xitucheng_position_building_match(tempx - 1, tempy - 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy - 1;
                        p.x = tempx - 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理右上侧
        if (tempx + 1 < xitucheng_map_len && tempy - 1 > 0 && map_xitucheng[tempy - 1][tempx + 1] != '0') {
            int step = map_flag[tempy][tempx] + 14;
            if (step < range && step < map_flag[tempy - 1][tempx + 1]) {
                incqueue((tempy - 1) * xitucheng_map_len + tempx + 1, Cptr);
                map_flag[tempy - 1][tempx + 1] = step;
                if (map_xitucheng[tempy - 1][tempx + 1] == '3') {
                    string temp_name = xitucheng_position_building_match(tempx + 1, tempy - 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy - 1;
                        p.x = tempx + 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
        //处理右下侧
        if (tempx + 1 < xitucheng_map_len && tempy + 1 < xitucheng_map_wid && map_xitucheng[tempy + 1][tempx + 1] != '0') {
            int step = map_flag[tempy][tempx] + 14;
            if (step < range && step < map_flag[tempy + 1][tempx + 1]) {
                incqueue((tempy + 1) * xitucheng_map_len + tempx + 1, Cptr);
                map_flag[tempy + 1][tempx + 1] = step;
                if (map_xitucheng[tempy + 1][tempx + 1] == '3') {
                    string temp_name = xitucheng_position_building_match(tempx + 1, tempy + 1);
                    if (temp_name != "null") {
                        struct building_position p;
                        p.building = temp_name;
                        p.range = step;
                        p.y = tempy + 1;
                        p.x = tempx + 1;
                        building_name->push_back(p);
                    }
                    else cout << "出现未在表中出现的建筑，错误";
                }
            }
        }
    }
    return building_name;//注意这个是用new的，没有释放；
}



void upper_locate(int x, int y, int range, int campus, vector<string>* locate_building_name) {
    vector<struct building_position>* result;
    if (campus == 1) {
        result = shahe_locate(x, y, range);
        for (int i = 0; i < (*result).size(); i++)
            locate_building_name->push_back((*result)[i].building);
    }
    if (campus == 2) {
        result = xitucheng_locate(x, y, range);
        for (int i = 0; i < (*result).size(); i++)
            locate_building_name->push_back((*result)[i].building);
    }

}




int get_school_bus(int now_time) {
    ifstream f(bus_time);
    string temp;
    int pre_result = 0;
    int now_result = 0;
    while (!f.eof()) {
        int i = 0;
        int hour = 0;
        int minute = 0;
        f >> temp;
        while (temp[i] != ':')
            hour = hour * 10 + temp[i++] - '0';
        i++;
        while (temp[i] != '\0')
            minute = minute * 10 + temp[i++] - '0';
        pre_result = now_result;
        now_result = hour * 60 + minute;
        if ((pre_result <= now_time) && (now_result >= now_time))
            return now_result;
    }
    return 0;
}

int* physical_navigation_min_time(string name, int start, int campus_now, int& time_cost) {
    time_t t;
    time(&t);
    struct building_campus_position temp;
    position_building_match(name, &temp);
    int end = 0;
    if (temp.campus == '1')
        end = temp.y * shahe_map_len + temp.x;
    else
        end = temp.y * xitucheng_map_len + temp.x;

    struct tm* info = gmtime(&t);
    int now_min = (info->tm_hour + 8) * 60 + info->tm_min;
    if (temp.campus == campus_now) {
        if (campus_now == '1') {
            int cost;
            int* path = new int[1000];
            path_pause = AStar_Search(start, end, 1, path, &cost, 1);
            start_campus = 1;
            end_campus = -1;
            path_end = -1;
            time_cost = cost;
            return path;
        }
        int cost;
        int* path = new int[1000];
        path_pause = AStar_Search(start, end, 1, path, &cost, 2);
        start_campus = 2;
        end_campus = -1;
        path_end = -1;
        time_cost = cost;
        return path;
    }
    else {
        int* path1 = new int[500];
        int* path2 = new int[500];
        int* path3 = new int[500];
        int* path4 = new int[500];
        int bus_shahe_cost;
        int bus_xitucheng_cost;
        int school_bus_shahe_cost;
        int school_bus_xitucheng_cost;
        int bus_shahe_length;
        int bus_xitucheng_length;
        int school_bus_shahe_length;
        int school_bus_xitucheng_length;
        if (campus_now == '1') {//如果起点在沙河
            bus_shahe_length = AStar_Search(start, shahe_bus_station, 1, path1, &bus_shahe_cost, 1);
            bus_xitucheng_length = AStar_Search(xitucheng_bus_station, temp.y * xitucheng_map_len + temp.x, 1, path2, &bus_xitucheng_cost, 2);
            school_bus_shahe_length = AStar_Search(start, shahe_school_bus_station, 1, path3, &school_bus_shahe_cost, 1);
            school_bus_xitucheng_length = AStar_Search(xitucheng_school_bus_station, temp.y * xitucheng_map_len + temp.x, 1, path4, &school_bus_xitucheng_cost, 2);

            int result;
            result = get_school_bus(now_min + school_bus_shahe_cost/60);
            if (result != 0)
                result -= now_min;//到达校车点和等待发车所需时间
            int actual_bus = 0;
            actual_bus = bus_shahe_cost + bus_xitucheng_cost + 300 + 4200;

            if (result != 0 && (school_bus_shahe_cost + result*60 + school_bus_xitucheng_cost + 2400) < actual_bus) {
                //若校车时间较短。
                int* path_sum = new int[school_bus_shahe_length + school_bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < school_bus_shahe_length; i++)
                    path_sum[index++] = path3[i];
                for (int j = 0; j < school_bus_xitucheng_length; j++)
                    path_sum[index++] = path4[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = school_bus_shahe_length;
                path_end = school_bus_xitucheng_length;
                start_campus = 1;
                end_campus = 2;
                time_cost = school_bus_shahe_cost + result + school_bus_xitucheng_cost + 40;
                return path_sum;
            }
            else {
                int* path_sum = new int[bus_shahe_length + bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < bus_shahe_length; i++)
                    path_sum[index++] = path1[i];
                for (int j = 0; j < bus_xitucheng_length; j++)
                    path_sum[index++] = path2[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = bus_shahe_length;
                path_end = bus_xitucheng_length;
                start_campus = 1;
                end_campus = 2;
                time_cost = actual_bus;
                return path_sum;
            }


        }
        else {//如果起点在西土城
            bus_xitucheng_length = AStar_Search(start, xitucheng_bus_station, 1, path1, &bus_xitucheng_cost, 2);
            bus_shahe_length = AStar_Search(shahe_bus_station, temp.y * shahe_map_len + temp.x, 1, path2, &bus_shahe_cost, 1);
            school_bus_xitucheng_length = AStar_Search(start, xitucheng_school_bus_station, 1, path3, &school_bus_xitucheng_cost, 2);
            school_bus_shahe_length = AStar_Search(shahe_school_bus_station, temp.y * shahe_map_len + temp.x, 1, path4, &school_bus_shahe_cost, 1);

            int result;
            result = get_school_bus(now_min + school_bus_xitucheng_cost/60);
            if (result != 0)
                result -= now_min;
            int actual_bus = 0;
            actual_bus = bus_shahe_cost + bus_xitucheng_cost + 300 + 4200;

            if (result != 0 && (school_bus_shahe_cost + result*60 + school_bus_xitucheng_cost + 2400) < actual_bus) {
                //校车时间较短
                int* path_sum = new int[school_bus_shahe_length + school_bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < school_bus_xitucheng_length; i++)
                    path_sum[index++] = path3[i];
                for (int j = 0; j < school_bus_shahe_length; j++)
                    path_sum[index++] = path4[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = school_bus_xitucheng_length;
                path_end = school_bus_shahe_length;
                start_campus = 2;
                end_campus = 1;
                time_cost = school_bus_shahe_cost/60 + result + school_bus_xitucheng_cost/60 + 40;
                return path_sum;
            }
            else {
                int* path_sum = new int[bus_shahe_length + bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < bus_xitucheng_length; i++)
                    path_sum[index++] = path1[i];
                for (int j = 0; j < bus_shahe_length; j++)
                    path_sum[index++] = path2[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = bus_xitucheng_length;
                path_end = bus_shahe_length;
                start_campus = 2;
                end_campus = 1;
                time_cost = actual_bus;
                return path_sum;
            }
        }
    }

}
int* upper_time_min(string start, string dest) {
    int cost_time;
    struct building_campus_position temp;
    position_building_match(start, &temp);
    int width = 0;
    if (temp.campus == '1')
        width = shahe_map_len;
    else width = xitucheng_map_len;
    int* path = physical_navigation_min_time(dest, temp.y * width + temp.x, temp.campus, cost_time);
    int minute = cost_time / 60;
    int second = cost_time % 60;
    cout << "预计花费时间" << minute << "分" << second << "秒" << endl;
    return path;
}
int* upper_time_min(int* data, string dest) {
    int cost_time;
    int width = 0;
    if (data[0] == 1)
        width = shahe_map_len;
    else width = xitucheng_map_len;
    int* path = physical_navigation_min_time(dest, data[2]/10 * width + data[1]/10, data[0]+'0', cost_time);
    int minute = cost_time / 60;
    int second = cost_time % 60;
    cout << "预计花费时间" << minute << "分" << second << "秒" << endl;
    return path;
}


//不需传入参数
//int*path=upper_time_min(start, dest);




int* by_navigation_min_time(string name, int start, int campus_now, int& time_cost) {
    time_t t;
    time(&t);
    struct building_campus_position temp;
    position_building_match(name, &temp);
    int end = 0;
    if (temp.campus == '1')
        end = temp.y * shahe_map_len + temp.x;
    else
        end = temp.y * xitucheng_map_len + temp.x;

    struct tm* info = gmtime(&t);
    int now_min = (info->tm_hour + 8) * 60 + info->tm_min;
    if (temp.campus == campus_now) {
        if (campus_now == '1') {
            int cost;
            int* path = new int[1000];
            path_pause = AStar_Search(start, end, TRAFFIC_TOOL_SHORTEST, path, &cost, 1);
            start_campus = 1;
            end_campus = -1;
            path_end = -1;
            time_cost = cost;
            return path;
        }
        int cost;
        int* path = new int[1000];
        path_pause = AStar_Search(start, end, TRAFFIC_TOOL_SHORTEST, path, &cost, 2);
        start_campus = 2;
        end_campus = -1;
        path_end = -1;
        time_cost = cost;
        return path;
    }
    else {
        int* path1 = new int[500];
        int* path2 = new int[500];
        int* path3 = new int[500];
        int* path4 = new int[500];
        int bus_shahe_cost;
        int bus_xitucheng_cost;
        int school_bus_shahe_cost;
        int school_bus_xitucheng_cost;
        int bus_shahe_length;
        int bus_xitucheng_length;
        int school_bus_shahe_length;
        int school_bus_xitucheng_length;
        if (campus_now == '1') {//如果起点在沙河
            bus_shahe_length = AStar_Search(start, shahe_bus_station, TRAFFIC_TOOL_SHORTEST, path1, &bus_shahe_cost, 1);
            bus_xitucheng_length = AStar_Search(xitucheng_bus_station, temp.y * xitucheng_map_len + temp.x, TRAFFIC_TOOL_SHORTEST, path2, &bus_xitucheng_cost, 2);
            school_bus_shahe_length = AStar_Search(start, shahe_school_bus_station, TRAFFIC_TOOL_SHORTEST, path3, &school_bus_shahe_cost, 1);
            school_bus_xitucheng_length = AStar_Search(xitucheng_school_bus_station, temp.y * xitucheng_map_len + temp.x, TRAFFIC_TOOL_SHORTEST, path4, &school_bus_xitucheng_cost, 2);

            int result;
            result = get_school_bus(now_min + school_bus_shahe_cost/60);
            if (result != 0)
                result -= now_min;//到达校车点和等待发车所需时间
            int actual_bus = 0;
            actual_bus = bus_shahe_cost + bus_xitucheng_cost + 300 + 4200;

            if (result != 0 && (school_bus_shahe_cost + result*60 + school_bus_xitucheng_cost + 2400) < actual_bus) {
                //若校车时间较短。
                int* path_sum = new int[school_bus_shahe_length + school_bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < school_bus_shahe_length; i++)
                    path_sum[index++] = path3[i];
                for (int j = 0; j < school_bus_xitucheng_length; j++)
                    path_sum[index++] = path4[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = school_bus_shahe_length;
                path_end = school_bus_xitucheng_length;
                start_campus = 1;
                end_campus = 2;
                time_cost = school_bus_shahe_cost + result + school_bus_xitucheng_cost + 40;
                return path_sum;
            }
            else {
                int* path_sum = new int[bus_shahe_length + bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < bus_shahe_length; i++)
                    path_sum[index++] = path1[i];
                for (int j = 0; j < bus_xitucheng_length; j++)
                    path_sum[index++] = path2[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = bus_shahe_length;
                path_end = bus_xitucheng_length;
                start_campus = 1;
                end_campus = 2;
                time_cost = actual_bus;
                return path_sum;
            }


        }
        else {//如果起点在西土城
            bus_xitucheng_length = AStar_Search(start, xitucheng_bus_station, TRAFFIC_TOOL_SHORTEST, path1, &bus_xitucheng_cost, 2);
            bus_shahe_length = AStar_Search(shahe_bus_station, temp.y * shahe_map_len + temp.x, TRAFFIC_TOOL_SHORTEST, path2, &bus_shahe_cost, 1);
            school_bus_xitucheng_length = AStar_Search(start, xitucheng_school_bus_station, TRAFFIC_TOOL_SHORTEST, path3, &school_bus_xitucheng_cost, 2);
            school_bus_shahe_length = AStar_Search(shahe_school_bus_station, temp.y * shahe_map_len + temp.x, TRAFFIC_TOOL_SHORTEST, path4, &school_bus_shahe_cost, 1);

            int result;
            result = get_school_bus(now_min + school_bus_xitucheng_cost/60);
            if (result != 0)
                result -= now_min;
            int actual_bus = 0;
            actual_bus = bus_shahe_cost + bus_xitucheng_cost + 300 + 4200;

            if (result != 0 && (school_bus_shahe_cost + result*60 + school_bus_xitucheng_cost + 2400) < actual_bus) {
                //校车时间较短
                int* path_sum = new int[school_bus_shahe_length + school_bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < school_bus_xitucheng_length; i++)
                    path_sum[index++] = path3[i];
                for (int j = 0; j < school_bus_shahe_length; j++)
                    path_sum[index++] = path4[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = school_bus_xitucheng_length;
                path_end = school_bus_shahe_length;
                start_campus = 2;
                end_campus = 1;
                time_cost = school_bus_shahe_cost + result + school_bus_xitucheng_cost + 40;
                return path_sum;
            }
            else {
                int* path_sum = new int[bus_shahe_length + bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < bus_xitucheng_length; i++)
                    path_sum[index++] = path1[i];
                for (int j = 0; j < bus_shahe_length; j++)
                    path_sum[index++] = path2[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = bus_xitucheng_length;
                path_end = bus_shahe_length;
                start_campus = 2;
                end_campus = 1;
                time_cost = actual_bus;
                return path_sum;
            }
        }
    }

}
int* by_upper_time_min(string start, string dest) {
    int cost_time;
    struct building_campus_position temp;
    position_building_match(start, &temp);
    int width = 0;
    if (temp.campus == '1')
        width = shahe_map_len;
    else width = xitucheng_map_len;
    int* path = physical_navigation_min_time(dest, temp.y * width + temp.x, temp.campus, cost_time);
    int minute = cost_time / 60;
    int second = cost_time % 60;
    cout << "预计花费时间" << minute << "分" << second << "秒" << endl;
    return path;
}
int* by_upper_time_min(int* data, string dest) {
    int cost_time;
    int width = 0;
    if (data[0] == 1)
        width = shahe_map_len;
    else width = xitucheng_map_len;
    int* path = physical_navigation_min_time(dest, data[2]/10 * width + data[1]/10, data[0] + '0', cost_time);
    int minute = cost_time / 60;
    int second = cost_time % 60;
    cout << "预计花费时间" << minute << "分" << second << "秒" << endl;
    return path;
}





//最短距离
int* physical_navigation_shortest_distance(string name, int start, int campus_now) {
    struct building_campus_position temp;
    position_building_match(name, &temp);
    int end = 0;
    if (temp.campus == '1')
        end = temp.y * shahe_map_len + temp.x;
    else
        end = temp.y * xitucheng_map_len + temp.x;

    if (temp.campus== campus_now) {
        if (campus_now == '1') {
            int cost;
            int* path = new int[1000];
            path_pause = AStar_Search(start, end, 0, path, &cost, 1);
            start_campus = 1;
            path_end = -1;
            end_campus = -1;
            return path;
        }
        int cost;
        int* path = new int[1000];
        path_pause = AStar_Search(start, end, 0, path, &cost, 2);
        start_campus = 2;
        path_end = -1;
        end_campus = -1;
        return path;
    }
    else {
        int* path1 = new int[500];
        int* path2 = new int[500];
        int* path3 = new int[500];
        int* path4 = new int[500];
        int bus_shahe_cost;
        int bus_xitucheng_cost;
        int school_bus_shahe_cost;
        int school_bus_xitucheng_cost;
        int bus_shahe_length;
        int bus_xitucheng_length;
        int school_bus_shahe_length;
        int school_bus_xitucheng_length;
        if (campus_now == '1') {//如果起点在沙河
            bus_shahe_length = AStar_Search(start, shahe_bus_station, 0, path1, &bus_shahe_cost, 1);
            bus_xitucheng_length = AStar_Search(xitucheng_bus_station, temp.y * xitucheng_map_len + temp.x, 0, path2, &bus_xitucheng_cost, 2);
            school_bus_shahe_length = AStar_Search(start, shahe_school_bus_station, 0, path3, &school_bus_shahe_cost, 1);
            school_bus_xitucheng_length = AStar_Search(xitucheng_school_bus_station, temp.y * xitucheng_map_len + temp.x, 0, path4, &school_bus_xitucheng_cost, 2);
            if (bus_xitucheng_cost + bus_shahe_cost > school_bus_xitucheng_cost + school_bus_xitucheng_cost) {
                //若校车距离较短。
                int* path_sum = new int[school_bus_shahe_length + school_bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < school_bus_shahe_length; i++)
                    path_sum[index++] = path3[i];
                for (int j = 0; j < school_bus_xitucheng_length; j++)
                    path_sum[index++] = path4[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = school_bus_shahe_length;
                path_end = school_bus_xitucheng_length;
                start_campus = 1;
                end_campus = 2;
                return path_sum;
            }
            else {
                int* path_sum = new int[bus_shahe_length + bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < bus_shahe_length; i++)
                    path_sum[index++] = path1[i];
                for (int j = 0; j < bus_xitucheng_length; j++)
                    path_sum[index++] = path2[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = bus_shahe_length;
                path_end = bus_xitucheng_length;
                start_campus = 1;
                end_campus = 2;
                return path_sum;
            }
        }
        else {//如果起点在西土城
            bus_xitucheng_length = AStar_Search(start, xitucheng_bus_station, 0, path1, &bus_xitucheng_cost, 2);
            bus_shahe_length = AStar_Search(shahe_bus_station, temp.y * shahe_map_len + temp.x, 0, path2, &bus_shahe_cost, 1);
            school_bus_xitucheng_length = AStar_Search(start, xitucheng_school_bus_station, 0, path3, &school_bus_xitucheng_cost, 2);
            school_bus_shahe_length = AStar_Search(shahe_school_bus_station, temp.y * shahe_map_len + temp.x, 0, path4, &school_bus_shahe_cost, 1);
            if (bus_xitucheng_cost + bus_shahe_cost > school_bus_xitucheng_cost + school_bus_xitucheng_cost) {
                //若校车距离较短。
                int* path_sum = new int[school_bus_shahe_length + school_bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < school_bus_xitucheng_length; i++)
                    path_sum[index++] = path3[i];
                for (int j = 0; j < school_bus_shahe_length; j++)
                    path_sum[index++] = path4[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = school_bus_xitucheng_length;
                path_end = school_bus_shahe_length;
                start_campus = 2;
                end_campus = 1;
                return path_sum;
            }
            else {
                int* path_sum = new int[bus_shahe_length + bus_xitucheng_length];
                int index = 0;
                for (int i = 0; i < bus_xitucheng_length; i++)
                    path_sum[index++] = path1[i];
                for (int j = 0; j < bus_shahe_length; j++)
                    path_sum[index++] = path2[j];
                delete[]path1;
                delete[]path2;
                delete[]path3;
                delete[]path4;
                path_pause = bus_xitucheng_length;
                path_end = bus_shahe_length;
                start_campus = 2;
                end_campus = 1;
                return path_sum;
            }
        }
    }
}
int* upper_shortest_path(string start, string dest) {
    struct building_campus_position temp;
    position_building_match(start, &temp);
    int width = 0;
    if (temp.campus == '1')
        width = shahe_map_len;
    else width = xitucheng_map_len;
    return physical_navigation_shortest_distance(dest, temp.y * width + temp.x, temp.campus);
}
int* upper_shortest_path(int* data, string dest) {
    int width = 0;
    if (data[0] == 1)
        width = shahe_map_len;
    else width = xitucheng_map_len;
    return physical_navigation_shortest_distance(dest, data[2]/10 * width + data[1]/10, data[0]+'0');
}
//不需传入参数
//int*path=upper_shortest_path(start, dest);



//用于优先队列
struct least_lb {
    bool operator()(TSP_node* a, TSP_node* b) {
        return a->lower_bound > b->lower_bound;
    }
};



void proper_element(int& col, int& row, int n, unsigned int** graph);
int initial_h(int number, unsigned int** graph, unsigned int** father_graph);

void greedy(int number, unsigned int** graph, int& cost);
void initial_matrix(int number, unsigned int** temp, struct name_number* position_array, struct number_path* path_array, int);
void destroy_matrix(int number, unsigned int** temp);
void initial_position(int n_x, int n_y, int number, vector<string> name_array, struct name_number* position_array, int);
int get_result(vector<TSP_node*>result, int number, vector<pair<int, int>>& final_result, struct number_path* path_array, unsigned int** graph);
void delete_tree(struct TSP_node* father);
int* get_multi_path_seq(int source_x, int source_y, vector<string> name_array, int number, vector<pair<int, int>>& seq_result, int& sum_length, int campus);
void greedy(int number, unsigned int** graph, int& cost) {
    int flag[100];//是否已经历过。点数需要加上起始点
    for (int i = 0; i < number + 1; i++)
        flag[i] = 0;
    flag[0] = 1;//起始点自动置1
    int done = 1;//从开始
    cost = 0;
    int source = 0;
    while (done < number + 1) {
        int min = inf;
        int min_n = -1;
        for (int i = 0; i < number + 1; i++) {//遍历一行
            if (min > graph[source][i] && flag[i] != 1) {
                min_n = i;
                min = graph[source][i];
            }
        }
        //注：必须保证都可达，即矩阵中除第一列以外不会存在inf
        source = min_n;//记录该行最小值，下一次遍历作为开始点
        flag[min_n] = 1;//记录此点为已达
        cost += min;
        done++;//已达数++；
    }
    for (int i = 0; i < number + 1; i++) {
        for (int j = 0; j < number + 1; j++) {
            cout << graph[i][j] << ' ';
        }
        cout << endl;
    }
    int min = inf;
    for (int i = 1; i < number + 1; i++)
        min = min < graph[source][i] ? min : graph[source][i];//理论上是不需要再加的，即到终点就结束了，但是这里为了与分支界限相匹配，下界必须多加一个值。


    int max = 0;
    for (int i = 1; i < number + 1; i++)
        if (graph[source][i] != 65535) max = max > graph[source][i] ? max : graph[source][i];//理论上是不需要再加的，即到终点就结束了，但是这里为了与分支界限相匹配，下界必须多加一个值。
    cost += (max +min);


}

//父节点最初的归约
int initial_h(int number, unsigned int** graph, unsigned int** father_graph) {
    int h = 0;
    for (int i = 0; i < number; i++) {
        for (int j = 0; j < number; j++)
            father_graph[i][j] = graph[i][j];
    }
    //所有行归约
    for (int i = 0; i < number; i++) {
        int min = inf;
        for (int j = 0; j < number; j++)
            min = min < father_graph[i][j] ? min : father_graph[i][j];
        for (int j = 0; j < number; j++) {
            if (father_graph[i][j] != 65535)
                father_graph[i][j] = father_graph[i][j] - min;
        }
        if (h != 65535)
            h += min;
    }
    //列归约
    for (int j = 1; j < number; j++) {//注意此处
        int min = inf;
        for (int i = 0; i < number; i++)
            min = min < father_graph[i][j] ? min : father_graph[i][j];
        for (int i = 0; i < number; i++) {
            if (father_graph[i][j] != 65535)
                father_graph[i][j] = father_graph[i][j] - min;
        }
        if (h != 65535)
            h += min;

    }
    cout << endl;
    for (int i = 0; i < number; i++) {
        for (int j = 0; j < number; j++)
            cout << father_graph[i][j] << ' ';
        cout << endl;
    }
    //行和列归约没有问题
    return h;
}



TSP_node::TSP_node(unsigned int** graph, int number) {//number直接为阶数
    this->graph = new unsigned int* [number];
    for (int i = 0; i < number; i++)
        this->graph[i] = new unsigned int[number];
    lower_bound = initial_h(number, graph, this->graph);
    this->n = number;
    //初始化行和列的邻接表
    row_number = new int[n];
    col_number = new int[n];
    start = new int[n];
    end = new int[n];
    for (int i = 0; i < n; i++) {
        row_number[i] = i;
        col_number[i] = i;
        start[i] = -1;
        end[i] = -1;
    }
    father = NULL;
    lchild = NULL;
    rchild = NULL;
}

TSP_node::TSP_node(int type, int row, int col, TSP_node& father_node, int number) {
    if (type == 1) {//左子节点，即选取该点作为下一步节点
        this->n = father_node.n - 1;//降一次阶
        //分配新空间
        graph = new unsigned int* [father_node.n];
        for (int i = 0; i < father_node.n; i++)
            graph[i] = new unsigned int[n];//分配了未降阶的空间;行数不变，列数-1
        row_number = new int[n];
        col_number = new int[n];
        start = new int[number];
        end = new int[number];
        for (int i = 0; i < number; i++) {
            start[i] = father_node.start[i];
            end[i] = father_node.end[i];
        }//复制父节点的哪些点已作为了起始点和终点
        for (int i = 0; i < father_node.n; i++) {
            for (int j = 0; j < n; j++)
                graph[i][j] = inf;//但初始化了前n列的空间
        }
        //处理删除之后的邻接表
        for (int i = 0; i < row; i++)
            row_number[i] = father_node.row_number[i];
        for (int i = row; i < n; i++)
            row_number[i] = father_node.row_number[i + 1];
        for (int i = 0; i < col; i++)
            col_number[i] = father_node.col_number[i];
        for (int i = col; i < n; i++)
            col_number[i] = father_node.col_number[i + 1];
        //复制矩阵；
        for (int i = 0; i < n + 1; i++) {
            for (int j = 0; j < col; j++)
                graph[i][j] = father_node.graph[i][j];
        }
        for (int i = 0; i < n + 1; i++) {
            for (int j = col; j < n; j++)
                graph[i][j] = father_node.graph[i][j + 1];
        }
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < n; j++)
                graph[i][j] = graph[i][j];
        }
        for (int i = row; i < n; i++) {
            for (int j = 0; j < n; j++)
                graph[i][j] = graph[i + 1][j];
        }
        delete graph[n];

        int h;
        //选这个点会不会形成回环
        int loop = father_node.col_number[col];
        while (start[loop] != -1) {
            loop = start[loop];
        }
        if (loop == father_node.row_number[row])
            h = 65535;
        else {
            h = this->reduced_matrix(father_node, row, col);
            start[father_node.row_number[row]] = father_node.col_number[col];
            end[father_node.col_number[col]] = father_node.row_number[row];
        }
        if (h != 65535)
            lower_bound = father_node.lower_bound + h;
        else
            lower_bound = inf;
        lchild = NULL;
        rchild = NULL;
        //更新指针
        father_node.lchild = this;
        father = &father_node;
        //算代价
    }
    //右子节点，即不选该点作为下一步节点
    else {
        n = father_node.n;
        //分配空间
        graph = new unsigned int* [n];
        for (int i = 0; i < n; i++)
            graph[i] = new unsigned int[n];
        row_number = new int[n];
        col_number = new int[n];
        start = new int[number];
        end = new int[number];
        for (int i = 0; i < number; i++) {
            start[i] = father_node.start[i];
            end[i] = father_node.end[i];
        }//复制父节点的哪些点已作为了起始点和终点
        //复制
        for (int i = 0; i < n; i++) {
            row_number[i] = father_node.row_number[i];
            col_number[i] = father_node.col_number[i];
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                graph[i][j] = father_node.graph[i][j];
        }
        graph[row][col] = inf;
        //归约
        int h = 0;
        char firstfind_rowinf = 1;
        for (int i = 0; i < n; i++) {
            int thisrow_min = inf;
            for (int j = 0; j < n; j++)
                thisrow_min = thisrow_min < graph[i][j] ? thisrow_min : graph[i][j];
            for (int j = 0; j < n; j++) {
                if (graph[i][j] != 65535)
                    graph[i][j] -= thisrow_min;
            }
            if (thisrow_min != 65535)
                h += thisrow_min;
            else if (thisrow_min == 65535 && firstfind_rowinf == 1)
                firstfind_rowinf = 0;
            else if (thisrow_min == 65535 && firstfind_rowinf == 0) {
                h = thisrow_min;
                break;
            }
        }
        char firstfind_colinf = 1;
        if (h != 65535) {
            for (int i = 0; i < n; i++) {
                int thiscol_min = inf;
                for (int j = 0; j < n; j++)
                    thiscol_min = thiscol_min < graph[j][i] ? thiscol_min : graph[j][i];
                for (int j = 0; j < n; j++) {
                    if (graph[j][i] != 65535)
                        graph[j][i] -= thiscol_min;
                }
                if (thiscol_min != 65535)
                    h += thiscol_min;
                else if (thiscol_min == 65535 && firstfind_colinf == 1)
                    firstfind_colinf = 0;
                else if (thiscol_min == 65535 && firstfind_colinf == 0) {
                    h = thiscol_min;
                    break;
                }//对多列或多行全inf的处理，此时矩阵已失效。
            }
        }
        if (h != 65535)
            lower_bound = father_node.lower_bound + h;
        else
            lower_bound = inf;
        lchild = NULL;
        rchild = NULL;
        father_node.rchild = this;
        father = &father_node;
    }
}



//归约矩阵。col和row都是相对的
int TSP_node::reduced_matrix(TSP_node& father_node, int row, int col) {

    //复制

    int find_col = -1;
    int find_row = -1;
    for (int i = 0; i < n; i++) {
        if (row_number[i] == father_node.col_number[col])
            find_row = i;
        if (col_number[i] == father_node.row_number[row])
            find_col = i;
    }
    //将删去点的对称位置1nf
    if (find_col != -1 && find_row != -1)
        graph[find_row][find_col] = inf;
    //归约
    //对多列或多行全inf的处理，此时矩阵已失效。
    //右子的归约对应在左子处封装在了这里
    int h = 0;
    int firstfind_rowinf = 1;
    for (int i = 0; i < n; i++) {//每一行
        int thisrow_min = inf;
        for (int j = 0; j < n; j++)//遍历该行，找到最小的元素
            thisrow_min = thisrow_min < graph[i][j] ? thisrow_min : graph[i][j];
        for (int j = 0; j < n; j++) {
            if (graph[i][j] != 65535)
                graph[i][j] -= thisrow_min;
        }
        if (thisrow_min != 65535)
            h += thisrow_min;
        else if (thisrow_min == 65535 && firstfind_rowinf == 1)
            firstfind_rowinf = 0;
        else if (thisrow_min == 65535 && firstfind_rowinf == 0) {
            h = inf;//出现了第二个全inf的行。
            break;
        }
    }
    if (h != 65535) {
        int firstfind_colinf = 1;
        for (int i = 0; i < n; i++) {
            int thiscol_min = inf;
            for (int j = 0; j < n; j++)
                thiscol_min = thiscol_min < graph[j][i] ? thiscol_min : graph[j][i];
            for (int j = 0; j < n; j++) {
                if (graph[j][i] != 65535)
                    graph[j][i] -= thiscol_min;
            }
            if (thiscol_min != 65535)//有可能会出现一行或一列全为inf的情况，不能加inf
                h += thiscol_min;
            else if (thiscol_min == 65535 && firstfind_colinf == 1)
                firstfind_colinf = 0;
            else if (thiscol_min == 65535 && firstfind_colinf == 0) {
                h = inf;//出现了第二个全inf的行。
                break;
            }
        }
    }

    return h;
}

void proper_element(int& col, int& row, int n, unsigned int** graph) {
    bool first_find = 0;
    int max = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i][j] == 0) {
                if (first_find == 0) {//初始化第一个值
                    col = j;
                    row = i;
                    first_find = 1;
                    int col_min = inf;
                    int row_min = inf;
                    for (int k = 0; k < n && k != i; k++)
                        col_min = col_min < graph[k][j] ? col_min : graph[k][j];
                    for (int k = 0; k < n && k != j; k++)
                        row_min = row_min < graph[i][k] ? row_min : graph[i][k];
                    max = row_min + col_min;
                }
                else {//如果比之前的值大则更新值
                    int temp = 0;
                    int col_min = inf;
                    int row_min = inf;
                    for (int k = 0; k < n && k != i; k++)
                        col_min = col_min < graph[k][j] ? col_min : graph[k][j];
                    for (int k = 0; k < n && k != j; k++)
                        row_min = row_min < graph[i][k] ? row_min : graph[i][k];
                    temp = col_min + row_min;
                    if (temp > max) {
                        col = j;
                        row = i;
                        max = temp;
                    }
                }
            }
        }
    }
}





//外部接口，传入信息并调用？
//传入名字数组，并得到名字-位置表
//注意使用完之后销毁；
void initial_position(int n_x, int n_y, int number, vector<string> name_array, struct name_number* position_array, int campus) {
    int map_len;
    if (campus == 1)
        map_len = shahe_map_len;
    else
        map_len = xitucheng_map_len;
    position_array[0].name = "now";
    position_array[0].number = n_y * map_len + n_x;
    for (int i = 1, j = 0; i < number + 1; i++, j++) {
        struct building_campus_position temp;
        position_building_match(name_array[j], &temp);
        position_array[i].name = name_array[j];
        position_array[i].number = temp.y * map_len + temp.x;
    }
}

void destroy_matrix(int number, unsigned int** temp) {
    for (int i = 0; i < number + 1; i++)
        delete temp[i];
    delete temp;
}

//初始化矩阵结构

void initial_matrix(int number, unsigned int** temp, struct name_number* position_array, struct number_path* path_array, int campus) {
    for (int i = 0; i < number + 1; i++) {
        for (int j = 0; j < number + 1; j++) {
            if ((temp[i][j] == 65535) && (i != j) && j > i) {
                path_array[(number + 1) * i + j].source = i;
                path_array[(number + 1) * i + j].dest = j;
                path_array[(number + 1) * i + j].path = new int[1000];
                path_array[(number + 1) * i + j].path_length = AStar_Search(position_array[i].number, position_array[j].number, 0, path_array[(number + 1) * i + j].path, (int*)&temp[i][j], campus);
                path_array[j * (number + 1) + i].path_length = path_array[(number + 1) * i + j].path_length;
                temp[j][i] = temp[i][j];
                path_array[j * (number + 1) + i].path = NULL;
            }
        }
    }
    for (int i = 1; i < number + 1; i++)
        temp[i][0] = inf;

}

//需要额外传入一个vector<pair<int, int>> 以存放结果
int* get_multi_path_seq(int source_x, int source_y, vector<string> name_array, int number, vector<pair<int, int>>& seq_result, int& sum_length, int campus) {

    struct number_path* path_array;
    struct name_number* position_array;//名字和坐标
    static unsigned int** graph;
    int upper_bound;
    //分配空间
    position_array = new struct name_number[number + 1];
    graph = new unsigned int* [number + 1];
    for (int i = 0; i < number + 1; i++)
        graph[i] = new unsigned int[number + 1];
    for (int i = 0; i < number + 1; i++) {
        for (int j = 0; j < number + 1; j++)
            graph[i][j] = inf;
    }
    path_array = new struct number_path[(number + 1) * (number + 1)];//两点间的距离，包括源、目的和int*路径
    for (int i = 0; i < (number + 1) * (number + 1); i++)
        path_array[i].path = NULL;
    //以下为必有
    initial_position(source_x, source_y, number, name_array, position_array, campus);//得到包括当前位置在内的position_array。下标与name_array的下标一一对应
    initial_matrix(number, graph, position_array, path_array, campus);//得到graph和两点间路径；
    //贪心算法求得上界
    greedy(number, graph, upper_bound);
    cout << "上界：";
    cout << upper_bound << endl;
    //初始化优先队列
    priority_queue<TSP_node*, vector<TSP_node*>, least_lb> q;
    TSP_node father(graph, number + 1);
    father.father = NULL;
    TSP_node* now_node;//now_node指向当前处理的节点
    vector<TSP_node*>result;
    q.push(&father);
    while (q.size() != 0) {
        now_node = q.top();
        q.pop();
        cout << "当前处理节点" << endl;
        for (int i = 0; i < now_node->n; i++) {
            for (int j = 0; j < now_node->n; j++)
                cout << now_node->graph[i][j] << ' ';
            cout << endl;
        }
        cout << "列邻接表" << endl;
        for (int i = 0; i < now_node->n; i++)
            cout << now_node->col_number[i] << ' ';
        cout << endl;
        cout << "行邻接表" << endl;
        for (int i = 0; i < now_node->n; i++)
            cout << now_node->row_number[i] << ' ';
        cout << endl;
        cout << "下界：" << now_node->lower_bound << endl;
        cout << "start邻接表" << endl;
        for (int i = 0; i < number + 1; i++)
            cout << now_node->start[i] << ' ';
        cout << endl;
        cout << "end邻接表" << endl;
        for (int i = 0; i < number + 1; i++)
            cout << now_node->end[i] << ' ';
        cout << endl;

        int col=0, row=0;
        proper_element(col, row, now_node->n, now_node->graph);
        TSP_node* lc_ptr = new TSP_node(1, row, col, *now_node, number + 1);
        cout << "左节点" << endl;
        for (int i = 0; i < lc_ptr->n; i++) {
            for (int j = 0; j < lc_ptr->n; j++)
                cout << lc_ptr->graph[i][j] << ' ';
            cout << endl;
        }
        cout << "列邻接表" << endl;
        for (int i = 0; i < lc_ptr->n; i++)
            cout << lc_ptr->col_number[i] << ' ';
        cout << endl;
        cout << "行邻接表" << endl;
        for (int i = 0; i < lc_ptr->n; i++)
            cout << lc_ptr->row_number[i] << ' ';
        cout << endl;
        cout << "下界：" << lc_ptr->lower_bound << endl;
        cout << "start邻接表" << endl;
        for (int i = 0; i < number + 1; i++)
            cout << lc_ptr->start[i] << ' ';
        cout << endl;
        cout << "end邻接表" << endl;
        for (int i = 0; i < number + 1; i++)
            cout << lc_ptr->end[i] << ' ';
        cout << endl;



        TSP_node* rc_ptr = new TSP_node(0, row, col, *now_node, number + 1);//必须new，否则会被释放
        cout << "右节点" << endl;
        for (int i = 0; i < rc_ptr->n; i++) {
            for (int j = 0; j < rc_ptr->n; j++)
                cout << rc_ptr->graph[i][j] << ' ';
            cout << endl;
        }
        cout << "列邻接表" << endl;
        for (int i = 0; i < rc_ptr->n; i++)
            cout << rc_ptr->col_number[i] << ' ';
        cout << endl;
        cout << "行邻接表" << endl;
        for (int i = 0; i < rc_ptr->n; i++)
            cout << rc_ptr->row_number[i] << ' ';
        cout << endl;
        cout << "下界：" << rc_ptr->lower_bound << endl;
        cout << "start邻接表" << endl;
        for (int i = 0; i < number + 1; i++)
            cout << rc_ptr->start[i] << ' ';
        cout << endl;
        cout << "end邻接表" << endl;
        for (int i = 0; i < number + 1; i++)
            cout << rc_ptr->end[i] << ' ';
        cout << endl;




        if (lc_ptr->lower_bound <= upper_bound) {
            if (lc_ptr->n != 1)
                q.push(lc_ptr);
            else {//如果阶数到达了一，则认为已经完成了寻路，将指针存于表中。
                result.push_back(lc_ptr);
                if (result.size() > 3)//选择三个最优结果
                    break;
            }
        }
        else {
            lc_ptr->father->lchild = NULL;
            delete lc_ptr;

        }
        if (rc_ptr->lower_bound <= upper_bound) {
            if (rc_ptr->n > 1)
                q.push(rc_ptr);
        }
        else {
            rc_ptr->father->rchild = NULL;
            delete rc_ptr;
        }
    }


    sum_length = get_result(result, number, seq_result, path_array, graph);
    //得到上界内满足要求的结果中最小的结果
    //处理并合并每一段的int数组，得到一个大数组，放于path_seqarray中；

    int* path_seqarray;
    path_seqarray = new int[sum_length];
    int index = 0;
    for (int i = 0; i < seq_result.size(); i++) {
        int path_now_index;//此段路的信息存在path_array中的哪一个元素中
        if (seq_result[i].first < seq_result[i].second)
            path_now_index = (number + 1) * seq_result[i].first + seq_result[i].second;
        else path_now_index = seq_result[i].first + seq_result[i].second * (number + 1);
        if (seq_result[i].first < seq_result[i].second) {
            for (int j = 0; j < path_array[path_now_index].path_length; j++)
                path_seqarray[index++] = path_array[path_now_index].path[j];
        }
        else {
            for (int j = 0; j < path_array[path_now_index].path_length; j++)
                path_seqarray[index++] = path_array[path_now_index].path[path_array[path_now_index].path_length - j - 1];
        }
    }


    int map_len;
    if (campus == 1)
        map_len = shahe_map_len;
    else map_len = xitucheng_map_len;
    //输出部分
    for (int i = 0; i < sum_length; i++) {
        cout << path_seqarray[i] % map_len << ' ';
        cout << path_seqarray[i] / map_len << endl;
    }
    for (int i = 0; i < number; i++)
        cout << name_array[i] << ' ';
    cout << "路径为：从当前起点出发，依次到达：" << endl;
    for (int i = 0; i < number - 1; i++)
        cout << name_array[seq_result[i + 1].first - 1] << ' ';
    cout << name_array[seq_result[seq_result.size() - 1].second - 1] << endl;



    //释放路空间
    for (int i = 0; i < (number + 1) * (number + 1); i++) {
        if (path_array[i].path != NULL)
            delete path_array[i].path;
    }
    delete_tree(&father);
    destroy_matrix(number, graph);
    return path_seqarray;
}

int get_result(vector<TSP_node*>result, int number, vector<pair<int, int>>& final_result, struct number_path* path_array, unsigned int** map) {//number为遍历目标数
    //vector<pair<int, int>> result_array[result.size()];
    vector<pair<int, int>>seq_result[100];
    pair<int, int>temp;
    for (int result_size = 0; result_size < result.size(); result_size++) {
        TSP_node* now_node = result[result_size];
        int loop = 0;
        while (now_node->start[loop] != -1) {
            temp.first = loop;
            temp.second = now_node->start[loop];
            seq_result[result_size].push_back(temp);
            loop = now_node->start[loop];
        }
    }
    int sum_number[100];
    int sum_cost[100];
    for (int i = 0; i < result.size(); i++) {
        sum_number[i] = 0;
        sum_cost[i] = 0;
    }
    for (int i = 0; i < result.size(); i++) {//i表示处理的总路径的序号
        for (int j = 0; j < seq_result[i].size(); j++) {//j表示处理的单个路径的下标
            sum_number[i] += path_array[(number + 1) * seq_result[i][j].first + seq_result[i][j].second].path_length;
            sum_cost[i] += map[seq_result[i][j].first][seq_result[i][j].second];
        }
    }
    int min_sum = 65535;
    int min_index = 0;
    for (int i = 0; i < result.size(); i++) {
        if (min_sum > sum_cost[i]) {
            min_sum = sum_cost[i];
            min_index = i;
        }
    }
    final_result = seq_result[min_index];
    return sum_number[min_index];

}

void delete_tree(struct TSP_node* father) {
    queue<struct TSP_node*> all_node;
    all_node.push(father);
    struct TSP_node* now;
    now = all_node.front();
    if (now->lchild != NULL)
        all_node.push(now->lchild);
    if (now->rchild != NULL)
        all_node.push(now->rchild);
    all_node.pop();
    while (all_node.size() != 0) {
        now = all_node.front();
        if (now->lchild != NULL)
            all_node.push(now->lchild);
        if (now->rchild != NULL)
            all_node.push(now->rchild);
        all_node.pop();
        delete now;
    }
}





int* multi_dest(vector<string> name_array, int start, int campus_now, vector<string>& dest_seq) {
    int source_x;
    int source_y;
    if (campus_now == 1) {
        source_x = start % shahe_map_len;
        source_y = start / shahe_map_len;
    }
    else {
        source_x = start % xitucheng_map_len;
        source_y = start / xitucheng_map_len;
    }
    vector<struct building_campus_position> name_result_array;
    struct building_campus_position temp;
    for (int i = 0; i < name_array.size(); i++) {
        position_building_match(name_array[i], &temp);
        name_result_array.push_back(temp);
    }
    vector<string> shahe_array;
    vector<string> xitucheng_array;
    for (int i = 0; i < name_result_array.size(); i++) {
        if (name_result_array[i].campus == '1')
            shahe_array.push_back(name_result_array[i].building);
        else
            xitucheng_array.push_back(name_result_array[i].building);
    }
    //得到沙河和西土城两个校区的name_array
    vector<pair<int, int> > seq_result1;
    vector<pair<int, int> > seq_result2;
    if (campus_now == 1 && xitucheng_array.size() == 0) {//若目的地全在沙河地区
        int* path;
        path = get_multi_path_seq(start % shahe_map_len, start / shahe_map_len, shahe_array, (int)shahe_array.size(), seq_result1, path_pause, 1);
        for (int i = 0; i < seq_result1.size() - 1; i++)
            dest_seq.push_back(shahe_array[seq_result1[i + 1].first - 1]);
        dest_seq.push_back(shahe_array[seq_result1[seq_result1.size() - 1].second - 1]);
        start_campus = 1;
        end_campus = -1;
        path_end = -1;

        return path;
    }
    else if (campus_now == 2 && shahe_array.size() == 0) {//若目的地全在西土城地区
        int* path;
        path = get_multi_path_seq(start % xitucheng_map_len, start / xitucheng_map_len, xitucheng_array, (int)xitucheng_array.size(), seq_result1, path_pause, 2);
        for (int i = 0; i < seq_result1.size() - 1; i++)
            dest_seq.push_back(xitucheng_array[seq_result1[i + 1].first - 1]);
        dest_seq.push_back(xitucheng_array[seq_result1[seq_result1.size() - 1].second - 1]);
        start_campus = 2;
        end_campus = -1;
        path_end = -1;
        return path;
    }

    if (campus_now == 1) {//如果起点在沙河
        int cost = 0;
        if (shahe_array.size() == 0) {//特殊情况，若目的地全在另一校区
            int* path1 = new int[400];
            int* path;
            int path1_length = AStar_Search(start, shahe_bus_station, 0, path1, &cost, 1);
            int sum_length;
            path_pause = path1_length;
            path = get_multi_path_seq(xitucheng_bus_station % xitucheng_map_len, xitucheng_bus_station / xitucheng_map_len, xitucheng_array, (int)xitucheng_array.size(), seq_result1, sum_length, 2);
            //dest_seq.push_back("now");
            for (int i = 0; i < seq_result1.size() - 1; i++)
                dest_seq.push_back(xitucheng_array[seq_result1[i + 1].first - 1]);
            dest_seq.push_back(xitucheng_array[seq_result1[seq_result1.size() - 1].second - 1]);
            for (int i = 0; i < sum_length; i++)
                path1[path1_length++] = path[i];
            delete[]path;
            start_campus = 1;
            end_campus = 2;

            path_end = sum_length;
            return path1;
        }
        else {//分两步进行处理
            //先导完沙河地区的所有目的地
            int* path1;
            int length1;
            path1 = get_multi_path_seq(start % shahe_map_len, start / shahe_map_len, shahe_array, (int)shahe_array.size(), seq_result1, length1, 1);
            //再从最后的一个目的地导到沙河校门
            int* path2 = new int[400];
            int length2;
            int cost;
            length2 = AStar_Search(path1[length1 - 1], shahe_bus_station, 0, path2, &cost, 1);
            //再从西土城的校门口开始导到目的地
            int* path3;
            int length3;
            path3 = get_multi_path_seq(xitucheng_bus_station % xitucheng_map_len, xitucheng_bus_station / xitucheng_map_len, xitucheng_array, (int)xitucheng_array.size(), seq_result2, length3, 2);
            int* path_all = new int[1000];
            int index = 0;
            for (int i = 0; i < length1; i++)
                path_all[index++] = path1[i];
            for (int i = 0; i < length2; i++)
                path_all[index++] = path2[i];
            for (int i = 0; i < length3; i++)
                path_all[index++] = path3[i];
            //dest_seq.push_back("now");
            for (int i = 0; i < seq_result1.size() - 1; i++)
                dest_seq.push_back(shahe_array[seq_result1[i + 1].first - 1]);
            dest_seq.push_back(shahe_array[seq_result1[seq_result1.size() - 1].second - 1]);
            for (int i = 0; i < seq_result2.size() - 1; i++)
                dest_seq.push_back(xitucheng_array[seq_result2[i + 1].first - 1]);
            dest_seq.push_back(xitucheng_array[seq_result2[seq_result2.size() - 1].second - 1]);
            delete[]path1;
            delete[]path2;
            delete[]path3;
            start_campus = 1;
            end_campus = 2;
            path_pause = length1 + length2;
            path_end = length3;
            return path_all;
        }
    }
    else {//如果起点在西土城
        int cost;
        if (xitucheng_array.size() == 0) {//特殊情况，若目的地全在另一校区
            int* path1 = new int[400];
            int* path;
            int path1_length = AStar_Search(start, xitucheng_bus_station, 0, path1, &cost, 2);
            path_pause = path1_length;
            int sum_length;
            path = get_multi_path_seq(shahe_bus_station % shahe_map_len, shahe_bus_station / shahe_map_len, shahe_array, (int)shahe_array.size(), seq_result1, sum_length, 1);
            for (int i = 0; i < sum_length; i++)
                path1[path1_length++] = path[i];
            //dest_seq.push_back("now");
            for (int i = 0; i < seq_result1.size() - 1; i++)
                dest_seq.push_back(shahe_array[seq_result1[i + 1].first - 1]);
            dest_seq.push_back(shahe_array[seq_result1[seq_result1.size() - 1].second - 1]);
            delete[]path;
            start_campus = 2;
            end_campus = 1;
            path_end = sum_length;
            return path1;
        }
        else {//分两步进行处理
            //先导完西土城地区的所有目的地
            int* path1;
            int length1;
            path1 = get_multi_path_seq(start % xitucheng_map_len, start / xitucheng_map_len, xitucheng_array, (int)xitucheng_array.size(), seq_result1, length1, 2);
            //再从最后的一个目的地导到西土城校门
            int* path2 = new int[400];
            int length2;
            int cost;
            length2 = AStar_Search(path1[length1 - 1], xitucheng_bus_station, 0, path2, &cost, 2);
            //再从沙河的校门口开始导到目的地
            int* path3;
            int length3;
            path3 = get_multi_path_seq(shahe_bus_station % shahe_map_len, shahe_bus_station / shahe_map_len, shahe_array, (int)shahe_array.size(), seq_result2, length3, 1);
            int* path_all = new int[1000];
            int index = 0;
            for (int i = 0; i < length1; i++)
                path_all[index++] = path1[i];
            for (int i = 0; i < length2; i++)
                path_all[index++] = path2[i];
            for (int i = 0; i < length3; i++)
                path_all[index++] = path3[i];
            for (int i = 0; i < seq_result1.size() - 1; i++)
                dest_seq.push_back(xitucheng_array[seq_result1[i + 1].first - 1]);
            dest_seq.push_back(xitucheng_array[seq_result1[seq_result1.size() - 1].second - 1]);
            for (int i = 0; i < seq_result2.size() - 1; i++)
                dest_seq.push_back(shahe_array[seq_result2[i + 1].first - 1]);
            dest_seq.push_back(shahe_array[seq_result2[seq_result2.size() - 1].second - 1]);
            delete[]path1;
            delete[]path2;
            delete[]path3;
            start_campus = 2;
            end_campus = 1;
            path_pause = length1 + length2;
            path_end = length3;
            return path_all;
        }
    }
    return NULL;
}


int* upper_multi_dest(vector<string> name_array, string start, vector<string>& dest_seq) {
    struct building_campus_position temp;
    position_building_match(start, &temp);
    int width = 0;
    if (temp.campus == '1')
        width = shahe_map_len;
    else width = xitucheng_map_len;
    return multi_dest(name_array, temp.y * width + temp.x, temp.campus - '0', dest_seq);
}
int* upper_multi_dest(vector<string> name_array, int start_x, int start_y, int campus, vector<string>& dest_seq) {

    int width = 0;
    if (campus == 1)
        width = shahe_map_len;
    else width = xitucheng_map_len;
    return multi_dest(name_array, start_y * width + start_x, campus, dest_seq);
}


//需传入vector<string> name_array;即要途径的地点和start即起始名
//vector<string>dest_seq;以存放结果
//upper_multi_dest(name_array, start, dest_seq);会返回一个path*






int process_class(string class_number, vector<struct T_C_P>** data_array);
void class_position_func(string class_number, string day, string time, string class_name, vector<string>* result);
int process_time(string time);



void class_position_func(string class_number, string day, string time, string class_name, vector<string>* result) {
    ifstream class_file;
    vector<class T_C_P_T>output_data_temp;
    vector<class T_C_P_T>output_data;
    class_file.open(class_data_file);
    vector<struct T_C_P>* data_array[5];
    for (int i = 0; i < 5; i++)
        data_array[i] = new vector<struct T_C_P>;
    process_class(class_number, data_array);
    //处理星期数
    if (day != "0") {
        for (int i = 0; i < 5; i++) {
            if (i != (day[0] - '1'))
                delete data_array[i];
        }
    }
    struct T_C_P_T odata;
    int time_flag = 0;
    if (time != "0") {
        time_flag = 1;
        if (day == "0") {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < data_array[i]->size(); j++) {
                    if (process_time((*data_array[i])[j].time) == process_time(time)) {
                        odata.data = (*data_array[i])[j];
                        odata.day = i + 1;
                        output_data_temp.push_back(odata);
                    }
                }
            }
        }
        else {
            for (int j = 0; j < data_array[day[0] - '1']->size(); j++) {
                if (process_time((*data_array[day[0] - '1'])[j].time) == process_time(time)) {
                    odata.data = (*data_array[day[0] - '1'])[j];
                    odata.day = day[0] - '0';
                    output_data_temp.push_back(odata);
                }
            }
        }
    }
    else {
        if (day == "0") {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < data_array[i]->size(); j++) {
                    odata.data = (*data_array[i])[j];
                    odata.day = i + 1;
                    output_data_temp.push_back(odata);
                }
            }
        }
        else {
            for (int j = 0; j < data_array[day[0] - '1']->size(); j++) {
                odata.data = (*data_array[day[0] - '1'])[j];
                odata.day = day[0] - '0';
                output_data_temp.push_back(odata);
            }
        }
    }

    if (class_name != "0") {
        for (int i = 0; i < output_data_temp.size(); i++) {
            string::size_type position = output_data_temp[i].data.class_name.find(class_name);
            if (output_data_temp[i].data.class_name == class_name || position != string::npos)
                output_data.push_back(output_data_temp[i]);
        }
    }
    else {
        for (int i = 0; i < output_data_temp.size(); i++)
            output_data.push_back(output_data_temp[i]);
    }

    for (int i = 0; i < output_data.size(); i++) {
        string temp1;
        string temp2;
        string temp3;
        temp1 += "周";
        temp1 += output_data[i].day + '0';
        temp1 += ' ';
        temp1 += output_data[i].data.time;
        result->push_back(temp1);
        temp2 += output_data[i].data.class_name;
        result->push_back(temp2);
        temp3 += output_data[i].data.position;

        result->push_back(temp3);
    }
}

int process_class(string class_number, vector<struct T_C_P>** data_array) {
    string class_numbers;
    string no_use;
    ifstream fp;
    fp.open(class_data_file);
    getline(fp, class_numbers);
    while (class_numbers != class_number) {
        for (int i = 0; i < 75; i++)
            getline(fp, no_use);
        getline(fp, class_numbers);
    }
    if (!fp.eof()) {
        struct T_C_P data;
        string line_temp;
        for (int i = 0; i < 5; i++) {
            fp >> line_temp;
            for (int j = 0; j < 14; j++) {
                fp >> data.time;
                fp >> data.class_name;
                if (data.class_name != "n") {
                    fp >> data.position;
                    data_array[i]->push_back(data);
                }
            }
        }
        return 0;
    }
    else
        return 1;
}

int process_time(string time) {
    int i = 0;
    int hour = 0;
    int minute = 0;
    while (time[i] != ':')
        hour = hour * 10 + time[i++] - '0';
    i++;
    while (time[i] != '\0')
        minute = minute * 60 + time[i++] - '0';
    return hour * 60 + minute;
}
//需传入四个string，以及外部分配一个vector string以存放结果
//class_position_func(class_number,day,time,class_name,&result);






/****************************************************************************************************************
*****************************************************************************************************************
*****************************************************************************************************************
*/

void Output()
{
    Point p;
    int sum;
    int s;
    pointl.clear();
    ifstream myfile("C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\坐标.txt");
    while (!myfile.eof())
    {
        myfile >> p.x;
        myfile >> p.y;
        pointl.push_back(p);
    }
    myfile.close();

    ofstream mycout("C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\坐标.txt");

    for (int i = 0; i < pointl.size(); i++)
    {
        mycout << pointl.at(i).x << endl;
        mycout << pointl.at(i).y << endl;
    }

    for (int i = 0; i < points.size(); i++)
    {
        mycout << points.at(i).x << endl;
        mycout << points.at(i).y << endl;
    }

    for (int j = 0; j < pointx.size(); j++)
    {
        mycout << pointx.at(j).x << endl;
        mycout << pointx.at(j).y << endl;
    }
    mycout.close();
}

int main()
{
	//初始化界面
	initgraph(1300, 630);
	setbkcolor(RGB(255, 255, 255));
	cleardevice();

	//画出界面边界
	setlinecolor(RGB(0, 0, 0));
	setlinestyle(PS_SOLID, 2);
	rectangle(1, 1, 1298, 628);
	rectangle(19, 19, 941, 601);
	line(970, 1, 970, 628);

	//加载有关界面的图片
	loadimage((IMAGE*)&img11[0], (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\00.png", 330, 630);
	loadimage((IMAGE*)&img11[1], (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\11.png", 330, 630);
	loadimage((IMAGE*)&img11[2], (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\22.png", 330, 630);
	loadimage((IMAGE*)&img11[3], (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\33.png", 330, 630);
	loadimage((IMAGE*)&img11[4], (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\44.png", 330, 630);
	loadimage((IMAGE*)&img11[5], (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\55.png", 330, 630);
	loadimage((IMAGE*)&img11[6], (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\66.png", 330, 630);
	loadimage((IMAGE*)&img11[7], (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\77.png", 330, 630);
	loadimage((IMAGE*)&img11[8], (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\88.png", 330, 630);
	loadimage((IMAGE*)&img11[9], (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\99.png", 330, 630);
	loadimage((IMAGE*)&img11[10],(LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\1010.png", 330, 630);
	loadimage((IMAGE*)&img11[11],(LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\1111.png", 330, 630);


	//加载移动时所需照片
	loadimage(&img1, (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\shahe.jpg", 920, 580);
	loadimage(&img2, (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\benbu.jpg", 920, 580);
	loadimage(&img3, (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\bkg.jpg", 1300, 630);
	loadimage(&img4, (LPCTSTR)"C:\\Users\\86184\\Desktop\\图形化图形化\\图形化\\图形化\\bus1.jpg", 500, 150);

	//展示初始主菜单界面
	putimage(970, 0, &img11[0]);

	//展示初始沙河地图
	putimage(20, 20, &img1);

	//设置矩形框输出格式
	setfillcolor(WHITE);					//设置输出文本框的背景色
	settextcolor(BLACK);					//设置输出文本字体颜色
	setlinestyle(PS_SOLID, 1);				//设置输出字体为实线型
	settextstyle(30, 0, (LPCTSTR)"楷体");			//设置输出文本字体

/*
*****************************************************************************************************************
*****************************************************************************************************************
*****************************************************************************************************************
*/
    map_initial();
    int* path;
	ifstream mycin;
    ofstream fileout;

	int Front;
	int Sizes, Sizex;
	int a, b;
	Point p;

	MOUSEMSG m; //鼠标指针
	int event = 0;	//用于判断鼠标点击坐标在哪个框中
	char c;		//用于接收输入的一个字符
	string str;	//用于接收输入的一个字符串
	while (1)
	{//沙河初始化界面
		m = GetMouseMsg();	//获取鼠标消息
		if (m.uMsg == WM_LBUTTONDOWN)	//若鼠标点击了
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				switch (event)
				{
					case 1://定位
						goto flag_dingwei;
					case 2://物理导航
						goto flag_wulidaohang;
					case 3://逻辑导航
						goto flag_luojidaohang;
					case 4://多目标导航
						goto flag_duomubiao;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

//主菜单界面
flag_menu:
	putimage(970, 0, &img11[2]);
	//输出在哪个校区
	if (ci[0] == 1)
	{
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		outtextxy(1000, 70, (LPCTSTR)"当前在沙河校区");	//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	else if (ci[0] == 2)
	{
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		outtextxy(1000, 70, (LPCTSTR)"当前在西土城校区");	//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}

	while (1)
	{//主菜单
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				switch (event)
				{
					case 1://定位
						goto flag_dingwei;
					case 2://物理导航
						goto flag_wulidaohang;
					case 3://逻辑导航
						goto flag_luojidaohang;
					case 4://多目标导航
						goto flag_duomubiao;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

//定位界面
flag_dingwei:
	putimage(970, 0, &img11[1]);
    //调用定位函数——参数为当前位置坐标x, y，范围, 校区，返回值为string向量
    dingwei.clear();
    upper_locate(ci[1] / 10, ci[2] / 10, 200, ci[0], &dingwei);
	//输出定位信息
	for (int i = 0; i < dingwei.size() && i < 13; i++)
	{
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		outtextxy(1000, 30 + i * 40,(LPCTSTR) dingwei.at(i).c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	while (1)
	{//定位界面
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				if (event == 4)
					goto flag_menu;	//返回主菜单
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

//物理导航界面
flag_wulidaohang:
	//判断用户选择输入起点还是当前位置导航
	putimage(970, 0, &img11[11]);
	while (1)
	{	//用户选择方式
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				switch (event)
				{
					case 6://重新输入起点
						flag = 1;
						goto flag_wulidaohang1;
					case 7://当前位置导航
						flag = 0;
						goto flag_wulidaohang1;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

flag_wulidaohang1:
	putimage(970, 0, &img11[3]);
	//起点
	if (flag == 1)
	{	//输入起点
		start.clear();
		while ((c = _getch()) != 13)
		{
			if (c == '\b')  //如果c是退格符，str1删掉一个
			{

				start = start.substr(0, start.size() - 1);		//substr复制子字符串，要求从指定位置开始，并具有指定的长度
			}
			else
			{
				start += (int)c;		//存储一个字符
			}
			setrop2(R2_COPYPEN);
			setfillcolor(WHITE);					//设置输出文本框的背景色
			settextcolor(BLACK);
			setlinestyle(PS_SOLID, 1);
			fillrectangle(1000, 70, 1275, 70 + 40);
			outtextxy(1000, 70, (LPCTSTR)start.c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
		}
	}
	else if (flag == 0)
	{
		//输出起点为当前位置
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		fillrectangle(1000, 70, 1275, 70 + 40);
		outtextxy(1000, 70, (LPCTSTR)"当前位置");		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	//终点
	endpoint.clear();
	while ((c = _getch()) != 13)
	{
		if (c == '\b')  //如果c是退格符，str1删掉一个
		{
			endpoint = endpoint.substr(0, endpoint.size() - 1);		//substr复制子字符串，要求从指定位置开始，并具有指定的长度
		}
		else
		{
			endpoint += (int)c;		//存储一个字符
		}
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		fillrectangle(1000, 230, 1275, 230 + 50);
		outtextxy(1000, 230, (LPCTSTR)endpoint.c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	while (1)
	{//物理导航选择导航界面
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				switch (event)
				{
				case 1://最短时间
					way = 1;
					goto flag_wuliyidong;
				case 2://最短距离
					way = 2;
					goto flag_wuliyidong;
				case 3://交通工具
					way = 3;
					goto flag_wuliyidong;
				case 4://返回主菜单
					goto flag_menu;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

//逻辑导航界面
flag_luojidaohang:
	//判断用户选择输入起点还是当前位置导航
	putimage(970, 0, &img11[11]);
	while (1)
	{	//用户选择方式
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				switch (event)
				{
					case 6://重新输入起点
						flag = 1;
						goto flag_luojidaohang1;
					case 7://当前位置导航
						flag = 0;
						goto flag_luojidaohang1;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

flag_luojidaohang1:
	putimage(970, 0, &img11[8]);
	//起点
	if (flag == 1)
	{	//输入起点
		start.clear();
		while ((c = _getch()) != 13)
		{
			if (c == '\b')  //如果c是退格符，str1删掉一个
			{

				start = start.substr(0, start.size() - 1);		//substr复制子字符串，要求从指定位置开始，并具有指定的长度
			}
			else
			{
				start += (int)c;		//存储一个字符
			}
			setrop2(R2_COPYPEN);
			setfillcolor(WHITE);					//设置输出文本框的背景色
			settextcolor(BLACK);
			setlinestyle(PS_SOLID, 1);
			fillrectangle(1000, 70, 1275, 70 + 50);
			outtextxy(1000, 70, (LPCTSTR)start.c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
		}
	}
	else if (flag == 0)
	{	//输出起点为当前位置
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		fillrectangle(1000, 70, 1275, 70 + 50);
		outtextxy(1000, 70, (LPCTSTR)"当前位置");		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	//终点信息——共四个
	str.clear();
	zhongdian.clear();
	for (int i = 0; i < 4; i++)
	{
		str.clear();
		while ((c = _getch()) != 13)
		{
			if (c == '\b')  //如果c是退格符，str1删掉一个
			{
				str = str.substr(0, str.size() - 1);		//substr复制子字符串，要求从指定位置开始，并具有指定的长度
			}
			else
			{
				str += (int)c;		//存储一个字符
			}
			setrop2(R2_COPYPEN);
			setfillcolor(WHITE);					//设置输出文本框的背景色
			settextcolor(BLACK);
			setlinestyle(PS_SOLID, 1);
			fillrectangle(1000, 230 + i * 40, 1275, 230 + 40 + i * 40);		//画出文本输出框
			outtextxy(1000, 230 + i * 40,(LPCTSTR) str.c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
		}
		zhongdian.push_back(str.c_str());
	}
	while (1)
	{//逻辑导航选择导航界面
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}

			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				if (event == 4)
				{
					goto flag_luojiyidong;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

//多目标导航界面
flag_duomubiao:
	//判断用户选择输入起点还是当前位置导航
	putimage(970, 0, &img11[11]);
	while (1)
	{	//用户选择方式
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				switch (event)
				{
				case 6://重新输入起点
					flag = 1;
					goto flag_duomubiao1;
				case 7://当前位置导航
					flag = 0;
					goto flag_duomubiao1;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

flag_duomubiao1:
	putimage(970, 0, &img11[9]);
	//起点
	if (flag == 1)
	{	//输入起点
		start.clear();
		while ((c = _getch()) != 13)
		{
			if (c == '\b')  //如果c是退格符，str1删掉一个
			{

				start = start.substr(0, start.size() - 1);		//substr复制子字符串，要求从指定位置开始，并具有指定的长度
			}
			else
			{
				start += (int)c;		//存储一个字符
			}
			setrop2(R2_COPYPEN);
			setfillcolor(WHITE);					//设置输出文本框的背景色
			settextcolor(BLACK);
			setlinestyle(PS_SOLID, 1);
			fillrectangle(1000, 70, 1275, 70 + 40);
			outtextxy(1000, 70,(LPCTSTR) start.c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
		}
	}
	else if (flag == 0)
	{	//输出起点为当前位置
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		fillrectangle(1000, 70, 1275, 70 + 40);
		outtextxy(1000, 70, (LPCTSTR)"当前位置");		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	//输入终点个数
	str.clear();
	while ((c = _getch()) != 13)
	{
		if (c == '\b')  //如果c是退格符，str1删掉一个
		{
			str = str.substr(0, str.size() - 1);		//substr复制子字符串，要求从指定位置开始，并具有指定的长度
		}
		else
		{
			str += (int)c;		//存储一个字符
		}
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		fillrectangle(1150, 110, 1275, 110 + 40);		//画出文本输出框
		outtextxy(1150, 110,(LPCTSTR) str.c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	endnum = atoi(str.c_str());
	//存储多个目标
	str.clear();
	mubiao.clear();
	for (int i = 0; i < endnum; i++)
	{
		str.clear();
		while ((c = _getch()) != 13)
		{
			if (c == '\b')  //如果c是退格符，str1删掉一个
			{
				str = str.substr(0, str.size() - 1);		//substr复制子字符串，要求从指定位置开始，并具有指定的长度
			}
			else
			{
				str += (int)c;		//存储一个字符
			}
			fillrectangle(1000, 190 + i * 40, 1275, 190 + 40 + i * 40);		//画出文本输出框
			outtextxy(1000, 190 + i * 40,(LPCTSTR) str.c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
		}
		mubiao.push_back(str.c_str());
	}
	while (1)
	{//多目标选择导航界面
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}

			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				if (event == 4)
				{
					goto flag_duomubiaoyidong;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

//物理导航移动界面
flag_wuliyidong:
	putimage(970, 0, &img11[4]);
    if (way == 1)//最短时间
    {
        if (flag == 1)
        {
            path = upper_time_min(start, endpoint);
            writes_path_into_file(path);
            delete(path);
        }
        else if (flag == 0)
        {
            path = upper_time_min(ci, endpoint);
            writes_path_into_file(path);
            delete(path);
        }
    }
    else if(way == 2)//最短距离
    {
        if (flag == 1)
        {
            path = upper_shortest_path(start, endpoint);
            writes_path_into_file(path);
            delete path;
        }
        else if (flag == 0)
        {
            path = upper_shortest_path(ci, endpoint);
            writes_path_into_file(path);
            delete path;
        }
    }
    else if (way == 3)//交通工具
    {
        if (flag == 1)
        {
            path = by_upper_time_min(start, endpoint);
            writes_path_into_file(path);
            delete path;
        }
        else if (flag == 0)
        {
            path = by_upper_time_min(ci, endpoint);
            writes_path_into_file(path);
            delete path;
        }
    }

	mycin.close();
	mycin.open(path_file_name);
	setlinestyle(PS_SOLID, 8);
	mycin >> Front;//读入初始坐标所在小区
	mycin >> Sizes;//读入沙河坐标总个数
	mycin >> Sizex;//读入本部坐标总个数
	points.clear();
	pointx.clear();
    int dog;
	//循环读入沙河各个坐标和本部各个坐标
	if (Front == 1)
	{
		for (int i = 0; i < Sizes; i++)
		{
			mycin >> a;
			p.x = (a % lens) * 10;
			p.y = (a / lens) * 10;
			points.push_back(p);
		}
		for (int i = 0; i < Sizex; i++)
		{
			mycin >> b;
			p.x = (b % lenx) * 10;
			p.y = (b / lenx) * 10;
			pointx.push_back(p);
		}
	}
	if (Front == 2)
	{
		for (int i = 0; i < Sizex; i++)
		{
			mycin >> b;
			p.x = (b % lenx) * 10;
			p.y = (b / lenx) * 10;
			pointx.push_back(p);
		}
		for (int i = 0; i < Sizes; i++)
		{
			mycin >> a;
			p.x = (a % lens) * 10;
			p.y = (a / lens) * 10;
			points.push_back(p);
		}
	}
	//关闭文件
	mycin.close();
	//初始地图和总路线加载
	if (Front == 1)
	{
		putimage(20, 20, &img1);
		for (int k = 0; k < (Sizes - 1); k++)
		{
			setrop2(R2_COPYPEN);
			setlinecolor(RED);
            setlinestyle(PS_SOLID, 8);
			line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
		}
	}
	if (Front == 2)
	{
		putimage(20, 20, &img2);
		for (int k = 0; k < (Sizex - 1); k++)
		{
			setrop2(R2_COPYPEN);
			setlinecolor(RED);
            setlinestyle(PS_SOLID, 8);
			line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
		}
	}
	Sleep(500);
	//进行路线移动
	if (Front == 1)
	{
		for (int j = 0; j < Sizes; j++)
		{
			setrop2(R2_COPYPEN);
			setcolor(YELLOW);
			fillcircle(points.at(j).x, points.at(j).y, 3);

			//物理导航暂停界面
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				for (int i = 0; i <= 10; i++)
				{
					setrop2(R2_NOTXORPEN);
					setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
					circle(m.x, m.y, 2 * i);
					Sleep(25);
					circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
				}
				if (button_judge(m.x, m.y) != 0)
				{
					event = button_judge(m.x, m.y);//记录这一次触发的按钮
					if (event == 5)
					{
						ci[0] = 1;//在沙河
						ci[1] = points.at(j).x;
						ci[2] = points.at(j).y;
						goto flag_wulizanting;
					}
				}
			}
			FlushMouseMsgBuffer();//清空鼠标消息缓存区

			Sleep(200);
			putimage(20, 20, &img1);

			for (int k = 0; k < (Sizes - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
			}
		}
		//如果要切换校区，首先加载校车动图
		if (Sizex != 0)
		{
			putimage(0, 0, &img3);
			putimage(0, 250, &img4);
			for (int i = 1; i < 14; i++)
			{
				putimage(0, 0, &img3);
				putimage(i * 100, 250, &img4);
				Sleep(100);
			}
			//
			setbkcolor(RGB(255, 255, 255));
			cleardevice();

			//画边界
			setlinecolor(RGB(0, 0, 0));
			setlinestyle(PS_SOLID, 2);
			rectangle(1, 1, 1298, 628);
			rectangle(19, 19, 941, 601);
			line(970, 1, 970, 628);
			rectangle(990, 20, 1280, 230);
			setlinestyle(PS_SOLID, 8);
			putimage(20, 20, &img2);
			putimage(970, 0, &img11[4]);
			//加载另一个校区地图和总路线
			for (int k = 0; k < (Sizex - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
			}
			Sleep(500);
			//在另一个校区进行动态移动
			for (int j = 0; j < Sizex; j++)
			{
				setrop2(R2_COPYPEN);
				setcolor(YELLOW);
				fillcircle(pointx.at(j).x, pointx.at(j).y, 3);
				//物理导航暂停界面

				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					for (int i = 0; i <= 10; i++)
					{
						setrop2(R2_NOTXORPEN);
						setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
						circle(m.x, m.y, 2 * i);
						Sleep(25);
						circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
					}
					if (button_judge(m.x, m.y) != 0)
					{
						event = button_judge(m.x, m.y);//记录这一次触发的按钮
						if (event == 5)
						{
							ci[0] = 2;//在沙河
							ci[1] = pointx.at(j).x;
							ci[2] = pointx.at(j).y;
							goto flag_wulizanting;
						}
					}
				}
				FlushMouseMsgBuffer();//清空鼠标消息缓存区


				Sleep(200);
				putimage(20, 20, &img2);

				for (int k = 0; k < (Sizex - 1); k++)
				{
					setrop2(R2_COPYPEN);
					setlinecolor(RED);
                    setlinestyle(PS_SOLID, 8);
					line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
				}
			}
            dog = pointx.size();
            ci[0] = 2;
            ci[1] = pointx.at(dog- 1).x;
            ci[2] = pointx.at(dog - 1).y;
		}
        else
        {
            dog = points.size();
            ci[0] = 1;
            ci[1] = points.at(dog - 1).x;
            ci[2] = points.at(dog - 1).y;
        }
	}
	//同上
	if (Front == 2)
	{
		for (int j = 0; j < Sizex; j++)
		{
			setrop2(R2_COPYPEN);
			setcolor(YELLOW);
			fillcircle(pointx.at(j).x, pointx.at(j).y, 3);

			//物理导航暂停界面

			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				for (int i = 0; i <= 10; i++)
				{
					setrop2(R2_NOTXORPEN);
					setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
					circle(m.x, m.y, 2 * i);
					Sleep(25);
					circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
				}
				if (button_judge(m.x, m.y) != 0)
				{
					event = button_judge(m.x, m.y);//记录这一次触发的按钮
					if (event == 5)
					{
						ci[0] = 2;//在沙河
						ci[1] = pointx.at(j).x;
						ci[2] = pointx.at(j).y;
						goto flag_wulizanting;
					}
				}
			}
			FlushMouseMsgBuffer();//清空鼠标消息缓存区


			Sleep(200);
			putimage(20, 20, &img2);

			for (int k = 0; k < (Sizex - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
			}
		}
		if (Sizes != 0)
		{
			putimage(0, 0, &img3);
			putimage(0, 250, &img4);
			for (int i = 1; i < 14; i++)
			{
				putimage(0, 0, &img3);
				putimage(i * 100, 250, &img4);
				Sleep(100);
			}
			//
			setbkcolor(RGB(255, 255, 255));
			cleardevice();

			//画边界
			setlinecolor(RGB(0, 0, 0));
			setlinestyle(PS_SOLID, 2);
			rectangle(1, 1, 1298, 628);
			rectangle(19, 19, 941, 601);
			line(970, 1, 970, 628);
			rectangle(990, 20, 1280, 230);
			setlinestyle(PS_SOLID, 8);
			putimage(20, 20, &img1);

			putimage(970, 0, &img11[4]);


			for (int k = 0; k < (Sizes - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
			}
			Sleep(500);

			for (int j = 0; j < Sizes; j++)
			{
				setrop2(R2_COPYPEN);
				setcolor(YELLOW);
				fillcircle(points.at(j).x, points.at(j).y, 3);

				//物理导航暂停界面

				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					for (int i = 0; i <= 10; i++)
					{
						setrop2(R2_NOTXORPEN);
						setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
						circle(m.x, m.y, 2 * i);
						Sleep(25);
						circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
					}
					if (button_judge(m.x, m.y) != 0)
					{
						event = button_judge(m.x, m.y);//记录这一次触发的按钮
						if (event == 5)
						{
							ci[0] = 1;//在沙河
							ci[1] = points.at(j).x;
							ci[2] = points.at(j).y;
							goto flag_wulizanting;
						}
					}
				}
				FlushMouseMsgBuffer();//清空鼠标消息缓存区


				Sleep(200);
				putimage(20, 20, &img1);

				for (int k = 0; k < (Sizes - 1); k++)
				{
					setrop2(R2_COPYPEN);
					setlinecolor(RED);
                    setlinestyle(PS_SOLID, 8);
					line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
				}
			}
            dog = points.size();
            ci[0] = 1;
            ci[1] = points.at(dog - 1).x;
            ci[2] = points.at(dog - 1).y;
		}
        else
        {
            dog = pointx.size();
            ci[0] = 2;
            ci[1] = pointx.at(dog - 1).x;
            ci[2] = pointx.at(dog - 1).y;
        }
	}
	Output();
	goto flag_menu;

//逻辑导航移动界面
flag_luojiyidong:
	putimage(970, 0, &img11[6]);
    endpoint.clear();
    luojixinxi.clear();
    if (zhongdian.at(0) == "食堂")
    {   
        if (flag == 1)
        {
            struct building_campus_position temp;
            position_building_match(start, &temp);
            ci[0] = temp.campus - '0';
            ci[1] = temp.x * 10;
            ci[2] = temp.y * 10;
        }
        //求取将导航到哪一个食堂
        decidelevel();
        endpoint = choice.at(0);
        luojixinxi.push_back(endpoint);
        //求取导航路线
        if (flag == 0)  //以当前位置为起点
        {
            path = upper_shortest_path(ci, endpoint);
            writes_path_into_file(path);
            delete path;
        }
        else if (flag == 1)  //用户输入起点
        {
            path = upper_shortest_path(start, endpoint);
            writes_path_into_file(path);
            delete path;
        }
    }
    else
    {   //参数为班级号、周几、几点钟、课程名称
        class_position_func(zhongdian.at(0), zhongdian.at(1), zhongdian.at(2), zhongdian.at(3), &luojixinxi);
        if (luojixinxi.size() != 0)
        {
            if (luojixinxi.at(2).at(0) == 'N')
            {
                endpoint = "教学楼N楼";
            }
            else if (luojixinxi.at(2).at(0) == 'S')
            {
                endpoint = "教学楼S楼";
            }
            else
            {
                endpoint = "报告厅";
            }
        }
        else
        {
            endpoint = "沙河大门";
        }
        //求取导航路线
        if (flag == 0 )  //以当前位置为起点
        {
            path = upper_shortest_path(ci, endpoint);
            writes_path_into_file(path);
            delete path;
        }
        else if (flag == 1)  //用户输入起点
        {
            path = upper_shortest_path(start, endpoint);
            writes_path_into_file(path);
            delete path;
        }
    }

	//在交互框中输出逻辑导航的相关信息
	for (int i = 0; i < luojixinxi.size() && i < 12; i++)
	{
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		outtextxy(1000, 30 + i * 40, (LPCTSTR)luojixinxi.at(i).c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	setlinestyle(PS_SOLID, 8);
	mycin.close();
	mycin.open(path_file_name);
	mycin >> Front;//初始地点位于哪个校区
	mycin >> Sizes;//沙河坐标总个数
	mycin >> Sizex;//西土城坐标总个数
	points.clear();
	pointx.clear();
	//存入沙河各个坐标和本部各个坐标
	if (Front == 1)
	{
		for (int i = 0; i < Sizes; i++)
		{
			mycin >> a;
			p.x = (a % lens) * 10;
			p.y = (a / lens) * 10;
			points.push_back(p);
		}
		for (int i = 0; i < Sizex; i++)
		{
			mycin >> b;
			p.x = (b % lenx) * 10;
			p.y = (b / lenx) * 10;
			pointx.push_back(p);
		}
	}
	if (Front == 2)
	{
		for (int i = 0; i < Sizex; i++)
		{
			mycin >> b;
			p.x = (b % lenx) * 10;
			p.y = (b / lenx) * 10;
			pointx.push_back(p);
		}
		for (int i = 0; i < Sizes; i++)
		{
			mycin >> a;
			p.x = (a % lens) * 10;
			p.y = (a / lens) * 10;
			points.push_back(p);
		}
	}
	
	mycin.close();
	//加载初始校区地图和总路线
	if (Front == 1)
	{
		putimage(20, 20, &img1);
		for (int k = 0; k < (Sizes - 1); k++)
		{
			setrop2(R2_COPYPEN);
			setlinecolor(RED);
            setlinestyle(PS_SOLID, 8);
			line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
		}
	}
	if (Front == 2)
	{
		putimage(20, 20, &img2);
		for (int k = 0; k < (Sizex - 1); k++)
		{
			setrop2(R2_COPYPEN);
			setlinecolor(RED);
            setlinestyle(PS_SOLID, 8);
			line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
		}
	}
	Sleep(500);
	//进行动态移动
	if (Front == 1)
	{
		for (int j = 0; j < Sizes; j++)
		{
			setrop2(R2_COPYPEN);
			setcolor(YELLOW);
			fillcircle(points.at(j).x, points.at(j).y, 3);

			//逻辑导航暂停界面

			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				for (int i = 0; i <= 10; i++)
				{
					setrop2(R2_NOTXORPEN);
					setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
					circle(m.x, m.y, 2 * i);
					Sleep(25);
					circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
				}
				if (button_judge(m.x, m.y) != 0)
				{
					event = button_judge(m.x, m.y);//记录这一次触发的按钮
					if (event == 4)
					{
						ci[0] = 1;//在沙河
						ci[1] = points.at(j).x;
						ci[2] = points.at(j).y;
						goto flag_luojizanting;
					}
				}
			}
			FlushMouseMsgBuffer();//清空鼠标消息缓存区


			Sleep(200);
			putimage(20, 20, &img1);

			for (int k = 0; k < (Sizes - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
			}
		}
		//如果要切换校区的话，先加载公交车
		if (Sizex != 0)
		{
			putimage(0, 0, &img3);
			putimage(0, 250, &img4);
			for (int i = 1; i < 14; i++)
			{
				putimage(0, 0, &img3);
				putimage(i * 100, 250, &img4);
				Sleep(100);
			}
	
			setbkcolor(RGB(255, 255, 255));
			cleardevice();

			//画边界
			setlinecolor(RGB(0, 0, 0));
			setlinestyle(PS_SOLID, 2);
			rectangle(1, 1, 1298, 628);
			rectangle(19, 19, 941, 601);
			line(970, 1, 970, 628);
			rectangle(990, 20, 1280, 230);
			setlinestyle(PS_SOLID, 8);
			putimage(20, 20, &img2);
			putimage(970, 0, &img11[6]);
            for (int i = 0; i < luojixinxi.size() && i < 12; i++)
            {
                setrop2(R2_COPYPEN);
                setfillcolor(WHITE);					//设置输出文本框的背景色
                settextcolor(BLACK);
                setlinestyle(PS_SOLID, 1);
                outtextxy(1000, 30 + i * 40, (LPCTSTR)luojixinxi.at(i).c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
            }
			for (int k = 0; k < (Sizex - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
			}
			Sleep(500);

			for (int j = 0; j < Sizex; j++)
			{
				setrop2(R2_COPYPEN);
				setcolor(YELLOW);
				fillcircle(pointx.at(j).x, pointx.at(j).y, 3);

				//逻辑导航暂停界面
				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					for (int i = 0; i <= 10; i++)
					{
						setrop2(R2_NOTXORPEN);
						setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
						circle(m.x, m.y, 2 * i);
						Sleep(25);
						circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
					}
					if (button_judge(m.x, m.y) != 0)
					{
						event = button_judge(m.x, m.y);//记录这一次触发的按钮
						if (event == 4)
						{
							ci[0] = 2;//在沙河
							ci[1] = pointx.at(j).x;
							ci[2] = pointx.at(j).y;
							goto flag_luojizanting;
						}
					}
				}
				FlushMouseMsgBuffer();//清空鼠标消息缓存区


				Sleep(200);
				putimage(20, 20, &img2);

				for (int k = 0; k < (Sizex - 1); k++)
				{
					setrop2(R2_COPYPEN);
					setlinecolor(RED);
                    setlinestyle(PS_SOLID, 8);
					line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
				}
			}
            dog = pointx.size();
            ci[0] = 2;
            ci[1] = pointx.at(dog - 1).x;
            ci[2] = pointx.at(dog - 1).y;
		}
        else
        {
            dog = points.size();
            ci[0] = 1;
            ci[1] = points.at(dog - 1).x;
            ci[2] = points.at(dog - 1).y;
        }
	}
	//同上
	if (Front == 2)
	{
		for (int j = 0; j < Sizex; j++)
		{
			setrop2(R2_COPYPEN);
			setcolor(YELLOW);
			fillcircle(pointx.at(j).x, pointx.at(j).y, 3);

			//逻辑导航暂停界面
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				for (int i = 0; i <= 10; i++)
				{
					setrop2(R2_NOTXORPEN);
					setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
					circle(m.x, m.y, 2 * i);
					Sleep(25);
					circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
				}
				if (button_judge(m.x, m.y) != 0)
				{
					event = button_judge(m.x, m.y);//记录这一次触发的按钮
					if (event == 4)
					{
						ci[0] = 2;//在沙河
						ci[1] = pointx.at(j).x;
						ci[2] = pointx.at(j).y;
						goto flag_luojizanting;
					}
				}
			}
			FlushMouseMsgBuffer();//清空鼠标消息缓存区


			Sleep(200);
			putimage(20, 20, &img2);

			for (int k = 0; k < (Sizex - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
			}
		}
		if (Sizes != 0)
		{
			putimage(0, 0, &img3);
			putimage(0, 250, &img4);
			for (int i = 1; i < 14; i++)
			{
				putimage(0, 0, &img3);
				putimage(i * 100, 250, &img4);
				Sleep(100);
			}
			//
			setbkcolor(RGB(255, 255, 255));
			cleardevice();

			//画边界
			setlinecolor(RGB(0, 0, 0));
			setlinestyle(PS_SOLID, 2);
			rectangle(1, 1, 1298, 628);
			rectangle(19, 19, 941, 601);
			line(970, 1, 970, 628);
			rectangle(990, 20, 1280, 230);
			setlinestyle(PS_SOLID, 8);
			putimage(20, 20, &img1);

			putimage(970, 0, &img11[6]);
            for (int i = 0; i < luojixinxi.size() && i < 12; i++)
            {
                setrop2(R2_COPYPEN);
                setfillcolor(WHITE);					//设置输出文本框的背景色
                settextcolor(BLACK);
                setlinestyle(PS_SOLID, 1);
                outtextxy(1000, 30 + i * 40,(LPCTSTR) luojixinxi.at(i).c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
            }

			for (int k = 0; k < (Sizes - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
			}
			Sleep(500);

			for (int j = 0; j < Sizes; j++)
			{
				setrop2(R2_COPYPEN);
				setcolor(YELLOW);
				fillcircle(points.at(j).x, points.at(j).y, 3);
				//逻辑导航暂停界面

				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					for (int i = 0; i <= 10; i++)
					{
						setrop2(R2_NOTXORPEN);
						setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
						circle(m.x, m.y, 2 * i);
						Sleep(25);
						circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
					}
					if (button_judge(m.x, m.y) != 0)
					{
						event = button_judge(m.x, m.y);//记录这一次触发的按钮
						if (event == 4)
						{
							ci[0] = 1;//在沙河
							ci[1] = points.at(j).x;
							ci[2] = points.at(j).y;
							goto flag_luojizanting;
						}
					}
				}
				FlushMouseMsgBuffer();//清空鼠标消息缓存区


				Sleep(200);
				putimage(20, 20, &img1);
				for (int k = 0; k < (Sizes - 1); k++)
				{
					setrop2(R2_COPYPEN);
					setlinecolor(RED);
                    setlinestyle(PS_SOLID, 8);
					line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
				}
			}
            dog = points.size();
            ci[0] = 1;
            ci[1] = points.at(dog - 1).x;
            ci[2] = points.at(dog - 1).y;
		}
        else
        {
            dog = pointx.size();
            ci[0] = 2;
            ci[1] = pointx.at(dog - 1).x;
            ci[2] = pointx.at(dog - 1).y;
        }
	}
	Output();
	goto flag_menu;

//多目标导航移动界面
flag_duomubiaoyidong:
	putimage(970, 0, &img11[6]);
	//在交互框中输出多目标导航的信息
    mubiaoxinxi.clear();
    if (flag == 0)  //当前位置作为起点
    {//mubiaoxinxi目标信息保存多个目标点的探索顺序
        path = upper_multi_dest(mubiao, ci[1]/10, ci[2]/10, ci[0], mubiaoxinxi);
        writes_path_into_file(path);
        delete(path);
    }
    else if (flag == 1) //用户输入起点
    {
        path = upper_multi_dest(mubiao, start, mubiaoxinxi);
        writes_path_into_file(path);
        delete(path);
    }
	for (int i = 0; i < mubiaoxinxi.size(); i++)
	{
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		outtextxy(1000, 30 + i * 40, (LPCTSTR)mubiaoxinxi.at(i).c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	setlinestyle(PS_SOLID, 8);
	mycin.close();
	mycin.open(path_file_name);
	mycin >> Front;
	mycin >> Sizes;
	mycin >> Sizex;
	points.clear();
	pointx.clear();
	if (Front == 1)
	{
		for (int i = 0; i < Sizes; i++)
		{
			mycin >> a;
			p.x = (a % lens) * 10;
			p.y = (a / lens) * 10;
			points.push_back(p);
		}
		for (int i = 0; i < Sizex; i++)
		{
			mycin >> b;
			p.x = (b % lenx) * 10;
			p.y = (b / lenx) * 10;
			pointx.push_back(p);
		}
	}
	if (Front == 2)
	{
		for (int i = 0; i < Sizex; i++)
		{
			mycin >> b;
			p.x = (b % lenx) * 10;
			p.y = (b / lenx) * 10;
			pointx.push_back(p);
		}
		for (int i = 0; i < Sizes; i++)
		{
			mycin >> a;
			p.x = (a % lens) * 10;
			p.y = (a / lens) * 10;
			points.push_back(p);
		}
	}
	mycin.close();

	if (Front == 1)
	{
		putimage(20, 20, &img1);
		for (int k = 0; k < (Sizes - 1); k++)
		{
			setrop2(R2_COPYPEN);
			setlinecolor(RED);
            setlinestyle(PS_SOLID, 8);
			line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
		}
	}
	if (Front == 2)
	{
		putimage(20, 20, &img2);
		for (int k = 0; k < (Sizex - 1); k++)
		{
			setrop2(R2_COPYPEN);
			setlinecolor(RED);
            setlinestyle(PS_SOLID, 8);
			line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
		}
	}
	Sleep(500);

	if (Front == 1)
	{
		for (int j = 0; j < Sizes; j++)
		{
			setrop2(R2_COPYPEN);
			setcolor(YELLOW);
			fillcircle(points.at(j).x, points.at(j).y, 3);

			//多目标导航暂停界面
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				for (int i = 0; i <= 10; i++)
				{
					setrop2(R2_NOTXORPEN);
					setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
					circle(m.x, m.y, 2 * i);
					Sleep(25);
					circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
				}
				if (button_judge(m.x, m.y) != 0)
				{
					event = button_judge(m.x, m.y);//记录这一次触发的按钮
					if (event == 4)
					{
						ci[0] = 1;//在沙河
						ci[1] = points.at(j).x;
						ci[2] = points.at(j).y;
						goto flag_duomubiaozanting;
					}
				}
			}
			FlushMouseMsgBuffer();//清空鼠标消息缓存区


			Sleep(200);
			putimage(20, 20, &img1);

			for (int k = 0; k < (Sizes - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
			}
		}
		if (Sizex != 0)
		{
			putimage(0, 0, &img3);
			putimage(0, 250, &img4);
			for (int i = 1; i < 14; i++)
			{
				putimage(0, 0, &img3);
				putimage(i * 100, 250, &img4);
				Sleep(100);
			}
			//
			setbkcolor(RGB(255, 255, 255));
			cleardevice();

			//画边界
			setlinecolor(RGB(0, 0, 0));
			setlinestyle(PS_SOLID, 2);
			rectangle(1, 1, 1298, 628);
			rectangle(19, 19, 941, 601);
			line(970, 1, 970, 628);
			rectangle(990, 20, 1280, 230);
			setlinestyle(PS_SOLID, 8);
			putimage(20, 20, &img2);
			putimage(970, 0, &img11[6]);
			for (int i = 0; i < mubiaoxinxi.size(); i++)
			{
				setrop2(R2_COPYPEN);
				setfillcolor(WHITE);					//设置输出文本框的背景色
				settextcolor(BLACK);
				setlinestyle(PS_SOLID, 1);
				outtextxy(1000, 30 + i * 40,(LPCTSTR) mubiaoxinxi.at(i).c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
			}
			for (int k = 0; k < (Sizex - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
			}
			Sleep(500);

			for (int j = 0; j < Sizex; j++)
			{
				setrop2(R2_COPYPEN);
				setcolor(YELLOW);
				fillcircle(pointx.at(j).x, pointx.at(j).y, 3);

				//多目标导航暂停界面

				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					for (int i = 0; i <= 10; i++)
					{
						setrop2(R2_NOTXORPEN);
						setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
						circle(m.x, m.y, 2 * i);
						Sleep(25);
						circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
					}
					if (button_judge(m.x, m.y) != 0)
					{
						event = button_judge(m.x, m.y);//记录这一次触发的按钮
						if (event == 4)
						{
							ci[0] = 2;//在沙河
							ci[1] = pointx.at(j).x;
							ci[2] = pointx.at(j).y;
							goto flag_duomubiaozanting;
						}
					}
				}
				FlushMouseMsgBuffer();//清空鼠标消息缓存区


				Sleep(200);
				putimage(20, 20, &img2);

				for (int k = 0; k < (Sizex - 1); k++)
				{
					setrop2(R2_COPYPEN);
					setlinecolor(RED);
                    setlinestyle(PS_SOLID, 8);
					line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
				}
			}
            dog = pointx.size();
            ci[0] = 2;
            ci[1] = pointx.at(dog - 1).x;
            ci[2] = pointx.at(dog - 1).y;
		}
        else
        {
            dog = points.size();
            ci[0] = 1;
            ci[1] = points.at(dog - 1).x;
            ci[2] = points.at(dog - 1).y;
        }
	}

	if (Front == 2)
	{
		for (int j = 0; j < Sizex; j++)
		{
			setrop2(R2_COPYPEN);
			setcolor(YELLOW);
			fillcircle(pointx.at(j).x, pointx.at(j).y, 3);

			//多目标导航暂停界面
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN)
			{
				for (int i = 0; i <= 10; i++)
				{
					setrop2(R2_NOTXORPEN);
					setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
					circle(m.x, m.y, 2 * i);
					Sleep(25);
					circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
				}
				if (button_judge(m.x, m.y) != 0)
				{
					event = button_judge(m.x, m.y);//记录这一次触发的按钮
					if (event == 4)
					{
						ci[0] = 2;//在沙河
						ci[1] = pointx.at(j).x;
						ci[2] = pointx.at(j).y;
						goto flag_duomubiaozanting;
					}
				}
			}
			FlushMouseMsgBuffer();//清空鼠标消息缓存区


			Sleep(200);
			putimage(20, 20, &img2);

			for (int k = 0; k < (Sizex - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(pointx.at(k).x, pointx.at(k).y, pointx.at(k + 1).x, pointx.at(k + 1).y);
			}
		}
		if (Sizes != 0)
		{
			putimage(0, 0, &img3);
			putimage(0, 250, &img4);
			for (int i = 1; i < 14; i++)
			{
				putimage(0, 0, &img3);
				putimage(i * 100, 250, &img4);
				Sleep(100);
			}
			//
			setbkcolor(RGB(255, 255, 255));
			cleardevice();

			//画边界
			setlinecolor(RGB(0, 0, 0));
			setlinestyle(PS_SOLID, 2);
			rectangle(1, 1, 1298, 628);
			rectangle(19, 19, 941, 601);
			line(970, 1, 970, 628);
			rectangle(990, 20, 1280, 230);
			setlinestyle(PS_SOLID, 8);
			putimage(20, 20, &img1);

			putimage(970, 0, &img11[6]);
			for (int i = 0; i < mubiaoxinxi.size(); i++)
			{
				setrop2(R2_COPYPEN);
				setfillcolor(WHITE);					//设置输出文本框的背景色
				settextcolor(BLACK);
				setlinestyle(PS_SOLID, 1);
				outtextxy(1000, 30 + i * 40, (LPCTSTR)mubiaoxinxi.at(i).c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
			}

			for (int k = 0; k < (Sizes - 1); k++)
			{
				setrop2(R2_COPYPEN);
				setlinecolor(RED);
                setlinestyle(PS_SOLID, 8);
				line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
			}
			Sleep(500);

			for (int j = 0; j < Sizes; j++)
			{
				setrop2(R2_COPYPEN);
				setcolor(YELLOW);
				fillcircle(points.at(j).x, points.at(j).y, 3);
				//多目标导航暂停界面

				m = GetMouseMsg();
				if (m.uMsg == WM_LBUTTONDOWN)
				{
					for (int i = 0; i <= 10; i++)
					{
						setrop2(R2_NOTXORPEN);
						setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
						circle(m.x, m.y, 2 * i);
						Sleep(25);
						circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
					}
					if (button_judge(m.x, m.y) != 0)
					{
						event = button_judge(m.x, m.y);//记录这一次触发的按钮
						if (event == 4)
						{
							ci[0] = 1;//在沙河
							ci[1] = points.at(j).x;
							ci[2] = points.at(j).y;
							goto flag_duomubiaozanting;
						}
					}
				}
				FlushMouseMsgBuffer();//清空鼠标消息缓存区

				Sleep(200);
				putimage(20, 20, &img1);
				for (int k = 0; k < (Sizes - 1); k++)
				{
					setrop2(R2_COPYPEN);
					setlinecolor(RED);
                    setlinestyle(PS_SOLID, 8);
					line(points.at(k).x, points.at(k).y, points.at(k + 1).x, points.at(k + 1).y);
				}
			}
            dog = points.size();
            ci[0] = 1;
            ci[1] = points.at(dog - 1).x;
            ci[2] = points.at(dog - 1).y;
		}
        else
        {
            dog = pointx.size();
            ci[0] = 2;
            ci[1] = pointx.at(dog - 1).x;
            ci[2] = pointx.at(dog - 1).y;
        }
	}
	Output();
	goto flag_menu;

//物理导航暂停界面
flag_wulizanting:
	putimage(970, 0, &img11[5]);
	//输出在哪个校区
	if (ci[0] == 1)
	{
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		outtextxy(1000, 70, (LPCTSTR)"当前在沙河校区");		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	else if (ci[0] == 2)
	{
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		outtextxy(1000, 70,(LPCTSTR) "当前在西土城校区");		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	while (1)
	{//物理导航已暂停界面
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				switch (event)
				{
				case 1://继续
                    flag = 0;
					goto flag_wuliyidong;
				case 2://定位
					goto flag_dingwei;
				case 3://更改目的地
					goto flag_genggaidi;
				case 4://更改策略
					goto flag_genggaifa;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

//逻辑导航暂停界面
flag_luojizanting:
	putimage(970, 0, &img11[10]);
	while (1)
	{//逻辑导航已暂停界面
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				switch (event)
				{
				case 6://继续
                    flag = 0;
					goto flag_luojiyidong;
				case 7://返回主菜单
					goto flag_menu;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

//多目标导航暂停界面
flag_duomubiaozanting:
	putimage(970, 0, &img11[10]);
	while (1)
	{//多目标导航已暂停界面
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				switch (event)
				{
				case 6://继续
                    flag = 0;
					goto flag_duomubiaoyidong;
				case 7://返回主菜单
					goto flag_menu;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

//物理导航过程中选择更改目的地界面
flag_genggaidi:
	putimage(970, 0, &img11[8]);
	//输出起点为当前位置
	setrop2(R2_COPYPEN);
	setfillcolor(WHITE);					//设置输出文本框的背景色
	settextcolor(BLACK);
	setlinestyle(PS_SOLID, 1);
	outtextxy(1000, 70, (LPCTSTR)"当前所在位置");		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	//终点
	endpoint.clear();
	while ((c = _getch()) != 13)
	{
		if (c == '\b')  //如果c是退格符，str1删掉一个
		{
			endpoint = endpoint.substr(0, endpoint.size() - 1);		//substr复制子字符串，要求从指定位置开始，并具有指定的长度
		}
		else
		{
			endpoint += (int)c;		//存储一个字符
		}
		setrop2(R2_COPYPEN);
		setfillcolor(WHITE);					//设置输出文本框的背景色
		settextcolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		fillrectangle(1000, 230, 1275, 230 + 40);
		outtextxy(1000, 230, (LPCTSTR)endpoint.c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	}
	while (1)
	{//更改目的地界面
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				if (event == 4)
				{	//用户选择继续导航
                    flag = 0;
					goto flag_wuliyidong;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

//物理导航过程中选择更改导航策略界面
flag_genggaifa:
	putimage(970, 0, &img11[7]);
	//输出当前终点
	setrop2(R2_COPYPEN);
	setfillcolor(WHITE);					//设置输出文本框的背景色
	settextcolor(BLACK);
	setlinestyle(PS_SOLID, 1);
	outtextxy(1000, 70, (LPCTSTR)endpoint.c_str());		//string类的c_str方法可以返回(地址)用C语言的char *的形式表示的字符串
	while (1)
	{//更改导航策略界面
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			for (int i = 0; i <= 10; i++)
			{
				setrop2(R2_NOTXORPEN);
				setlinecolor(RGB(25 * i, 25 * i, 25 * i));//设置圆颜色；
				circle(m.x, m.y, 2 * i);
				Sleep(25);
				circle(m.x, m.y, 2 * i);	//抹去刚刚画的圆,此时半径变大，原来的圆被覆盖
			}
			if (button_judge(m.x, m.y) != 0)
			{
				event = button_judge(m.x, m.y);//记录这一次触发的按钮
				switch (event)
				{
				case 1://选择导航策略为物理导航——最短时间
					way = 1;
                    flag = 0;
					goto flag_wuliyidong;
				case 2://选择导航策略为物理导航——最短距离
					way = 2;
                    flag = 0;
					goto flag_wuliyidong;
				case 3://选择导航策略为物理导航——交通工具
					way = 3;
                    flag = 0;
					goto flag_wuliyidong;
				case 4://返回导航主菜单界面
					goto flag_menu;
				}
			}
		}
		FlushMouseMsgBuffer();//清空鼠标消息缓存区
	}

	system("pause");
	return 0;
}
