/* *************************************************************************** *
*   Modified Crochemore's repetition algorithm to compute the MRC Array        *
*   Modified in October 2024                                                   *
*   (C) Author: S.K. Jain, N. Mhaskar                                          *
*                                                                              *
*   For exact changes in code kindly do a diff on this file and the            *
*   original file - https://www.cas.mcmaster.ca/~franek/research/croch10.cpp   *
*                                                                              *
*   Original C++ implementation of Franek-Smyth-Xiao Algorithm FSX10()         *
*   works along the same principles as Crochemore's algorithm                  *  
*   in O(n log(n)) time where n is the length of the input string              *
*   The space complexity is 10*n*sizeof(unsigned int) bytes                    *
*                                                                              *
*   This program can be compiled and optimized -O3 level with no               *
*   complications                                                              *
*                                                                              *
*         Author: F. Franek                                                    *
*         (C) F. Franek, W.F. Smyth, X. Xiao                                   *
*             ARG, McMaster University, October 2001                           *
*                                                                              *
*                                                                              *
*         Corrected July 30, 2003 (the gap handling functions)                 *
*                                                                              *
* ******************************************************************************/


// when using compiler that cannot inline, just uncomment the
// following line 
// #define NOINLINING 



#ifdef NOINLINING
 #define INLINE
#else
 #define INLINE inline
#endif



// FSX10 global data 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <list>
#include <iostream>

/* *************************************************
           FSX10 data and functions begin
   ************************************************* */
// FSX10 global data 
unsigned int null;
unsigned int N;
unsigned int N_1;

unsigned int* CNext = NULL;         // will be array 
unsigned int* CPrev = NULL;         // will be array
unsigned int* CMember = NULL;       // will be array
unsigned int* CStart = NULL;        // will be array
unsigned int* FNext = NULL;         // will be array
unsigned int* FPrev = NULL;         // will be array
unsigned int* FMember = NULL;       // will be array
unsigned int* Refine = NULL;        // will be array

unsigned int* FStart = NULL;        // will be a stack-array
unsigned int FStartTop;

unsigned int* CEmptyStack = NULL;   // will be a stack sharing memory 
                                    // with ScQueue
unsigned int CEmptyTop;

unsigned int* SelQueue = NULL;      // will be a queue sharing memory 
                                    // with RefStack
unsigned int SelQueueFirst;
unsigned int SelQueueLast;

unsigned int* ScQueue = NULL;       // will be queue sharing memory 
                                    // with CEmptyStack
unsigned int ScQueueFirst;
unsigned int ScQueueLast;

unsigned int* RefStack = NULL;      // will be a stack sharing memory 
                                    // with SelQueue 
unsigned int RefTop;

// datastructures for tracking repetitions 
unsigned int* Gap = NULL;          // will be an array
unsigned int* GapList = NULL;      // will be an array
unsigned int* GNext = NULL;        // will be an array
unsigned int* GPrev = NULL;        // will be an array

std::vector<std::list<std::pair<uint64_t, uint64_t>>> MRC; 
unsigned int L;
char* string;
unsigned int n;

/* *************************************************
   test and debugging functions, if not needed 
   they can be removed or blocked from show_Classes() 
   down to show_GPrev().
   They are only used to see the contents of the various
   data structures used in the program.
   ************************************************* */

// function show_Classes -------------------------------------- 
void show_Classes()
{

unsigned int i, j;
char c;

for(j = 0; j < N; j++) {
 if (CStart[j] == null) continue;
 printf("c%d=",j);
 if (CNext[CPrev[CStart[j]]] == 1) 
   printf("{%d}",CStart[j]);
 else{
   c = '{';
   for(i = CStart[j]; i != CPrev[CStart[j]]; i = CNext[i]) {
    printf("%c%d",c,i);
    c = ',';
   }//endfor
   printf(",%d}",i);
 }
}//endfor
 
putchar('\n');
fflush(stdout);

}//end show_Classes


// function show_CEmptyStack ------------------------------ 
void show_CEmptyStack()
{

unsigned int i;

if (CEmptyTop == null) {
  printf("CEmptyStack is empty\n");
  fflush(stdout);
  return;
}

printf("CEmptyStack:");
for(i = CEmptyTop; i != 0; i--) 
 printf("%d,",CEmptyStack[i]);
printf("%d\n",CEmptyStack[i]);
fflush(stdout);

}//end show_CEmptyStack






// function show_SelQueue ------------------------------
void show_SelQueue()
{

unsigned int i;

if (SelQueueFirst == null) {
  printf("SelQueue is empty\n");
  fflush(stdout);
  return;
}

printf("SelQueue:");
for(i = SelQueueFirst; i != SelQueueLast; i++)
 printf("%d,",SelQueue[i]);
printf("%d\n",SelQueue[i]);
fflush(stdout);

}//end show_SelQueue






// function show_ScQueue ------------------------------
void show_ScQueue()
{

unsigned int i;

if (ScQueueFirst == null) {
  printf("ScQueue is empty\n");
  fflush(stdout);
  return;
}

printf("ScQueue:");
for(i = ScQueueFirst; i != ScQueueLast; i++)
 printf("%d,",ScQueue[i]);
printf("%d\n",ScQueue[i]);
fflush(stdout);

}//end show_ScQueue






// function show_RefStack ------------------------------
void show_RefStack()
{

unsigned int i;

if (RefTop == null) {
  printf("RefStack is empty\n");
  fflush(stdout);
  return;
}

printf("RefStack:");
for(i = RefTop; i != 0; i--)
 printf("%d,",RefStack[i]);
printf("%d\n",RefStack[i]);
fflush(stdout);

}//end show_RefStack






// function show_FStart ------------------------------
void show_FStart()
{

unsigned int i;

printf("FStart:");
for(i = 0; i < N_1; i++)
 if (FStart[i] == null)
   printf("*,");
 else
  printf("%d,",FStart[i]);

if (FStart[i] == null)
  printf("*\n");
else
 printf("%d\n",FStart[i]);
fflush(stdout);

}//end show_FStart




// function show_CStart -------------------------------
void show_CStart()
{

unsigned int i;

printf("CStart:");
for(i = 0; i < N_1; i++) 
 if (CStart[i] == null) 
   printf("*,");
 else
   printf("%d,",CStart[i]);
   
if (CStart[i] == null) 
  printf("*\n");
else
  printf("%d\n",CStart[i]);

fflush(stdout);

}//end show_CStart






// function show_CNext -------------------------------
void show_CNext()
{

unsigned int i;

printf("CNext:");
for(i = 0; i < N_1; i++) 
 if (CNext[i] == null) 
   printf("*,");
 else
   printf("%d,",CNext[i]);

if (CNext[i] == null) 
  printf("*\n");
else
  printf("%d\n",CNext[i]);

fflush(stdout);

}//end show_CNext




// function show_CPrev -------------------------------
void show_CPrev()
{

unsigned int i;

printf("CPrev:");
for(i = 0; i < N_1; i++) 
 if (CPrev[i] == null) 
   printf("*,");
 else
   printf("%d,",CPrev[i]);

if (CPrev[i] == null) 
  printf("*\n");
else
  printf("%d\n",CPrev[i]);

fflush(stdout);

}//end show_CPrev 




// function show_CMember -------------------------------
void show_CMember()
{

unsigned int i;

printf("CMember:");
for(i = 0; i < N_1; i++) 
 if (CMember[i] == null) 
   printf("*,");
 else
   printf("%d,",CMember[i]);

if (CMember[i] == null) 
  printf("*\n");
else
  printf("%d\n",CMember[i]);

fflush(stdout);

}//end show_CMember





// function show_Refine -------------------------------
void show_Refine()
{

unsigned int i, r, j;

printf("Refine:");
for(i = 0; i < N_1; i++) {
 j = N-(i+1);
 if (FStartTop == null || j > FStartTop)
   r = FStart[j];
 else
   r = FNext[FPrev[FStart[j]]];
 if (r == null)
   printf("*,");
 else
   printf("%d,",r);
}

j = N-(i+1);
if (FStartTop == null || j > FStartTop)
  r = FStart[j];
else
  r = FNext[FPrev[FStart[j]]];
if (r == null)
  printf("*\n");
else
  printf("%d\n",r);

fflush(stdout);

}//end show_Refine





// function show_FNext -------------------------------
void show_FNext()
{

unsigned int i;

printf("FNext:");
for(i = 0; i < N_1; i++) 
 if (FNext[i] == null) 
   printf("*,");
 else
   printf("%d,",FNext[i]);

if (FNext[i] == null) 
  printf("*\n");
else
  printf("%d\n",FNext[i]);

fflush(stdout);

}//end show_FNext




// function show_FPrev -------------------------------
void show_FPrev()
{

unsigned int i;

printf("FPrev:");
for(i = 0; i < N_1; i++) 
 if (FPrev[i] == null) 
   printf("*,");
 else
   printf("%d,",FPrev[i]);

if (FPrev[i] == null) 
  printf("*\n");
else
  printf("%d\n",FPrev[i]);

fflush(stdout);

}//end show_FPrev





// function show_FMember -------------------------------
void show_FMember()
{

unsigned int i;

printf("FMember:");
for(i = 0; i < N_1; i++) 
 if (FMember[i] == null) 
   printf("*,");
 else
   printf("%d,",FMember[i]);

if (FMember[i] == null) 
  printf("*\n");
else
  printf("%d\n",FMember[i]);

fflush(stdout);

}//end show_FMember




// function show_Families ----------------------------
void show_Families()
{

unsigned int i, j;
char c;

if (FStartTop == null) {
  printf("no families\n");
  return;
}

for(i = 0; i <= FStartTop; i++) {
 printf("family[%d]=",i);
 c = '{';
 j = FStart[i]; 
 while(1) {
  printf("%cc%d",c,j);
  c = ',';
  if (j == FPrev[FStart[i]]) break;
  j = FNext[j];
 }
 printf("}\n");
 fflush(stdout);
}

}//end show_Families




// function show_Index -----------------------------------
void show_Index()
{
 void show_Index1(unsigned int);

 show_Index1(FNext[0]);
 show_Index1(FPrev[0]);
 printf("%c=0\n",FMember[0]);
 
}//end show_Index




// function show_Index1 -----------------------------------
void show_Index1(unsigned int x)
{

 if (x == null) return;
 show_Index1(FNext[x]);
 show_Index1(FPrev[x]);
 printf("%c=%d\n",FMember[x],x);
}//end show_Index





// function show_Gaps -------------------------------
void show_Gaps()
{

unsigned int i, g;
char c;
int x;

x = 0;
for(g = 0; g < N; g++) {
 if (GapList[g] == null) 
   continue;
 x = 1;
 printf("[Gap=%d",g);
 c = ']';
 for(i = GapList[g]; i != null; i = GNext[i], c=',')
  printf("%c%d",c,i);
 putchar('\n');
}
if (x == 0) 
  printf("no Gaps\n");
fflush(stdout);

}//end show_Gaps






// function show_Gap -------------------------------
void show_Gap()
{

unsigned int i;

printf("Gap:");
for(i = 0; i < N_1; i++) 
 if (Gap[i] == null) 
   printf("*,");
 else
   printf("%d,",Gap[i]);

if (Gap[i] == null) 
  printf("*\n");
else
  printf("%d\n",Gap[i]);

fflush(stdout);

}//end show_Gap





// function show_GapList -------------------------------
void show_GapList()
{

unsigned int i;

printf("GapList:");
for(i = 0; i < N_1; i++) 
 if (GapList[i] == null) 
   printf("*,");
 else
   printf("%d,",GapList[i]);

if (GapList[i] == null) 
  printf("*\n");
else
  printf("%d\n",GapList[i]);

fflush(stdout);

}//end show_GapList





// function show_GNext -------------------------------
void show_GNext()
{

unsigned int i;

printf("GNext:");
for(i = 0; i < N_1; i++) 
 if (GNext[i] == null) 
   printf("*,");
 else
   printf("%d,",GNext[i]);

if (GNext[i] == null) 
  printf("*\n");
else
  printf("%d\n",GNext[i]);

fflush(stdout);

}//end show_GNext





// function show_GPrev -------------------------------
void show_GPrev()
{

unsigned int i;

printf("GPrev:");
for(i = 0; i < N_1; i++) 
 if (GPrev[i] == null) 
   printf("*,");
 else
   printf("%d,",GPrev[i]);

if (GPrev[i] == null) 
  printf("*\n");
else
  printf("%d\n",GPrev[i]);

fflush(stdout);

}//end show_GPrev




/* *******************************************************
          FSX10 datastructure handling functions 
          Since they are simple, they are inlined,
          if inlining is possible
   ******************************************************* */


// function CEmptyStackInit ------------------------------
INLINE void CEmptyStackInit() 
{
  CEmptyTop = null;

}//end CEmptyStackInit




// function CEmptyStackPush -----------------------------
INLINE unsigned int CEmptyStackPush(unsigned int i) 
{
 if (CEmptyTop == null) 
   CEmptyStack[CEmptyTop = 0] = i; 
 else 
   CEmptyStack[++CEmptyTop] = i;
 return CEmptyTop;

}//end CEmptyStackPush



// function CEmptyStackPop --------------------------------
INLINE unsigned int CEmptyStackPop() 
{
 if (CEmptyTop==null)
   return null;
 else if (CEmptyTop == 0) {
   CEmptyTop = null;
   return CEmptyStack[0];
 }else
   return CEmptyStack[CEmptyTop--];

}//end CEmptyStackPop



// function SelQueueInit --------------------------------
INLINE void SelQueueInit() 
{
  SelQueueFirst =  SelQueueLast = null; 

}//end SelQueueInit




// function SelQueueInsert --------------------------------
INLINE unsigned int SelQueueInsert(unsigned int i) 
{
 if (SelQueueFirst == null) 
   SelQueue[SelQueueFirst = SelQueueLast = 0] = i;
 else
   SelQueue[++SelQueueLast] = i; 
 return SelQueueLast;

}//end SelQueueInsert




// function SelQueueHead -----------------------------------
INLINE unsigned int SelQueueHead() 
{
 unsigned int i;
 if (SelQueueFirst==null) 
   return null; 
 else if (SelQueueFirst == SelQueueLast) {
   i = SelQueueFirst;
   SelQueueFirst=SelQueueLast=null;
   return SelQueue[i];
 }else
   return SelQueue[SelQueueFirst++]; 

}//end SelQueueHead




// function RefStackInit ----------------------------------
INLINE void RefStackInit() 
{
  RefTop = null;

}//end RefStackInit





// function RefStackPop ------------------------------------
INLINE unsigned int RefStackPop() 
{
 if (RefTop == null) 
   return null;
 else if (RefTop == 0) {
   RefTop = null;
   return RefStack[0];
 }else
   return RefStack[RefTop--];

}//end RefStackPop




// function RefStackPush ----------------------------------
INLINE unsigned int RefStackPush(unsigned int i) 
{
 if (RefTop == null) 
   RefStack[RefTop=0] = i; 
 else 
   RefStack[++RefTop] = i;
 return RefTop;
}//end RefStackPush




// function ScQueueInit ----------------------------------
INLINE void ScQueueInit() 
{
  ScQueueFirst = ScQueueLast = null;

}//end ScQueueInit





// function ScQueueHead ------------------------------------
INLINE unsigned int ScQueueHead() 
{
 unsigned int i;
 if (ScQueueFirst == null) 
   return null;
 else if (ScQueueFirst == ScQueueLast) {
    i = ScQueue[ScQueueFirst];
   ScQueueLast = ScQueueFirst = null;
   return i;
}else
   return ScQueue[ScQueueFirst++];

}//end ScQueueHead




// function ScQueueInsert ----------------------------------
INLINE unsigned int ScQueueInsert(unsigned int i) 
{
 if (ScQueueFirst == null) 
   ScQueue[ScQueueLast=ScQueueFirst=0] = i; 
 else 
   ScQueue[++ScQueueLast] = i;
 return ScQueueLast;

}//end ScQueueInsert




/* ******************************************************
                       FSX10 functions 
   ****************************************************** */




// no need to inline, called only once
// function CreateData -----------------------------------
void CreateData(char* string) 
{
  N = n;
  N_1 = N-1;
  null = N+1;

  CEmptyStack = (unsigned int*) malloc(10*sizeof(unsigned int)*N);
  if (CEmptyStack == NULL) {
    printf("memory allocation error\n");
    exit(1);
  }
  ScQueue = CEmptyStack;     // sharing 
  CStart = CEmptyStack + N;
  CNext = CStart + N;
  CPrev = CNext + N;
  CMember = CPrev + N;
  SelQueue = CMember + N;
  RefStack = SelQueue;      // sharing 
  FStart = SelQueue + N;
  FNext = FStart + N;
  FPrev = FNext + N;
  FMember = FPrev + N;

  Gap = (unsigned int*) malloc(4*N*sizeof(unsigned int));
  if (Gap == NULL) {
    printf("memory allocation error\n");
    exit(1);
  }
  GapList = Gap + N;
  GNext = GapList + N;
  GPrev = GNext + N;

}//end CreateData





// no need to inline, called only once
// function DeallocateData ----------------------------
void DeallocateData()
{

free((void*) CEmptyStack);
free((void*) Gap);

}//end DeallocateData*/






// We will build a binary search tree, and we will use
// FNext[], FPrev[], and FMember[] to do so
// FNext will fake right children
// FPrev will fake left children
// FMember will fake the parent
// This tree is just temporary and used only by InitLevel 
// The puprose for Index is to replace characters by indexes to work
// with an index alphabet

// no need to inline, called only once
// function CreateIndex -----------------------------------
void CreateIndex(char* string) 
{

unsigned int i, j, root, last;

   root = null; 
   for(i = 0; i < N; i++) {
    // insert string[i] unsigned into the search tree 
    if (root == null) {
      root = last = 0;
      FMember[root] = string[i];
      FNext[root] = FPrev[root] = null;
      continue;
    }
    j = root;
    while(1) {
     if (string[i] == (char) FMember[j]) {
       break;                                // break the while loop 
     }else if (string[i] < (char) FMember[j]) {     // go left=CPrev 
       if (FPrev[j] == null) {
         FMember[++last] = string[i];
         FPrev[last] = FNext[last] = null;
         FPrev[j] = last;
         break;                              // break the while loop 
       }else{
         j = FPrev[j];
         continue;
       }
     }else{                                  // go rigth=CNext
       if (FNext[j] == null) {
         FMember[++last] = string[i];
         FNext[last] = FPrev[last] = null;
         FNext[j] = last;
         break;                              // break the while loop 
       }else{
         j = FNext[j];
         continue;
       }
     }
    }//endwhile
   }//endfor

}//end CreateIndex



// given a character from the alphabet, returns its index
// function Index -------------------------------------
INLINE unsigned int Index(char c) 
{

unsigned int i;

i = 0;
while(1) {
 if ((char) FMember[i]==c)
   return i;
 else if (c < (char) FMember[i])
    i = FPrev[i];
 else 
    i = FNext[i];
}

}//end Index






// modified July 30, 2003
// add position i to the gaplist of gap-size g at the beginning of the list
// function AddToGapList ------------------------------------
INLINE void AddToGapList(unsigned int i,unsigned int g)
{

 if (GapList[g] == null) {
    GapList[g] = i;
    GNext[i] = null;
    GPrev[i] = null;
    Gap[i] = g;
  }else{
    GNext[i] = GapList[g];
    GPrev[i] = null;
    GPrev[GapList[g]] = i;
    GapList[g] = i;
    Gap[i] = g;
  }

}//end AddToGapList




// modified July 200, 2003
// remove position i from the gaplist of gap-size g 
// function RemoveFromGapList -----------------------------
INLINE void RemoveFromGapList(unsigned int i,unsigned int g)
{

  if (Gap[i]!=g) return;  // not really in GapList

  // remove from GapList[g] 

  if (GapList[g] != null) {
   if (GapList[g] == i && GNext[i] == null) {           // {i} 
     GapList[g] = null;
     Gap[i]=null;
   }else if (GapList[g] == i) {                         // {i,...} 
     GapList[g] = GNext[i];
     GPrev[GapList[g]] = null;
     GNext[i] = null;
     Gap[i]=null;
   }else if (GNext[i] == null) {                        // {...,i} 
     GNext[GPrev[i]] = null;
     GPrev[i] = null;
     Gap[i]=null;
   }else{                                               // {..i..} 
     GPrev[GNext[i]] = GPrev[i];
     GNext[GPrev[i]] = GNext[i];
     GNext[i] = null;
     GPrev[i] = null;
     Gap[i]=null;
   }
 }


}//end RemoveFromGapList




// modified July 30, 2003
// output all repetitions on level L
// function output_Reps ----------------------------------
INLINE void output_Reps(unsigned int L)
{

int s, j, m, xs, r;
static int first = 1;

while((j = GapList[L]) != null) {
 RemoveFromGapList(j,L);
 xs = s = j;
 r = 2;
 // so we now that there is a rep (xs,L,r)
 // can we extend it to the left?
 m = xs;
 while(1) {
  m = m-L;
  if (m < 0) break;
  if (Gap[m]!=L) break;
  if (CMember[m]!=CMember[xs]) break;
  // yes, we can extend it to the left, so do so
  RemoveFromGapList(m,L);
  r++;
  s = m;
 }//endwhile

 // now (s,L,r) is the leftmost extension of (xs,L,r)
 // can we extend (xs,L,r) to the right ?
 m = xs+L;
 while(1) {
  m = m+L;
  if (m >= N) break;
  if (Gap[m]!=L) break;
  if (CMember[m]!=CMember[xs]) break;
  // yes, we can extend it to the right, so do so
  RemoveFromGapList(m,L);
  r++;
 }//endwhile

 // now (s,L,r) is the maximal repetition

//  if (first) {
//    printf("indexing starts at position 0\n");
//    printf("repetitions are output in the following format: (s,l,p)\n");
//    printf("     s = starting position of the generator\n");
//    printf("     l = length of the generator\n");
//    printf("     p = power of the repetition\n");
//    first=0;
//  }
 //printf("(%d,%d,%d)\n",s,L,r);
 fflush(stdout);

}//endwhile
}//end output_Reps





// function StartNewFamily ---------------------------
INLINE unsigned int StartNewFamily(unsigned int j)
{

unsigned int r;
 
 // create new family f_i = {c_j} 

  if (FStartTop == null) {
    FStartTop = 0;
    r = FStart[FStartTop];     // remember Refine[] so it can be moved
    FStart[FStartTop] = j;     // set start to j
    FPrev[j] = j;              // set end
    FNext[j] = r;              // set new Refine[]
    FMember[j] = FStartTop;    // set membership
  }else{
    ++FStartTop;
    r = FStart[FStartTop];     // remember Refine[] so it can be moved
    FStart[FStartTop] = j;     // set start to j
    FPrev[j] = j;              // set end
    FNext[j] = r;              // set new Refine[]
    FMember[j] = FStartTop;
  }

  return FStartTop;

}//end StartNewFamily




// function AddToFamily ---------------------------
INLINE void AddToFamily(unsigned int i,unsigned int j)
{
 
 // add c_j to family f_i

 FNext[j] = FStart[i];             // add as the first in the family
 FPrev[j] = FPrev[FStart[i]];      // move end link
 FPrev[FStart[i]] = j;             // new start is previous to old start
 FStart[i] = j;                    // new start is j
 FMember[j] = i;                   // c_j belongs to f_i

}//end AddToFamily




// function RemoveFromFamily ---------------------------
INLINE void RemoveFromFamily(unsigned int i,unsigned int j)
{

unsigned int r;

 // remove c_j from family f_i

     if (FStart[i]==j) {             // f_i = {c_j,..}
       FPrev[FNext[j]] = FPrev[j];   // prev to new start will be link to end
       FStart[i] = FNext[j];         // new start
       FNext[j] = null;              // disconnect c_j
       FPrev[j] = null;
       FMember[j] = null;
     }else if (FNext[j] == null) {   // f_i = {..,c_j}
       r = FNext[j];                 // remember Refine[]
       FPrev[FStart[i]] = FPrev[j];  // link to new end
       FNext[FPrev[j]] = r;          // move Refine[] there
       FNext[j] = null;              // disconnect c_j
       FPrev[j] = null;
       FMember[j] = null;
     }else{                          // f_i = {..,c_j,..}
       FNext[FPrev[j]] = FNext[j];   // bridge over c_j
       FPrev[FNext[j]] = FPrev[j];
       FNext[j] = null;              // disconnect c_j
       FPrev[j] = null;
       FMember[j] = null;
     }

}//end RemoveFromFamily

void addValidMRC(uint64_t index, uint64_t l, uint64_t b)
{
    if(MRC[index].size() > 0 && MRC[index].front().second == b)
      return;
    if(index + l == n || (index + l < n && string[index + b] != string[index + l]))
        MRC[index].push_front({l, b});
}

// function RemoveFromClass ------------------------------
INLINE void RemoveFromClass(unsigned int e,unsigned int j)
{

unsigned int m;

 // remove e from c_j

 if (CNext[CPrev[CStart[j]]] == 1) {   // c_j = {e}
   CEmptyStackPush(j);                 // mark c_j as empty
   CStart[j] = null;                   // make c_j empty
   if ((m=FMember[j]) != null)         // c_j is member of f_m
     RemoveFromFamily(m,j);            // remove c_j from f_m
   CNext[e] = null;                    // disconnect e
   CPrev[e] = null;
   CMember[e] = null;
 }else if (CStart[j] == e) {           // c_j = {e,..}
   addValidMRC(e, CNext[e] - e + L, L); // index, length, borderLength
   RemoveFromGapList(e,CNext[e]-e);
   m = CNext[CPrev[CStart[j]]]-1;      // decrement size
   CPrev[CNext[e]] = CPrev[e];         // move end
   CNext[CPrev[CNext[e]]] = m;         // move size
   CStart[j] = CNext[e];               // new start
   CNext[e] = null;                    // disconnect e
   CPrev[e] = null;
   CMember[e] = null;
 }else if (CPrev[CStart[j]] == e) {    // c_j = {..,e}
   addValidMRC(CPrev[e], e - CPrev[e] + L, L); // index, length, borderLength
   RemoveFromGapList(CPrev[e],e-CPrev[e]);
   m = CNext[CPrev[CStart[j]]]-1;      // decrement size
   CPrev[CStart[j]] = CPrev[e];        // new end
   CNext[CPrev[e]] = m;                // move size
   CNext[e] = null;                    // disconnect e
   CPrev[e] = null;
   CMember[e] = null;
 }else{                                // c_j = {..,e,..}
   addValidMRC(e, CNext[e] - e + L, L);        // index, length, borderLength
   addValidMRC(CPrev[e], e - CPrev[e] + L, L); // index, length, borderLength
   RemoveFromGapList(e,CNext[e]-e);
   RemoveFromGapList(CPrev[e],e-CPrev[e]);
   AddToGapList(CPrev[e],CNext[e]-CPrev[e]);
   CNext[CPrev[CStart[j]]]=CNext[CPrev[CStart[j]]]-1;  // decrement size
   CNext[CPrev[e]] = CNext[e];         // bridge over e
   CPrev[CNext[e]] = CPrev[e];
   CNext[e] = null;                    // disconnect e
   CPrev[e] = null;
   CMember[e] = null;
 }

}//end RemoveFromClass*/



// add position i to the end of class c_j
// function AddToClass ---------------------------------
INLINE void AddToClass(unsigned int i,unsigned int j)
{

unsigned int m;

  if (CStart[j] == null) {          // c_j is empty
    CStart[j] = i;                  // start
    CPrev[CStart[j]] = i;           // end
    CNext[i] = 1;                   // size
    CMember[i] = j;                 // membership
  }else{                            // c_j is not empty
    m = CNext[CPrev[CStart[j]]]+1;  // increment size
    CPrev[i] = CPrev[CStart[j]];    // prev to i is the old end
    CNext[CPrev[CStart[j]]]=i;      // next to old end is i
    CPrev[CStart[j]] = i;           // i is new end
    CNext[i] = m;                   // move size
    CMember[i] = j;                 // membership
    AddToGapList(CPrev[i],i-CPrev[i]);
  }

}//end AddToClass





// function GetRefine ------------------------------------
INLINE unsigned int GetRefine(unsigned int i)
{

unsigned int j;

j = N-(i+1);
if (FStartTop==null || j > FStartTop)
  return FStart[j];
else
  return FNext[FPrev[FStart[j]]];

}//end GetRefine




// function SetRefine ------------------------------------
INLINE void SetRefine(unsigned int i,unsigned int r)
{

unsigned int j;

j = N-(i+1);
if (FStartTop==null || j > FStartTop)
  FStart[j] = r;
else
  FNext[FPrev[FStart[j]]] = r;


}//end SetRefine




// restore Refine array to its initial state with all 
// entries being null and restore RefStack to its initial 
// state when it is empty.

// function CleanRefine ----------------------------------
INLINE void CleanRefine() 
{

unsigned int i;

 while((i = RefStackPop()) != null) 
  SetRefine(i,null);

}//end CleanRefine

 

// We are assuming an indexed alphabet and thus we are assuming 
// to have Index(letter) that provides an index in the range 0..n 
// for each letter occuring in string.

// modified July 30, 2003
// no need to inline, called only once
// function InitLevel -----------------------------------
void InitLevel(char* string) 
{
 
 unsigned int i, j, k, k1;

 // first we initialize all data structures  with the
 // exception of FMember[], FNext[], and FPrev[] as they 
 // contain the search tree for the alphabet

 for(i = 0; i < N; i++) {
  CNext[i] = null;
  CPrev[i] = null;
  CMember[i] = null;
  CStart[i] = null;
  FStart[i] = null;
  Gap[i] = null;
  GapList[i] = null;
  GNext[i] = null;
  GPrev[i] = null;
 }//endfor

 FStartTop = null;
 CEmptyStackInit();
 SelQueueInit();
 RefStackInit();
 ScQueueInit();

 // now we traverse string and create the classes 
 // according to letters.

 // in this loop we cannot touch FNext[], FPrev[], FMember[]
 // for they are needed by Index()

 k = 0;
 for(i = 0; i < N ; i++) {            // traverse the string
  j = Index(string[i]);               // i goes to c_j
  if (CStart[j] == null)              // c_j is empty
    FStart[k++] = j;
  AddToClass(i,j); 
 }//endfor

 // so we have created classes c_0 .. c_k-1
 // thus c_k .. c_N-1 are still empty, mark them as such 
 for(i = N_1; i >= k; i--)
  CEmptyStackPush(i);

 // at this point we do not need Index() any more, and hence
 // we can get rid of the binary search tree stored in
 // FMember[], FNext[], and FPrev[]

 for(i = 0; i < N; i++)
  FMember[i] = FNext[i] = FPrev[i] = null;

 // all created classes are considered small, so we insert their
 // elements to SelQueue and the classes to ScQueue.
 // We also initialize Gap structures
 // we know that all classes are small, so there will be N small 
 // class elements. Thus we can start filling SelQueue from position 
 // 0. But we have k small classes and we want ScQueue to be right 
 // justified in the memory segment, so we "tinker" with ScQueueFirst 
 // and Last, for we want to store it in ScQueue[N-k .. N-1]

 SelQueueInit();
 ScQueueInit();
 ScQueueFirst = N-k;
 ScQueueLast = ScQueueFirst-1;
 

 for(k1 = 0; k1 < k; k1++) {
  j = FStart[k1];
  FStart[k1] = null;
  // traverse c_j
  i = CStart[j]; 
  SelQueueInsert(i);
  ScQueueInsert(i);
  if (CNext[CPrev[CStart[j]]] == 1) continue;
  for(i = CNext[i]; i != CPrev[CStart[j]]; i = CNext[i]) 
   SelQueueInsert(i);
  SelQueueInsert(i);
 }//endfor

 // now FStart is all null's and since FStartTop = null
 // we have in effect Refine[] initialized to null

}//end InitLevel







// function Refineby -------------------------------------
INLINE int Refineby(unsigned int e,unsigned int j,int init) 
{

 unsigned int e1, j1, j2, j1_family;
 static unsigned int curr_class;
 
 if (init) {
   curr_class = null;
   return 0;
 }

 if (e == 0) return 0;               // no refinement done
 e1 = e-1;
 j1 = CMember[e1];                  // e1 is member of c_j1

 // we are refining c_j1 by class that started with j*/

 // but what if c_j1 is a singleton?
 if (CNext[CPrev[CStart[j1]]]==1) {
   if (j == curr_class) {
     if (GetRefine(j1)==null) // we have done some refinement by curr_class, 
     return 0;                // but c_j1 has not been a part of it, so it is 
                              // a true singleton
   }else       
     return 0;                // we are starting refinement by a new class
 }

 curr_class = j;

 // if it is a singleton, then it is a remainder of a class
 // being refined

 // remove e1 from c_j1, at this point c_j1 may vanish 
 // but we will need to know in what family it belonged
 // so we must remember it.

 j1_family = FMember[j1];
 RemoveFromClass(e1,j1);   // it may remove c_j1 from family[j1_family]

 // now we have to find where to move e1

 if (GetRefine(j1) == null) {     // we have not done any refinement of c_j1
                                  // by curr_class yet
   SetRefine(j1,CEmptyStackPop()); // take the first empty class to serve
                                  // as refinement of c_j1 by curr_class
   RefStackPush(j1);              // remember which location in Refine[] we
                                  // must restore
   j2 = GetRefine(j1);

   // we must add c_j2 to the same family as c_j1
   // but c_j1 might not be in a family yet
   if (j1_family == null)  
     j1_family = StartNewFamily(j1);
   AddToFamily(j1_family,j2);     // add to existing family
 }else
   j2 = GetRefine(j1);            // continuation of refinement
                                  // by c_j1

 // we must move e1 to c_j2
 AddToClass(e1,j2);

return 1;   // we have done some refinement

}//end Refineby





// create level L+1 from L
// function NextLevel ----------------------------------
INLINE int NextLevel() 
{

unsigned int curr_class, next_class, r, e, j, i, k, i1;
unsigned int sel,sc;

Refineby(0,0,1);       // initialize Refineby()

 // elements in SelQueue are grouped by classes, within each
 // class the order is increasing. ScQueue contains the first
 // elements of each class, so we can determine when a new
 // class starts.
   
 r = 0;
 curr_class = ScQueueHead(); 
 next_class = ScQueueHead(); 
 while((e = SelQueueHead()) != null) {
  if (e == next_class) {
    CleanRefine();
    curr_class = next_class;
    next_class = ScQueueHead();
  }
  r += Refineby(e,curr_class,0);
 }//endwhile

 CleanRefine();

 if (r == 0)   // no further refinement, we are done
   return 0;

 // now we have to determine small classes
 
 // Note that at this stage we do not need FMember[] any more, 
 // so we can use it for remembering the family member of max size, 
 // but we have to first initialize it to null

 for(j = 0; j <= FStartTop; j++) {         // traversing FStart array
  i = FStart[j];                           // traversing family f_j
  while(1) {
   FMember[i] = null;
   if (i == FPrev[FStart[j]]) break;
   i = FNext[i];
  }//endwhile
 }//endfor
 
 
 // now we traverse FStart again and store a biggest family 
 // member in FStartMamber[]
 for(j = 0; j <= FStartTop; j++) {        // traversing FStart array
  i = FStart[j];                          // traversing family f_j
  while(1) {
   if (FMember[j] == null)    // we do not have a biggest guy yet
     FMember[j] = i;          // so we make c_i the current biggest
   else if (CNext[CPrev[CStart[FMember[j]]]] < CNext[CPrev[CStart[i]]])
     FMember[j] = i;          // c_i is the new champion
   if (i == FPrev[FStart[j]]) break;
   i = FNext[i];
  }//endwhile
 }//endfor

 // now we traverse FStart again and store all elements in 
 // SelQueue and ScQueue, but since we want both to be right 
 // justified in there memory segment, first we count them

 for(sel=sc=j= 0; j <= FStartTop; j++) {  // traversing FStart array
  i = FStart[j];                          // traversing family f_j
  while(1) {
   if (FMember[j] == i) {      // skip c_i, it is big
     if (i == FPrev[FStart[j]]) break;  // end of f_j
     i = FNext[i];
     continue;
   }
   // now we traverse c_i and insert all its elements to SelQueue 
   // and the first element in ScQueue
   k = CStart[i];
   sc++;
   sel++;
   if (CNext[CPrev[CStart[i]]] == 1) {
     if (i == FPrev[FStart[j]]) break;  // end of f_j
     i = FNext[i];
     continue;
   }
   for(k = CNext[k]; k != CPrev[CStart[i]]; k = CNext[k]) 
    sel++;
   sel++;
   if (i == FPrev[FStart[j]]) break;
   i = FNext[i];
  }//endwhile
 }//endfor

 SelQueueInit();   
 // but we will only have sel small class elements, we want to 
 // store them in SelQueue[N-sel .. N-1], hence we "tinker" with 
 // SelQueueFirst and SelQueueLast
 SelQueueFirst = N-sel;
 SelQueueLast = SelQueueFirst-1;

 ScQueueInit();
 // but we will only have sc small classes, we want to 
 // store them in ScQueue[N-sc .. N-1], hence we "tinker" with 
 // ScQueueFirst and ScQueueLast
 ScQueueFirst = N-sc;
 ScQueueLast = ScQueueFirst-1;

 for(j = 0; j <= FStartTop; j++) {  // traversing FStart array
  i = FStart[j];                    // traversing family f_j
  while(1) {
   if (FMember[j] == i) {      // skip it, it is big
     if (i == FPrev[FStart[j]]) break;
     i = FNext[i];
     continue;
   }
   // now we traverse c_i and insert all its elements to SelQueue 
   // and the first element in ScQueue
   k = CStart[i];
   ScQueueInsert(k);
   SelQueueInsert(k);
   if (CNext[CPrev[CStart[i]]] == 1) {
     if (i == FPrev[FStart[j]]) break;
     i = FNext[i];
     continue;
   }
   for(k = CNext[k]; k != CPrev[CStart[i]]; k = CNext[k]) 
    SelQueueInsert(k);
   SelQueueInsert(k);
   if (i == FPrev[FStart[j]]) break;
   i = FNext[i];
  }//endwhile
 }//endfor


 //finally we have to restore all family data structures to initial
 // state, it also reinitializes Refine[] to null's

 for(j = 0; j <= FStartTop; j++) { // traversing Family array
  FMember[j] = null;     // used to hold max guy for f_j
  // now traverse the family f_j 
  i = FStart[j];
  e = FPrev[FStart[j]];
  FStart[j] = null;
  while(1) {
   i1 = FNext[i];
   FNext[i] = null;
   FPrev[i] = null;
   if (i == e) break;
   i = i1;
  }//endwhile
 }//endfor
 FStartTop = null;
 
return 1;       // we have done some refinement

}//end NextLevel





// modified July 30, 2003
// function FSX10 -----------------------------------------------
void FSX10(char* string)
{

int i;

CreateData(string);  // allocate memory and initialize global data
CreateIndex(string); // create index for letters occuring in string
InitLevel(string);
        //printf("Level %d:\n",L);show_Classes();    // if desired
L = 1;
output_Reps(L);
while(1) {
  i = NextLevel();
  L++;
         //printf("Level %d:\n",L);show_Classes();   // if desired
  if (!i) break;
  output_Reps(L);
}//endwhile

DeallocateData();

}//end FSX10



/* ***********************************************
        FSX10 data and functions end
   *********************************************** */




#include <time.h>
// generate a random string
// function NewString ----------------------------------------
char* NewString(char* p)
{

static int first = 1;
static unsigned int t;
unsigned int size;
unsigned int asize;
char *string;
unsigned int i;

if (first) {
  time((time_t*) &t);
  srand(t);
  first = 0;
}


L1: t = rand();
    size = t % 50;
    if (size < 4)
      goto L1;
 
L2: t = rand();
    asize = t % 8;
    if (asize < 2)
      goto L2;

string = (char*) malloc(size+1);
 if (string == NULL) {
   printf("memory allocation problem\n");
   exit(1);
 }
 
 for(i = 0; i < size; i++) {
  t = rand();
  string[i] = 'a'+t % asize;
 }
 string[size] = '\0';

 if (p != NULL) {
   if (strcmp(p,string) == 0) {
     goto L1;
   }
   free((void*) p);
 }
 
 return string;

}//end NewString


void computeSingletonMRCS()
{
    for (uint64_t i = 0; i < n; ++i)
    {
        if ((i == 0 && ((n > 1 && string[i] != string[i + 1]) || (n == 1))) || // First character case
            (i == n - 1 && n > 1) ||     // Last character case
            (i > 0 && i < n - 1 && string[i] != string[i + 1])) // Middle characters case
        {
            MRC[i].push_back({1, 0});
        }
    }
}

void printMRCArray()
{
    printf("The MRC Array for the string %s : \n", string);
    for(uint64_t i = 0; i < n; ++i)
    {
        std::cout << i << ": "; 
        for(const auto& pair_length_border : MRC[i])
        {
            printf("(%ld,%ld), ", pair_length_border.first, pair_length_border.second);
        }
        printf("\n");
    }
}

// function main ----------------------------------------------
int main(int argc, char **argv) 
{

 //char* string = NewString(NULL);  // randomly generated string

    if (argc != 2) {
        fprintf(stderr, "Usage: ./closedStringsCrochemore <text_file without extension>\n");
        return -1;
    }

    // File pointers for string file
    FILE *fp = nullptr;

    // Open string file and read into W array
    fp = fopen(argv[1], "rb");
    if (fp == nullptr) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return -1;
    }

    // Get the number of integers in the .sa file
    fseek(fp, 0L, SEEK_END);
    n = ftell(fp);  // Each char is 1
    rewind(fp);

    // Allocate memory for W array
    string = (char*)malloc(sizeof(char) * (n));
    if (string == nullptr) {
        fprintf(stderr, "Memory allocation failed for W\n");
        fclose(fp);
        return -1;
    }

    // Read text file into W array
    fread(string, sizeof(char), n, fp);
    fclose(fp);

    string[n + 1] = '\0';
    MRC.resize(n, std::list<std::pair<uint64_t, uint64_t>>());
    //printf("input:\"%s\"\n",string);
    computeSingletonMRCS();
    FSX10(string);
    printMRCArray();
 
    return 0;

}//end main