/***
*qsort.c - quicksort algorithm; qsort() library function for sorting arrays
*
*   Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*   To implement the qsort() routine for sorting arrays.
*
*******************************************************************************/
 
#include <stdlib.h>
#include <search.h>
#include <internal.h>
 
/* 一下为给定的两组函数,个人感觉比较像prinfs和print的区别 */
#ifdef __USE_CONTEXT
static void __fileDECL shortsort_s(char *lo, char *hi, size_t width,
    int (__fileDECL *comp)(void *, const void *, const void *), void *);
#define swap swap_c
#else  /* __USE_CONTEXT */
static void __fileDECL shortsort(char *lo, char *hi, size_t width,
    int (__fileDECL *comp)(const void *, const void *));
#endif  /* __USE_CONTEXT */
/*交换*/
static void __fileDECL swap(char *p, char *q, size_t width);
 
/* 定义宏CUTOFF来判断是否使用插入排序还是快速排序小于等于CUTOFF的时候使用
插入排序否则使用快速排序不过貌似没给出确切的解释*/
 
#define CUTOFF 8         
/*栈的大小(也就是宏STKSIZ)进行说明对于栈的理论大小是不大于 1 + log2(num)
当由于使用CUTOFF这样的宏来判断是否使用插入排序,因此仅仅需要1 + log2(num) - log2(CUTOFF)
的栈空间*/
 
#define STKSIZ (8*sizeof(void*) - 2)
/*qsort(base, num, wid, comp) - 对于待排序数组进行快速排序的函数
*
*Purpose:
*对数组元素进行快速排序
*   内部排序
*   可排序的数组大小被限制在进程在虚内存下可允许使用的地址下
*
*Entry :
*char *base = 数组指针(不过根据定义而言这里应该是void *base)
*   size_t num = 数组元素数目
*   size_t width = 定义的数组元素大小
*   int(*comp)() = 定义的函数指针用于定义两个输入串的大小,
*                  用于比较数组中元素的大小	
*                  int compar (const void* p1, const void* p2);
*                  返回对p1 p2元素的比较结果确定p1 p2的比较方法 具体的定义可以看是
*                  选择顺序排序还是逆序排序 通常而言顺序的话大于返回>0小于返回<0等于返回0
*
*Exit:
*returns void
*******************************************************************************/
 
/*又是宏定义,这里的USE_CONTEXT宏没有找到定义*/
 
#ifdef __USE_CONTEXT
#define __COMPARE(context, p1, p2) comp(context, p1, p2)
#define __SHORTSORT(lo, hi, width, comp, context) shortsort_s(lo, hi, width, comp, context);
#else  /* __USE_CONTEXT */
#define __COMPARE(context, p1, p2) comp(p1, p2)
#define __SHORTSORT(lo, hi, width, comp, context) shortsort(lo, hi, width, comp);
#endif  /* __USE_CONTEXT */
 
SECURITYSAFECRITICAL_ATTRIBUTE
#ifdef __USE_CONTEXT
void __fileDECL qsort_s(
void *base,
size_t num,
size_t width,
int (__fileDECL *comp)(void *, const void *, const void *),
void *context
)
#else  /* __USE_CONTEXT */
void __fileDECL qsort(
void *base,
size_t num,
size_t width,
int (__fileDECL *comp)(const void *, const void *)
)
#endif  /* __USE_CONTEXT */
{
    char *lo, *hi;              /* 当前正在排序的数组(sub-array)的边界指针(high和low) */
    char *mid;                  /* 指向sub-array中部的指针*/
    char *loguy, *higuy;        /*在排序过程中使用的游移指针 */
    size_t size;                /*  sub-array的大小(命名的真烂) */
    char *lostk[STKSIZ], *histk[STKSIZ];
    int stkptr;                 /* 保存sub-array信息的栈 ,stkptr为定义的栈顶指针*/
 
    /* validation section */
    _VALIDATE_RETURN_VOID(base != NULL || num == 0, EINVAL);
    _VALIDATE_RETURN_VOID(width > 0, EINVAL);
    _VALIDATE_RETURN_VOID(comp != NULL, EINVAL);
 
    if (num < 2)
        return;                 /* 数组小于二的时候直接返回 */
 
    stkptr = 0;                 /* 初始化stkptr */
 
    lo = (char *)base;
    hi = (char *)base + width * (num - 1);        /* 初始化定义,其中hi指向数组的最后一个元素 */
    /*在这里是使用一个递归调用(使用了标签和goto)使用lo和hi来进行递归跳跃,
    需要进stack里面的元素进行保存*/
 
recurse:
 
    size = (hi - lo) / width + 1;        /*在待排序数组里面的元素数目 */
 
    /*跟上面曾经提到过的一样size<=CUTOFF时使用定义好的__SHORTSORT函数(定义参见上面的宏定义部分)
    在这里给出的理由是采用 O(n^2)的排序算法比快速排序更快*/
    if (size <= CUTOFF) {
        __SHORTSORT(lo, hi, width, comp, context);
    }
    else {
        /*首先我们要选择一个分区项(partition)。算法的高效性要求我们找到一个近似数组中间值
        的项，但我们要保证能够很快找到它。我们选择数组的第一项、中间项和最后一项的中
        间值，来避免最坏情况下的低效率。测试表明，选择三个数的中间值，比单纯选择数组
        的中间项的效率要高。在这里需要数学证明,本人才疏学浅只想到了近似的证明
        我们解释一下为什么要避免最坏情况和怎样避免。在最坏情况下，快速排序算法
        的运行时间复杂度是O(n^2)。这种情况的一个例子是已经排序的文件。如果我们选择最
        后一个项作为划分项，也就是已排序数组中的最大项最后算出的O(n^2)。
        而如果选择前 中 后三个数的中间值，这种最坏情
        数组的时间复杂度就只有O(n)*/
 
        mid = lo + (size / 2) * width;      /* find middle element */
 
        /* 将数组的第一项、中间项和最后一项进行排序,这样mid就是三者的中位数*/
        if (__COMPARE(context, lo, mid) > 0) {
            swap(lo, mid, width);
        }
        if (__COMPARE(context, lo, hi) > 0) {
            swap(lo, hi, width);
        }
        if (__COMPARE(context, mid, hi) > 0) {
            swap(mid, hi, width);
        }
        /*现在将数组划分为三个部分,大于划分值,小于划分值,等于划分值的*/
 
 
        loguy = lo;
        higuy = hi;
 
        /* 保证loguy递增higuy递减,保证循环能够结束 */
        for (;;) {
            /* lo <= loguy < hi, lo < higuy <= hi,
            A[i] <= A[mid] for lo <= i <= loguy,
            A[i] > A[mid] for higuy <= i < hi,
            A[hi] >= A[mid] (三个部分的定义)*/
 
            /*保证loguy之前的元素都比mid小(或者相等),由于可能会经过mid,所以使用双重循环的目的在于避免
            出现comp(mid,mid)在很多情况下给定的函数无法处理这样的情况*/
            if (mid > loguy) {
                do  {
                    loguy += width;
                } while (loguy < mid && __COMPARE(context, loguy, mid) <= 0);
            }
            if (mid <= loguy) {
                do  {
                    loguy += width;
                } while (loguy <= hi && __COMPARE(context, loguy, mid) <= 0);
            }
 
            /* lo < loguy <= hi+1, A[i] <= A[mid] for lo <= i < loguy,
            either loguy > hi or A[loguy] > A[mid] (*/
            /*保证loguy之前的元素小于等于*mid,结束的条件
            要么是loguy > hi 或者是 A[loguy] > A[mid]*/
 
            do  {
                higuy -= width;
            } while (higuy > mid && __COMPARE(context, higuy, mid) > 0);
 
            /* lo <= higuy < hi, A[i] > A[mid] for higuy < i < hi,
            either higuy == lo or A[higuy] <= A[mid] */
            /*保证higuy之前的元素比*mid大,结束条件higuy == lo 或者是 A[higuy] <= A[mid]*/
 
            if (higuy < loguy)
                break;
            /* if loguy > hi or higuy == lo, then we would have exited, so
            A[loguy] > A[mid], A[higuy] <= A[mid],
            loguy <= hi, higuy > lo */
            /* if loguy > hi or higuy == lo, 循环结束,
            而保持找循环内的条件A[loguy] > A[mid], A[higuy] <= A[mid],loguy <= hi, higuy > lo*/
 
            swap(loguy, higuy, width);
 
            /* 将*loguy和*higuy进行交换(也就是lo < loguy <= hi+1, A[i] <= A[mid]或者是A[i] > A[mid] for higuy < i < hi
            不成立的值,使得循环能够继续下去)*/
            /*需要注意的是可能存在loguy==mid,导致mid被交换,因此需要,下面的判断*/
 
            if (mid == higuy)
                mid = loguy;
 
            /* A[loguy] <= A[mid], A[higuy] > A[mid]; so condition at top
            of loop is re-established */
        }
        /*上述循环结束后,就形成一个双分的局面,以loguy和higuy为界,loguy之前的元素都不大于*mid,higuy之前的元素都大于*mid*/
 
        /* 找寻等于mid的元素,与上面相似为了避免出现com(mid,mid)的情况,使用了两重循环 */
 
        higuy += width;
        if (mid < higuy) {
            do  {
                higuy -= width;
            } while (higuy > mid && __COMPARE(context, higuy, mid) == 0);
        }
        if (mid >= higuy) {
            do  {
                higuy -= width;
            } while (higuy > lo && __COMPARE(context, higuy, mid) == 0);
        }
 
        /* 此时变成三路,关系如下
        higuy < loguy
        lo <= higuy <= hi
        A[i]  <= A[mid] for lo <= i <= higuy
        A[i]  == A[mid] for higuy < i < loguy
        A[i]  >  A[mid] for loguy <= i < hi
        A[hi] >= A[mid] */
 
        /*如果学过分治法的话,就应该明白,接下来的就应该是喜闻乐见的递归了,将前后两个部分进行递归
        但是在这里使用了栈区,来模拟递归因此与实际的递归有很大的不同*/
        /*在这里先将较小的块,进行递归同时保存较大的块中的数据(主要是边界值),与深度优先遍历相似*/
        if (higuy - lo >= hi - loguy) {
            if (lo < higuy) {
                lostk[stkptr] = lo;
                histk[stkptr] = higuy;
                ++stkptr;
            }                           /* save big recursion for later */
 
            if (loguy < hi) {
                lo = loguy;
                goto recurse;           /* do small recursion */
            }
        }
        else {
            if (loguy < hi) {
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr;               /* save big recursion for later */
            }
 
            if (lo < higuy) {
                hi = higuy;
                goto recurse;           /* do small recursion */
            }
        }
    }
 
    /*深度遍历之后,需要逐层递归较大的那一块,因此需要恢复数据*/
 
    --stkptr;
    if (stkptr >= 0) {
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;           /* 出栈 */
    }
    else
        return;                 /* 栈区所有的数据递归完毕 */
}
 
 
/***
*shortsort(hi, lo, width, comp) - 对数组进行插入排序
*shortsort_s(hi, lo, width, comp, context) -
*
*Purpose:
*       将hi,lo之间的数进行排序
*       内部排序
*       假定lo<=hi
*
*Entry:
*       char *lo = 指向最低位的指针
*       char *hi = 指向最高位的指针
*       size_t width = 数组元素的大小(bytes)
*       int (*comp)() = 定义的函数指针用于定义两个输入串的大小,
*                  用于比较数组中元素的大小	
*                  int compar (const void* p1, const void* p2);
*                  返回对p1 p2元素的比较结果确定p1 p2的比较方法 具体的定义可以看是
*                  选择顺序排序还是逆序排序 通常而言顺序的话大于返回>0小于返回<0等于返回0;
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/
 
SECURITYSAFECRITICAL_ATTRIBUTE
#ifdef __USE_CONTEXT
static void __fileDECL shortsort_s(
char *lo,
char *hi,
size_t width,
int (__fileDECL *comp)(void *, const void *, const void *),
void * context
)
#else  /* __USE_CONTEXT */
static void __fileDECL shortsort(
char *lo,
char *hi,
size_t width,
int (__fileDECL *comp)(const void *, const void *)
)
#endif  /* __USE_CONTEXT */
{
    char *p, *max;
 
 
 
    while (hi > lo) {
        /* A[i] <= A[j] for i <= j, j > hi */
        max = lo;
        for (p = lo + width; p <= hi; p += width) {
            /* A[i] <= A[max] for lo <= i < p */
            if (__COMPARE(context, p, max) > 0) {
                max = p;
            }
            /* A[i] <= A[max] for lo <= i <= p */
        }
 
        /* A[i] <= A[max] for lo <= i <= hi */
 
        swap(max, hi, width);
 
        /* A[i] <= A[hi] for i <= hi, so A[i] <= A[j] for i <= j, j >= hi */
 
        hi -= width;
 
        /* A[i] <= A[j] for i <= j, j > hi, loop top condition established */
    }
   
}
 
/***
*swap(a, b, width) - 交换指针指向的两个元素
*
*Purpose:
*       swaps the two array elements of size width
*
*Entry:
*       char *a, *b = 指向带交换元素的指针
*       size_t width = 元素大小(byte)
*
*Exit:
*       returns void
*
*Exceptions:
*
*******************************************************************************/
 
SECURITYSAFECRITICAL_ATTRIBUTE
static void __fileDECL swap(
char *a,
char *b,
size_t width
)
{
    char tmp;
 
    if (a != b)
    while (width--) {
        tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    }
}
 
#undef __fileDECL
#undef __COMPARE
#undef __SHORTSORT
#undef swap
