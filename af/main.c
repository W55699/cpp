#include<stdio.h>
#include <string.h>
#define MAXSIZE 65535
typedef char           INT;
typedef unsigned char  UCHAR;
typedef unsigned short UINT;
typedef unsigned long  ULONG;
typedef int ElementType;

int Sizecount=2;
#define MC_GET_CHAR(__data__)   (*((char *)(__data__)))
#define MC_GET_SHORT(__data__)  ((UINT)((((UINT)(*((char *)(__data__))))<<8)|(((UINT)(*((char *)(__data__)+1)))&0x00ff)))
#define MC_GET_LONG(__data__)   ((((ULONG)MC_GET_SHORT((__data__)))<<16 )|(((ULONG)MC_GET_SHORT((char *)(__data__)+2))&0x0000ffff))
#define MC_GET_3BN(__data__)    ((((ULONG)MC_GET_CHAR((__data__)))<<16)|(((ULONG)MC_GET_SHORT((char *)(__data__)+1))&0x0000ffff))
#define MCGETCHAR(data)  (*((char *)(data)))
#define MCGETSHORT(data)  ((unsigned short)(( ((unsigned short)(*((char *)(data))))<<8 )|( ((unsigned short)(*((char *)(data)+1)))&0x00ff )))
#define MCGETLONG(data)  ( ( ((unsigned long)MCGETSHORT((data)))<<16 )|( ((unsigned long)MCGETSHORT((char *)(data)+2))&0x0000ffff ) )
#define MCGET3BN(data)  ( ( ((unsigned long)MCGETCHAR((data)))<<16 )|( ((unsigned long)MCGETSHORT((char *)(data)+1))&0x0000ffff ) )
typedef struct S_RoadRecord{
	unsigned int LinkID;		//��·��LinkID
	unsigned int recordInF;		//��·����Ϣ���������š�����Flag����·����
	long int skip;    			//�õ�·��Ϣ������ļ���ͷ��ƫ����
	unsigned int size;
	unsigned int loadSize;
	char roadName[10];
	int bb;
}S_RoadRecord;
struct RoadRecord
{
	short ussize;
	long ulLinkID;
	short usRoadnamesize;
	int usDispclass;
	int usBrunch;
	int usRoadnameflag;
	char Roadname[20];
};
struct RoadRecord road[MAXSIZE];

typedef struct egra{
	S_RoadRecord road[MAXSIZE];		//��������洢�����еĵ�·��Ϣ
	int num,sorted;				//num��ʾ��·������sorted=0��ʾδ����
}egra;

egra eGra;


struct dataout
{
	long LinkID;
	int Dispclass;
	int Brunch;
	int Roadnameflag;
	char Roadname[20];
};
struct dataout data[MAXSIZE];

struct Datastore
{
	int n;
	char Totalsize[4];
	char LinkID_1[5];
	char Roadnamesize[2];
	char Road_flag[2];
	char Roadname[20];
};
static struct Datastore Ds[MAXSIZE];
void Readfile(egra *eGra)
{   int m;
	int y,i=0,read_file_choice=1;
	char path[60],*roadname;
	unsigned short ustotalsize;
	unsigned long ulLinkID;
	unsigned short usRoadnamesize;
	FILE *source_file,*file_write,*file_write1;
	long int temp_skip=0;
	char temp_size[2];
	short branch_num,class_num,flag;
	source_file=fopen("GTBL.dat","rb+");
	file_write=fopen("result.txt","w");
	printf("\n��ȡ�ļ�...\n");
	while(!source_file){
		printf("��ȡʧ��\n\n");
		printf("��ѡ��:\n     1.�ֶ�����·��\n     0.����������\n     \n��ѡ��");
		scanf("%d",&y);
		if(y==1){
			printf("\n���ֶ�����·����");
			scanf("%s",&path);
			source_file=fopen("path","rb");
		}
			else {
                read_file_choice=0;
				break;
			}
	}
	printf("��ȡ��...\n");
	int temp,tempid,temprec;
	char loadsize[2];
	if(read_file_choice==1){
		while(1){
			eGra->road[i].skip =temp_skip;//ƫ���� tskip
			fread(temp_size,2,1,source_file);//�����ֽ��ƶ���
			if(feof(source_file))
			break;
			temp=MC_GET_SHORT(temp_size);
			eGra->road[i].size =temp;
			temp_skip+=MC_GET_SHORT(temp_size);//��ǰƫ����

			fread(&tempid,4,1,source_file);//4���ֽ�
			eGra->road[i].LinkID=MC_GET_LONG(&tempid);

			fread(loadsize,2,1,source_file);
		    eGra->road[i].loadSize=MC_GET_SHORT(loadsize);

		    fread(&temprec,4,1,source_file);
		    eGra->road[i].recordInF=MC_GET_LONG(&temprec);
		    fseek(source_file,temp_skip,0);
		    i++;
		}

	printf("�ļ���ȡ�ɹ�!����%d����¼\n��Щ��¼�Ѿ�ת�浽��result.txt\n",i);
	eGra->num=i;






	for(i=0;i<eGra->num;i++)
	{
		fseek(source_file,eGra->road[i].skip,0);

		flag=(short)(eGra->road[i].recordInF>>7);
		branch_num=(short)((eGra->road[i].recordInF<<25)>>29);
		class_num=(short)((eGra->road[i].recordInF<<28)>>28);
		fprintf(file_write,"LinkID:%d\t����Flag:%d\t��·����%d\t����Link�б�:",
		i+1,eGra->road[i].LinkID,flag,branch_num,class_num);
		if(eGra->road[i].size>12){
			fseek(source_file,eGra->road[i].skip+12,0);
			memset(roadname,0,MC_GET_SHORT(temp_size)-12);
			fread(roadname,MC_GET_SHORT(temp_size)-12,1,source_file);
			fread((eGra->road[i]).roadName,eGra->road[i].size-12,1,source_file);
			fprintf(file_write,"��·���ƣ�%s\n",(eGra->road[i]).roadName+4);
			}
			else
			fprintf(file_write,"�õ�·������\n");
			(eGra->road[i]).roadName[eGra->road[i].size-12]='\0';
	}





	fclose(file_write);
	fclose(source_file);
	}
}
int LoadFile()
{
	int m;
	char filename[50];
	unsigned short ustotalsize;
	unsigned long ulLinkID;
	unsigned short usRoadnamesize;

	FILE*fp_1=fopen("GTBL.dat","rb+");
	FILE*fp_2=fopen("��ȡ.txt","w+");
	if(fp_1==NULL)
	{
		printf("\n\t�޷����ļ���GTBL.dat��\n\n���ֶ������ļ�·��:");
		scanf("%s",filename);
		if((fp_1=fopen(filename,"rb+"))==NULL)
		{
			printf("\n\t�޷����ļ���GTBL.dat��\n\n");
			return 0;
		}
		else
		{
			printf("\n\t�ļ��򿪳ɹ�!\n\n");
		}
	}
	if(fp_2==NULL)
	{
		printf("\n\t�޷����ļ� ��explain.txt��\n\n");
		return 0;
	}
	printf("�ļ���ȡ��......\n");

	while(fread(Ds[Sizecount].Totalsize,2,1,fp_1)==1)
	{
		fread(Ds[Sizecount].LinkID_1,4,1,fp_1);
		fread(Ds[Sizecount].Roadnamesize,2,1,fp_1);
		fread(Ds[Sizecount].Road_flag,4,1,fp_1);
		ustotalsize=MCGETSHORT(Ds[Sizecount].Totalsize);
		ulLinkID=MCGETLONG(Ds[Sizecount].LinkID_1);
		usRoadnamesize=MCGETSHORT(Ds[Sizecount].Roadnamesize);
		Ds[Sizecount].n=ustotalsize-12;
		road[Sizecount].ussize=ustotalsize;
		road[Sizecount].ulLinkID=ulLinkID;
		road[Sizecount].usRoadnamesize=usRoadnamesize;

		m=(int)Ds[Sizecount].Road_flag[3]&255;

		road[Sizecount].usDispclass=m&15;
		road[Sizecount].usBrunch=(m&112)/16;
		road[Sizecount].usRoadnameflag=(m&128)/128;
		data[Sizecount].Dispclass=road[Sizecount].usDispclass;
		data[Sizecount].Brunch=road[Sizecount].usBrunch;
		data[Sizecount].Roadnameflag=road[Sizecount].usRoadnameflag;
		data[Sizecount].LinkID=ulLinkID;

		fread(Ds[Sizecount].Roadname,sizeof(char),ustotalsize-12,fp_1);
		strcpy(data[Sizecount].Roadname,Ds[Sizecount].Roadname+4);

		fprintf(fp_2,"LinkID=");
		fprintf(fp_2,"%d\t",data[Sizecount].LinkID);
		fprintf(fp_2,"Flag=");
		fprintf(fp_2,"%d\t",data[Sizecount].Roadnameflag);
		fprintf(fp_2,"Brunch=");
		fprintf(fp_2,"%d\t",data[Sizecount].Brunch);
		fprintf(fp_2,"Dispclass=");
		fprintf(fp_2,"%d\t",data[Sizecount].Dispclass);
		fprintf(fp_2,"Roadname=");
		fprintf(fp_2,"%s\t",data[Sizecount].Roadname);
		fprintf(fp_2,"\n");

	Sizecount++;

	}
	fclose(fp_1);
	fclose(fp_2);

	return 1;
}



void Sort_Modify(egra *eGra,int flag12)
{
//	clock_t	start,stop;
	double duration;

	int n = eGra->num;
	int sortChoice;
	int doChoice = 0;

	printf("\n��ѡ�����򷽷�:\n\t1.ð������\n\t2.��������\n\t0����������\n���ѡ���ǣ�");
	scanf("%d",&sortChoice);

	while(1)
	{
		if(sortChoice == 0)
			break;
		if(flag12 == 1)
			break;
	}
}



void SimpleSelectionSort( ElementType A[], int N )
{   /* ��ѡ������ */
	int i, j, min, temp;

	for (i = 0; i < N - 1; i++) { /*Ѱ����СԪ��*/
		min = i;
		for (j = i+1; j < N; j++)
			if (A[j] < A[min])
				min = j;
			/* ����i��Ԫ������СԪ�ؽ��� */
			temp = A[i];
			A[i] = A[min];
			A[min] = temp;
	}
}

void Adjust( ElementType A[], int i, int N )
{	/* ��A[]�е�ǰN��Ԫ�شӵ�i��Ԫ�ؿ�ʼ����Ǩ�Ƶ��� */
	int Child;
	ElementType temp;

	for( temp = A[i]; (2*i + 1) < N; i = Child){
		Child = (2*i + 1);  /* ���ӽڵ� */
		if ( (Child != N-1) && A[Child + 1] > A[Child])
			Child++; /* Childָ�������ӽ��Ľϴ��� */
		if( temp < A[Child] )
			A[i] = A[Child];  /* �ƶ�ChildԪ�ص��ϲ� */
		else	break;
	}
	A[i] = temp;    /*��temp�ŵ���ǰλ�á�*/
}

void HeapSort( ElementType A[], int N )
{   /* ������ */
	int i;
	ElementType temp;

    /* ��������*/
	for( i = (N-1)/2; i >= 0; i-- )  /*���ж��ӵ����һ����㿪ʼ*/
		Adjust(A, i, N);
	for( i = N-1; i > 0; i-- ){
        /* ���Ѷ�Ԫ��A[0]�뵱ǰ�ѵ����һ��Ԫ��A[i]��λ */
		temp = A[0]; A[0] = A[i]; A[i] = temp;
        /* ����i��Ԫ�ص��¶ѴӸ�������¹��˵��� */
		Adjust(A, 0, i);
	}
}

void Swap(ElementType *a,  ElementType *b )
{
	ElementType t =*a;  *a = *b; *b = t;

}

void Swap2(int *a, int *b)
{

	int temp;
	temp = *a;
	*a = *b;
	*b =  temp;

}

void QSort ( ElementType A[ ],  int Low,  int High )
{                  /* ��A[Low]~A[Hight]���п������� */
         ElementType Pivot = A[Low]; /* ��ȡ��Ԫ��Ϊ��׼ */
         int Left = Low, Right = High;
        if ( Low >= High ) return;
         Swap ( &A[Low], &A[Right] ) ; /*����׼�����һ��Ԫ�ؽ���*/
         while (1){    /*�������бȻ�׼С���Ƶ���׼��ߣ�����Ƶ��ұ�*/
	while ( (Low < High) && (Pivot  >=  A[Low]) )  Low++ ;
	while ( (Low < High) && (Pivot  <=  A[High]) )  High-- ;
                  if ( Low < High )     Swap ( &A[Low], &A[High] ) ;
	else  break;
         }
         Swap ( &A[Low], &A[Right] ) ; /* �����Ļ�׼������ȷ��λ�� */
                    /*�ֱ�������Ӽ����п���*/
         QSort ( A, Left, Low - 1 ) ;
         QSort ( A, Low + 1, Right ) ;
}
void QuickSort( ElementType A[ ], int N )
{      /* �������� */
       QSort( A, 0, N-1 );
}


void paixu(int l,int r)
{

	int n=data[l].LinkID,i=l,j=r;
	data[MAXSIZE-2]=data[l];
	Ds[MAXSIZE-2]=Ds[l];
	if(l>=r)
	{
		return ;
	}
	while(i<j)
	{
		while(i<j&&data[j].LinkID>=n)
		{
			--j;
		}
		{
			data[i]=data[j];
			Ds[i]=Ds[j];
		}
		while(i<j&&data[i].LinkID<=n)
		{
			++i;
		}
		data[j]=data[i];
		Ds[j]=Ds[i];
	}
	data[i]=data[MAXSIZE-2];
	Ds[i]=Ds[MAXSIZE-2];
	paixu(l,i-1);
	paixu(i+1,r);



}


//ð������
void Bubble(egra *eGra)
{
	int n = eGra->num;
	S_RoadRecord temp_load;

	int i,j;
	int flagBubble;
	for(i=n-1;i>0;i--)
	{
		flagBubble = 0;

		for(j=0;j<i;j++)
		{
			if(eGra->road[j].LinkID>eGra->road[j+1].LinkID)
			{
				temp_load=eGra->road[j];
				eGra->road[j]=eGra->road[j+1];
				eGra->road[j+1]=temp_load;
				flagBubble = 1;
			}
		}
		if(!flagBubble)
		break;
	}
 }




int Binsearch(int s,int t,int key)
{
	int low=s,high=t,mid;
	if(s<=t)
	{
		mid=low+(high-low)/2;
		if(data[mid].LinkID==key)
		{
			return mid;
		}
		else if(data[mid].LinkID>key)
		{
			return Binsearch(low,mid-1,key);
		}
		else{
			return Binsearch(mid+1,high,key);
		}
	}
	return -1;
}

int jiansuo()
{
	int sel,i,flag=0,n,key,mid,s=2,t=Sizecount;
	char Roadname[20];
	FILE*p,*p1,*p2;
	printf("\n�����������ʽ��\n\t1: ָ�� LinkID ����\n\t2: ָ����� Link �б�ʾ Class ���ż���\n\t3: ָ�����Ҳ�·������\n\t4: ָ����·���Ƽ���\n\t0: ����\n��ѡ��");
	while((scanf("%d",&sel))!=1)
	{
		fflush(stdin);
		printf("\n\t����������������룡\n");
		printf("\n�����������ʽ��\n\t1:ָ��LinkID����\n\t2:ָ������ Link�б�ʾClass���ż���\n\t3:ָ�����Ҳ�·������\n\t4:ָ����·���Ƽ���\n\t0:����\n��ѡ��");
	}
	switch(sel)
	{
		case 1:
		{
			printf("������LinkID:");
			scanf("%d",&n);
			key=n;
			mid=Binsearch(s,t,key);
			if(mid!=-1)
			{
				printf("\nLinkID=%d\t Roadnameflag=%d\t Brunh=%d\t Dispclass=%d\t Roadname=%s\n\n",data[mid].LinkID,data[mid].Roadnameflag,data[mid].Brunch,data[mid].Dispclass,data[mid].Roadname);
				flag++;
			}
			if(flag==0)
			{
				printf("\n\tû��ƥ������������ѡ�������ʽ��\n\n");
				jiansuo();
			}
			return 0;
		}
			break;
		case 2:
		{
			printf("�����뽻��Link�б�ʾClass���ţ�");
			scanf("%d",&n);
			for(i=2;i<=Sizecount;i++)
			{
				if(data[i].Dispclass==n)
				{
					flag++;
				}
			}
			if(flag==0)
			{
				printf("\n\tû��Class����Ϊ%d�ĵ�·\n\n",n);
				return 0;
			}
			if(flag<=5)
			{
				printf("\n\t����������Ϣ����5������Ϣ����ʾ����Ļ�ϣ�\n");
				for(i=2;i<=Sizecount;i++)
				{
					if(data[i].Dispclass==n)
					{
						printf("\nLinkID=%d\tRoadnameflag=%d\tBrunh=%d\tDispclass=%d\tRoadname=%s\n\n",data[mid].LinkID,data[mid].Roadnameflag,data[mid].Brunch,data[mid].Dispclass,data[mid].Roadname);
					}
				}
				return 0;
			}
			else
			{
				printf("\n\t�������5����ת�浽D��\\Class���ż���.TXT\n\n");
				p=fopen("Class���ż���.TXT","w+");
				for(i=2;i<=Sizecount;i++)
				{
					if(data[i].Dispclass==n)
					{
							fprintf(p,"\nLinkID=%d\tRoadnameflag=%d\tBrunh=%d\tDispclass=%d\tRoadname=%s\n\n",data[i].LinkID,data[i].Roadnameflag,data[i].Brunch,data[i].Dispclass,data[i].Roadname);
					}
				}
				fclose(p);
				return 0;
			}
		}
			break;
		case 3:
			{
				printf("�������·����");
			scanf("%d",&n);
			for(i=2;i<=Sizecount;i++)
			{
				if(data[i].Brunch==n)
				{
					flag++;
				}
			}
			if(flag==0)
			{
				printf("\n\tû�в�·��Ϊ%d�ĵ�·\n\n",n);
				return 0;
			}
			else if(flag<=5)
			{
				printf("\n\t����������Ϣ����5������Ϣ����ʾ����Ļ�ϣ�\n");
				for(i=2;i<=Sizecount;i++)
				{
					if(data[i].Brunch==n)
					{
						printf("\nLinkID=%d\tRoadnameflag=%d\tBrunh=%d\tDispclass=%d\tRoadname=%s\n\n",data[mid].LinkID,data[mid].Roadnameflag,data[mid].Brunch,data[mid].Dispclass,data[mid].Roadname);
					}
				}
				return 0;
			}
			else{
				printf("\n\t�������5����ת�浽D��\\��·�ڼ���.TXT\n\n");
				p1=fopen("��·�ڼ���.TXT","w+");
				for(i=2;i<=Sizecount;i++)
				{
					if(data[i].Brunch==n)
					{
							fprintf(p1,"\nLinkID=%d\tRoadnameflag=%d\tBrunh=%d\tDispclass=%d\tRoadname=%s\n\n",data[i].LinkID,data[i].Roadnameflag,data[i].Brunch,data[i].Dispclass,data[i].Roadname);
					}
				}
				fclose(p1);
				return 0;
			}
		}
			break;
			case 4:
			{
					printf("�������·���ƣ�");
			scanf("%s",&Roadname);
			for(i=2;i<=Sizecount;i++)
			{
				if(strcmp(data[i].Roadname,Roadname)==0)
				{
					flag++;
				}
			}
			if(flag==0)
			{
				printf("\n\tû�е�·����Ϊ��%s���ĵ�·\n\n",Roadname);
				return 0;
			}
			else if(flag<=5)
			{
				printf("\n\t����������Ϣ����5������Ϣ����ʾ����Ļ�ϣ�\n");
				for(i=2;i<=Sizecount;i++)
				{
					if(strcmp(data[i].Roadname,Roadname)==0)
					{
						printf("\nLinkID=%d\tRoadnameflag=%d\tBrunh=%d\tDispclass=%d\tRoadname=%s\n\n",data[mid].LinkID,data[mid].Roadnameflag,data[mid].Brunch,data[mid].Dispclass,data[mid].Roadname);
					}
				}
				return 0;
			}
			else
			{
				printf("\n\t�������5����ת�浽D��\\��·���Ƽ���.TXT\n\n");
				p2=fopen("��·���Ƽ���.txt","w+");
				for(i=2;i<=Sizecount;i++)
				{
						if(strcmp(data[i].Roadname,Roadname)==0)
					{
							fprintf(p2,"\nLinkID=%d\tRoadnameflag=%d\tBrunh=%d\tDispclass=%d\tRoadname=%s\n\n",data[i].LinkID,data[i].Roadnameflag,data[i].Brunch,data[i].Dispclass,data[i].Roadname);
					}
				}
				fclose(p2);
				return 0;
			}
		}
			break;
			case 0:
			{
				return 0;
			}
			break;
			default:
			{
				printf("\n\t������Ϣ����������ѡ�������ʽ��\n\n");
				jiansuo();
				return 0;
			}
			}
			}


int main(void)
{
	int x=1,flag12=0;
    FILE *p1,*p2;
    int i;
	eGra.num=0;
	eGra.sorted=0;
	printf("/**********************---Welcome!---**********************/\n");
	printf("/***	���ӵ�ͼ��Ϣͳ��ϵͳ				***/\n");
	printf("/***				�����	2020��8��10��	***/\n");
	printf("/**********************************************************/\n");
	printf("\n");
	while(x)
	{
		printf("\n��ѡ����������:\n");
		printf("     1.��ȡ�ļ�(��./data/GTBL.dat��)\n");
		printf("     2.����(��������)�����������(��./data/sort.dat��)\n");
		printf("     3.����\n");
		printf("     4.����\n");
		printf("     0.�˳�\n");
		printf("     ��ѡ��");
		scanf("%d",&x);
		switch(x)
		{
		case 1:
		Readfile(&eGra);
		LoadFile();
		printf("%d",data[2].LinkID);
		break;
		case 2:
		if(data[2].LinkID!=1)
					{
						paixu(2,Sizecount-1);
						printf("����ɹ�");
						p1=fopen("GTBL.dat","wb+");
						p2=fopen("sort.txt","w+");
						for(i=2;i<Sizecount;i++)
						{
							fprintf(p2,"LinkID=%d\t",data[i].LinkID);
							fprintf(p2,"\tBrunch=%d\t",data[i].Brunch);
							fprintf(p2,"\tDispclass=%d\t",data[i].Dispclass);
							fprintf(p2,"\tRoadnameflag=%d\t",data[i].Roadnameflag);
							fprintf(p2,"\tRoadname=1=%s\t",data[i].Roadname);
							fprintf(p2,"\n");

							fwrite(Ds[i].Totalsize,2,1,p1);
							fwrite(Ds[i].LinkID_1,4,1,p1);
							fwrite(Ds[i].Roadnamesize,2,1,p1);
							fwrite(Ds[i].Road_flag,4,1,p1);
							fwrite(Ds[i].Roadname,sizeof(char),Ds[i].n,p1);
						}
						fclose(p1);
						fclose(p2);

				}
					else
					{
						printf("\n\t��������ɣ�");
					}
		break;
		case 3:
		jiansuo();
		printf("  \n");
		break;
		case 4:
		printf("\n���´���\n\n");
		break;
		case 0:
        break;
		default:
		printf("\n������������������\n\n");
		break;
	   }
	}
	return 0;
 }
