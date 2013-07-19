#include"StdAfx.h"
#include "Program.h"
#include"Event.h"
#include"Activity.h"
#include<vector>
#include"OpenGLView.h"
#include"MainFrm.h"
Program::Program(void)
{
	gHit=NULL;
	gOver=NULL;
	sortOk=false;
	//初始化时默认加载开始和结束事件,和对应的活动，这是最简单的AOE
	Event*start=addEvent(START_EVENT,-200,0);
	Event*end=addEvent(END_EVENT,200,0);
	//addActivity(start,end,1);	
	/*for(int i=1;i<8;i++)
	{
	Event*end=addEvent(NORMAL_EVENT,100*cos(i*PI/4),100*sin(i*PI/4));
	addActivity(start,end,1);
	}*/
	Event*p1,*p2,*p3,*p4,*p5,*p6,*p7;
	addActivity(start,p1=addEvent(NORMAL_EVENT,-100,50),6);
	addActivity(start,p2=addEvent(NORMAL_EVENT,-100,-50),4);
	addActivity(start,p3=addEvent(NORMAL_EVENT,-100,-100),5);
	addActivity(p1,p4=addEvent(NORMAL_EVENT,0,0),1);
	addActivity(p2,p4,1);
	addActivity(p3,p5=addEvent(NORMAL_EVENT,0,-100),2);
	addActivity(p4,p6=addEvent(NORMAL_EVENT,100,50),9);
	addActivity(p4,p7=addEvent(NORMAL_EVENT,100,-50),7);
	addActivity(p5,p7,4);
	addActivity(p6,end,2);
	addActivity(p7,end,4);
	this->KeyPath();
}


Program::~Program(void)
{
	list<Event*>::iterator evtIt;
	for(evtIt=evtList.begin();evtIt!=evtList.end();++evtIt)
	{
		delete *evtIt;//清除事件对象
	}
	list<Activity*>::iterator actIt;
	for(actIt=actList.begin();actIt!=actList.end();++actIt)
	{
		delete *actIt;//清除活动对象
	}
}

Event* Program::addEvent(EVENT_TYPE type,float x,float y)
{
	Event*evt=new Event(type,x,y);//创建新的事件，加入到事件列表中
	this->evtList.push_back(evt);
	return evt;
}
void Program::delEvent(Event*evt)
{
	if(evt->type!=NORMAL_EVENT)
		return;//不删除开始和结束节点
	//删除所有前驱和后继活动
	list<Activity*>::iterator i;
	for(i=evt->preList.begin();i!=evt->preList.end();++i)//当前事件的所有的前驱活动
	{
		(*i)->startEvent->nextList.remove(*i);//所有前驱活动的开始事件清除后继活动
		//从活动列表中移除
		this->actList.remove(*i);
		//释放活动对象
		delete *i;
	}
	for(i=evt->nextList.begin();i!=evt->nextList.end();++i)//当前事件的所有的后继活动
	{
		(*i)->endEvent->preList.remove(*i);//所有后继活动的结束事件清除前驱活动
		//从活动列表中移除
		this->actList.remove(*i);
		//释放活动对象
		delete *i;
	}
	//清除所有活动关联信息
	evt->preList.clear();
	evt->nextList.clear();
	//从事件列表中移除
	this->evtList.remove(evt);
	//释放对象
	delete evt;
}

void Program::addActivity(Event*start,Event*end,float cost,float x,float y)
{
	if(end==NULL)//新建活动
	{
		if(pickType!=-1)//没选中事件，没法添加活动
			return ;
		start=(Event*)gHit;
		if(start->type!=END_EVENT)
		{
			Activity*act=new Activity(start,NULL,cost);
			//关联事件活动
			start->nextList.push_back(act);
			//end->preList.push_back(act);
			this->actList.push_back(act);//添加到活动列表
			gHit=(Graphic*)act;//选中这个活动
			gHit->moveTo(x,y);
		}
	}
	else
	{
		if(start->type!=END_EVENT&&end->type!=START_EVENT)
		{
			Activity*act=new Activity(start,end,cost);
			//关联事件活动
			start->nextList.push_back(act);
			end->preList.push_back(act);
			this->actList.push_back(act);//添加到活动列表
		}
	}
}
void Program::delActivity(Activity*act)
{
	//先删除事件关联
	act->startEvent->nextList.remove(act);
	if(act->endEvent!=NULL)//新建活动
		act->endEvent->preList.remove(act);
	//从活动列表中移除
	this->actList.remove(act);
	//施放对象
	delete act;
}
void Program::refresh()
{
	list<Event*>::iterator evtIt;
	for(evtIt=evtList.begin();evtIt!=evtList.end();++evtIt)
	{
		(*evtIt)->name=0;
		(*evtIt)->isKeyEvent=false;
	}
	list<Activity*>::iterator actIt;
	for(actIt=actList.begin();actIt!=actList.end();++actIt)
	{
		(*actIt)->isKeyActivity=false;
	}
}
bool Program::topoSort()
{
	refresh();//排序之前清除上一次排序的结果
	vector<Event*> stack;//拓扑排序栈
	list<Event*>::iterator i;
	for(i=evtList.begin();i!=evtList.end();++i)
	{
		if(!(*i)->initInDegree())//初始化所有事件节点的入度,保证了所有节点出入度的合法性
		{
			//存在非法节点
			return false;
		}
	}
	topoList.clear();//清空上次排序结果
	//开始拓扑排序，主要检测是否有有向环，并在排序成后生成事件名称
	Event*start=*evtList.begin();
	stack.push_back(start);//开始节点入栈<初始化时开始节点在第一个位置，第二个位置是结束节点>
	//for(i=evtList.begin();i!=evtList.end();++i)
	//{
	//	if((*i)->inDegree==0)//开始节点
	//	{
	//		stack.push_back(*i);//开始节点入栈
	//		break;
	//	}
	//}
	int count=0;//计数器
	while(!stack.empty())
	{
		Event*top=stack.back();//栈顶元素
		topoList.push_back(top);//记录拓扑顺序
		stack.pop_back();//弹出该元素
		count++;//获得一个拓扑顺序元素
		top->name=count;//将编号命名为事件的名称
		//修改下一个节点入度
		for(list<Activity*>::iterator i=top->nextList.begin();i!=top->nextList.end();++i)
		{
			if(--(*i)->endEvent->inDegree==0)//下一个事件的入度-1后为0
			{
				stack.push_back((*i)->endEvent);//0入度事件进栈
			}
		}
	}
	if(count!=evtList.size())//拓扑排序的节点个数和实际节点个数不吻合，说明存在有向环
	{
		refresh();
		return false;//排序失败
	}
	return true;
}

void Program::KeyPath()
{
	if(!(sortOk=topoSort()))//拓扑排序失败，不能继续计算
	{
		return;
	}
	list<Event*>::iterator evtIt=topoList.begin();
	list<Activity*>::iterator actIt;
	(*evtIt)->ve=0;//开始节点的Ve=0
	for(++evtIt;evtIt!=topoList.end();++evtIt)//遍历拓扑序列，求出Ve和Ae
	{
		//遍历事件前驱活动，求取最大路径长度
		float max=0;
		for(actIt=(*evtIt)->preList.begin();actIt!=(*evtIt)->preList.end();++actIt)
		{
			(*actIt)->ae=(*actIt)->startEvent->ve;//Ae=Ve
			float cur=(*actIt)->startEvent->ve+(*actIt)->timeCost;//Ve[j]=Ve[i]+dur<i,j>:=>max
			max=max>cur?max:cur;//最大路径长度
		}
		(*evtIt)->ve=max;//求出Ve
	}
	topoList.reverse();//反转拓扑序列
	evtIt=topoList.begin();
	this->period=(*evtIt)->vl=(*evtIt)->ve;//最后一个拓扑节点Vl[n-1]=Ve[n-1],即工期
	for(++evtIt;evtIt!=topoList.end();++evtIt)//遍历逆拓扑序列，求出Vl和Al
	{
		//遍历事件后继活动，求取最大路径长度
		float min=0;min=1/min;//获得浮点数最大值
		for(actIt=(*evtIt)->nextList.begin();actIt!=(*evtIt)->nextList.end();++actIt)
		{
			(*actIt)->al=(*actIt)->endEvent->vl-(*actIt)->timeCost;//Al=Vl[j]-dur<i,j>
			//判断是否是关键活动Ae=Al
			if((*actIt)->ae==(*actIt)->al)
				(*actIt)->isKeyActivity=true;
			float cur=(*actIt)->endEvent->vl-(*actIt)->timeCost;//Ve[i]=Vl[j]-dur<i,j>:=>min
			min=min<cur?min:cur;//最大路径长度,值越小离终点越远
		}
		(*evtIt)->vl=min;//求出Vl
		//判断是否是关键事件
		if((*evtIt)->ve==(*evtIt)->vl)
			(*evtIt)->isKeyEvent=true;
	}
}

void Program::draw()
{
	/*CMainFrame*pMain=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	COpenGLView*pView=(COpenGLView*)pMain->GetActiveView();*/
	//绘制主体
	list<Event*>::iterator evtIt;
	for(evtIt=evtList.begin();evtIt!=evtList.end();++evtIt)
	{
		(*evtIt)->draw();
	}
	list<Activity*>::iterator actIt;
	for(actIt=actList.begin();actIt!=actList.end();++actIt)
	{
		(*actIt)->draw();
	}
}

void Program::pick(float x,float y,bool hasRet)
{
	if(hasRet)
		pickType=0;
	list<Event*>::iterator evtIt;
	evtOver=NULL;
	gOver=NULL;
	for(evtIt=evtList.begin();evtIt!=evtList.end();++evtIt)
	{
		if((*evtIt)->pick(x,y))//选中事件
		{
			if(hasRet)
			{
				gHit=(Graphic*)*evtIt;
				pickType=-1;
				break;
			}
			evtOver=*evtIt;//记录扫过的图形
			gOver=(Graphic*)evtOver;
		}
	}
	list<Activity*>::iterator actIt;
	for(actIt=actList.begin();actIt!=actList.end();++actIt)
	{
		if((gHit!=(Graphic*)(*actIt))&&(*actIt)->pick(x,y))//选中活动,不再扫描自身，否则影响选择因子
		{
			if(hasRet)
			{
				gHit=(Graphic*)*actIt;
				pickType=1;
				break;
			}
			gOver=(Graphic*)*actIt;
		}
	}
	if(pickType==0)
	{
		if(hasRet)
			gHit=NULL;//没选中
	}
	else if(hasRet)
	{
		gHit->moveTo(x,y);
	}
}

void Program::move(float x,float y)
{
	if(gHit==NULL)
		return ;
	gHit->move(x,y);
}


void Program::up(float x,float y)
{
	if(gHit==NULL)
		return;
	pick(x,y,false);//得到弹起时最新的evtOver
	//gHit如果是事件就不做处理，是活动看返回时是否有错误
	bool done=gHit->up(x,y,evtOver);//测试能否新建路径：1-终点不是起始点；2-终点不能是该活动起点；
	//3-起点不是终止点；4-起点不是该活动终点；
	if(!done||evtOver==NULL)//出现问题,必然是活动创建出现问题：
		//evtOver=NULL时，gHit要是事件evtOver不可能是空，evtOver本身至少能被击中，gHit必然是活动
	{
		Activity*act=(Activity*)gHit;
		if(act->endEvent==NULL)//新建的活动失败
		{
			//删除活动
			this->delActivity(act);
		}
		else//修改活动失败
		{
			act->rePos();//还原位置
		}
	}
	gHit->selected=0;
	gHit=NULL;
	pick(x,y,false);//恢复鼠标悬停	
}

void Program::del(float x,float y)
{
	pick(x,y,true);
	if(gHit==NULL)
		return;
	if(pickType==-1)//事件
	{
		this->delEvent((Event*)gHit);
	}
	else if(pickType==1)//活动
	{
		this->delActivity((Activity*)gHit);
	}
}

void Program::drawTips()
{
	if(gOver!=NULL)
		gOver->drawTips();
}