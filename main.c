/************************************************************************
 * 
 * pow(2,10)<1024*2<pow(2,11)
 * 编码的二进制长度为22 
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "math.h"

#define N 200                   // 分解取值范围，精确到小数点后4位
#define MAX(a, b)   ((a)>(b)?(a):(b))
#define MIN(a, b)   ((a)<(b)?(a):(b))

#define SIZE        30          // population
#define MAXGEN      1500        //
#define SEED        10593       // initial GNG seed 
#define P_CORSS     0.75
#define P_MUTATION  0.05

#define LEN 8           // 数据位数,也就是基因位数
#define randd()         ((double)rand() /RAND_MAX)
#define randi(k)        ((int)(randd() *k +0.5))

/******************************************************************
 * 种群个体的结构体 
 ******************************************************************/
typedef struct node
{
    char    x[LEN];         //
    long    fitness;        //
    long    fitsum;         // 
}node;

node cur[SIZE], next[SIZE], max, min;

int gen_cnt;                // 迭代次数

/****************************************************************************
 * Calculate fitness of 
 ****************************************************************************/
void cal_fitness()
{
    int     i, j;
    int     k;
    long    d;
    
    for ( i=0,d=0; i<SIZE; i++ ) {
        k = 0;
        for ( j=LEN-1; j>=0; j-- ) {   
            k = (k<<1)+cur[i].x[j];
        }
        k = (k<100?(100-k):(k-100));
        cur[i].fitness = k*k;
        cur[i].fitsum = i>0?(cur[i].fitness+cur[i-1].fitsum):(cur[0].fitness);
    }
}

/*****************************************************************************
 * Population initial
 *****************************************************************************/
void population_init()
{
    int tmp;
    int i;
    int j;
  
    for ( i=0; i<SIZE; i++ ) {
        tmp = randi(N);
        for ( j=0; j<LEN; j++ ) {
            cur[i].x[j] = tmp%2;
            tmp = tmp>>1;
        }
    }
    cal_fitness();
}

int sel()
{
    double p = randd();
    double sum = cur[SIZE-1].fitsum;
    int i;
    
    for ( i=0; i<SIZE; i++ ) {
        if( cur[i].fitsum/sum>p ) {
            return i;
        } 
    }
}

/**********************************************************************
 * 迭代
 **********************************************************************/
void tran()
{
    int i, j, pos;
    int k;
    //找当前种群最优个体 
    min = cur[0];
    for ( i=1; i<SIZE-1; i++ ) {
        if ( cur[i].fitness<min.fitness ) {
            min = cur[i];
        }
    }
    for ( k=0; k<SIZE; k+=2 ) {
        // 选择交叉个体 
        i = sel();
        j = sel();
        
        // 选择交叉位置 
        pos = randi(LEN-1);
        
        //交叉
        if ( randd()<P_CORSS ) {
            memcpy(next[k].x, cur[i].x, pos);
            memcpy(next[k].x+pos, cur[j].x+pos, LEN-pos);
            memcpy(next[k+1].x,cur[j].x,pos);
            memcpy(next[k+1].x+pos,cur[i].x+pos,LEN-pos);
        } else {
            memcpy(next[k].x,cur[i].x,LEN);
            memcpy(next[k+1].x,cur[j].x,LEN);
        }
        //变异
        if ( randd()<P_MUTATION ) {
            pos = randi(LEN-1);
            next[k].x[pos] ^= next[k].x[pos];
         
            pos = randi(LEN-1);
            next[k+1].x[pos] ^= next[k+1].x[pos];
        }
    }
    //找下一代的最差个体 
    max = next[0];
    j = 0;
    
    for ( i=1; i<SIZE-1; i++ ) {
        if ( next[i].fitness<min.fitness ) {
            max = next[i];
            j = i;
        }
    }
    //用上一代的最优个体替换下一代的最差个体
    next[j] = min;
    
    memcpy(cur, next, sizeof(cur));
    
    cal_fitness();
}

//打印个体适应度和二进制编码 
void print(node tmp)
{
    int i;
    printf("%8ld:", tmp.fitness);
    
    for ( i=0; i<LEN; i++ ) {
        printf("%2d",tmp.x[i]);
    }
    printf("\n");
}

//打印种群
void printcur()
{
    int i;
    for ( i=0; i<SIZE; i++ ) { 
        print(cur[i]);
    }
}


void GA()
{
    int     cnt=0;
    long    k;
    long    ans;
    long    val;
    int     i, j;
    
    while ( cnt++<MAXGEN ) {
        tran(); 
//    printf("%.6lf\n",max.fitness);
//    printcur();
    }
    ans = cur[0].fitness;
    cnt = 0;
#if 0
    for( i=1; i<SIZE; i++ ) {
        ans = MIN(ans, cur[i].fitness);
    }
    printf("%.6lf\n", ans);
#else
    for( i=1; i<SIZE; i++ ) {
        if ( cur[i].fitness>ans ) {
            ans = cur[i].fitness;
            cnt = i;
        }
    }
    k = 0;
    for ( j=LEN-1; j>=0; j--) {   
        k = (k<<1)+cur[cnt].x[j];
    }
    k -= 100;
    printf("%8ld, %8ld\n", k, ans);
#endif
}

int main()
{
    //srand((unsigned)time(NULL));
    srand(SEED);
    
    population_init();
    GA();
    
    system("pause");
    return 0;
}
