/*
*  (c)2014 Ahmed Dinar,CSE,JUST
*/
#include<cstdio>
#include<sstream>
#include<cstdlib>
#include<cctype>
#include<cmath>
#include<algorithm>
#include<set>
#include<queue>
#include<stack>
#include<list>
#include<iostream>
#include<fstream>
#include<numeric>
#include<string>
#include<vector>
#include<cstring>
#include<map>
#include<iterator>
#include<ncurses.h>
#include<curses.h>
#include <menu.h>


using namespace std;

bool is_constant[10][10];
int S[10][10];

int puzzle[10][10]={
    0,0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,0, 6, 0, 1, 0, 4, 0, 5, 0,
    0,0, 0, 8, 3, 0, 5, 6, 0, 0,
    0,2, 0, 0, 0, 0, 0, 0, 0, 1,
    0,8, 0, 0, 4, 0, 7, 0, 0, 6,
    0,0, 0, 6, 0, 0, 0, 3, 0, 0,
    0,7, 0, 0, 9, 0, 1, 0, 0, 4,
    0,5, 0, 0, 0, 0, 0, 0, 0, 2,
    0,0, 0, 7, 2, 0, 6, 9, 0, 0,
    0,0, 4, 0, 5, 0, 8, 0, 7, 0
};

int Solution[10][10]={
    0,0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,9, 6, 3, 1, 7, 4, 2, 5, 8,
    0,1, 7, 8, 3, 2, 5, 6, 4, 9,
    0,2, 5, 4, 6, 8, 9, 7, 3, 1,
    0,8, 2, 1, 4, 3, 7, 5, 9, 6,
    0,4, 9, 6, 8, 5, 2, 3, 1, 7,
    0,7, 3, 5, 9, 6, 1, 8, 2, 4,
    0,5, 8, 9, 7, 1, 3, 4, 6, 2,
    0,3, 1, 7, 2, 4, 6, 9, 8, 5,
    0,6, 4, 2, 5, 9, 8, 1, 7, 3
};

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void load_puzzle(WINDOW *win);
bool is_solved();
int solve_soduko(WINDOW *win,int x,int y,int v);
void return_solve(WINDOW *win);
void show_status(WINDOW *win);
int take_input(WINDOW *win);
int congrats(WINDOW *win);


int main()
{
    WINDOW *win;
    initscr();
    cbreak();
    noecho();
    start_color();
    use_default_colors();
    init_pair(1,-1,COLOR_WHITE);
    init_pair(2,COLOR_BLACK,COLOR_WHITE);
    init_pair(3,COLOR_GREEN,COLOR_BLACK);
    init_pair(4,COLOR_BLUE,COLOR_BLACK);
    init_pair(5,COLOR_RED,COLOR_BLACK);
    init_pair(6,COLOR_YELLOW,COLOR_BLACK);
    init_pair(7,COLOR_WHITE,COLOR_GREEN);
    init_pair(8,COLOR_WHITE,COLOR_RED);
    init_pair(9,COLOR_BLUE,COLOR_WHITE);
    init_pair(10,-1,COLOR_BLACK);

    win=create_newwin(19,80,0,0);
    load_puzzle(win);
    int flag=take_input(win);
    while( flag == 1 || flag==2 ){
        if(flag==2){
            destroy_win(win);
            win=create_newwin(19,80,0,0);
            if(!congrats(win))
                break;
        }
        destroy_win(win);
        win=create_newwin(19,80,0,0);
        load_puzzle(win);
        flag = take_input(win);
    }
	endwin();
    return 0;
}


WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
	local_win = newwin(height, width, starty, startx);
	wrefresh(local_win);
	return local_win;
}

void destroy_win(WINDOW *local_win)
{
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}


void load_puzzle(WINDOW *win)
{
    for(int i=1; i<=9; i++){
        for(int j=1; j<=9; j++){
            if(puzzle[i][j] != 0){
                is_constant[i][j]=true;
            }
            S[i][j]=puzzle[i][j];
        }
    }
    wbkgd(win,COLOR_PAIR(1));
    keypad(win, TRUE);

    mvwprintw(win,0,1,".");
    mvwprintw(win,0,37,".");
    for(int i=0,j=1; i<18; i+=2,j++){
        if(j==4 || j==7 || j==11 || j==1 ){
            wattron(win,COLOR_PAIR(2));
            mvwhline(win,i,2,0,35);
            wattroff(win,COLOR_PAIR(2));
        }
        else
            mvwhline(win,i,2,0,35);
    }
    for(int i=1,j=0; i<38; i+=4,j++){
        if(j==3 || j==6 || j==9 || j==0){
            wattron(win,COLOR_PAIR(2));
            mvwvline(win,1,i,0,17);
            wattroff(win,COLOR_PAIR(2));
        }
        else
            mvwvline(win,1,i,0,17);
    }
    wattron(win,COLOR_PAIR(2));
    mvwhline(win,18,2,0,35);
    wattroff(win,COLOR_PAIR(2));
    for(int i=1,Y=1; i<=9; i++,Y+=2){
        for(int j=1,X=3; j<=9; j++,X+=4){
            if(is_constant[i][j]){
                wattron(win,COLOR_PAIR(9));
                 mvwprintw(win,Y,X,"%d",S[i][j]);
                wattroff(win,COLOR_PAIR(9));
            }
            else
                mvwprintw(win,Y,X,"%d",S[i][j]);
        }
    }

    int NE=0,NP=39;
    char space[]={"                                         "};
    wattron(win,COLOR_PAIR(3) | A_BOLD);
    mvwprintw(win,NE++,NP,"%s",space);
    wattroff(win,COLOR_PAIR(3) | A_BOLD);


    wattron(win,COLOR_PAIR(3) | A_BOLD);
    mvwprintw(win,NE++,NP,"         *******************             ");
    mvwprintw(win,NE++,NP,"         *     SUDOKU!     *             ");
    mvwprintw(win,NE++,NP,"         *******************             ");
    wattroff(win,COLOR_PAIR(3) | A_BOLD);

    wattron(win,COLOR_PAIR(3) | A_BOLD);
    mvwprintw(win,NE++,NP,"%s",space);
    wattroff(win,COLOR_PAIR(3) | A_BOLD);

    wattron(win,COLOR_PAIR(4) | A_BOLD);
    mvwprintw(win,NE++,NP,"         ++++++++++++++                  ");
    mvwprintw(win,NE++,NP,"          check status                   ");
    mvwprintw(win,NE++,NP,"         ++++++++++++++                  ");
    wattroff(win,COLOR_PAIR(4) | A_BOLD);

    wattron(win,COLOR_PAIR(3) | A_BOLD);
    mvwprintw(win,NE++,NP,"%s",space);
    wattroff(win,COLOR_PAIR(3) | A_BOLD);

    wattron(win,COLOR_PAIR(6) | A_BOLD);
    mvwprintw(win,NE++,NP,"         =======                         ");
    mvwprintw(win,NE++,NP,"          RESET                          ");
    mvwprintw(win,NE++,NP,"         =======                         ");
    wattroff(win,COLOR_PAIR(6) | A_BOLD);

    wattron(win,COLOR_PAIR(3) | A_BOLD);
    mvwprintw(win,NE++,NP,"%s",space);
    wattroff(win,COLOR_PAIR(3) | A_BOLD);

    wattron(win,COLOR_PAIR(5) | A_BOLD);
    mvwprintw(win,NE++,NP,"         ------                          ");
    mvwprintw(win,NE++,NP,"          EXIT                           ");
    mvwprintw(win,NE++,NP,"         ------                          ");
    wattroff(win,COLOR_PAIR(5) | A_BOLD);


    wattron(win,COLOR_PAIR(3) | A_BOLD);
    mvwprintw(win,NE++,NP,"%s",space);
    mvwprintw(win,NE++,NP,"%s",space);
    wattroff(win,COLOR_PAIR(3) | A_BOLD);


    wattron(win,COLOR_PAIR(6));
    mvwprintw(win,NE++,NP,"%s",space);
    wattroff(win,COLOR_PAIR(6));

    wmove(win,1,3);
}

bool is_solved()
{
    for(int i=1; i<=9; i++){
        for(int j=1; j<=9; j++){
            if(S[i][j] != Solution[i][j]){
                return false;
            }
        }
    }
    return true;
}


int solve_soduko(WINDOW *win,int x,int y,int v)
{
    if( (v>=1 && v<=9) && !is_constant[x][y]){
        S[x][y]=v;
        if(is_solved()){
            return 1;
        }
        else
            wprintw(win,"%d",S[x][y]);
    }
    return 0;
}

void return_solve(WINDOW *win)
{
    for(int i=1,Y=1; i<=9; i++,Y+=2){
        for(int j=1,X=2; j<=9; j++,X+=4){
            if(is_constant[i][j]){
                wattron(win,COLOR_PAIR(9));
                mvwprintw(win,Y,X," %d ",S[i][j]);
                wattroff(win,COLOR_PAIR(9));
            }
            else
                mvwprintw(win,Y,X," %d ",S[i][j]);
        }
    }
    return;
}

void show_status(WINDOW *win)
{
    wattron(win,COLOR_PAIR(7));
    for(int i=1,Y=1; i<=9; i++,Y+=2){
        for(int j=1,X=2; j<=9; j++,X+=4){
            if(!is_constant[i][j]  && S[i][j]==Solution[i][j])
            {
                wattron(win,COLOR_PAIR(7) | A_BOLD);
                mvwprintw(win,Y,X," %d ",S[i][j]);
                wattroff(win,COLOR_PAIR(7) | A_BOLD);
            }
            else if(!is_constant[i][j]  && S[i][j]!=Solution[i][j])
            {
                wattron(win,COLOR_PAIR(8) | A_BOLD);
                mvwprintw(win,Y,X," %d ",S[i][j]);
                wattroff(win,COLOR_PAIR(8) | A_BOLD);
            }
        }
    }
    return;
}

int take_input(WINDOW *win)
{
    MEVENT event;
    int c,pox=3,poy=1,inx=1,iny=1,status_flag=0;
    mousemask(ALL_MOUSE_EVENTS, NULL);
    while (  (c = wgetch(win)) )
    {
        if(status_flag==1){
            status_flag=0;
            return_solve(win);
            wmove(win,poy,pox);
        }

        if(isprint(c)){
            int p=c-48;
            if( solve_soduko(win,inx,iny,p) == 1){
                return 2;
            }
            wmove(win,poy,pox);
        }
        else if(c==KEY_LEFT){
            if(pox==3)
                pox=35;
            else
                pox -= 4;

            if(iny==1)
                iny=9;
            else
                iny--;

            wmove(win,poy,pox);
        }
        else if(c==KEY_RIGHT){

            if(pox==35)
                pox=3;
            else
                pox += 4;

            if(iny==9)
                iny=1;
            else
                iny++;

            wmove(win,poy,pox);
        }
        else if(c==KEY_UP){
            if(poy==1)
                poy=17;
            else
                poy -= 2;

            if(inx==1)
                inx=9;
            else
                inx--;

            wmove(win,poy,pox);
        }
        else if(c==KEY_DOWN){
            if(poy==17)
                poy=1;
            else
                poy += 2;

            if(inx==9)
                inx=1;
            else
                inx++;

            wmove(win,poy,pox);
        }
        else if (KEY_MOUSE == c)
        {
            if (OK == getmouse(&event)){
                int mx=event.x,my=event.y;
                if(((mx>=48&&mx<=60) || (mx>=490&&mx<=600)) && my==6){
                        show_status(win);
                        status_flag=1;
                        wmove(win,poy,pox);
                }
               else if((mx>=49&&mx<=54) && my==10 )
                    return 1;
                else if((mx>=49&&mx<=52) && my==14 )
                    return 0;
            }
        }
        else if(c==27)
            return 0;
    }
}

int congrats(WINDOW *win)
{
    wbkgd(win,COLOR_PAIR(10));
    box(win,0,0);

    wattron(win,COLOR_PAIR(3) | A_BOLD);
    mvwprintw(win,4,15,"************************************");
    mvwprintw(win,5,20,"CONGRATULATION!");
    mvwprintw(win,5,15,"*");
    mvwprintw(win,6,15,"*");
    mvwprintw(win,5,50,"*");
    mvwprintw(win,6,50,"*");
    mvwprintw(win,6,20,"You are a Sudoku Genius!!");
    mvwprintw(win,7,15,"************************************");
    wattroff(win,COLOR_PAIR(3) | A_BOLD);

    wattron(win,COLOR_PAIR(4) | A_BOLD);
    mvwprintw(win,9,20,"Press anykey to play again");
    mvwprintw(win,10,20,"Press ESC to EXIT");
    wmove(win,11,25);
    int ch=wgetch(win);
    if(ch == 27)
        return 0;
    wattroff(win,COLOR_PAIR(4) | A_BOLD);
    return 1;
}

