#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "MahjongGB/MahjongGB.h"
#ifdef _BOTZONE_ONLINE
#include "jsoncpp/json.h"
#else
#include "jsoncpp/json.h" 
#endif

#define SIMPLEIO 0
//由玩家自己定义，0表示JSON交互，1表示简单交互。

using namespace std;

vector<string> request, response;
vector<string> hand;

vector<pair<int, string> > MahjongFanCalculator(
    vector<pair<string, pair<string, int> > > pack,
    vector<string> hand,
    string winTile,
    int flowerCount,
    bool isZIMO,
    bool isJUEZHANG,
    bool isGANG,
    bool isLAST,
    int menFeng,
    int quanFeng);
    
    
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
int main()
{
    //初始化牌池
    int num[6][10];
    for(int j=1;j<=4;j++)num[j][0]=36;num[5][0]=16;num[6][0]=12;
	for(int i=1;i<=6;i++)for(int j=1;j<=9;j++)num[i][j]=4; 
	
	int turnID;
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
        for(int j = 0; j < 4; j++) sin >> hua[j];
        for(int j = 0; j < 13; j++) {
            sin >> stmp;
            hand.push_back(stmp);
            pii a=f(stmp);
			num[a.first][0]--;num[a.first][a.second]--;//牌堆减少 
        }
        string LastCard="",Laststmp=""; 
        for(int i = 2; i < turnID; i++) {
            sin.clear();
            sin.str(request[i]);
            sin >> itmp;
            if(itmp == 2) {
                sin >> stmp;
                hand.push_back(stmp);
                sin.clear();
                sin.str(response[i]);
                string stmp1,stmp2;
                sin >> stmp1 >> stmp2;
                if(stmp1=="PLAY")hand.erase(find(hand.begin(), hand.end(), stmp2));//直接打出 
                else if(stmp1=="GANG"){
                	sort(hand.begin(),hand.end());
                	for(auto i:hand){
                		if(i==stmp2)i[0]-='A'-'a';//把杠牌全部变成小写 
					}
					sort(hand.begin(),hand.end());
				}
                else if(stmp1=="BUGANG"){
                	for(auto i:hand){
                		if(i==stmp2)i[0]-='A'-'a';//把杠牌全部变成小写 
					}
					sort(hand.begin(),hand.end());
				}
            }
            else {
            	sin>>itmp;
            	sin>>stmp;
            	if(stmp=="BUHUA"){
            	    hua[itmp]++; 
				}
				else if(stmp=="DRAW"){
					//空 
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
			        	for(auto i:hand){
                		    if(i==LastCard)i[0]-='A'-'a';//把碰牌全部变成小写 
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
					}
					pii b=a;//备份 
			        string zhongCard;
			        sin>>zhongCard;
			        a=f(zhongCard);
			        num[a.first][0]-=3;num[a.first][a.second-1]--;num[a.first][a.second]--;num[a.first][a.second+1]--;
			        if(myPlayerID==itmp){
			        	int dx[]={-1,0,1};
					    for(int i=0;i<3;i++){
					    	if(b.second==a.second+dx[i])continue;//那张之前判过了 
					    	string s=ff(a.first,a.second+dx[i]);
						    for(auto i:hand){//吃的牌转化为死牌 
			            		if(i==s){
								    i[0]-='A'-'a';break;//一个就好 
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
					}
					else{
						//明杠 
						pii a=f(LastCard);
			            num[a.first][0]-=3;num[a.first][a.second]-=3;
					} 
				}
				else if(stmp=="BUGANG"){
					string Card;
					sin>> Card;
					pii a=f(Card);
					num[a.first][0]--;num[a.first][a.second]--;
				}
				
			}
			Laststmp=stmp;//记录上一次操作 
        }
        
		sin.clear();
        sin.str(request[turnID]);
        //正题开始 
        
        sin >> itmp;
        if(itmp == 2) {
            sin>>stmp;//当前摸到的牌
			hand.push_back(stmp); 
			sort(hand.begin(),hand.end());
			bool ok=false;//表示是否已经给出回应 
			if(Hu()){
			    	//算番
					void MahjongInit();
					 
					response.push_back("Hu");
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
								response.push_back(s);
								ok=true;
							}
						}
						if(ok=true)break;
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
							response.push_back(s);
							ok=true;
						}
					}
					if(!ok)
					{
					    //策略	
					} 
					
			}
			
			
        } 
		else {
            sout << "PASS";
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
