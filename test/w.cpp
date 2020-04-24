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
//#include "MahjongGB/MahjongGB.h"
#ifdef _BOTZONE_ONLINE
#include "jsoncpp/json.h"
#else
#include "jsoncpp/json.h" 
#endif

#define SIMPLEIO 0
//由玩家自己定义，0表示JSON交互，1表示简单交互。

using namespace std;

vector<string> request, response;
vector<string> hand,unhand;//当前的手牌 暗杠的手牌 
vector<pair<string, pair<string, int> > > pack;
    
    
typedef pair<int,int> pii;
pii f(string stmp)//牌型转化 
{
	pii a;
	switch(stmp[0]){
		case 'W':a.first=1;break;
		case 'B':a.first=2;break;
		case 'T':a.first=3;break;
		case 'F':a.first=4;break;
		case 'J':a.first=5;break;
	}
	a.second=stmp[1]-'0';
	return a;
}
string ff(int x,int y)//牌型转化2
{
   string s="";
   switch(x){
		case 1:s+='W';break;
		case 2:s+='B';break;
		case 3:s+='T';break;
		case 4:s+='F';break;
		case 5:s+='J';break;
	}
	s+=char('0'+y);
	return  s;
}
int fff(int myID,int playID)//判断是那家供牌 
{
	if((myID-1+4)%4==playID)return 0;//上家供牌 
	if((myID+1)%4==playID)return 2;//下家供牌 
	return 1;//对家供牌 
}
bool CheckHu(int num[10],int k){
	for(int i=1;i<=k;i++)if(num[i]<0)return false;
	if(num[0]==0)return true;
	if(num[0]==1)return false;
	if(num[0]==2){
		for(int i=1;i<=k;i++)if(num[i]==2)return true;
		return false;
	}
	if(k==9){
		for(int i=1;i<=9;i++){
			if(num[i]>0){
				if(i<=7){
					num[i]--,num[i+1]--,num[i+2]--;
					num[0]=num[0]-3;
					if(CheckHu(num,k))return true;
					num[0]=num[0]+3;
					num[i]++,num[i+1]++,num[i+2]++;
				}
				if(num[i]>=3){
					num[i]-=3;num[0]-=3;
					if(CheckHu(num,k))return true;
					num[i]+=3;num[0]+=3;
				}
			}
		}return false;
	}
    else 
    {
    	for(int i=1;i<=k;i++){
    		if(num[i]>0){
    			if(num[i]>=3){
					num[i]-=3;num[0]-=3;
					if(CheckHu(num,k))return true;
					num[i]+=3;num[0]+=3;
				}
			}
		}return false;
	}
}
bool Hu()
{
	if(hand.size()<14||hand.size()>18)return false;
	int CardCount=0,i;
	for( i=0;i<hand.size();i++){
		if(hand[i][0]>='a')break;
	} 
	CardCount=i;//当前可操作的牌数
	if((CardCount-2)%3!=0)return false;
	int num[6][10]={0};//当前手牌统计
	for(int i=0;i<CardCount;i++){
		pii a=f(hand[i]);
		num[a.first][a.second]++;
		num[a.first][0]++;
	}
	return CheckHu(num[1],9)&&CheckHu(num[2],9)&&CheckHu(num[3],9)&&CheckHu(num[4],4)&&CheckHu(num[5],3);
}

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
        ret+=0.0001;
		ans+=5000-500*ret;
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
                ans+=(double)(((double)num[ttmp.first][ttmp.second]/(double)lastans))*300;
            }
        }
        //cout<<endl;
        
        root.push_back(ans);
        
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
    //初始化牌池
    int num[6][10];
    for(int j=1;j<=3;j++)num[j][0]=36;num[4][0]=16;num[5][0]=12;
	for(int i=1;i<=5;i++)for(int j=1;j<=9;j++)num[i][j]=4; 
	
	int turnID,paiqiang=144;//当前牌墙剩余多少牌 
    string stmp;
#if SIMPLEIO
    cin >> turnID;
    turnID--;
    getline(cin, stmp);
    for(int i = 0; i < turnID; i++) {
        getline(cin, stmp);
        request.push_back(stmp);
        getline(cin, stmp);
        response.push_back(stmp);
    }
    getline(cin, stmp);
    request.push_back(stmp);
#else
    Json::Value inputJSON;
    cin >> inputJSON;
    turnID = inputJSON["responses"].size();
    for(int i = 0; i < turnID; i++) {
        request.push_back(inputJSON["requests"][i].asString());
        response.push_back(inputJSON["responses"][i].asString());
    }
    request.push_back(inputJSON["requests"][turnID].asString());
#endif

    if(turnID < 2) {
		response.push_back("PASS");
    } 
	else {
        int itmp, myPlayerID, quan,hua[4]={0};
        ostringstream sout;
        istringstream sin;
        sin.str(request[0]);
        sin >> itmp >> myPlayerID >> quan;
        sin.clear();
        sin.str(request[1]);
        sin>>itmp;
        for(int j = 0; j < 4; j++) sin >> hua[j],paiqiang-=hua[j];
        for(int j = 0; j < 13; j++) {
            sin >> stmp;
            hand.push_back(stmp);
            pii a=f(stmp);
			num[a.first][0]--;num[a.first][a.second]--;//牌堆减少 
        }paiqiang-=4*13;
        string LastCard="",Laststmp="";int Lastuser; 
        for(int i = 2; i < turnID; i++) {
            sin.clear();
            sin.str(request[i]);
            sin >> itmp;
            if(itmp == 2) {
                sin >> stmp;  paiqiang--;
                hand.push_back(stmp);
                pii a=f(stmp);
                num[a.first][0]--;num[a.first][a.second]--;
                sin.clear();
                sin.str(response[i]);
                string stmp1,stmp2;
                sin >> stmp1 >> stmp2;
                if(stmp1=="PLAY")hand.erase(find(hand.begin(), hand.end(), stmp2));//直接打出 
                else if(stmp1=="GANG"){
                	sort(hand.begin(),hand.end());
                	for(int i=0;i<4;i++)unhand.push_back(stmp2);  //加入暗杠的手牌 
                	for(int i=0;i<hand.size();i++){
                		if(hand[i]==stmp2)hand[i][0]-='A'-'a';//把杠牌全部变成小写 
					}
					sort(hand.begin(),hand.end());
				}
                else if(stmp1=="BUGANG"){
                	for(int i=0;i<hand.size();i++){
                		if(hand[i]==stmp2)hand[i][0]-='A'-'a';//把杠牌全部变成小写 
					}
					for(int i=0;i<pack.size();i++){
						if(pack[i].first=="PENG"&&pack[i].second.first==stmp2){  //之前一定碰过,直接改 
							pack[i].first="GANG";
						}
					} 
					sort(hand.begin(),hand.end());
				}
				Lastuser=myPlayerID; 
            }
            else {
            	sin>>itmp;
            	sin>>stmp;
            	if(stmp=="BUHUA"){
            	    hua[itmp]++; paiqiang--;
				}
				else if(stmp=="DRAW"){
					//空 
					paiqiang--;
				}
				else if(stmp=="PLAY"){
					sin>>LastCard;
					pii a=f(LastCard);
			        num[a.first][0]--;num[a.first][a.second]--;//牌堆减少
				}
				else if(stmp=="PENG"){
					pii a=f(LastCard);
			        num[a.first][0]-=2;num[a.first][a.second]-=2;//牌堆减少
			        if(myPlayerID==itmp){  //相当于我碰牌成功 
			        	
						pack.push_back({"PENG",{LastCard,fff(myPlayerID,Lastuser)}});
						for(int i=0;i<hand.size();i++){
                		    if(hand[i]==LastCard)hand[i][0]-='A'-'a';//把碰牌全部变成小写 
					    }LastCard[0]-='A'-'a';
					    hand.push_back(LastCard); 
					    
					    sort(hand.begin(),hand.end());
					} 
			        sin>>LastCard;
			        a=f(LastCard);
			        num[a.first][0]--;num[a.first][a.second]--;//牌堆减少
			        if(myPlayerID==itmp){
			        	hand.erase(find(hand.begin(), hand.end(), LastCard));//打出手牌 
					}
				}
				else if(stmp=="CHI"){
					pii a=f(LastCard);
			        num[a.first][0]++;num[a.first][a.second]++;
			        if(myPlayerID==itmp){   //我吃的 
			        	LastCard[0]-='A'-'a';
						hand.push_back(LastCard); 
						sort(hand.begin(),hand.end());
					}
					pii b=a;//备份 
			        string zhongCard;
			        sin>>zhongCard;
			        a=f(zhongCard);
			        num[a.first][0]-=3;num[a.first][a.second-1]--;num[a.first][a.second]--;num[a.first][a.second+1]--;
			        if(myPlayerID==itmp){
			        	int dx[]={-1,0,1};
					    for(int i=0;i<3;i++){
					    	if(b.second==a.second+dx[i]){//这张是上家给的牌 
							      pack.push_back({"CHI",{zhongCard,i}});
								  continue;//那张之前判过了
							} 
					    	string s=ff(a.first,a.second+dx[i]);
						    for(int i=0;i<hand.size();i++){//吃的牌转化为死牌 
			            		if(hand[i]==s){
								    hand[i][0]-='A'-'a';
									//sout<<hand[i]<<endl;
									break;//一个就好 
								}
							}
						} 
						sort(hand.begin(),hand.end()); 
					}
			        sin>>LastCard;
			        if(myPlayerID==itmp){
			        	hand.erase(find(hand.begin(), hand.end(), LastCard));//打出手牌 
					}
					a=f(LastCard);
			        num[a.first][0]--;num[a.first][a.second]--;//牌堆减少
				}
				else if(stmp=="GANG"){
					if(Laststmp=="DRAW"){
						//暗杠 
						//如果是别人 那我什么也做不了
						//如果是自己 上面已经写过了 
					}
					else{
						//明杠 
						pii a=f(LastCard);
			            num[a.first][0]-=3;num[a.first][a.second]-=3;
			            if(myPlayerID==itmp){
			            	for(int i=0;i<hand.size();i++)
			            	{
			            		if(hand[i]==LastCard){
			            			hand[i][0]-='A'-'a';
								}
							}
						}
					} 
				}
				else if(stmp=="BUGANG"){
					string Card;
					sin>> Card;
					pii a=f(Card);
					num[a.first][0]--;num[a.first][a.second]--;
				}
				
			}
			Laststmp=stmp;Lastuser=itmp;//记录上一次操作 
        }
        
		sin.clear();
        sin.str(request[turnID]);
        //正题开始 
        sort(hand.begin(),hand.end()); 
        //for(int i=0;i<hand.size();i++)sout<<hand[i]<<" ";
        sin >> itmp;
		bool ok=false;//表示是否已经给出回应
        if(itmp == 2) {
            sin>>stmp;//当前摸到的牌
			hand.push_back(stmp); 
			pii a=f(stmp);
			num[a.first][0]--;num[a.first][a.second]--;
			sort(hand.begin(),hand.end());
			if(Hu()){
			    	//算番
					MahjongInit();
					vector<string>myhand;
					sort(hand.begin(),hand.end());
					for(auto i:hand){
						if(i[0]<'a')myhand.push_back(i);
						else break;
					}
					for(auto i:unhand){
						myhand.push_back(i);
					}
					bool isZIMO=true,isJUEZHANG=0,isGANG=0,isLAST=0;
					if(num[a.first][a.second]==0)isJUEZHANG=true;
					sin.clear();
					sin.str(request[turnID-1]);
					string stmp1;int ID;
					sin>>stmp1>>ID>>stmp1;
					if(ID==myPlayerID&&(stmp1=="GANG"||stmp1=="BUGANG"))isGANG=1;
					if(paiqiang==0)isLAST=true;
				    vector<pair<int,string>> Fan=MahjongFanCalculator(pack, myhand, stmp, hua[myPlayerID], isZIMO, isJUEZHANG, isGANG, isLAST, myPlayerID, quan);
		            int ans=0;
					for(auto i:Fan){
		            	ans+=i.first;
					}
					if(ans>=8){ok=true;sout<<"HU";}
			}
			if(!ok){
					int CardCount=0,i;
					for( i=0;i<hand.size();i++){
						if(hand[i][0]>='a')break;
					} 
					CardCount=i;//当前可操作的牌数
					int mynums[6][10]={0};//当前手牌统计
					for(int i=0;i<CardCount;i++){
						pii a=f(hand[i]);
						mynums[a.first][a.second]++;
						mynums[a.first][0]++;
					} 
					
					int dx[]={0,9,9,9,4,3};
					for(int i=1;i<=5;i++)    
					{
						for(int j=1;j<=dx[i];j++)
						{
							if(mynums[i][j]==4){//检测是否有四张一样的 
								string s="GANG "+ff(i,j);
								sout<<s;
								ok=true;
								break;
							}
						}
						if(ok==true)break;
					}
					if(!ok)
					{
						//检测是否需要补杠
						stmp[0]-='A'-'a';int cnt=0;
						for(int i=CardCount;i<hand.size();i++){
							if(stmp==hand[i])cnt++;
						}
						if(cnt==3){
							stmp[0]-='a'-'A';//手牌还原 
							string s="BUGANG "+stmp;
							sout<<s;
							ok=true;
						}
					}
					if(!ok)
					{
					    //策略
						string Card=Dapai(hand,pack,num,myPlayerID,quan);
						hand.erase(find(hand.begin(), hand.end(), Card));
						sout<<"PLAY "<<Card;ok=true;
                    } 
					
			}
			
			
        } 
		else if(itmp==3){
            int  playerID;
            sin>>playerID>>stmp;
            if(stmp=="PLAY"&&playerID!=myPlayerID){
				int CardCount=0,i;
				for( i=0;i<hand.size();i++){
					if(hand[i][0]>='a')break;
				} 
				CardCount=i;//当前可操作的牌数
				int con[6][10]={0};
				for(int i=0;i<CardCount;i++){
					pii a=f(hand[i]);
					con[a.first][a.second]++;
					con[a.first][0]++;
				}
				string Card;sin>>Card;
				pii a=f(Card);
				if(con[a.first][a.second]==2){
					sout<<"PENG ";
					//出牌 
					vector<string>uhand=hand;
					uhand.erase(find(uhand.begin(), uhand.end(), Card));
					uhand.erase(find(uhand.begin(), uhand.end(), Card));//删掉两次
					vector<pair<string, pair<string, int> > > upack=pack;
					upack.push_back({"PENG",{Card,fff(myPlayerID,playerID)}}); 
					string Card=Dapai(uhand,upack,num,myPlayerID,quan);
					sout<<Card;
					ok=true;
				}
				else if(con[a.first][a.second]==3){
				sout<<"GANG";ok=true;
				} 
				else if(fff(myPlayerID,playerID)==0&&a.first<=3&&((a.second>2&&con[a.first][a.second-2]>0&&con[a.first][a.second-1]>0)||(a.second>1&&a.second<9&&con[a.first][a.second-1]>0&&con[a.first][a.second+1]>0)||(a.second<=7&&con[a.first][a.second+1]>0&&con[a.first][a.second+2]>0)))
				{
				    sout<<"CHI ";
				    vector<string>uhand=hand;
					vector<pair<string, pair<string, int> > > upack=pack;
					if((a.second>2&&con[a.first][a.second-2]>0&&con[a.first][a.second-1]>0))
					{
					    upack.push_back({"CHI",{ff(a.first,a.second-1),3}});
						string s1=ff(a.first,a.second-2); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						       s1=ff(a.first,a.second-1); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						     //s1=ff(a.first,a.second  ); uhand.erase(find(uhand.begin(), uhand.end(), s1));
					    string Card2=Dapai(uhand,upack,num,myPlayerID,quan);
					    sout<<ff(a.first,a.second-1)<<" "<<Card2;ok=true;
					}
					else if((a.second>1&&a.second<9&&con[a.first][a.second-1]>0&&con[a.first][a.second+1]>0))
					{
						upack.push_back({"CHI",{ff(a.first,a.second),2}});
						string s1=ff(a.first,a.second-1); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						     //s1=ff(a.first,a.second  ); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						       s1=ff(a.first,a.second+1); uhand.erase(find(uhand.begin(), uhand.end(), s1));
					    string Card2=Dapai(uhand,upack,num,myPlayerID,quan);
					    sout<<ff(a.first,a.second)<<" "<<Card2;ok=true;
					}
					else if((a.second<=7&&con[a.first][a.second+1]>0&&con[a.first][a.second+2]>0))
					{
						
						upack.push_back({"CHI",{ff(a.first,a.second+1),1}});
						string s1=ff(a.first,a.second+1); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						     //s1=ff(a.first,a.second  ); uhand.erase(find(uhand.begin(), uhand.end(), s1));
						       s1=ff(a.first,a.second+2); uhand.erase(find(uhand.begin(), uhand.end(), s1));     
					    string Card2=Dapai(uhand,upack,num,myPlayerID,quan);
					    sout<<ff(a.first,a.second+1)<<" "<<Card2;ok=true;
					}		
				}
				// 
			}
            if(!ok)sout<<"PASS"; 
        }
        
        response.push_back(sout.str());
    }

#if SIMPLEIO
    cout << response[turnID] << endl;
#else
    Json::Value outputJSON;
    outputJSON["response"] = response[turnID];
    cout << outputJSON << endl;
#endif
    return 0;
}

#include "stringify.cpp"

