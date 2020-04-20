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
//������Լ����壬0��ʾJSON������1��ʾ�򵥽�����

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
pii f(string stmp)//����ת�� 
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
string ff(int x,int y)//����ת��2
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
	CardCount=i;//��ǰ�ɲ���������
	if((CardCount-2)%3!=0)return false;
	int num[6][10]={0};//��ǰ����ͳ��
	for(int i=0;i<CardCount;i++){
		pii a=f(hand[i]);
		num[a.first][a.second]++;
		num[a.first][0]++;
	}
	return CheckHu(num[1],9)&&CheckHu(num[2],9)&&CheckHu(num[3],9)&&CheckHu(num[4],4)&&CheckHu(num[5],3);
}
int main()
{
    //��ʼ���Ƴ�
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
			num[a.first][0]--;num[a.first][a.second]--;//�ƶѼ��� 
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
                if(stmp1=="PLAY")hand.erase(find(hand.begin(), hand.end(), stmp2));//ֱ�Ӵ�� 
                else if(stmp1=="GANG"){
                	sort(hand.begin(),hand.end());
                	for(auto i:hand){
                		if(i==stmp2)i[0]-='A'-'a';//�Ѹ���ȫ�����Сд 
					}
					sort(hand.begin(),hand.end());
				}
                else if(stmp1=="BUGANG"){
                	for(auto i:hand){
                		if(i==stmp2)i[0]-='A'-'a';//�Ѹ���ȫ�����Сд 
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
					//�� 
				}
				else if(stmp=="PLAY"){
					sin>>LastCard;
					pii a=f(LastCard);
			        num[a.first][0]--;num[a.first][a.second]--;//�ƶѼ���
				}
				else if(stmp=="PENG"){
					pii a=f(LastCard);
			        num[a.first][0]-=2;num[a.first][a.second]-=2;//�ƶѼ���
			        if(myPlayerID==itmp){  //�൱�������Ƴɹ� 
			        	for(auto i:hand){
                		    if(i==LastCard)i[0]-='A'-'a';//������ȫ�����Сд 
					    }LastCard[0]-='A'-'a';
					    hand.push_back(LastCard); 
					    sort(hand.begin(),hand.end());
					} 
			        sin>>LastCard;
			        a=f(LastCard);
			        num[a.first][0]--;num[a.first][a.second]--;//�ƶѼ���
			        if(myPlayerID==itmp){
			        	hand.erase(find(hand.begin(), hand.end(), LastCard));//������� 
					}
				}
				else if(stmp=="CHI"){
					pii a=f(LastCard);
			        num[a.first][0]++;num[a.first][a.second]++;
			        if(myPlayerID==itmp){   //�ҳԵ� 
			        	LastCard[0]-='A'-'a';
						hand.push_back(LastCard); 
					}
					pii b=a;//���� 
			        string zhongCard;
			        sin>>zhongCard;
			        a=f(zhongCard);
			        num[a.first][0]-=3;num[a.first][a.second-1]--;num[a.first][a.second]--;num[a.first][a.second+1]--;
			        if(myPlayerID==itmp){
			        	int dx[]={-1,0,1};
					    for(int i=0;i<3;i++){
					    	if(b.second==a.second+dx[i])continue;//����֮ǰ�й��� 
					    	string s=ff(a.first,a.second+dx[i]);
						    for(auto i:hand){//�Ե���ת��Ϊ���� 
			            		if(i==s){
								    i[0]-='A'-'a';break;//һ���ͺ� 
								}
							}
						} 
						sort(hand.begin(),hand.end()); 
					}
			        sin>>LastCard;
			        if(myPlayerID==itmp){
			        	hand.erase(find(hand.begin(), hand.end(), LastCard));//������� 
					}
					a=f(LastCard);
			        num[a.first][0]--;num[a.first][a.second]--;//�ƶѼ���
				}
				else if(stmp=="GANG"){
					if(Laststmp=="DRAW"){
						//���� 
					}
					else{
						//���� 
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
			Laststmp=stmp;//��¼��һ�β��� 
        }
        
		sin.clear();
        sin.str(request[turnID]);
        //���⿪ʼ 
        
        sin >> itmp;
        if(itmp == 2) {
            sin>>stmp;//��ǰ��������
			hand.push_back(stmp); 
			sort(hand.begin(),hand.end());
			bool ok=false;//��ʾ�Ƿ��Ѿ�������Ӧ 
			if(Hu()){
			    	//�㷬
					void MahjongInit();
					 
					response.push_back("Hu");
			}
			if(!ok){
					int CardCount=0,i;
					for( i=0;i<hand.size();i++){
						if(hand[i][0]>='a')break;
					} 
					CardCount=i;//��ǰ�ɲ���������
					int mynums[6][10]={0};//��ǰ����ͳ��
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
							if(mynums[i][j]==4){//����Ƿ�������һ���� 
								string s="GANG "+ff(i,j);
								response.push_back(s);
								ok=true;
							}
						}
						if(ok=true)break;
					}
					if(!ok)
					{
						//����Ƿ���Ҫ����
						stmp[0]-='A'-'a';int cnt=0;
						for(int i=CardCount;i<hand.size();i++){
							if(stmp==hand[i])cnt++;
						}
						if(cnt==3){
							stmp[0]-='a'-'A';//���ƻ�ԭ 
							string s="BUGANG "+stmp;
							response.push_back(s);
							ok=true;
						}
					}
					if(!ok)
					{
					    //����	
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
