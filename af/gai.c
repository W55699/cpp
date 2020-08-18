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
	unsigned int LinkID;		//道路的LinkID
	unsigned int recordInF;		//道路的信息（包括番号、有无Flag，岔路数）
	long int skip;    			//该道路信息相对于文件开头的偏移量
	unsigned int size;
	unsigned int loadSize;
	char roadName[10];
	int bb;
}S_RoadRecord;

typedef struct egra{
	S_RoadRecord road[MAXSIZE];		//这个结果提存储了所有的道路信息
	int num,sorted;				//num表示道路总数，sorted=0表示未排序
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
	printf("\n读取文件...\n");
	while(!source_file){
		printf("读取失败\n\n");
		printf("请选择:\n     1.手动输入路径\n     0.返回主界面\n     \n请选择：");
		scanf("%d",&y);
		if(y==1){
			printf("\n请手动输入路径：");
			scanf("%s",&path);
			source_file=fopen("path","rb");
		}
			else {
                read_file_choice=0;
				break;
			}
	}
	printf("读取中...\n");
	int temp,tempid,temprec;
	char loadsize[2];
	if(read_file_choice==1){
		while(1){
			eGra->road[i].skip =temp_skip;//偏移量 tskip
			fread(temp_size,2,1,source_file);//两个字节移动到
			if(feof(source_file))
			break;
			temp=MC_GET_SHORT(temp_size);
			eGra->road[i].size =temp;
			temp_skip+=MC_GET_SHORT(temp_size);//当前偏移量

			fread(&tempid,4,1,source_file);//4个字节
			eGra->road[i].LinkID=MC_GET_LONG(&tempid);

			fread(loadsize,2,1,source_file);
		    eGra->road[i].loadSize=MC_GET_SHORT(loadsize);

		    fread(&temprec,4,1,source_file);
		    eGra->road[i].recordInF=MC_GET_LONG(&temprec);
		    fseek(source_file,temp_skip,0);
		    i++;
		}
	printf("文件读取成功!共有%d条记录\n这些记录已经转存到了result.txt\n",i);
	eGra->num=i;
	for(i=0;i<eGra->num;i++){
		fseek(source_file,eGra->road[i].skip,0);

		flag=(short)(eGra->road[i].recordInF>>7);
		branch_num=(short)((eGra->road[i].recordInF<<25)>>29);
		class_num=(short)((eGra->road[i].recordInF<<28)>>28);
		fprintf(file_write,"%d\t信息大小：%d\tLinkID:%d\t有无Flag:%d\t岔路数：%d\t交叉Link列表:",
		i+1,eGra->road[i].size,eGra->road[i].LinkID,flag,branch_num,class_num);
		if(eGra->road[i].size>12){
			fseek(source_file,eGra->road[i].skip+12,0);
			memset(roadname,0,MC_GET_SHORT(temp_size)-12);
			fread(roadname,MC_GET_SHORT(temp_size)-12,1,source_file);
			fread((eGra->road[i]).roadName,eGra->road[i].size-12,1,source_file);
			fprintf(file_write,"道路名称：%s\n",(eGra->road[i]).roadName+4);
			}
			else
			fprintf(file_write,"该道路无名称\n");
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

	printf("\n请选择排序方法:\n\t1.冒泡排序\n\t2.快速排序\n\t0返回主界面\n你的选择是：");
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
{   /* 简单选择排序 */
	int i, j, min, temp;

	for (i = 0; i < N - 1; i++) { /*寻找最小元素*/
		min = i;
		for (j = i+1; j < N; j++)
			if (A[j] < A[min])
				min = j;
			/* 将第i个元素与最小元素交换 */
			temp = A[i];
			A[i] = A[min];
			A[min] = temp;
	}
}

void Adjust( ElementType A[], int i, int N )
{	/* 对A[]中的前N个元素从第i个元素开始向下迁移调整 */
	int Child;
	ElementType temp;

	for( temp = A[i]; (2*i + 1) < N; i = Child){
		Child = (2*i + 1);  /* 左孩子节点 */
		if ( (Child != N-1) && A[Child + 1] > A[Child])
			Child++; /* Child指向左右子结点的较大者 */
		if( temp < A[Child] )
			A[i] = A[Child];  /* 移动Child元素到上层 */
		else	break;
	}
	A[i] = temp;    /*将temp放到当前位置　*/
}

void HeapSort( ElementType A[], int N )
{   /* 堆排序 */
	int i;
	ElementType temp;

    /* 建立最大堆*/
	for( i = (N-1)/2; i >= 0; i-- )  /*从有儿子的最后一个结点开始*/
		Adjust(A, i, N);
	for( i = N-1; i > 0; i-- ){
        /* 将堆顶元素A[0]与当前堆的最后一个元素A[i]换位 */
		temp = A[0]; A[0] = A[i]; A[i] = temp;
        /* 将有i个元素的新堆从根结点向下过滤调整 */
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
{                  /* 对A[Low]~A[Hight]进行快速排序 */
         ElementType Pivot = A[Low]; /* 简单取首元素为基准 */
         int Left = Low, Right = High;
        if ( Low >= High ) return;
         Swap ( &A[Low], &A[Right] ) ; /*将基准与最后一个元素交换*/
         while (1){    /*将序列中比基准小的移到基准左边，大的移到右边*/
	while ( (Low < High) && (Pivot  >=  A[Low]) )  Low++ ;
	while ( (Low < High) && (Pivot  <=  A[High]) )  High-- ;
                  if ( Low < High )     Swap ( &A[Low], &A[High] ) ;
	else  break;
         }
         Swap ( &A[Low], &A[Right] ) ; /* 将最后的基准换到正确的位置 */
                    /*分别对两个子集进行快排*/
         QSort ( A, Left, Low - 1 ) ;
         QSort ( A, Low + 1, Right ) ;
}
void QuickSort( ElementType A[ ], int N )
{      /* 快速排序 */
       QSort( A, 0, N-1 );
}


void paixu()
{	int n;
	printf("请输入要排序的个数n： ");
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


//冒泡排序
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
	printf("	请选择检索的方式：\n");
	printf("		1.指定;LinkID 检索\n");
	printf("		2.指定交叉link列表示岔路数 检索\n");
	printf("		3.指定查找岔路数 检索\n");
	printf("		4.指定道路名称 检索\n");
	printf("		0.返回\n");
	printf("	请选择：");
	scanf("%d,&b");
	printf("\n");
}



int main(void)
{
	int x=1,flag12=0;
	eGra.num=0;
	eGra.sorted=0;
	printf("/**********************---Welcome!---**********************/\n");
	printf("/***	电子地图信息统计系统				***/\n");
	printf("/***				李俊伟	2019年6月25日	***/\n");
	printf("/**********************************************************/\n");
	printf("\n");
	while(x)
	{
		printf("\n请选择服务的种类:\n");
		printf("     1.读取文件(“./data/GTBL.dat”)\n");
		printf("     2.排序(快速排序)并输出排序结果(“./data/SortGTBL.dat”)\n");
		printf("     3.检索\n");
		printf("     4.更新\n");
		printf("     0.退出\n");
		printf("     请选择：");
		scanf("%d",&x);
		switch(x)
		{
		case 1:
		Readfile(&eGra);
		break;
		case 2:
		printf("\n请选择排序方法:\n\t1.快速排序\n\t2.冒泡排序\n\t0返回主界面\n你的选择是：");
//		scanf("%d",&sortChoice);
		paixu();
		break;
		case 3:
			jiansuo();
		printf("  \n");
		break;
		case 4:
		printf("\n更新错误\n\n");
		break;
		case 0:
        break;
		default:
		printf("\n输入有误，请重新输入\n\n");
		break;
	   }
	}
	return 0;
 }
