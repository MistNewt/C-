#include<fstream.h>
#include<conio.h>
#include<dos.h>
#include<process.h>
#include<graphics.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

REGS in,out;
void callmouse();
void mouseposi(int&,int&,int&);
void setlimits(int,int);
void hidemouse();
void INIT();
int bounce(int);
void dpaddle(int,int);
void rpaddle(int,int);
void transit(int,int,int,int);
double calcdeg(double,int);
void dbricks(int,int,int);
void rbricks(int,int);
void scores(int,int,int,char*);
void welcome(int,int);
void pause(int,int);
void level1();
void level2();
void level3();
void resetmouse();
void savegame(int,int);
int loadgame(int,int);
void newgame();
void resume();
void getname(char arr[]);
void highscoresIN();
void highscoresOUT();

char  name[50];
int points=0;
//int brkx[20];//brkx[0]=16;
//int brky[10];//brky[0]=8;
int brkst[20][10];
int gstate=0;
int level=0;



class HIGH
{
	public:
	char Name[50];
	int Points;
	HIGH()
	{
		strcpy(Name,"");
		Points=0;
	}
	HIGH(char N[50],int pt)
	{
		strcpy(Name,N);
		Points=pt;
	}
	HIGH(HIGH& ob)
	{
		strcpy(Name,ob.Name);
		Points=ob.Points;
	}
};
class game
{
	public:
	char  NAME[50];
	int POINTS;
	int BRKST[20][10];
	int LEVEL;

	game()
	{
		strcpy(NAME,"");
		POINTS=0;
		LEVEL=0;
		for(int i=0;i<20;i++)
		{
			for(int j=0;j<10;j++)
			{
				BRKST[i][j]=0;
			}
		}
	}

	game(char  N[50],int p,int bs[20][10],int l)
	{
		strcpy(NAME,N);
		POINTS=p;
		LEVEL=l;
		for(int i=0;i<20;i++)
		{
			for(int j=0;j<10;j++)
			{
				BRKST[i][j]=bs[i][j];
			}
		}
	}
};

void getname(char arr[50])
{
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	settextjustify(CENTER_TEXT,CENTER_TEXT);

	int maxx=getmaxx();
	int maxy=getmaxy();

	arr[0]='\0';

	char ch=' ';
	int ctr=0;
	int ch1=0;
	setcolor(RED);

	settextjustify(0,0);
	while(ch1!=27&&ch1!=13)
	{
		setcolor(RED);
		outtextxy(maxx/2,maxy/2,"Enter your name: ");
		if(kbhit()!=0)
		{
			cleardevice();

			ch=getch();
				ch1=(int)ch;
			if(ch1!=13&&ch1!=8&&ch1!=27)
			{
				arr[ctr]=ch;
				arr[ctr+1]='\0';
				ctr++;
			}
			else if(ch1==8&&ctr>=0)
			{
				arr[ctr]='\0';
				ctr--;
			}

			setcolor(GREEN);
			outtextxy(maxx/2,maxy/2+25,arr);
		}
	}
}



void newgame()
{
	cleardevice();
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	setcolor(RED);
	getname(::name);
	level1();
}
void savegame(int maxx,int maxy)
{
	game ob2(::name,::points,::brkst,::level);
	game ob1;
	gotoxy(10,10);
	//cout<<"SGame Name: "<<::name
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	//outtextxy(maxx/2,maxy/2,"ENTER YOUR NAME: \n");
	setcolor(RED);


	int flag=0;
	ifstream f1("sgame.dat",ios::binary);
	ofstream f2("sbk.dat",ios::binary|ios::ate);
	int p=maxx/2-50;
	if(!f1)
	{
		outtextxy(10,maxy/2,"File opening error....savegame error 007\n");
		getch();
		exit(1);
	}
	else
	{
		while(!f1.eof())
		{
			f1.read((char*)&ob1,sizeof(ob1));
			if(strcmpi(ob1.NAME,ob2.NAME)==0)
			{
				setcolor(RED);
				outtextxy(p,maxy/2,"FILE ALREADY EXISTS..OVERWRITE(y/n)");
				int chz;
				chz=getche();
				setcolor(getbkcolor());
				outtextxy(p,maxy/2,"FILE ALREADY EXISTS..OVERWRITE(y/n)");

				if(chz=='y'||chz=='Y')
				{
					f2.write((char*)&ob2,sizeof(ob2));
					flag=1;
					setcolor(RED);
					outtextxy(10,maxy/2,"SAVE SUCCESSFUL..");
					delay(1000);
					break;
				}
				else
				{
					flag=1;
					f2.write((char*)&ob1,sizeof(ob1));
					setcolor(RED);
					outtextxy(p,maxy/2,"SAVE CANCELLED..");
					break;
				}

			}
			else
			{
				f2.write((char*)&ob1,sizeof(ob1));

			}
		}

		if(flag!=1)
		{
			f2.write((char*)&ob2,sizeof(ob2));
			setcolor(RED);
			outtextxy(p,maxy/2,"SAVE SUCCESSFUL..");
			delay(1000);
		}
		f1.close();
		f2.close();

	}
	remove("sgame.dat");
	rename("sbk.dat","sgame.dat");
}

int loadgame(int maxx,int maxy)
{
	char N[50];
	cleardevice();
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	getname(N);
	setcolor(RED);
	//gets(N);
	game ob;
	int flag=0;
	fstream f("sgame.dat",ios::in|ios::binary);
	if(!f)
	{
		outtextxy(10,maxy/2,"File opening error..loadgame error 007\n");
	}
	else
	{
		while(!f.eof())
		{
			f.read((char*)&ob,sizeof(ob));
			if(strcmp(ob.NAME,N)==0)
			{
				cleardevice();
				outtextxy(10,maxy/2,"Load Successful");
				delay(500);
				flag=1;
				strcpy(::name,N);
				::points=ob.POINTS;
				::level=ob.LEVEL;
				for(int i=0;i<20;i++)
				{
					for(int j=0;j<10;j++)
					{
					       ::brkst[i][j]=ob.BRKST[i][j];
					}
				}
				break;

			}
		}
		if(flag==0)
		{
			cleardevice();
			outtextxy(10,maxy/2,"Load failed..file not found");
			delay(500);
			return -1;
		}
		if(flag==1)
			return 0;

	 }

}











void callmouse()
{
	in.x.ax=1;
	int86(51,&in,&out);
}
void level1()
{
	cleardevice();
	int maxx=getmaxx();
	int maxy=getmaxy();
	int pass=1;
	int pt;

	settextstyle(SANS_SERIF_FONT,HORIZ_DIR,10);
	settextjustify(CENTER_TEXT,CENTER_TEXT);

	setcolor(RED);

	outtextxy(maxx/2,maxy/2,"LEVEL 1 ");

	transit(RED,BLUE,maxx,maxy);
	pt=bounce(pass);
	cout<<"Pt: "<<pt;
	delay(1000);
	if(pass==-1)
		level2();
}
void level2()
{
	cleardevice();
	int maxx=getmaxx();
	int maxy=getmaxy();
	gstate=1;

	settextstyle(SANS_SERIF_FONT,HORIZ_DIR,10);
	settextjustify(CENTER_TEXT,CENTER_TEXT);

	setcolor(RED);

	outtextxy(maxx/2,maxy/2,"LEVEL 2 ");
	int pass=2;

	transit(RED,BLUE,maxx,maxy);
	int i,j;
	for(i=0;i<20;i++)
		for(j=0;j<10;j++)
			::brkst[i][j]=0;

	for(i=0;i<10;i++)
	{
		for(j=0;j<10-i;j++)
		{
		       //	::brkst[i][j]=1;
			::brkst[i][j]=1;
		}
		for(j=0;j<=i;j++)
		{
			::brkst[i+10][j]=1;
		}
	}
	for(i=0;i<20;i++)
	{
		::brkst[i][9]=1;
	}



	pass=bounce(pass);
	if(pass==-1)
		level3();
}
void level3()
{
	cleardevice();
	int maxx=getmaxx();
	int maxy=getmaxy();
	gstate=1;
	int pass=3;

	settextstyle(SANS_SERIF_FONT,HORIZ_DIR,10);
	settextjustify(CENTER_TEXT,CENTER_TEXT);

	setcolor(RED);

	outtextxy(maxx/2,maxy/2,"LEVEL 3 ");

	transit(RED,BLUE,maxx,maxy);
	int i,j;
	for(i=0;i<20;i++)
		for(j=0;j<10;j++)
			::brkst[i][j]=1;

	for(i=0;i<10;i++)
	{
		for(j=0;j<10-i;j++)
		{
		       //	::brkst[i][j]=1;
			::brkst[i][j]=0;
		}
		for(j=0;j<=i;j++)
		{
			::brkst[i+10][j]=0;
		}
	}
	for(i=0;i<20;i++)
	{
		::brkst[i][9]=0;
	}



	pass=bounce(pass);
	if(pass==-1)
	{
		cleardevice();
		int maxx=getmaxx();
		int maxy=getmaxy();

		settextstyle(GOTHIC_FONT,HORIZ_DIR,3);
		settextjustify(CENTER_TEXT,CENTER_TEXT);

		char arr[]={'G','A','M','E',' ','O','V','E','R'};
		char str[10];
		int i;
		for(i=0;i<9;i++)
		{
			str[i]=arr[i];
			str[i+1]='\0';
			setcolor(i);
			outtextxy(maxx/2,maxy/2,str);
			delay(500);
			setcolor(getbkcolor());
			outtextxy(maxx/2,maxy/2,str);
		}
		setcolor(i);
		outtextxy(maxx/2,maxy/2,str);
		settextjustify(LEFT_TEXT,TOP_TEXT);
		settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
		outtextxy(10,maxy/2+30,"Wait for more levels in coming versions......");
	}

}


void transit(int c1,int c2,int maxx,int maxy)
{
	for(int i=0;i<=maxx;i+=2)
	{
		setcolor(c1);
		circle(maxx/2,maxy/2,i);
		delay(2);
	}
	for(int j=maxx;j>=0;)
	{
		setcolor(c2);
		circle(maxx/2,maxy/2,j);
		delay(2);
		setcolor(getbkcolor());
		for(int k=0;k<2;k++)
		circle(maxx/2,maxy/2,j--);
	}
}





void scores(int x,int y,int point,char*str)
{
	highscoresIN();
	settextstyle(DEFAULT_FONT,HORIZ_DIR,4);
	setcolor(RED);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	outtextxy(x/2,y/2-40,str);
	setcolor(LIGHTRED);
	outtextxy(x/2,y/2+40,"POINTS:        ");
	char p[20];
	itoa(point,p,10);
	setcolor(GREEN);
	outtextxy(x/2+120,y/2+40,p);
}

double calcdeg(double diff,int sign)
{
	double angle;
	if(diff>=45)
	angle=90+sign*70;
	else if(diff>=35&&diff<45)
	angle=90+sign*60;
	else if(diff>=25&&diff<35)
	angle=90+sign*40;
	else if(diff>=15&&diff<25)
	angle=90+sign*20;
	else if(diff<15&&diff>=5)
	angle=90+sign*10;
	else if(diff<5)
	angle=90;

	return angle;
}

void welcome(int maxx,int maxy)
{
	cleardevice();
	int mx,my,mclk=0;
	//mouseposi(mx,my,clk);
 //	int ctr=0;
	int ch=0;
	while(ch!=27)
	{
		settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
		setcolor(RED);
		outtextxy(10,20,"Welcome to the GAME of angry ball...");
		setcolor(MAGENTA);
		outtextxy(10,maxy/2-50,"NEW GAME");
		outtextxy(10,maxy/2,"LOAD GAME");
		outtextxy(10,maxy/2+50,"HIGH SCORES");
		outtextxy(10,maxy/2+100,"EXIT");
		settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
		setcolor(RED);
		outtextxy(10,maxy-15,"Press Esc to quit");
		callmouse();
		mouseposi(mx,my,mclk);
		gotoxy(1,3);
		cout<<"CLK: "<<mclk;
		if(mx<180&&mx>8)
		{
			if(my<maxy/2-30&&my>maxy/2-50)
			{
				if(mclk==1)
				{
					hidemouse();
					newgame();
				}

			}
			if(my<maxy/2+20&&my>maxy/2)
			{
				if(mclk==1)
				{
					hidemouse();
					int x=loadgame(maxx,maxy);
					if(x==-1)
					{
						delay(1000);
						exit(1);
					}
					if(x==0)
					{
						resume();
					}



				}
			}
			if(my<maxy/2+70&&my>maxy/2+50)
			{


				if(mclk==1)
				{
					gotoxy(16,20);
					highscoresOUT();
				}

			}
			if(my<maxy/2+120&&my>maxy/2+100)
			if(mclk==1)
			{
				resetmouse();
				exit(1);
			}

		}
		if(kbhit()==0)
		{
			ch=0;
		}
		if(kbhit()!=0)
		{
			ch=(int)getch();
		}
		gotoxy(1,3);
	     //	cout<<"Ch: "<<ch;





	       //	gotoxy(15,18);
	       //	cout<<"X: "<<mx<<" Y: "<<my<<" Clk: "<<clk<<" MaxY/2 "<<maxy/2;
	}
	hidemouse();

}
void pause(int maxx,int maxy)
{
	cleardevice();
	int mx,my,clk;
	//mouseposi(mx,my,clk);
 //	int ctr=0;
	resetmouse();
	int ch1=0;
	while(ch1!=27)
	{
		gotoxy(1,3);
		cout<<"CLK: "<<clk;
		settextstyle(DEFAULT_FONT,HORIZ_DIR,4);
		settextjustify(LEFT_TEXT,TOP_TEXT);
		setbkcolor(BLACK);
		setcolor(RED);
		outtextxy(10,20,"PAUSE");
		settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
		setcolor(MAGENTA);
		//outtextxy(maxx/2,maxy/2-100,"HELLO");

		outtextxy(10,maxy/2-100,"RESUME");
		outtextxy(10,maxy/2-50,"NEW GAME");
		outtextxy(10,maxy/2,"LOAD GAME");
		outtextxy(10,maxy/2+50,"SAVE GAME");
		outtextxy(10,maxy/2+100,"HIGH SCORES");
		outtextxy(10,maxy/2+150,"EXIT");
		settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
		setcolor(RED);
		outtextxy(10,maxy-15,"Press Esc to quit");
		callmouse();
		mouseposi(mx,my,clk);
		if(mx<180&&mx>8)
		{
			if((my<maxy/2-100+20)&&(my>maxy/2-100))
			{
				if(clk==1)
				{
					resume();
				}
			}
			if((my<maxy/2+50+20)&&(my>maxy/2+50))
			{
				if(clk==1)
				{
					savegame(maxx,maxy);
					resume();
				}
			}
			if((my<maxy/2+20)&&my>maxy/2)
			{
				if(clk==1)
				{
					int x=loadgame(maxx,maxy);
					if(x==0)
					resume();
					hidemouse();
				}

			}


			if((my<maxy/2+20-50)&&(my>maxy/2-50))
			{
				if(clk==1)
				{

					hidemouse();
					newgame();
				}

			}
			if((my<maxy/2+70+50)&&(my>maxy/2+50+50))
			{


				if(clk==1)
				{
					gotoxy(16,20);
					highscoresOUT();
				}

			}
			if((my<maxy/2+120+50)&&(my>maxy/2+100+50))
			if(clk==1)
			{
				resetmouse();
				exit(1);
			}

		}

		if(kbhit()==0)
		{
			ch1=0;
		}
		else if(kbhit()!=0)
		{
			ch1=(int)getch();
		}







	       //	gotoxy(15,18);
	       //	cout<<"X: "<<mx<<" Y: "<<my<<" Clk: "<<clk<<" MaxY/2 "<<maxy/2;
	}
	hidemouse();

}


void mouseposi(int&mx,int&my,int&clk)
{
	in.x.ax=3;
	int86(51,&in,&out);
	mx=out.x.cx;
	my=out.x.dx;
	clk=out.x.bx;
}
void resetmouse()
{
	in.x.ax=0;
	int86(51,&in,&out);
}
void setlimits(int x,int y)
{
	in.x.ax=7;
	in.x.cx=0;
	in.x.dx=x;
	int86(51,&in,&out);
	in.x.ax=8;
	in.x.cx=0;
	in.x.dx=y;
	int86(51,&in,&out);
}
void hidemouse()
{
	in.x.ax=2;
	int86(51,&in,&out);
}
void INIT()
{
	int driver=DETECT,mode;
	initgraph(&driver,&mode,"\\turboc3\\bgi");
      /*	if(graphresult()!=grOk)
	{
		cout<<"Graphics error: "<<grapherrormsg(graphresult())<<endl;
		cout<<"Exiting program..."<<endl;
		getch();
		exit(0);
	}  */
}
void dpaddle(int x,int y)
{
	setcolor(BROWN);
	rectangle(x-50,y-15,x+50,y-5);
	setfillstyle(SOLID_FILL,BROWN);
	floodfill(x,y-10,BROWN);
}
void rpaddle(int x,int y)
{
	int c=getbkcolor();
	setcolor(c);
	rectangle(x-50,y-15,x+50,y-5);
	setfillstyle(SOLID_FILL,c);
	floodfill(x,y-10,c);
}
void dbricks(int x,int y,int color)
{
	setcolor(RED);
	rectangle(x-16,y-8,x+16,y+8);
	setfillstyle(SOLID_FILL,color);
	floodfill(x,y,RED);
}
void rbricks(int x,int y)
{
	setcolor(getbkcolor());
	rectangle(x-16,y-8,x+16,y+8);
	setfillstyle(SOLID_FILL,getbkcolor());
	floodfill(x,y,getbkcolor());
}

int bounce(int pass)
{
	int brkx[20],brky[10];


	::level=pass;
	setbkcolor(BLUE);
	int maxx=getmaxx();
	int maxy=getmaxy();
	int pts=0;
	int x,y,x1=0,y1=170,rx=0,ry=0,signx=1,signy=1;
	int mx,my,clk,i,j,ctr=1;
	int pctr=0,npctr=0;
	double deg,angle=45;
	//int brkx[20];
	brkx[0]=16;		//int brky[10];
	brky[0]=8;
       //	int brkst[20][10];
	cleardevice();
	if(gstate==0)
	{
		for(i=0;i<20;i++)
		for(j=0;j<10;j++)
		{
			brkst[i][j]=1;
		}
	}
	for(i=1;i<20;i++)
	{
		brkx[i]=brkx[i-1]+32;
	}
	for(j=1;j<10;j++)
	{
		brky[j]=brky[j-1]+16;
	}
	for(i=0;i<20;i++)
	{
		for(j=0;j<10;j++)
		if(brkst[i][j]==1)
		dbricks(brkx[i],brky[j],LIGHTRED);
	}
	delay(1000);

	for(i=0;i<20;i++)
	{
		for(j=0;j<10;j++)
		{
			if(::brkst[i][j]==0)
			pctr++;
		}
	}

	setlimits(maxx,maxy);



	int ch=0;

	while(ch!=27)
	{
		mouseposi(mx,my,clk);
		deg=(3.14*angle)/180;
		x=x1+signx*rx*fabs(cos(deg));
		y=y1+signy*ry*fabs(sin(deg));
		dpaddle(mx,maxy);

		gotoxy(10,15);
		cout<<"Pctr: "<<pctr<<"Npctr:: "<<npctr;
		gotoxy(10,16);
		cout<<"Level: "<<level<<"Pass: "<<pass;
		if(x>=maxx)
		{
			x1=maxx;
			rx=0;
			signx=-1;
		}
		for(i=0;i<20;i++)
		{
			for(j=0;j<10;j++)
			{
				if(signx==-1&&((x<=brkx[i]+16&&x>=brkx[i])&&(y<=brky[j]+8&&y>=brky[j]-8)))
				{
				if(brkst[i][j]==1)
				{
					x1=brkx[i]+16;
					signx=1;
					rx=0;
					rbricks(brkx[i],brky[j]);
					brkst[i][j]=0;
					pctr++;
					npctr++;

				}
				}
				else if(signx==1&&((x<=brkx[i]+16&&x>brkx[i])&&(y<=brky[j]+8&&y>=brky[j]-8)))
				{
				if(brkst[i][j]==1)
				{
					rx=0;
					x1=brkx[i]-16;
					signx=-1;
					rbricks(brkx[i],brky[j]);
					brkst[i][j]=0;
					pctr++;
					npctr++;

				}
				}

			}
		}
		if(x<=0)
		{
			x1=0;
			signx=1;
			rx=0;
		}




		if(y>=maxy-15&&(x<=(mx+50)&&x>=(mx-50)))
		{
			y1=maxy-15;
			signy=-1;
			ry=0;
			if(mx>=x)
			{
				x1=x;
				rx=0;
				signx=-1;
				angle=calcdeg(mx-x,-1);
			}
			else
			{
				x1=x;
				rx=0;
				signx=1;
				angle=calcdeg(x-mx,1);

			}
		}
		else if(y>=maxy-15&&(x>mx+50||x<mx-50))
		{
			cleardevice();
			::points=::points+pts;
			scores(maxx,maxy,::points,"GAME OVER");

			getch();
			resetmouse();
			exit(1);
		}
		for(i=0;i<10;i++)
		{
			for(j=0;j<20;j++)
			{
				if(signy==-1&&((y<=(brky[i]+8)&&y>brky[i]-8)&&(x>=brkx[j]-16&&x<=brkx[j]+16)))
				{
				if(brkst[j][i]==1)
				{
					y1=brky[i]+8;
					signy=1;
					ry=0;
					rbricks(brkx[j],brky[i]);
					brkst[j][i]=0;
					pctr++;
					npctr++;

				}
				}
				else if(signy==1&&((y>=brky[i]-8&&y<brky[i]+8)&&(x>=brkx[j]-16&&x<=brkx[j]+16)))
				{
				if(brkst[j][i]==1)
				{
					y1=brky[i];
					signy=-1;
					ry=0;
					rbricks(brkx[j],brky[i]);
					brkst[j][i]=0;
					pctr++;
					npctr++;

				}
				}

			}


		}
		if(y<=0)
		{
			ry=0;
			signy=1;
			y1=0;
		}
		ctr++;
		if(ctr==5)
		{
			for(i=0;i<20;i++)
			{
				for(j=0;j<10;j++)
				{
					if(brkst[i][j]==1)
					dbricks(brkx[i],brky[j],LIGHTRED);
				}
			}
		}
		pts=npctr*10;


		setcolor(RED);
		circle(x,y,5);
		setfillstyle(SOLID_FILL,RED);
		floodfill(x,y,RED);

		delay(30);

		rx+=10;
		ry+=10;


		setcolor(getbkcolor());
		circle(x,y,5);
		setfillstyle(SOLID_FILL,getbkcolor());
		floodfill(x,y,getbkcolor());
		rpaddle(mx,maxy);

		if(ctr==5)
		ctr=1;
		if(pass==1&&pctr==200)
		{
			cleardevice();
			::points=::points+pts;
			scores(maxx,maxy,::points,"END OF LEVEL 1");
			pass=-1;
			delay(1000);
			resetmouse();
			break;

		}
		else if(pass==2&&pctr==200)
		{
			cleardevice();
			::points=::points+pts;
			scores(maxx,maxy,::points,"END OF LEVEL 2");
			pass=-1;
			delay(1000);
			resetmouse();
			break;
		}
		else if(pass==3&&pctr==200)
		{
			cleardevice();
			::points=::points+pts;
			scores(maxx,maxy,::points,"END OF LEVEL 3");
			pass=-1;
			delay(1000);
			resetmouse();
			break;
		}



		if(kbhit()!=0)
		{
			ch=(int)getch();
		}
		gotoxy(5,15);
		cout<<::points;
	}
	cout<<"Points: "<<::points;
	if(ch==27)
	{
		pause(maxx,maxy);
	}



	hidemouse();
	return pass;
}
void resume()
{
	gstate=1;
	hidemouse();
	int pt;
	pt=bounce(::level);
	if(pt==-1)
	{
		if(level==1)
			level2();
		else if(level==2)
			level3();
	}

}

void highscoresOUT()
{
	fstream f2("hscores.dat",ios::in);
	HIGH out;
	int t=20;
	int ctr=0;
	if(!f2)
	{
		outtextxy(200,200,"File opening error...code highscoresOUT 007");
		getch();
		return;
	}
	else
	{
		while(!f2.eof()&&ctr<5)
		{
			f2.read((char*)&out,sizeof(out));
			gotoxy(20,t++);
			if(out.Points!=0)
			{
				cout<<out.Name<<" :: "<<out.Points<<endl;
				ctr++;
			}
		}
	}
	if(ctr==0)
	{
		gotoxy(20,t);
		cout<<"No high scores yet!! ";
	}
	f2.close();
}



void highscoresIN()
{
	fstream f1("hscores.dat",ios::in|ios::binary);
	fstream f2("bkup.dat",ios::out|ios::binary|ios::ate);
	HIGH ob1,ob2(::name,::points);
	int rec=1;
	if(!f1)//||!f2)
	{
		outtextxy(200,200,"File opening error...code highscoresIN 007");
		getch();
		return;
	}
	else
	{
		while(!f1.eof())
		{
			f1.read((char*)&ob1,sizeof(ob1));
			if(ob1.Points<ob2.Points)
			{
				f2.write((char*)&ob2,sizeof(ob2));
				ob2=ob1;
			}
			else
			{
				f2.write((char*)&ob1,sizeof(ob1));
			}

			rec++;
			if(rec>5)
				break;
		}

	}
	f1.close();
	f2.close();
	remove("hscores.dat");
	rename("bkup.dat","hscores.dat");
}





void main()
{
	clrscr();
	INIT();
	//level1();
	welcome(getmaxx(),getmaxy());
	getch();
	closegraph();
	resetmouse();
}