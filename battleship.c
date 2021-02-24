#include <WINSOCK2.H>  
#include <STDIO.H>  

#pragma comment(lib, "ws2_32")  

#define SERVER_PORT 5555 
#define SERVER_IP "127.0.0.1" 

typedef struct
{
char name[20];
int size;
char icon;
}Ships;
int Score;
int Status=-1;
char *message[5]={"you choise a point already attacked!","You input not right!","You Win!","You failed!","Unexpected error!"};
int S,N;
Ships *Creation();//Read the data from local
char **Creation_Z();
char **Random(char **zone,int size,char icon);//Recursive
char **Arrangement(Ships *ship,char **zone);
int Check(int x,int y,char **zone);//0 normal 1 already attacked  2 error input
char Attack(int x,int y,char **zone,int *score);
void Refresh(char **A,char **B);
void Play();
int Server();
int Client();
void Rule();
void gotoxy(int x, int y);
int Table();

void main(){
	int c;
	while(1){
			c=Table();
			switch(c)
			{
			case 1:
				Play();
				break;
			case 2:
				Server();	
				break;
			case 3:
				Client();
				break;
			case 4:
				Rule();
				break;
			case 5:
				exit(0);
			default:
				break;
		}
	}
}

Ships *Creation(){//Read the data from local Initialize these battleships
	Score=0;
	FILE *fp;
	if(!(fp=fopen("battleship.txt","r")))
	{
		printf("exit!");
		exit(0);
	}
	fscanf(fp,"%d\n",&S);
	//printf("%d\n",S);
	fscanf(fp,"%d\n",&N);
	//printf("%d\n",N);
	Ships *ship = (Ships*)malloc(sizeof(Ships)*N);
	int i;
	for(i = 0; i<N ;i++)
	{
		fscanf(fp,"%s %d %c\n",&ship[i].name,&ship[i].size,&ship[i].icon);
		//printf("%s %d %c\n",ship[i].name,ship[i].size,ship[i].icon);
		Score+=ship[i].size;
	}
	fclose(fp);
	return ship;
}
char **Creation_Z(){//Initialize the board
	char **Z=(char**)malloc(sizeof(char*)*S);
		int i;
		for (i = 0; i < S ; i++)
		{
			Z[i] = (char*)malloc(sizeof(char)*S);
			int j;
			for(j = 0 ; j < S ; j++)
			{
				Z[i][j]='-';
			}
		}	
		return Z;
}
char **Random(char **zone,int size,char icon){//Recursive
		//printf("size = %d, icon = %c ",size,icon);
		//printf("loading\n");
		//system("cls");
		Sleep(10); 
		srand((unsigned int)time(NULL));
		int turn=rand()%2;
		//printf("turn = %d, ",turn);
		int flag = 1 ;
		if(turn==1){//0 horizontal arrangement 1 vertical arrangement
			srand((unsigned int)time(NULL));
			int x=rand()%(S-size);
			int y=rand()%S;
			//printf("x = %d, y = %d\n",x,y);
			int s=0;
			while(s<size)//check 
			{
				if(zone[x+s][y]!='-')
				{
					flag = 0;
					break;
				}
				s++;
			}
			//printf("%d \n",flag);
			if(flag == 1){
				int s=0;
				while(s<size)
				{
					zone[x+s][y]=icon;
					s++;
				}
				return zone;
			}
			else
			{
				return Random(zone,size,icon);
			}
		}
		else if(turn==0)
		{
			srand((unsigned int)time(NULL));
			int y=rand()%(S-size);
			int x=rand()%S;
			//printf("x = %d, y = %d\n",x,y);
			int s=0;
			while(s<size)
			{
				if(zone[x][y+s]!='-')
				{
					flag=0;
					break;
				}	
				s++;
			}
			if(flag == 1){
				int s=0;
				while(s<size)
				{
					zone[x][y+s]=icon;
					s++;
				}
				return zone;
			}
			else
			{
				return Random(zone,size,icon);
			}
		}
}
char **Arrangement_Random(Ships *ship,char **zone){
	int n;
	for(n=0;n<N;n++){
		zone = Random(zone,ship[n].size,ship[n].icon);
	}
	return zone;
}

char **Arrangement_Choose(Ships *ship,char **zone){
	int n,m=0;
	char **z=Creation_Z();
	for(n=0;n<N;n++){
		int flag=1;
		int x=0,y=0,t=0;
		while(flag==1){	
			int i,j;
			for(i=0;i<S;i++){
				for(j=0;j<S;j++){
					z[i][j]=zone[i][j];
				}
			}
			int s=0;
			while(s<ship[n].size)
			{
				if(t==1){
					z[x][y+s]=ship[n].icon;
				}
				else if(t==0)
				{
					z[x+s][y]=ship[n].icon;
				}
				s++;
			}
			system("cls");
			printf("\tMain board\n");
			int d;
			for(d=0;d<10;d++){
				printf("%2d\t",d+1);
				int j;
				for(j=0;j<10;j++)
					printf("%c ",z[j][d]);
				printf("\n");
			}
			printf("  \tA B C D E F G H I J \n Set the %s at %c%d to %c%d\n",ship[n].name,x+65,y+1,(t==0)?x+64+ship[n].size:x+65,(t==1)?y+ship[n].size:y+1);
			if(m==1)
				printf("Can not set in there!\n");
			else if(m==2) 
				printf("Can not rotate!\n");
			m=0;
			
			char c=getch();
			switch(c){
				case 'w':if(y>0)y-=1;
						break;
				case 's':if(t==0&&y<S-1||t==1&&y<S-ship[n].size)y+=1;
						break;
				case 'a':if(x>0)x-=1;
						break;
				case 'd':if(t==0&&x<S-ship[n].size||t==1&&x<S-1)x+=1;
						break;
				case 't':if(t==0&&y<S-ship[n].size+1)
							t=1;
						else if(t==1&&x<S-ship[n].size+1)
							t=0;
						else
							m=2;
						break;
				case ' ':
					s=0;
					int f=1;
					if(t==0) 
						while(s<ship[n].size)
						{
							
							if(zone[x+s][y]!='-'){
								f=0;
								break;
							}
							s++;
						}
					if(t==1) 
						while(s<ship[n].size)
						{
							
							if(zone[x][y+s]!='-'){
								f=0;
								break;
							}
							s++;
						}
					s=0;
					if(f==1)
					{
						while(s<ship[n].size)
						{
							if(t==1){
								zone[x][y+s]=ship[n].icon;
							}
							else if(t==0)
							{
								zone[x+s][y]=ship[n].icon;
							}
							else
								printf("%s\n",message[4]);
							s++;
						}
						flag=0;
					}
					else
						m=1;
					break;
			}
		}
		
	}
	return zone;
}
char **Arrangement(Ships *ship,char **zone){
	
	while(1)
	{
		system("cls");
		char c;
		printf("Choose a way to arrange you ships:\n1 Choose\n2 Random\n");
		c=getch();
		switch(c){
			case '1':return Arrangement_Choose(ship,zone);
			case '2':return Arrangement_Random(ship,zone);
		}
	}
	return zone;
}
int Check(int x,int y,char **zone){//0 normal 1 already attacked  2 error input
	//printf("%d,%d : %c",x,y,zone[x][y]);
	if(x>=0&&x<S&&y>=0&&y<S)
	{
	if(zone[x][y]=='*')
		return 1;
	else if(zone[x][y]>='a'&&zone[x][y]<='z')
		return 1;
	else if(zone[x][y]>='A'&&zone[x][y]<='Z')
		return 0;
	else if(zone[x][y]=='-')
		return 0;
	else
		return 2;
	}
	else
		return 2;
}
char Attack(int x,int y,char **zone,int *score){
	if(zone[x][y]>='A'&&zone[x][y]<='Z')
	{
		int s=*score;
		s++;
		zone[x][y]+=32;
		*score=s;
	}
	else if(zone[x][y]=='-')
	{
		zone[x][y]='*';	
	}
	return zone[x][y];
	//return zone;
}
void Refresh(char **A,char **B){//
	system("cls");
	printf("\tMain board\t\tMark board\n");
	int i;
	for(i=0;i<10;i++){
		printf("%2d\t",i+1);
		int j;
		for(j=0;j<10;j++)
			printf("%c ",A[j][i]);
		printf("\t");
		for(j=0;j<10;j++)
			printf("%c ",B[j][i]);
		printf("\n");
	}
	printf("  \tA B C D E F G H I J \tA B C D E F G H I J\n");
}

void Play(){//Stand-alone game
	Ships *ship = Creation();
	char **A =  Creation_Z();
	char **B =  Creation_Z();
	char **fog = Creation_Z();
	A = Arrangement(ship,A);
	B = Arrangement_Random(ship,B);
	int scoreA = 0;
	int scoreB = 0;
	int x1,y1;
	int x2,y2;
	while(1){
		Refresh(A,fog);
		if(Status==0)
			printf("You attacked %c%d, Enemy attacked %c%d.\n",x1+65,y1+1,x2+65,y2+1);
		else if(Status==1)
		{
			printf("%s\n",message[0]);
		}
		else if(Status==2)
		{
			printf("%s\n",message[1]);
		}
		if(scoreA==Score)
		{
			printf("%s\n",message[2]);
			break;
		}
		else if(scoreB==Score)
		{
			printf("%s\n",message[3]);
			break;
		}
		//printf("A:%d,B:%d\n",scoreA,scoreB);
		char X;
		int Y;
		fflush(stdin);
		scanf("%c%d",&X,&Y);
		
		if(X>='A'&&X<='Z'){
			x1=X-65;
		}
		else if(X>='a'&&X<='z'){
			x1=X-97;
		}
		else 
			x1=X;
		y1=Y-1;
		Status=Check(x1,y1,B);
		//printf("%d,%d\n",x1,y1);
		if(Status==0)
		{
			fog[x1][y1]=Attack(x1,y1,B,&scoreA);
			while(1){
				srand((unsigned int)time(NULL));
				x2=rand()%S;
				y2=rand()%S;
				if(Check(x2,y2,A)==0)
				{
					Attack(x2,y2,A,&scoreB);
					break;
				}
			}
		}
		
	}
}
int Server()  
{  
    WORD sockVision = MAKEWORD(2,2);  
    WSADATA wsadata;  
    SOCKET slisten;  
    SOCKET sClient;  
    struct sockaddr_in remoteAddr;  
    struct sockaddr_in sin;  
    //int ret = -1;  
    int nAddrlen = sizeof(remoteAddr);  
    char revdata[255];  
    char senddata[255];  
    if( WSAStartup(sockVision,&wsadata) != 0 )  
    {  
        printf("WSA initialization failed.\n");  
        return 0;  
    }  
    //Create a socket  
    slisten = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
    if(slisten == INVALID_SOCKET)  
    {  
        printf("Failed to create socket listener.\n");  
        return 0;  
    }  
    //Bind IP and port 
    sin.sin_family = AF_INET;  
    sin.sin_port = htons(SERVER_PORT);  
    sin.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);  
    if( bind(slisten,(LPSOCKADDR)&sin,sizeof(sin)) == SOCKET_ERROR )  
    {  
        printf("Failed to bind IP and port.\n");  
        return 0;  
    }  
    //监听  
    if(listen(slisten,5) == SOCKET_ERROR)  
    {  
        printf("Failed to create listener.\n");  
        return 0;  
    }  
  	
    //Loop to receive data
    while(1)  
    {  
    	Ships *ship = Creation();
		char **A =  Creation_Z();
		char **fogA = Creation_Z();
		A = Arrangement(ship,A);
        printf("Waiting for connect.........\n");  
        sClient = accept(slisten,(SOCKADDR *)&remoteAddr,&nAddrlen);  
        if(sClient == INVALID_SOCKET)  
        {  
            printf("Accept client failed, is retrying.........\n");  
            continue;  
        } 
	  	
	  	int scoreA = 0;
		int scoreB = 0;
		Refresh(A,fogA);
		printf("Accept the client's successful：%s\n",inet_ntoa(remoteAddr.sin_addr));  
        //recive data  
        while(1)  
        {  
            recv(sClient,revdata,255,0);  //Accept client attack coordinates
            /*if(ret > 0)  
            {  
                revdata[ret] = 0x00;  
                printf("\n");  
                printf("Client：");  
                printf(revdata);  
                printf("\n");  
            }  
  			*/
            /*if(strcmp(revdata,"end") == 0)  
                break;  */
  			//Client attack the server
  			int x2=(int)revdata[0]-1;
  			int y2=(int)revdata[1]-1;//ASCII code is no 0 
  			char c2 = Attack(x2,y2,A,&scoreB);
			senddata[0] = c2; 
  			send(sClient,senddata,strlen(senddata),0); //Returned the results to the client 
  			
			Refresh(A,fogA);
  			printf("Enemy attacked %c%d.\n",x2+65,y2+1);
  			
			while(1)
			{
				char X;
				int Y;
				int x1,y1;
				printf("input :");
				fflush(stdin);
				scanf("%c%d",&X,&Y);
				if(X>='A'&&X<='Z'){
					x1=X-65;
				}
				else if(X>='a'&&X<='z'){
					x1=X-97;
				}
				else
				x1=X;
				//printf("x:%d",x1);
				y1=Y-1;
				//printf("y:%d",y1);
				Status=Check(x1,y1,fogA);
				//printf("%d",Status);
				if(Status==0)
				{
					//发送数据
					senddata[0]=(char)x1+1;
					senddata[1]=(char)y1+1;
					send(sClient,senddata,strlen(senddata),0); //Sent to client server attack coordinates
					//printf("%s\n",senddata);
					recv(sClient,revdata,255,0);//Receive attack results 
		            char c1 = revdata[0];
		            if(c1>='a'&&c1<='z'){
		            	scoreA++;	
					}
		            fogA[x1][y1]=c1;
					Refresh(A,fogA);
		  			printf("Your attacked %c%d.\n",x1+65,y1+1);
		  			 if(scoreA==Score)
					{
						printf("%s\n",message[2]);
						break;
					}
					else if(scoreB==Score)
					{
						printf("%s\n",message[3]);
						break;
			        }
					break;
				}
		            else if(Status==1)
					{
						printf("%s\n",message[0]);
					}
					else if(Status==2)
					{
						printf("%s\n",message[1]);
					}
            }
        }  
        closesocket(sClient);  
    }  
  
    closesocket(sClient);  
    WSACleanup();  
    system("pause");  
    return 0;  
}  

int Client()  
{  
    WORD sockVision = MAKEWORD(2,2);  
    WSADATA wsadata;  
    SOCKET sServer ;  
    struct sockaddr_in serAddr;  
    char senddata[255];  
    char revdata[255];  
    int ret = -1;  
    if(WSAStartup(sockVision,&wsadata) != 0)  
    {  
        printf("WSA initialization failed.\n");  
        return 0;  
    }  
  
    sServer = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
    if(sServer == INVALID_SOCKET)  
    {  
        printf("Socket client creation failed.\n");  
        return 0;  
    }  
  
    serAddr.sin_family = AF_INET;  
    serAddr.sin_port = htons(SERVER_PORT);  
    serAddr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);  
    if( connect(sServer,(SOCKADDR *)&serAddr,sizeof(serAddr)) == SOCKET_ERROR )  
    {  
        printf("Socket server connection failed.\n");  
        return 0;  
    }  
    
    Ships *ship = Creation();
	char **B =  Creation_Z();
	char **fogB = Creation_Z();
	B = Arrangement(ship,B);
	int scoreA = 0;
	int scoreB = 0;
	Refresh(B,fogB);
	printf("Connect to server successful!\n");
    while(1)  
    {  
    	
		while(1)
		{		
			char X;
			int Y;
			int x2,y2;
			printf("Input : ");
			fflush(stdin);
			scanf("%c%d",&X,&Y);
					
			if(X>='A'&&X<='Z'){
				x2=X-65;
			}
			else if(X>='a'&&X<='z'){
					x2=X-97;
			}
			else
				x2=X;
			//printf("x:%d ",x2);
			y2=Y-1;
			//printf("Y:%d y:%d ",Y,y2);
			//printf("%d,%d : %c\n",x2,y2,fogB[x2][y2]);
			Status=Check(x2,y2,fogB);
			//Refresh(B,fogB);
			//printf("%d\n",Status);
			if(Status==0)
			{
				//发送数据
				senddata[0]=(char)x2+1;
				senddata[1]=(char)y2+1;
				send(sServer,senddata,strlen(senddata),0); //Send the attack coordinates to the server
				
				recv(sServer,revdata,255,0);//Receive attack results
	            char c2 = revdata[0];
	            if(c2>='a'&&c2<='z'){
	            	scoreB++;	 
				}
	            fogB[x2][y2]=c2;
				Refresh(B,fogB);
				printf("Your attacked %c%d.\n",x2+65,y2+1);
				 if(scoreA==Score)
					{
						printf("%s\n",message[2]);
						break;
					}
					else if(scoreB==Score)
					{
						printf("%s\n",message[3]);
						break;
			        }
				break;
			}
		    else if(Status==1)
			{
				printf("%s\n",message[0]);
			}
			else if(Status==2)
			{
				printf("%s\n",message[1]);
			}
        }
		/* 
        if(ret > 0)  
        {  
            revdata[ret] = 0x00;  
            printf("\n");  
            printf("Server End：");  
            printf(revdata);  
            printf("\n");  
        }  */
        
        recv(sServer,revdata,255,0); //Receive the attack from the server coordinates
        int x1=(int)revdata[0]-1;
  		int y1=(int)revdata[1]-1;
  		char c1 = Attack(x1,y1,B,&scoreA);
		senddata[0] = c1; 
  		send(sServer,senddata,strlen(senddata),0); //Return the result to server
  		if(scoreA==Score)
				{
					printf("%s\n",message[2]);
					break;
				}
				else if(scoreB==Score)
				{
					printf("%s\n",message[3]);
					break;
		        }
		Refresh(B,fogB); 
  		printf("Enemy attacked %c%d.\n",x1+65,y1+1);
    }  
    closesocket(sServer);  
    WSACleanup();  
    system("pause");  
    return 0;  
}  


void Rule()//规则函数
{
	system("cls");
	printf("\t\t Game Rule \n");
	printf("1.Choose \"Single Play\" to play with AI.\n");
    printf("2.Choose \"Double Play\" to play with another player.\n");
	printf("3.Double Play (Server) to create a Server.\n");
    printf("4.Double Play (Client) to create a Client.\n");
    printf("4.You can put all the ships and movie the position by using \"W\", \"S\",\n");
    printf("  \"A\", \"D\", for UP, DOWN, LEFT, RIGHT and “T” to change the position\n");
    printf("  type between vertical and horizontal and \"SPACE\" to commit the position.\n");
	printf("5.Fire the point by input the coordinates(EX:a1 or A1).\n");
	printf("6.If you hit the right place the \"-\" will turn to \"c/b/s/d\",\n");
	printf("  then you can get one point for each right hit.\n");
	printf("7.If you miss the shoot, the \"-\" will turn to \"*\", and no score.\n");
	printf("8. 4 types of ships:\n");
	printf("Amount\t\tShips\t\tSize\t\tNot hit\t\tHit\n");
	printf("1\t\tCarrier\t5\t\tC\t\tc\n");
	printf("2\t\tBattleships\t4\t\tB\t\tb\n");
	printf("3\t\tSubmarines\t3\t\tS\t\ts\n");
	printf("4\t\tDestroyers\t2\t\tD\t\td\n");
	printf("\nPress any key to return.\n");
	getch();
}

void gotoxy(int x, int y)
{
	COORD coord = {x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

int Table()
{
	int c;
	system("cls");
	gotoxy(37,2);
	printf("Battleship Game");
	gotoxy(15,6);
	printf("1.Single Play\n");
	gotoxy(15,8);
	printf("2.Double Play (Server)\n");
	gotoxy(15,10);
	printf("3.Double Play (Client)\n");
	gotoxy(15,12);
	printf("4.Game Rule\n");
	gotoxy(15,14);
	printf("5.Quit\n");
	gotoxy(25,19);
	scanf("%d",&c);
	return c;
}

