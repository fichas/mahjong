//W万 B饼 T条 F风 J🀄️
#include "tile.h"
#include "shanten.h"
#include "stringify.h"
#include "fan_calculator.h"
#include "MahjongGB/MahjongGB.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <assert.h>
#include <time.h>

using namespace mahjong;
using namespace std;

static int count_useful_tile(const tile_table_t &used_table, const useful_table_t &useful_table) {
    int cnt = 0;
    for (int i = 0; i < 34; ++i) {
        tile_t t = all_tiles[i];
        if (useful_table[t]) {
            cnt += 4 - used_table[t];
        }
    }
    return cnt;
}

char chang(int x)
{
    if(x==1)return 'E';
    if(x==2)return 'S';
    if(x==3)return 'W';
    if(x==4)return 'N';
    if(x==5)return 'C';
    if(x==6)return 'F';
    if(x==7)return 'P';
    return ' ';
}
string sw(string x,int t)
{
    string sww;
    if(t==1)
    {
        if(x[0]=='W')
        {
            sww+=x[1];
            sww+=x[1];
            sww+=x[1];
            sww+='m';
        }
        if(x[0]=='B')
        {
            sww+=x[1];
            sww+=x[1];
            sww+=x[1];
            sww+='p';
        }
        if(x[0]=='T')
        {
            sww+=x[1];
            sww+=x[1];
            sww+=x[1];
            sww+='s';
        }
        if(x[0]=='F')
        {
            sww+=chang(x[1]-'0');
            sww+=chang(x[1]-'0');
            sww+=chang(x[1]-'0');
        }
        if(x[0]=='J')
        {
            sww+=chang(x[1]-'0'+4);
            sww+=chang(x[1]-'0'+4);
            sww+=chang(x[1]-'0'+4);
        }
    }
    if(t==2)
    {
        if(x[0]=='W')
        {
            sww+=x[1];
            sww+=x[1];
            sww+=x[1];
            sww+=x[1];
            sww+='m';
        }
        if(x[0]=='B')
        {
            sww+=x[1];
            sww+=x[1];
            sww+=x[1];
            sww+=x[1];
            sww+='p';
        }
        if(x[0]=='T')
        {
            sww+=x[1];
            sww+=x[1];
            sww+=x[1];
            sww+=x[1];
            sww+='s';
        }
        if(x[0]=='F')
        {
            sww+=chang(x[1]-'0');
            sww+=chang(x[1]-'0');
            sww+=chang(x[1]-'0');
            sww+=chang(x[1]-'0');
        }
        if(x[0]=='J')
        {
            sww+=chang(x[1]-'0'+4);
            sww+=chang(x[1]-'0'+4);
            sww+=chang(x[1]-'0'+4);
            sww+=chang(x[1]-'0'+4);
        }
    }
    if(t==3)
    {
        if(x[0]=='W')
        {
            sww+=x[1]-1;
            sww+=x[1];
            sww+=x[1]+1;
            sww+='m';
        }
        if(x[0]=='B')
        {
            sww+=x[1]-1;
            sww+=x[1];
            sww+=x[1]+1;
            sww+='p';
        }
        if(x[0]=='T')
        {
            sww+=x[1]-1;
            sww+=x[1];
            sww+=x[1]+1;
            sww+='s';
        }
    }
    return sww;
}
char change(int x)
{
    int tmp=x+'0';
    return tmp;
}
string toans(int i,int j)
{   
    string tt;
    if(i==1)
    {
        tt+='W';
    }
    if(i==2)
    {
        tt+='T';
    }
    if(i==3)
    {
        tt+='B';
    }
    if(i==4)
    {
        if(j>=5)
        {
            tt+='J';
            j-=4;
        }
        else
        {
            tt+='F';
        }
    }
    tt+=change(j);
    return tt;
}
string cover(int i,int j)
{
    //cout<<"zhelima "<<endl;
    string tmp;
    if(i<4)
    {
        tmp+=change(j);
        if(i==1)tmp+='m';
        if(i==3)tmp+='s';
        if(i==2)tmp+='p';
    }
    else
    {
        if(i==4)
        tmp=chang(j);
        else
        {
            tmp=chang(j+4);
        }
        
    }
    return tmp;
}

string meow(const hand_tiles_t *hand_tiles, useful_table_t &useful_table)
{
    string tans;
    char buf[64];
        for (tile_t t = TILE_1m; t < TILE_TABLE_SIZE; ++t) {
            if (useful_table[t]) {
                tiles_to_string(&t, 1, buf, sizeof(buf));
                string tmp=buf;
                tans+=tmp+' ';
                
            }
        }
    
    return tans;       
}

pair<int,string> test_shanten(const char *str)
{
    hand_tiles_t hand_tiles;
    tile_t serving_tile;
    long ret = string_to_tiles(str, &hand_tiles, &serving_tile);
    if (ret != 0) {
        //printf("error at line %d error = %ld\n", __LINE__, ret);
        return make_pair(-1,"");
    }

    char buf[20];
    ret = hand_tiles_to_string(&hand_tiles, buf, sizeof(buf));
    //puts(buf);
    
    string tans,ans;
    auto display = [](const hand_tiles_t *hand_tiles, useful_table_t &useful_table) {
        char buf[64];
        for (tile_t t = TILE_1m; t < TILE_TABLE_SIZE; ++t) {
            if (useful_table[t]) {
                tiles_to_string(&t, 1, buf, sizeof(buf));
                //printf("%s ", buf);
            }
            
        }
        
        

        tile_table_t cnt_table;
        map_hand_tiles(hand_tiles, &cnt_table);
        count_useful_tile(cnt_table, useful_table);
       // printf("%d枚\n", count_useful_tile(cnt_table, useful_table));
    };

    //puts(str);
    useful_table_t useful_table/* = {false}*/;
    int ret0;
    ret0 = thirteen_orphans_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
    
    //printf("131=== %d shanten\n", ret0);
    if (ret0 != std::numeric_limits<int>::max()) {
        display(&hand_tiles, useful_table);
        tans=meow(&hand_tiles, useful_table);
    }

    if(ret>ret0)
    {
        ret=ret0;
        ans=tans;
    }

    ret0 = seven_pairs_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
    //printf("7d=== %d shanten\n", ret0);
    if (ret0 != std::numeric_limits<int>::max()) {
        display(&hand_tiles, useful_table);
        tans=meow(&hand_tiles, useful_table);
    }

    if(ret>ret0)
    {
        ret=ret0;
        ans=tans;
    }
    ret0 = honors_and_knitted_tiles_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
    //printf("honors and knitted tiles  %d shanten\n", ret0);
    if (ret0 != std::numeric_limits<int>::max()){
        display(&hand_tiles, useful_table);
        tans=meow(&hand_tiles, useful_table);
    }

    if(ret>ret0)
    {
        ret=ret0;
        ans=tans;
    }

    ret0 = knitted_straight_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
    //printf("knitted straight in basic form %d shanten\n", ret0);
    if (ret0 != std::numeric_limits<int>::max()){
        display(&hand_tiles, useful_table);
        tans=meow(&hand_tiles, useful_table);
    }
    
    if(ret>ret0)
    {
        ret=ret0;
        ans=tans;
    }

    ret0 = basic_form_shanten(hand_tiles.standing_tiles, hand_tiles.tile_count, &useful_table);
    //printf("basic form %d shanten\n", ret0);
    if (ret0 != std::numeric_limits<int>::max()) {
        display(&hand_tiles, useful_table);
        tans=meow(&hand_tiles, useful_table);
        
    }


    if(ret>ret0)
    {
        ret=ret0;
        ans=tans;
    }
    //cout<<"??wtf"<<endl;
    return make_pair(ret,ans);
}

string Dapai( vector<string> hand,vector<pair<string, pair<string, int> > > pack,int num[6][10],int myPlayerID,int quan)
{
    //"PENG" "GANG" "CHI"
    //cout<<"wobuhuole"<<endl;
    int map[10][15]={0};
    for(int i=0;i<=5;i++)
    {
        for(int j=0;j<=10;j++)
        {
            map[i][j]=0;
           // cout<<i<<" "<<j<<endl;
        }
        
    }
    //cout<<"wobuhuole"<<endl;
    string ming;
   // cout<<"wobuhuole"<<endl;
   // cout<<pack.size()<<endl;
    for(int i=0;i<pack.size();i++)
    {
        ming+='[';
        if(pack[i].first=="PENG")
        {
            ming+=sw(pack[i].second.first,1);
        }
        if(pack[i].first=="GANG")
        {
            ming+=sw(pack[i].second.first,2);
        }
        if(pack[i].first=="CHI")
        {
            ming+=sw(pack[i].second.first,3);
        }
        ming+=']';
    }

//cout<<"wobuhuole"<<endl;

    for(int i=0;i<hand.size();i++)
    {
        if(hand[i][0]=='W')
        {
            int t=hand[i][1]-'0';
            map[1][t]++;
        }
        if(hand[i][0]=='B')
        {
            int t=hand[i][1]-'0';
            map[3][t]++;
        }
        if(hand[i][0]=='T')
        {
            int t=hand[i][1]-'0';
            map[2][t]++;
        }
        if(hand[i][0]=='F')
        {
            int t=hand[i][1]-'0';
            map[4][t]++;
        }
        if(hand[i][0]=='J')
        {
            int t=hand[i][1]-'0';
            map[4][4+t]++;
        }

    }
    
    int cnt=0;
    int last=0;
    int lastans=0;
    vector <pair<int,int> > lastp;
    
    for(int i=1;i<=4;i++)
    {
        for(int j=1;j<=9;j++)
        {
            if(map[i][j]!=0)cnt++;
        }
    }
    
    for(int i=1;i<=5;i++)
    {
        for(int j=1;j<=9;j++)
        {
            if(i==4&&j>4)continue;
            if(i==5&&j>3)continue;
            if(num[i][j])
            {
                
                pair<int,int> t;
                t.first=i;
                t.second=j;
                lastp.push_back(t);
                last++;
            }
            lastans+=num[i][j];
        }
    }

    
    vector <double> root;
    vector <string> rootn;
    
    for(int k=1;k<=cnt;k++)
    {
        
        string tmp;
        int onc=0;
        tmp+=ming;
        int tcnt=0;
        double ans=0;
        int tx=0,ty=0;
        int tmap[6][10]={0};
        for(int i=1;i<=4;i++)
        {
            int ad=0;
            for(int j=1;j<=9;j++)
            {
                if(map[i][j])tcnt++;
                tmap[i][j]=map[i][j];
                if(tcnt==k&&!onc)
                {
                    onc=1;
                    tmap[i][j]--;  
                    rootn.push_back(toans(i,j));
                    
                }
                for(int t=1;t<=tmap[i][j];t++)
                {
                    ad=1;
                    if(i!=4)
                    {
                        tmp +=change(j);
                    }
                    else
                    {
                        tmp += chang(j);
                    }
                }
            }
            if(i!=4&&ad)
            {
                if(i==1)tmp+='m';
                if(i==2)tmp+='s';
                if(i==3)tmp+='p';
            }
        }
        
        char sff[80];
        for(int i=0;i<=tmp.size();i++)
        {
            sff[i]=tmp[i];
            
        }
        //cout<<tmp<<endl;
        double ret=10086;
        pair<int,string> rt;
        string anse;
        rt=test_shanten(sff);
        ret=rt.first;
        ret+=0.01;
        anse=rt.second;
        //cout<<anse<<endl;
        //cout<<"???"<<endl;
        for(int i=0;i<last;i++)
        {
            //cout<<"???"<<endl;
            pair<int,int> ttmp;
            // cout<<"???"<<endl;
            ttmp=lastp[i];
            
            // cout<<"???"<<endl;
            string wt=cover(ttmp.first,ttmp.second);
            
            //cout<<wt<<endl;
            //cout<<anse<<endl;
             //cout<<"???"<<endl;
            string::size_type position;
 
            //find 函数 返回jk 在s 中的下标位置 
            position = anse.find(wt);
            if (position != anse.npos)
            {
                //cout<<"!!@!@!@#!@#"<<endl;
                ans+=(double)((double)num[ttmp.first][ttmp.second]/(double)lastans)/(double)ret;
            }
        }
        //cout<<endl;
        
        root.push_back(ans);
        cout<<rootn[root.size()-1]<<" "<<ans<<endl;
        cout<<endl;
    }
    double tmax=-10000.0;
    string tmaxs;
    for(int i=0;i<root.size();i++)
    {
        if(tmax<root[i])
        {
            tmax=root[i];
            tmaxs=rootn[i];
        }
    }
    return tmaxs;
}
int main()
{
	vector<string>hand;int myPlayID=0,quan=1;
	int num[6][10];
    for(int j=1;j<=3;j++)num[j][0]=36;num[4][0]=16;num[5][0]=12;
	for(int i=1;i<=5;i++)for(int j=1;j<=9;j++)num[i][j]=4;
	num[1][0]-=2,num[1][7]--;num[1][9]--;
	num[2][0]-=13;num[2][3]-=3;num[2][2]-=2;num[2][1]-=3;num[2][4]--;num[2][5]--;num[2][6]--;num[2][9]-=2;
	num[3][0]-=9;num[3][1]--;num[3][2]-=3;num[3][3]-=2;num[3][7]--;num[3][8]-=2;
    num[4][0]-=7;num[4][1]-=3;num[4][2]-=2;num[4][4]-=2;//1��2��3��4��
    num[5][0]-=5;num[5][1]-=2;num[5][2]-=3;//1��2��3��

	vector<pair<string, pair<string, int> > >pack;
	string s[8]={"B4","B5","B9","T7","W3","W5","W5","W8"};
	for(int i=0;i<8;i++)hand.push_back(s[i]);
	
	pack.push_back({"PENG",{"J1",0}});
	pack.push_back({"PENG",{"W6",0}});
    
	cout<<Dapai(hand,pack,num,myPlayID,quan);
	//cout<<"???"<<endl;
}


#include "stringify.cpp"


