#include <iostream>
#include <deque>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <deque>

#define checktime sc::duration_cast<sc::milliseconds>(sc::system_clock::now().time_since_epoch()).count()


namespace sc=std::chrono;

int main()
{

	auto currentTime = checktime;
    std::cout<<"START\n";
    int tickspersecond = 1000/5;
    int dir=0;

    //Up 0
    //Down 1
    //Left 2
    //Right 3
    srand(currentTime);
    bool gameOver = false;
    int posx=19,posy=14,fruitx=rand()%40,fruity=rand()%30;
    bool tab[40][30];
    
    std::deque <std::pair <int,int>> que;

    //declare starting snake
    que.push_back({19,8});
    que.push_back({19,9});
    que.push_back({19,10});
    que.push_back({19,11});
    que.push_back({19,12});
    que.push_back({19,13});

    while(!gameOver)
    {
        //controls
        if(GetKeyState('W') < 0 && dir!=1) dir=0;
        if(GetKeyState('S') < 0 && dir!=0) dir=1;
        if(GetKeyState('A') < 0 && dir!=3) dir=2;
        if(GetKeyState('D') < 0 && dir!=2) dir=3;
        if(GetKeyState(27)<0 ) break;

        


        if(checktime>=currentTime+tickspersecond) 
        {
            if((posx==0 && dir==2) || (posx==39 && dir==3) || (posy==0 && dir==1) || (posy==29 && dir==0))
            {
                gameOver=true;
                break;
            }
            que.push_back({posx,posy});
            //clear the screen
            for(int i=0;i<40;++i) for(int j=0;j<30;++j) tab[i][j]=false;
                
            //copy segments
            for(auto j:que) tab[j.first][j.second]=true;

            switch(dir)
            {
                case 0:
                    //std::cout<<"HE WENT UP\n";
                    ++posy;
                break;
                case 1:
                    //std::cout<<"HE WENT DOWN\n";
                    --posy;
                break;
                case 2:
                    //std::cout<<"HE WENT LEFT\n";
                    --posx;
                break;
                case 3:
                    //std::cout<<"HE WENT RIGHT\n";
                    ++posx;
                break;
            }

            if(tab[posx][posy]) 
            {
                gameOver=true;
                break;
            }
            else tab[posx][posy]=true;
            //remove the last snake segment if fruit not eaten
            if(posx==fruitx && posy==fruity)
            {
                while(tab[fruitx][fruity])
                {
                    fruitx=rand()%40;
                    if(!tab[fruitx][fruity]) break;
                    fruity=rand()%30;
                }
            } 
            else que.pop_front();
            //draw the snake
            for(int i=29;i>=0;--i) 
            {
                for(int j=0;j<40;++j) 
                {
                    if(tab[j][i]) std::cout<<"#";
                    else if(j==fruitx and i==fruity)
                    {
                        std::cout<<"@";
                    }
                    else std::cout<<".";
                }
                std::cout<<"\n";
            }
            currentTime+=tickspersecond;
            //std::cout<<"TICK\n";
        }
    }

    std::cout<<"GAMEOVER\n";
	
	return 0;
}