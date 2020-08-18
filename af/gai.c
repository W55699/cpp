#include<stdio.h>
#include <string.h>
#define MAXSIZE 65535
typedef char           INT;
typedef unsigned char  UCHAR;
typedef unsigned short UINT;
typedef unsigned long  ULONG;
typedef int ElementType;
#define MC_GET_CHAR(__data__)   (*((char *)(__data__)))
#define MC_GET_SHORT(__data__)  ((UINT)((((UINT)(*((char *)(__data__))))<<8)|(((UINT)(*((char *)(__data__)+1)))&0x00ff)))
#define MC_GET_LONG(__data__)   ((((ULONG)MC_GET_SHORT((__data__)))<<16 )|(((ULONG)MC_GET_SHORT((char *)(__data__)+2))&0x0000ffff))
#define MC_GET_3BN(__data__)    ((((ULONG)MC_GET_CHAR((__data__)))<<16)|(((ULONG)MC_GET_SHORT((char *)(__data__)+1))&0x0000ffff))
typedef struct S_RoadRecord{
	unsigned int LinkID;		//��·��LinkID
	unsigned int recordInF;		//��·����Ϣ���������š�����Flag����·����
	long int skip;    			//�õ�·��Ϣ������ļ���ͷ��ƫ����
	unsigned int size;
	unsigned int loadSize;
	char roadName[10];
	int bb;
}S_RoadRecord;

typedef struct egra{
	S_RoadRecord road[MAXSIZE];		//��������洢�����еĵ�·��Ϣ
	int num,sorted;				//num��ʾ��·������sorted=0��ʾδ����
}egra;

egra eGra;


void Readfile(egra *eGra)
{
	int y,i=0,read_file_choice=1;
	char path[60],*roadname;
	FILE *source_file,*file_write;
	long int temp_skip=0;
	char temp_size[2];
	short branch_num,class_num,flag;
	source_file=fopen("GTBL.dat","rb");
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
	for(i=0;i<eGra->num;i++){
		fseek(source_file,eGra->road[i].skip,0);

		flag=(short)(eGra->road[i].recordInF>>7);
		branch_num=(short)((eGra->road[i].recordInF<<25)>>29);
		class_num=(short)((eGra->road[i].recordInF<<28)>>28);
		fprintf(file_write,"%d\t��Ϣ��С��%d\tLinkID:%d\t����Flag:%d\t��·����%d\t����Link�б�:",
		i+1,eGra->road[i].size,eGra->road[i].LinkID,flag,branch_num,class_num);
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


void paixu()
{	int n;
	printf("������Ҫ����ĸ���n�� ");
	scanf("%d",&n) ;

	int a[n];
	int i,j;
	for(i=0;i<n;i++)
	{
		scanf("%d",&a[i]);
	}
	//SimpleSelectionSort( a, 10 );
	//HeapSort( a, 10 );

	 QuickSort( a,n);

	for(i=0;i<n;i++)
	{
		printf("%d ",a[i]);
	}
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




void jiansuo()
{
	printf("\n");
	printf("	��ѡ������ķ�ʽ��\n");
	printf("		1.ָ��;LinkID ����\n");
	printf("		2.ָ������link�б�ʾ��·�� ����\n");
	printf("		3.ָ�����Ҳ�·�� ����\n");
	printf("		4.ָ����·���� ����\n");
	printf("		0.����\n");
	printf("	��ѡ��");
	scanf("%d,&b");
	printf("\n");
}



int main(void)
{
	int x=1,flag12=0;
	eGra.num=0;
	eGra.sorted=0;
	printf("/**********************---Welcome!---**********************/\n");
	printf("/***	���ӵ�ͼ��Ϣͳ��ϵͳ				***/\n");
	printf("/***				�ΰ	2019��6��25��	***/\n");
	printf("/**********************************************************/\n");
	printf("\n");
	while(x)
	{
		printf("\n��ѡ����������:\n");
		printf("     1.��ȡ�ļ�(��./data/GTBL.dat��)\n");
		printf("     2.����(��������)�����������(��./data/SortGTBL.dat��)\n");
		printf("     3.����\n");
		printf("     4.����\n");
		printf("     0.�˳�\n");
		printf("     ��ѡ��");
		scanf("%d",&x);
		switch(x)
		{
		case 1:
		Readfile(&eGra);
		break;
		case 2:
		printf("\n��ѡ�����򷽷�:\n\t1.��������\n\t2.ð������\n\t0����������\n���ѡ���ǣ�");
//		scanf("%d",&sortChoice);
		paixu();
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
