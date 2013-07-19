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
	//��ʼ��ʱĬ�ϼ��ؿ�ʼ�ͽ����¼�,�Ͷ�Ӧ�Ļ��������򵥵�AOE
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
		delete *evtIt;//����¼�����
	}
	list<Activity*>::iterator actIt;
	for(actIt=actList.begin();actIt!=actList.end();++actIt)
	{
		delete *actIt;//��������
	}
}

Event* Program::addEvent(EVENT_TYPE type,float x,float y)
{
	Event*evt=new Event(type,x,y);//�����µ��¼������뵽�¼��б���
	this->evtList.push_back(evt);
	return evt;
}
void Program::delEvent(Event*evt)
{
	if(evt->type!=NORMAL_EVENT)
		return;//��ɾ����ʼ�ͽ����ڵ�
	//ɾ������ǰ���ͺ�̻
	list<Activity*>::iterator i;
	for(i=evt->preList.begin();i!=evt->preList.end();++i)//��ǰ�¼������е�ǰ���
	{
		(*i)->startEvent->nextList.remove(*i);//����ǰ����Ŀ�ʼ�¼������̻
		//�ӻ�б����Ƴ�
		this->actList.remove(*i);
		//�ͷŻ����
		delete *i;
	}
	for(i=evt->nextList.begin();i!=evt->nextList.end();++i)//��ǰ�¼������еĺ�̻
	{
		(*i)->endEvent->preList.remove(*i);//���к�̻�Ľ����¼����ǰ���
		//�ӻ�б����Ƴ�
		this->actList.remove(*i);
		//�ͷŻ����
		delete *i;
	}
	//������л������Ϣ
	evt->preList.clear();
	evt->nextList.clear();
	//���¼��б����Ƴ�
	this->evtList.remove(evt);
	//�ͷŶ���
	delete evt;
}

void Program::addActivity(Event*start,Event*end,float cost,float x,float y)
{
	if(end==NULL)//�½��
	{
		if(pickType!=-1)//ûѡ���¼���û����ӻ
			return ;
		start=(Event*)gHit;
		if(start->type!=END_EVENT)
		{
			Activity*act=new Activity(start,NULL,cost);
			//�����¼��
			start->nextList.push_back(act);
			//end->preList.push_back(act);
			this->actList.push_back(act);//��ӵ���б�
			gHit=(Graphic*)act;//ѡ������
			gHit->moveTo(x,y);
		}
	}
	else
	{
		if(start->type!=END_EVENT&&end->type!=START_EVENT)
		{
			Activity*act=new Activity(start,end,cost);
			//�����¼��
			start->nextList.push_back(act);
			end->preList.push_back(act);
			this->actList.push_back(act);//��ӵ���б�
		}
	}
}
void Program::delActivity(Activity*act)
{
	//��ɾ���¼�����
	act->startEvent->nextList.remove(act);
	if(act->endEvent!=NULL)//�½��
		act->endEvent->preList.remove(act);
	//�ӻ�б����Ƴ�
	this->actList.remove(act);
	//ʩ�Ŷ���
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
	refresh();//����֮ǰ�����һ������Ľ��
	vector<Event*> stack;//��������ջ
	list<Event*>::iterator i;
	for(i=evtList.begin();i!=evtList.end();++i)
	{
		if(!(*i)->initInDegree())//��ʼ�������¼��ڵ�����,��֤�����нڵ����ȵĺϷ���
		{
			//���ڷǷ��ڵ�
			return false;
		}
	}
	topoList.clear();//����ϴ�������
	//��ʼ����������Ҫ����Ƿ������򻷣���������ɺ������¼�����
	Event*start=*evtList.begin();
	stack.push_back(start);//��ʼ�ڵ���ջ<��ʼ��ʱ��ʼ�ڵ��ڵ�һ��λ�ã��ڶ���λ���ǽ����ڵ�>
	//for(i=evtList.begin();i!=evtList.end();++i)
	//{
	//	if((*i)->inDegree==0)//��ʼ�ڵ�
	//	{
	//		stack.push_back(*i);//��ʼ�ڵ���ջ
	//		break;
	//	}
	//}
	int count=0;//������
	while(!stack.empty())
	{
		Event*top=stack.back();//ջ��Ԫ��
		topoList.push_back(top);//��¼����˳��
		stack.pop_back();//������Ԫ��
		count++;//���һ������˳��Ԫ��
		top->name=count;//���������Ϊ�¼�������
		//�޸���һ���ڵ����
		for(list<Activity*>::iterator i=top->nextList.begin();i!=top->nextList.end();++i)
		{
			if(--(*i)->endEvent->inDegree==0)//��һ���¼������-1��Ϊ0
			{
				stack.push_back((*i)->endEvent);//0����¼���ջ
			}
		}
	}
	if(count!=evtList.size())//��������Ľڵ������ʵ�ʽڵ�������Ǻϣ�˵����������
	{
		refresh();
		return false;//����ʧ��
	}
	return true;
}

void Program::KeyPath()
{
	if(!(sortOk=topoSort()))//��������ʧ�ܣ����ܼ�������
	{
		return;
	}
	list<Event*>::iterator evtIt=topoList.begin();
	list<Activity*>::iterator actIt;
	(*evtIt)->ve=0;//��ʼ�ڵ��Ve=0
	for(++evtIt;evtIt!=topoList.end();++evtIt)//�����������У����Ve��Ae
	{
		//�����¼�ǰ�������ȡ���·������
		float max=0;
		for(actIt=(*evtIt)->preList.begin();actIt!=(*evtIt)->preList.end();++actIt)
		{
			(*actIt)->ae=(*actIt)->startEvent->ve;//Ae=Ve
			float cur=(*actIt)->startEvent->ve+(*actIt)->timeCost;//Ve[j]=Ve[i]+dur<i,j>:=>max
			max=max>cur?max:cur;//���·������
		}
		(*evtIt)->ve=max;//���Ve
	}
	topoList.reverse();//��ת��������
	evtIt=topoList.begin();
	this->period=(*evtIt)->vl=(*evtIt)->ve;//���һ�����˽ڵ�Vl[n-1]=Ve[n-1],������
	for(++evtIt;evtIt!=topoList.end();++evtIt)//�������������У����Vl��Al
	{
		//�����¼���̻����ȡ���·������
		float min=0;min=1/min;//��ø��������ֵ
		for(actIt=(*evtIt)->nextList.begin();actIt!=(*evtIt)->nextList.end();++actIt)
		{
			(*actIt)->al=(*actIt)->endEvent->vl-(*actIt)->timeCost;//Al=Vl[j]-dur<i,j>
			//�ж��Ƿ��ǹؼ��Ae=Al
			if((*actIt)->ae==(*actIt)->al)
				(*actIt)->isKeyActivity=true;
			float cur=(*actIt)->endEvent->vl-(*actIt)->timeCost;//Ve[i]=Vl[j]-dur<i,j>:=>min
			min=min<cur?min:cur;//���·������,ֵԽС���յ�ԽԶ
		}
		(*evtIt)->vl=min;//���Vl
		//�ж��Ƿ��ǹؼ��¼�
		if((*evtIt)->ve==(*evtIt)->vl)
			(*evtIt)->isKeyEvent=true;
	}
}

void Program::draw()
{
	/*CMainFrame*pMain=(CMainFrame*)::AfxGetApp()->m_pMainWnd;
	COpenGLView*pView=(COpenGLView*)pMain->GetActiveView();*/
	//��������
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
		if((*evtIt)->pick(x,y))//ѡ���¼�
		{
			if(hasRet)
			{
				gHit=(Graphic*)*evtIt;
				pickType=-1;
				break;
			}
			evtOver=*evtIt;//��¼ɨ����ͼ��
			gOver=(Graphic*)evtOver;
		}
	}
	list<Activity*>::iterator actIt;
	for(actIt=actList.begin();actIt!=actList.end();++actIt)
	{
		if((gHit!=(Graphic*)(*actIt))&&(*actIt)->pick(x,y))//ѡ�л,����ɨ����������Ӱ��ѡ������
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
			gHit=NULL;//ûѡ��
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
	pick(x,y,false);//�õ�����ʱ���µ�evtOver
	//gHit������¼��Ͳ��������ǻ������ʱ�Ƿ��д���
	bool done=gHit->up(x,y,evtOver);//�����ܷ��½�·����1-�յ㲻����ʼ�㣻2-�յ㲻���Ǹû��㣻
	//3-��㲻����ֹ�㣻4-��㲻�Ǹû�յ㣻
	if(!done||evtOver==NULL)//��������,��Ȼ�ǻ�����������⣺
		//evtOver=NULLʱ��gHitҪ���¼�evtOver�������ǿգ�evtOver���������ܱ����У�gHit��Ȼ�ǻ
	{
		Activity*act=(Activity*)gHit;
		if(act->endEvent==NULL)//�½��Ļʧ��
		{
			//ɾ���
			this->delActivity(act);
		}
		else//�޸Ļʧ��
		{
			act->rePos();//��ԭλ��
		}
	}
	gHit->selected=0;
	gHit=NULL;
	pick(x,y,false);//�ָ������ͣ	
}

void Program::del(float x,float y)
{
	pick(x,y,true);
	if(gHit==NULL)
		return;
	if(pickType==-1)//�¼�
	{
		this->delEvent((Event*)gHit);
	}
	else if(pickType==1)//�
	{
		this->delActivity((Activity*)gHit);
	}
}

void Program::drawTips()
{
	if(gOver!=NULL)
		gOver->drawTips();
}