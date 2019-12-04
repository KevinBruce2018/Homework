#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>

typedef struct MENU
{
	int price;
	char name[50];
	char guest[20];
	int sum;
}menus;
typedef struct LOGIN
{
	char account[20];
	int flag;
}LOGIN;

LOGIN login(void);
void manager_menu(void);
void guest_menu(char* account);
int menu_search_input_update(void);
void display_menu(void);
int sum(char* account);
int search_order(char* account);
void input(void);
void order(char* account);
void delete_order(char* account);
void look_up(void);
int account_management(void);
void new_account(void);
void del_account(void);
void chang_password(void);
void del_menu(void);

int main(void)
{
	int i=0;
	LOGIN log;
	//用户登录
	printf("欢迎访问饭店点餐系统！\n");
	log=login();
	//登陆次数达到一定次数时自动退出系统
	while(log.flag==-1)
	{
		i++;
		if(i==3) {
			printf("账号不存在或密码错误多次，系统将自动退出!\n");
			exit(0);
		}
		printf("请重新输入您的账号密码!\n");
		log=login();
	}
	//登陆成功
	//printf("\n成功登陆%s\n",log.flag==1?"饭店管理系统!":"!");
	//饭店功能界面 管理员进入manager_menu函数进行操作，客人则进入guest_menu中进行操作
	if(log.flag==1)
		manager_menu();
	else
		guest_menu(log.account);

	return 0;
}

LOGIN login(void)
{
	char account[20]={0},password[16]={0},buff[50]={0};
	char account_right[20],password_right[20];
	FILE* fp;
	LOGIN log;

	//打开保存有登陆信息的文件
	if((fp=fopen("login.txt","r"))==NULL){
		printf("文件打开失败");
		exit(1);
	}
	//输入用户的账号和密码，利用fegts函数防止出现安全问题
	printf("请输入您的账号:");
	fgets(account,19,stdin);
	for(int i = 19;i>=0;i--)
		if(account[i]=='\n')
			account[i] = '\0';
	printf("请输入您的密码(区分大小写):");
	//在输入密码时借助 grtch()进行隐式输入，防止密码被周围人看到
	for(int i = 0;i < 16;)
    {
        char a;
        if((a=getch())!='\r') {
        	if(a=='\b' && i > 0){
        		printf("\b \b");
        		i--;
        		password[i] = '\0';
        		continue;
        	}
        	else if(a!='\b'){
            	printf("*");
            	password[i] = a;
            }
        }
        else{
            password[i]='\0';
            break;
        }
        i++;
    }
	strcpy(log.account,account);
	//从保存有登录信息的文件中将用户名和密码一一进行比对，如果用户名
	//与密码均匹配则登陆成功，返回一个成功的状态
	while(fgets(buff,50,fp)) {
		int flag;
		sscanf(buff,"%s %s %d",account_right,password_right,&flag);
		if(strcmp(account,account_right)==0 && strcmp(password,password_right) == 0)
		{
			log.flag = flag;
			fclose(fp);
			return log;
		}
	}

	fclose(fp);
	fp = NULL;
	//如果检测到用户名或密码有误，则返回一个登陆失败的状态
	printf("\n登录失败，账号或密码错误！\n\n");
	log.flag = -1;
	return log;

}

void manager_menu(void)
{
	int fun_code;

	fflush(stdin);
	//借助死循环保证用户可以多次进行操作
	while(1)
	{
		system("cls");
		printf("\n请选择您要进行操作的编号：\n");
		printf("1.查看及更改用户账户信息\n");
		printf("2.输入,查询及更新,删除菜单信息\n");
		printf("3.顾客点单信息浏览\n");
		printf("4.退出登录\n");
		scanf("%d%*c",&fun_code);
		//通过输入选项的数字代码进入不同的功能管理界面
		//每个管理界面都做了不同程度的细化
		switch(fun_code)
		{
			case 1:
			//对所有用户的账号密码进行管理
				account_management();
				break;
			case 2:
			//对菜单进行增加，查找和更新以及删除操作
				menu_search_input_update();
				break;
			case 3:
			//浏览顾客信息
				look_up();
				break;
				//退出程序
			case 4:
				  printf("退出成功!\n");
				  exit(0);
				//处理输入错误代码测情况
			default:
				printf("请输入正确的代码!\n");
				break;
		}
	}
}
//顾客可以进行的操作
void guest_menu(char* account)
{
	int fun_code,tol;

	fflush(stdin);
	//清空缓冲区防止受残余字符影响
	//借助死循环实现多次操作，内置循环结束的出口
	while(1){
		system("cls");
		printf("\n请选择您要进行操作的编号：\n");
		printf("1.菜单信息查询\n");
		printf("2.点餐信息查询与修改\n");
		printf("3.费用结算\n");
		printf("4.退出登录\n");
		scanf("%d%*c",&fun_code);

		switch(fun_code)
		{
			case 1:
			//菜单显示
				display_menu();
				break;
			case 2:
			//包含点餐及点餐信息查询等功能
				search_order(account);
				break;
			case 3:
			//计算点餐总价
				tol=sum(account);
				printf("你总计消费了%d元\n",tol);
				break;
			case 4:
			//退出程序
				printf("退出成功!\n");
				exit(0);
				break;  
			default:
				printf("请输入正确的代码!\n");
				break;
		}
		printf("按任意键继续进行后续操作!");
		getch();
	}
}
//管理员对菜单进行插入，浏览和更新等操作
int menu_search_input_update(void)
{
	int fun_code = 0;
	display_menu();
	
	while(1){
		printf("请选择您要进行的操作:\n");
		printf("1.添加菜单信息\n");
		printf("2.更新菜单信息\n");
		printf("3.删除部分菜单\n");
		printf("4.返回上一级\n");
		scanf("%d%*c",&fun_code);
		switch(fun_code)
		{
			case 1:
				input();
				break;
			case 2:
			//更新菜单后，重新显示菜单中的信息
				display_menu();
				break;
				//返回上一级
			case 3:
				del_menu();
				break;
			case 4:
				return 0;
			default :
				printf("请重新输入!\n");
				break;
		}
	}
	return 0;

}
//显示菜单函数
void display_menu(void)
{
	char buff[60]={0};
	int n = 0;
	//打开菜单所在的文件
	FILE* fp = fopen("menu.txt","r");
	menus menu;

	if(!fp){
		printf("文件打开失败！\n");
		exit(0);
	}

	printf("编号      菜名          价格\n");
	//分行读取菜单信息并利用格式化字符串输出
	while(fgets(buff,60,fp))
	{
		sscanf(buff,"%s %d%*c",menu.name,&menu.price);
		printf("%04d      %-10s    %d\n",++n,menu.name,menu.price);
	}

	fclose(fp);
	fp = NULL;
}
//顾客对菜单信息的查询以及点餐等操作
int search_order(char* account)
{
	fflush(stdin);
	int fun_code;
	//首先显示顾客的用餐情况
	sum(account);

	while(1)
	{
		printf("请选择您要进行的操作\n");
		printf("1.点餐或追加点餐\n");
		printf("2.删除菜单\n");
		printf("3.点餐情况更新\n");
		printf("4.返回上一级\n");
		scanf("%d%*c",&fun_code);
		switch(fun_code)
		{
			case 1:
			//若已经点餐，则追加点餐，否则，重新点餐
				order(account);
				break;
			case 2:
			//删除用户已经点的餐
				delete_order(account);
				break;
			case 3:
			//计算顾客的总共花费
				sum(account);
				break;
			case 4:
			//返回上一级
				return 0;
			default :
				printf("代码错误请重新输入\n");
				break;
		}
	}

}
//用户总共消费情况
int sum(char* account)
{
	//打开存有用户消费信息的文件
	FILE* fp=fopen("message.txt","r");
	char buff[1000];
	int price,sum=0;

	if(fp==NULL)
	{
		printf("文件打开失败!\n");
		exit(0);
	}
	//读取文件内容
	while(fgets(buff,1000,fp))
	{
		char *tok;
		tok = strtok(buff," ");
		if(strcmp(tok,account)==0)
		{
			printf("请核对您的点餐信息，有问题请咨询管理员!\n");
			while(tok=strtok(NULL," "))
			{
				//如果没有进行点餐，直接结束程序，返回0值，否则，计算总共花费，并输出用户的点餐情况
				if(strcmp("未点餐",tok)==0)
				{
					printf("您没有点餐!\n");
					break;
				}
				printf("%s ",tok);
				tok = strtok(NULL," ");
				printf("%s\n",tok);
				sum += (price = atoi(tok));
			}
			break;
		}
	}
	fclose(fp);
	return sum;
}
//添加菜单信息
void input(void)
{
	FILE* fp=fopen("menu.txt","a");
	int n;
	//在存有菜单信息的文件中追加新的菜品信息
	if(fp==NULL)
	{
		printf("文件打开失败！\n");
		exit(0);
	}
	printf("请输入您需要增加菜单的数量：");
	scanf("%d%*c",&n);
	//输入菜品信息并保存
	printf("请在每行输入菜名和价格，以空格分开:\n");
	for(int i = 0;i < n;i++)
	{
		char buff[200];
		fgets(buff,200,stdin);
		fputs(buff,fp);
	}
	fclose(fp);
	fp = NULL;
}
//用户进行点餐
void order(char* account)
{
	FILE *fp = fopen("message.txt","r");
	FILE *in = fopen("tmp.txt","w");

	char buff[200],newa[200],tmp_buff[200];
	char finally[200];
	if(fp==NULL || in==NULL)
	{
		printf("文件打开失败!\n");
		exit(0);
	}
	//显示当前菜单
	display_menu();
	fflush(stdin);
	printf("请在一行中输入要点的菜，以空格隔开:\n");
	//读取用户输入的菜品信息，并将用餐信息保存到一个新的文件中，最终删除旧文件，并对新文件进行改名，改成已删除文件的名字
	while(fgets(buff,200,fp))
	{
		strcpy(tmp_buff,buff);
		char* tok = strtok(buff," ");
		if(strcmp(account,tok)==0)
		{
			tok = strtok(NULL," ");
			fgets(newa,200,stdin);
			if(strcmp(tok,"未点餐\n")==0)
				sprintf(finally,"%s %s",account,newa);
			else{
				int len=strlen(tmp_buff);
				tmp_buff[len-1] = '\0';
				
				sprintf(finally,"%s %s",tmp_buff,newa);
			}
			fputs(finally,in);
		}
		else
			fputs(tmp_buff,in);
	}
	printf("点餐成功!\n");
	fclose(fp),fclose(in);
	//点餐成功，关闭文件，并且对原文件进行删除，对新的文件改成原来文件的名字
	remove("message.txt");
	rename("tmp.txt","message.txt");
}
//菜单删除
void delete_order(char* account)
{
	FILE* fp = fopen("message.txt","r");
	FILE* in = fopen("tmp.txt","w");
	char buff[200],tmp_buff[200],rlt[200];
	if(fp==NULL || in==NULL)
	{
		printf("文件打开失败!\n");
		exit(0);
	}

	while(fgets(buff,200,fp))
	{
		strcpy(tmp_buff,buff);
		char* tok = strtok(buff," ");
		if(strcmp(account,tok)==0){
			sprintf(rlt,"%s %s\n",account,"未点餐");
			fputs(rlt,in);
		}
		else
			fputs(tmp_buff,in);
	}

	printf("菜单删除成功!\n");
	fclose(fp),fclose(in);
	remove("message.txt");
	rename("tmp.txt","message.txt");
}

//管理员浏览客户点单信息
void look_up(void)
{
	FILE* fp = fopen("message.txt","r");
	char buff[200];
	if(!fp)
	{
		printf("文件打开失败\n");
		exit(0);
	}
	printf("\n客户点单信息总览:\n");
	while(fgets(buff,200,fp))
	{
		printf("%s",buff);
	}
	fclose(fp);
	system("pause");
}
//管理员对所有账户信息进行修改
int account_management(void)
{
	FILE* fp = fopen("login.txt","r");
	char buff[200];
	int fun_code;
	fflush(stdin);
	//显示当前所有用户的一个列表
	printf("当前的用户列表:\n");
	while(fgets(buff,200,fp))
	{
		printf("%s\n",strtok(buff," "));
	}
	fclose(fp);
	//用户操作
	printf("您可以:\n");
	printf("1.新建用户\n");
	printf("2.删除用户\n");
	printf("3.修改用户密码\n");
	printf("4.返回上一级\n");
	scanf("%d",&fun_code);
	switch(fun_code)
	{
		case 1:
		//创建新用户
			new_account();
			break;
		case 2:
		//删除已有的账户
			del_account();
			break;
		case 3:
		//修改用户密码
			chang_password();
			break;
		case 4:
			return 0;
		default :
		//处理代码输入错误的问题
			printf("你输入的代码不正确，请重新输入!");
			break;
	}
}
//创建新用户，方法是在原文件末尾追加新的用户信息
void new_account(void)
{
	fflush(stdin);
	FILE* fp = fopen("login.txt","a");
	if(fp==NULL)
	{
		fprintf(stderr, "%s\n","文件打开失败");
		exit(0);
	}
	char tmp[200];
	printf("请输入你要创建的用户名以及密码，在同一行中输入,以空格隔开:\n");
	gets(tmp);
	fprintf(fp, "%s %d\n",tmp,0);
	fclose(fp);
	printf("新用户创建成功!\n");
}
//删除旧有的一些账号
void del_account(void)
{
	//读取需要删除的账号信息，然后其他账号信息储存到一个新文件中，移除旧文件，把新文件的名字改为与旧文件相同
	FILE* fp = fopen("login.txt","r");
	FILE* tmp = fopen("tmp.txt","w");
	char a[80],buff[200],account[80];
	if(!fp || !tmp)
	{
		printf("文件打开失败\n");
		exit(0);
	}
	printf("请输入您要删除的账户名：\n");
	scanf("%s%*c",a);
	while(fgets(buff,200,fp))
	{
		sscanf(buff,"%s",account);
		if(strcmp(account,a) != 0)
			fputs(buff,tmp);
	}
	printf("删除成功!\n");
	fclose(fp),fclose(tmp);
	remove("login.txt");
	rename("tmp.txt","login.txt");
}
//用户密码修改 方法与删除账户类似
void chang_password(void)
{
	FILE* fp = fopen("login.txt","r");
	FILE* tmp = fopen("tmp.txt","w");
	char account[80],password[80],buff[200];
	if(!fp || !tmp)
	{
		printf("文件打开失败\n");
		exit(0);
	}
	printf("请输入您要进行修改密码的账号以及新密码，以空格隔开\n");
	scanf("%s %s",account,password);
	while(fgets(buff,200,fp))
	{
		char o_account[80],o_password[80],flag,new_line[200];
		sscanf(buff,"%s %s %d",o_account,o_password,&flag);
		if(strcmp(o_account,account) == 0){
			sprintf(new_line,"%s %s %d\n",o_account,password,flag);
			fputs(new_line,tmp);
		}
		else
			fputs(buff,tmp);
	}
	printf("密码修改成功!\n");
	fclose(fp),fclose(tmp);	
	remove("login.txt");
	rename("tmp.txt","login.txt");
}
//删除部分菜单
void del_menu(void)
{
	FILE* fp = fopen("menu.txt","r");
	FILE* tmp = fopen("tmp.txt","w");
	char menu[80],buff[200];
	if(!fp || !tmp)
	{
		printf("文件打开失败\n");
		exit(0);
	}
	printf("请输入您要进行删除的菜品\n");
	scanf("%s",menu);
	while(fgets(buff,200,fp))
	{
		char o_menu[80];
		sscanf(buff,"%s",o_menu);
		if(strcmp(o_menu,menu) != 0)
			fputs(buff,tmp);
	}
	printf("菜单修改成功!\n");
	fclose(fp),fclose(tmp);
	remove("menu.txt");
	rename("tmp.txt","menu.txt");
}
