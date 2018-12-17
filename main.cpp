#include<bits/stdc++.h>
#include<conio.h>
#define MaxVertexNum 200
#define MAX 999999
using namespace std;

typedef struct {
    int num;//编号
    char name[100];//景点名称
    string features;//景点描述
}VertexType;//景点
typedef struct{
    VertexType  vexs[MaxVertexNum];
    //保存路径长度的邻接矩阵
    int  edges[MaxVertexNum][MaxVertexNum];
    int n,e;//边数和结点数
}MGraph;//图
int P[MAX],D[MAX];
int visit[200];
int dis[200];

void create(MGraph* G);
void Init_matrix(MGraph* G);
void Init_features(MGraph *G);
void Display(void);
void manager(MGraph *G);
void guest(MGraph *G);
void GetVex(MGraph *G, int v);
void ShortestPath(MGraph *G, int *P, int *D,int pos);
void ToDestination(MGraph *G, int v, int w);
void InsertArc(MGraph *G,int v, int w);
int login(void);
void DeleteArc(MGraph *G ,int v,int w);
void recreate_graph(MGraph *G);
void del_place(MGraph *G,int code);
void change_palce(MGraph *G,int code);
void add_place(MGraph *G);

int main(void)
{
    // 从文件中读取图并建立邻接矩阵存储
    MGraph* G=(MGraph*)calloc(1,sizeof(MGraph));
    int flag=-1;
    create(G);
    //登陆
    while(flag==-1)
        flag=login();
    system("cls");
    //进入管理界面  结束后回收空间
    if(flag)
        manager(G);
    else
       guest(G);
    free(G);
    return 0;
}


void create(MGraph* G)
{
    FILE* fp = fopen("message.txt","r");
    char message[300];
    G->e = 0;
    if(fp==NULL)
    {
        cout<<"文件打开失败\n";
        exit(0);
    }
    //从文件中读取用于建立矩阵的相关信息并构建邻接矩阵存储结构
    while(fgets(message,300,fp)!=NULL)
    {
        int id,id2,distance=0;
        char name[20],relation[200];
        sscanf(message,"%d %s",&id,name);
        strcpy(G->vexs[id].name,name);
        fgets(relation,200,fp);
        relation[strlen(relation)-1]='\0';
        char* token = strtok(relation," ");
        id2 = atoi(token);
        token = strtok(NULL," ");
        distance = atol(token);
        G->edges[id][id2] = distance;
        G->e += 1;
        //根据文件结构读取每行的数据并对数据进行分割
        while(token!=NULL){
            token = strtok(NULL," ");
            if(!token)
                break;
            id2 = atoi(token);
            token = strtok(NULL," ");
            distance = atol(token);
            G->edges[id][id2] = distance;
            G->e += 1;
        }
        G->n = id;
    }
    G->e/=2;
    //初始化矩阵中的点 将不连通的点设为最大值，并初始化每个景点的描述
    Init_matrix(G);
    Init_features(G);
    fclose(fp);
}
void Init_features(MGraph *G)
{
    //利用构建好的邻接矩阵来初始化描述信息
    for(int i = 1;i <= G->n;i++)
    {
        G->vexs[i].features = "";
        for(int j = 1;j<=G->n;j++)
        {
            if(G->edges[i][j]!=MAX &&i!=j)
            {
                string s(G->vexs[j].name);
                G->vexs[i].features += s+",距离为："+std::to_string(G->edges[i][j])+"米。\n";
            }
        }
    }
}
void GetVex(MGraph *G, int v)
//用于对景点进行介绍
{
    if(v<1 ||v>G->n)
    {
        cout<<"请输入正确的编号"<<endl;
        return;
    }
    cout<<"该景点为"<<G->vexs[v].name<<" 附近有："<<endl;
    cout<<G->vexs[v].features<<endl;
}
int login(void)
{
    //登陆程序 账户信息从文件中读取 返回一个状态值 -1表示失败 0 1表示游客和管理员
	char account[20]={0},password[16]={0},buff[50]={0};
	char account_right[20],password_right[20];
	FILE* fp;
	if((fp=fopen("login.txt","r"))==NULL){
		cout<<"文件打开失败";
		exit(0);
	}
	cout<<"请输入您的账号:";
	fgets(account,19,stdin);
    account[strlen(account)-1] = '\0';
	cout<<"请输入您的密码(区分大小写):";
    //可以对密码进行隐式输入
	for(int i = 0;i < 16;)
    {
        char a;
        if((a=getch())!='\r') {
        	if(a=='\b' && i > 0){
        		cout<<"\b \b";
        		i--;
        		password[i] = '\0';
        		continue;
        	}
        	else if(a!='\b'){
            	cout<<"*";
            	password[i] = a;
            }
        }
        else{
            password[i]='\0';
            break;
        }
        i++;
    }
    //与文件中的信息进行比对
	while(fgets(buff,50,fp)) {
		int flag;
		sscanf(buff,"%s %s %d",account_right,password_right,&flag);
		if(strcmp(account,account_right)==0 && strcmp(password,password_right) == 0)
		{
			fclose(fp);
			return flag;
		}
	}
	cout<<endl<<"登录失败，账号或密码错误！"<<endl<<endl;
    fclose(fp);
	return -1;
}
void ToDestination(MGraph *G, int v, int w)
{
    //利用Dijkstra算法求两个点的最短路径

    if(v>G->n ||w>G->n ||v<1 ||w<1) {
        cout<<"数据非法，请重新输入!"<<endl;
        return;
    }
    int j;
    vector<int>FInal(G->n+1,0);
    FInal[v] = 1;
    for(int i = 1;i <= G->n;i++)
    {
        D[i] = G->edges[v][i];
        P[i] = 0;
    }
    D[v] = 0,P[v] = -1;
    for(int i = 1;i <=G->n;i++)
    {
        if(i==v)
            continue;
        int min = MAX + 1;
        for(int k =1;k <= G->n;k++)
        {
            if(FInal[k]==0&&D[k]<min)
            {
                j = k;
                min = D[k];
            }
        }
        FInal[j] = 1;
        for(int k = 1;k<=G->n;k++)
        {
            if(FInal[k]==0 && D[j]+G->edges[j][k]<D[k])
            {
                D[k] = D[j]+G->edges[j][k];
                P[k] = j;
            }
        }
    }
    int pre;
    stack<string>res;
    cout<<G->vexs[v].name<<"到"<<G->vexs[w].name<<"的最短路径为:"<<D[w]<<endl;
    pre = P[w];
    while(pre>=v)
    {
        string tmp(G->vexs[pre].name);
        res.push(tmp);
        pre = P[pre];
    }
    cout<<G->vexs[v].name;
    while(!res.empty())
    {
        cout<<"->"+res.top();
        res.pop();
    }
    cout<<"->"<<G->vexs[w].name<<endl;
}
void InsertArc(MGraph *G,int v, int w)
{
    if(v==w ||v>G->n||v<1||w>G->n||w<1)
    {
        cout<<"数据有误，请重新输入!"<<endl;
        return;
    }
    if(G->edges[v][w]!=MAX)
        cout<<"两个结点之间已经存在道路"<<endl;
    else
    {
        int v1=v,v2=w,dis;
        cout<<"请输入要添加道路的两个点之间的距离:";
        cin>>dis;
        G->edges[v1][v2] = G->edges[v2][v1] = dis;
        cout<<"插入成功"<<endl;
        Init_features(G);
    }
}
void DeleteArc(MGraph *G ,int v,int w)
{
    if(G->edges[v][w]!=MAX &&v!=w){
        G->edges[v][w] = G->edges[w][v] = MAX;
        cout<<"删除成功";
        Init_features(G);
    }
    else
        cout<<"两个景点间不存在道路!"<<endl;
}
void Init_matrix(MGraph* G)
{
    //对邻接矩阵初始化  无路的赋值为MAX
    for(int i = 1;i<= G->n;i++)
        for(int j = 1;j <=G->n;j++)
            if(G->edges[i][j]==0 &&i!=j)
                G->edges[i][j] = MAX;
}
void Display(void)
{
    //从文件中读取所有景点的列表
    FILE* fp = fopen("places.txt","r");
    char message[100];
    if(fp==NULL)
    {
        cout<<"文件打开失败\n";
        exit(0);
    }
    while(fgets(message,100,fp))
    {
        cout<<message;
    }
    fclose(fp);
}
void recreate_graph(MGraph *G)
{
    FILE* fp = fopen("temp.txt","w");
    if(!fp)
    {
        cout<<"文件打开或创建失败!"<<endl;
        exit(0);
    }
    for(int i = 1;i <=G->n;i++)
    {
        //根据邻接矩阵中的内容和原来的文件格式重建邻接矩阵
        char result[100];
        sprintf(result,"%d %s\n",i,G->vexs[i].name);
        fputs(result,fp);
        for(int j = 1;j <=G->n;j++)
        {
            if(G->edges[i][j]!=0 && G->edges[i][j]!=MAX)
            {
                sprintf(result,"%d %d ",j,G->edges[i][j]);
                fputs(result,fp);
            }
        }
        fputs("\n",fp);
    }
    fclose(fp);
    remove("message.txt");
    rename("temp.txt","message.txt");
}
void ShortestPath(MGraph *G, int *P, int *D,int pos)
{
    int j;//利用Dijkstra算法求任意两点间的最短路径
    vector<int>FInal(G->n+1,0);
    FInal[pos] = 1;
    for(int i = 1;i <= G->n;i++)
    {
        D[i] = G->edges[pos][i];
        P[i] = 0;
    }
    D[pos] = 0,P[pos] = -1;
    for(int i = 1;i <=G->n;i++)
    {
        if(i==pos)
            continue;
        int min = MAX + 1;
        for(int k =1;k <= G->n;k++)
        {
            if(FInal[k]==0&&D[k]<min)
            {
                j = k;
                min = D[k];
            }
        }
        FInal[j] = 1;
        for(int k = 1;k<=G->n;k++)
        {
            if(FInal[k]==0 && D[j]+G->edges[j][k]<D[k])
            {
                D[k] = D[j]+G->edges[j][k];
                P[k] = j;
            }
        }
    }
    int pre;
    for(int i = 1;i <= G->n;i++)
    {
        if(i==pos)
            continue;
        stack<string>res;
        cout<<G->vexs[pos].name<<"到"<<G->vexs[i].name<<"的最短路径为:"<<D[i]<<endl;
        cout<<G->vexs[pos].name;
        pre = P[i];
        while(pre>=pos)
        {
            string tmp(G->vexs[pre].name);
            res.push(tmp);
            pre = P[pre];
        }
        //输出的时候借助一个栈来存储字符串
        while(!res.empty())
        {
            cout<<"->"+res.top();
            res.pop();
        }
        cout<<"->"<<G->vexs[i].name<<endl;
    }
}
void manager(MGraph *G)
{//管理员系统，主要结构就是一个switch语句
    int flag =1,option,code,code2,mark=0;
    while(flag){
        cout<<"\n欢迎管理员访问本系统!"<<endl;
        cout<<"你可以进行以下操作："<<endl;
        cout<<"1.查看当前所有景点"<<endl;
        cout<<"2.介绍某一景点"<<endl;
        cout<<"3.查看某一景点到其他所有结点的最短路径"<<endl;
        cout<<"4.查看任意某两个景点间的最短路径"<<endl;
        cout<<"5.增加道路"<<endl;
        cout<<"6.删除道路"<<endl;
        cout<<"7.修改景点信息"<<endl;
        cout<<"8.增加景点"<<endl;
        cout<<"9.删除景点"<<endl;
        cout<<"10.退出程序"<<endl;
        cout<<"请输入你要进行的操作:";
        cin>>option;
        switch(option)
        {
            case 1:
                Display();
                break;
            case 2:
                cout<<"请输入景点编号:"<<endl;
                cin>>code;
                GetVex(G,code);
                break;
            case 3:
                for(int i = 1;i<=G->n;i++){
                    cout<<endl;
                    ShortestPath(G,P,D,i);
                }
                break;
            case 4:
                cout<<"请输入两个景点的编号:"<<endl;
                cin>>code>>code2;
                ToDestination(G,code,code2);
                break;
            case 5:
                cout<<"请输入两个位置的编号:";
                cin>>code>>code2;
                InsertArc(G,code,code2);
                mark = 1;
                break;
            case 6:
                cout<<"请输入两个位置的编号:";
                cin>>code>>code2;
                DeleteArc(G,code,code2);
                mark = 1;
                break;
            case 7:
                cout<<"请输入要修改的景点编号:"<<endl;
                cin>>code;
                change_palce(G,code);
                mark = 1;
                break;
            case 8:
                add_place(G);
                mark = 1;
                break;
            case 9:
                cout<<"请输入要删除的景点编号:"<<endl;
                cin>>code;
                mark = 1;
                del_place(G,code);
                break;
            case 10:
                flag = 0;
                break;
            default:
                cout<<"参数错误，请重新输入!"<<endl;
                break;
        }
        system("pause");
        system("cls");
    }
    if(mark)
        recreate_graph(G);
}
void guest(MGraph *G)
{
    int option,code,code2,flag=1;
    while(flag)
    {//与manager相比，除了功能少一些，主要结构相同
        cout<<"\n欢迎您访问本导航系统!"<<endl;
        cout<<"你可以进行以下操作："<<endl;
        cout<<"1.查看当前所有景点"<<endl;
        cout<<"2.介绍某一景点"<<endl;
        cout<<"3.查看某一景点到其他所有结点的最短路径"<<endl;
        cout<<"4.查看任意某两个景点间的最短路径"<<endl;
        cout<<"5.退出程序"<<endl;
        cout<<"请输入你要进行的操作:";
        cin>>option;
        switch(option)
        {
            case 1:
                Display();
                break;
            case 2:
                cout<<"请输入景点编号:"<<endl;
                cin>>code;
                GetVex(G,code);
                break;
            case 3:
                for(int i = 1;i<=G->n;i++){
                    cout<<endl;
                    ShortestPath(G,P,D,i);
                }
                break;
            case 4:
                cout<<"请输入两个景点的编号:"<<endl;
                cin>>code>>code2;
                ToDestination(G,code,code2);
                break;
            case 5:
                flag=0;
                break;
            default:
                cout<<"参数错误，请重新输入!"<<endl;
                break;
        }
        system("pause");
        system("cls");
    }
}
void del_place(MGraph *G,int code)
{
    //删除节点并对景点描述进行初始化
    if(code>G->n ||code < 1)
    {
        cout<<"该点不存在!"<<endl;
        return;
    }
    for(int i = 1;i <= G->n;i++)
    {
        if(G->edges[code][i]!=0 && G->edges[code][i]!=MAX)
            G->edges[code][i] = G->edges[i][code] = MAX;
    }
    cout<<"删除成功!"<<endl;
    G->n-=1;
    Init_features(G);
}
void change_palce(MGraph *G,int code)
{
    int node,dis;
    cout<<"请输入你要改变的与"<<code<<"相邻的结点的编号:";
    cin>>node;
    cout<<"请输入一个新的的距离"<<endl;
    cin>>dis;
    G->edges[code][node] = G->edges[node][code] = dis;
    Init_features(G);
    cout<<"修改成功!"<<endl;
}
void add_place(MGraph *G)
{
    //添加景点并初始化景点描述
    char name[20];
    int n;
    cout<<"请输入你要添加的景点名称："<<endl;
    cin>>name;
    cout<<"请输入你要加入的道路数量:";
    cin>>n;
    cout<<"请输入与该点相邻的结点和距离:"<<endl;
    G->n+=1;
    strcpy(G->vexs[G->n].name,name);
    for(int i = 1;i<=n;i++)
    {
        int code,dis;
        cin>>code>>dis;
        G->edges[code][G->n] = G->edges[G->n][code] = dis;
    }
    Init_features(G);
}
