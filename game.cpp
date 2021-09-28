Code :
# include <stdio.h>
# include <conio.h>
# include <graphics.h>
# include <alloc.h>
# include <dos.h>
# include <stdlib.h>
# define up 72
# define down 80
# define left 75
# define right 77
# define pause 25
# define space 57
# define settings 31
struct body
{
 int x,y;
 struct body *next;
};
typedef struct body body;
void create_snake(body*);
void show_settings();
void draw_borders();
void draw_egg();
void draw_snake();
void status_bar();
void draw_head();
void erase_tail();
void disp_gameover();
int getkey();
int check_snake(int,int);
body *head,*tail;
int dir=1;
int speed=100;
int leftx,rightx,topx,bottomx;
int color = RED,fil_style=1,bor_color=BLACK,size = 10;
int score=0,egg_drawn=0,jump = 9;
int preveggx,preveggy;
void far *egg;
char scr[15];
int eggx,eggy;
void main()
{
 int gm=DETECT,gd;
 int key=0,num;
 struct time tim;
 void move_snake_right();
 void move_snake_down();
 void move_snake_left();
 void move_snake_up();
 initgraph(&gm,&gm,"g:\tc\bgi");
 leftx=(size*2-1);
 rightx=getmaxx()-(size*2);
 topx=(size*2-1);
 bottomx = getmaxy()-40;
 setcolor(YELLOW);
 setfillstyle(SOLID_FILL,YELLOW);
 fillellipse(10,10,size/3,size/2);
 egg = farmalloc(imagesize(10-size/2,10-size/2,10+size/2,10+size/2));
 if(egg ==NULL)
 {
  printf("Insufficient Memory!");
  exit(0);
 }
 getimage(10-size/2,10-size/2,10+size/2,10+size/2,egg);
 draw_borders();
 create_snake(head);
 start:
 fflush(stdin);
 flushall();
 while(!kbhit())
 {
 delay(speed);
 if(egg_drawn==0)
 {
  status_bar();
  eggx=eggy=0;
  repeat:
  gettime(&tim);
  num = (int)(tim.ti_hund)*(int)(tim.ti_sec);
  num = num%(rightx-leftx)/size;
  eggx=num;
  num = (int)(tim.ti_hund)*(int)tim.ti_sec;
  num = num%(bottomx-topx)/size;
  eggy=num;
   if(check_snake(eggx,eggy))
   goto repeat;
  eggx =leftx+(eggx*size);
  eggy =topx+(eggy*size);
  preveggx =leftx+(eggx*size);
  preveggy =topx+(eggy*size);
 }
 draw_egg(eggx,eggy);
  switch(dir)
  {
   case 1:move_snake_right();
	  break;
   case 2:move_snake_down();
	  break;
   case 3:move_snake_left();
	  break;
   case 4:move_snake_up();
  }
 }
 key = getkey();
 switch(key)
 {
  case up:if(dir!=2)
	  dir = 4;
	  break;
  case down:if(dir!=4)
	   dir = 2;
	   break;
  case right:if(dir!=3)
	     dir = 1;
	     break;
  case left:if(dir!=1)
	    dir = 3;
	    break;
  case pause:getch();
	    break;
  case settings:show_settings();
		setfillstyle(EMPTY_FILL,color);
		bar(leftx,topx,rightx,bottomx);
		draw_snake();
		break;
  case 1:closegraph();
	 exit(0);
 }
 goto start;
}
int getkey()
{
 union REGS i,o;
 i.h.ah = 0;
 int86(0x16,&i,&o);
 return o.h.ah;
}
void draw_borders()
{
  setfillstyle(SOLID_FILL,BLUE);
  floodfill(300,300,1);
  setfillstyle(SOLID_FILL,BLACK);
  setcolor(BLACK);
  rectangle(leftx,topx,rightx,bottomx);
  setfillstyle(SOLID_FILL,BLACK);
  floodfill(300,300,BLACK);
  rectangle(leftx,bottomx+5,rightx,getmaxy()-5);
}
void create_snake()
{
 body *x;
 setfillstyle(fil_style,color);
 setcolor(bor_color);
 if((x = malloc(sizeof(body)))==NULL)
 {
  printf("Short of memory!");
  return;
 }
 x->x = size*3+leftx;
 x->y = topx;
 head = x;
 bar(x->x,x->y,x->x+size,x->y+size);
 rectangle(x->x,x->y,x->x+size,x->y+size);
 if((x->next = malloc(sizeof(body)))==NULL)
 {
  printf("Short of memory!");
  return;
 }
 x = x->next;
 x->x = size*2+leftx;
 x->y = topx;
 bar(x->x,x->y,x->x+size,x->y+size);
 rectangle(x->x,x->y,x->x+size,x->y+size);
 if((x->next = malloc(sizeof(body)))==NULL)
 {
  printf("Short of memory!");
  return;
 }
 x = x->next;
 x->x = size+leftx;
 x->y = topx;
 bar(x->x,x->y,x->x+size,x->y+size);
 rectangle(x->x,x->y,x->x+size,x->y+size);
 if((x->next = malloc(sizeof(body)))==NULL)
 {
  printf("Short of memory!");
  return;
 }
 x = x->next;
 x->x = leftx;
 x->y = topx;
 bar(x->x,x->y,x->x+size,x->y+size);
 rectangle(x->x,x->y,x->x+size,x->y+size);
 x->next = NULL;
 tail = x;
}
void move_snake_right()
{
 body *x;
 int nextx,nexty;
 nexty = head->y;
 if(head->x+size>=rightx)
  nextx = leftx;
 else
  nextx = head->x+size;
 if(check_snake(nextx,nexty)==1)
  {
   disp_gameover();
  }
 if(nextx == eggx&&nexty == eggy)
 {
  egg_drawn=0;
  score+=jump;
  x = malloc(sizeof(body));
  if(x==NULL)
  {
   printf("Insufficient Memory!");
   exit(0);
  }
  x->x = nextx;
  x->y = nexty;
  x->next = head;
  head = x;
  draw_head();
 }
 else
  {
   erase_tail();
   x = head;
   while((x->next)->next !=NULL)
   x = x->next;
   x->next = NULL;
   tail->x = nextx;
   tail->y = nexty;
   tail->next = head;
   head = tail;
   tail = x;
   draw_head();
  }
}
void move_snake_left()
{
 body *x;
 int nextx,nexty;
 if(head->x-size<leftx)
  nextx = rightx-size;
 else
  nextx = head->x-size;
 nexty = head->y;
 if(check_snake(nextx,nexty)==1)
  {
   disp_gameover();
  }
 if(nextx==eggx&&nexty==eggy)
 {
  egg_drawn=0;
  score+=jump;
  x = malloc(sizeof(body));
  if(x==NULL)
  {
   printf("Insufficient Memory!");
   exit(0);
  }
  x->x = nextx;
  x->y = nexty;
  x->next = head;
  head = x;
  draw_head();
 }
 else
  {
   erase_tail();
   x = head;
   while((x->next)->next !=NULL)
   x = x->next;
   x->next = NULL;
   tail->x = nextx;
   tail->y = nexty;
   tail->next = head;
   head = tail;
   tail = x;
   draw_head();
  }

}
void move_snake_up()
{
 body *x;
 int nextx,nexty;
 if(head->y-size<topx)
  nexty = bottomx-size;
 else
  nexty = head->y-size;
 nextx = head->x;
 if(check_snake(nextx,nexty)==1)
  {
   disp_gameover();
  }
 if(nextx==eggx&&nexty==eggy)
 {
  egg_drawn=0;
  score+=jump;
  x = malloc(sizeof(body));
  if(x==NULL)
  {
   printf("Insufficient Memory!");
   exit(0);
  }
  x->x = nextx;
  x->y = nexty;
  x->next = head;
  head = x;
  draw_head();
 }
 else
  {
   erase_tail();
   x = head;
   while((x->next)->next !=NULL)
   x = x->next;
   x->next = NULL;
   tail->x = nextx;
   tail->y = nexty;
   tail->next = head;
   head = tail;
   tail = x;
   draw_head();
  }
}
void move_snake_down()
{
 body *x;
 int nextx,nexty;
 if(head->y+size>=bottomx)
  nexty = topx;
 else
  nexty = head->y+size;
 nextx = head->x;
 if(check_snake(nextx,nexty)==1)
  {
   disp_gameover();
  }
 if(nextx==eggx&&nexty==eggy)
 {
  egg_drawn=0;
  score+=jump;
  x = malloc(sizeof(body));
  if(x==NULL)
  {
   printf("Insufficient Memory!");
   exit(0);
  }
  x->x = nextx;
  x->y = nexty;
  x->next = head;
  head = x;
  draw_head();
 }
 else
  {
   erase_tail();
   x = head;
   while((x->next)->next !=NULL)
   x = x->next;
   x->next = NULL;
   tail->x = nextx;
   tail->y = nexty;
   tail->next = head;
   head = tail;
   tail = x;
   draw_head();
  }

}
void draw_egg(int x,int y)
{
 putimage(x,y,egg,COPY_PUT);
 egg_drawn = 1;
}

void status_bar()
{
  setfillstyle(SOLID_FILL,LIGHTGRAY);
  bar(leftx+1,bottomx+4,rightx-1,getmaxy()-6);
  sprintf(scr,"Score:%d",score);
  setcolor(RED);
  settextstyle(2,0,6);
  outtextxy(leftx+5,bottomx+10,scr);
  outtextxy(rightx-75,bottomx+10,"S");
  setcolor(DARKGRAY);
  outtextxy(rightx-66,bottomx+10,"ettings");
  setcolor(RED);
  outtextxy(rightx-150,bottomx+10,"P");
  setcolor(DARKGRAY);
  outtextxy(rightx-142,bottomx+10,"ause");
}
void draw_head()
{
 setfillstyle(fil_style,color);
 bar(head->x,head->y,head->x+size,head->y+size);
 setcolor(bor_color);
 rectangle(head->x,head->y,head->x+size,head->y+size);
}
void erase_tail()
{
 setfillstyle(EMPTY_FILL,color);
 bar(tail->x,tail->y,tail->x+size,tail->y+size);
}
int check_snake(int x,int y)
{
 body *z;
 z = head;
 while(z->next!=NULL)
 {
  if(z->x==x&&z->y==y)
   return 1;
  z=z->next;
 }
  if(z->x==x&&z->y==y)
   return 1;
  return 0;
}
void disp_gameover()
{
 int x1,x2,y1,y2;
 x1 = getmaxx()/2-100;
 y1 = getmaxy()/2-65;
 x2 = getmaxx()/2+100;
 y2 = getmaxy()/2+65;
 setfillstyle(SOLID_FILL,9);
 bar(x1,y1,x2-30,y2-75);
 setcolor(YELLOW);
 settextstyle(3,0,3);
 outtextxy(x1+10,y1+10,"GAME OVER!!");
 getch();
 closegraph();
 exit(0);
}
void show_settings()
{
 int key=0,maxspeed=25,least = 145;
 int x1,y1,x2,y2,width;
 int no,i;
 setfillstyle(SOLID_FILL,LIGHTGRAY);
 x1 = (getmaxx()+1)/2-150;
 x2 = (getmaxx()+1)/2+150;
 y1 = (getmaxy()+1)/2-150;
 y2 = (getmaxy()+1)/2+150;
 setcolor(BLUE);
 bar3d(x1,y1,x2,y2,20,1);
 settextstyle(2,0,6);
 setcolor(RED);
 outtextxy(x1+20,y1+100,"Speed:");
 outtextxy(x1+85,y1+95,"-");
 outtextxy(x2-10,y1+95,"+");
 outtextxy(x1+20,y1+150," Use the left and right");
 outtextxy(x1+20,y1+180,"arrow keys to increase and ");
 outtextxy(x1+20,y1+210,"  decrease the speed ");
 outtextxy(x1+20,y1+240,"Press escape to return");
 outtextxy(x1+20,y1+270,"    to the game");
 setcolor(BLUE);
 line(x1+100,y1+120,x2-20,y1+120);
 line(x1+100,y1+120,x1+100,y1+90);
 line(x2-20,y1+120,x2-20,y1+90);
 width = ((x2-20)-(x1+100))/8;
 start:
 setfillstyle(SOLID_FILL,LIGHTGRAY);
 for(i=1;i<=no;i++)
 {
  bar(x1+100+1,y1+90+1,x1+100+(i*width)-1,y1+120-1);
  setcolor(LIGHTGRAY);
  rectangle(x1+100+1,y1+90+1,x1+100+(i*width)-1,y1+120-1);
 }
 no = (speed-25)/15;
 setfillstyle(SOLID_FILL,BLUE);
 for(i=1;i<=no;i++)
 {
  bar(x1+100+1,y1+90+1,x1+100+(i*width)-1,y1+120-1);
  setcolor(LIGHTGRAY);
  rectangle(x1+100+1,y1+90+1,x1+100+(i*width)-1,y1+120-1);
 }
 def:
 key = getkey();
 switch(key)
 {
  case right:if(speed<least)
	     speed+=15;
	     goto start;
  case left:if(speed>maxspeed)
	    speed-=25;
	     goto start;
  case 1:return;
  default:goto def;
 }
}
void draw_snake()
{
 body *x;
 x = head;
 setfillstyle(SOLID_FILL,color);
 setcolor(bor_color);
 while(x->next!=NULL)
 {
  bar(x->x,x->y,x->x+size,x->y+size);
  rectangle(x->x,x->y,x->x+size,x->y+size);
  x = x->next;
 }
}
