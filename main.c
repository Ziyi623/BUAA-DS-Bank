#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#define LIM 7
#define MINWINDOW 3
#define MAXWINDOW 5

typedef struct customer{
    int customerIndex;
    int startTurn;
    int costTurn;
    int waitTurn;
}customer;

int cmp(const void*a,const void*b){
    customer ca = *(customer*)a;
    customer cb = *(customer*)b;
    return ca.costTurn - cb.costTurn;
}


int main(){
    int customerAddTurn;
    int customerIndexCount = 1;
    int customerNumsPerTurn[100] = {0};
    customer customers[500];
    customer customerQueue[500];
//  Data Read
    scanf("%d",&customerAddTurn);
    for(int i=0;i<customerAddTurn;i++)
        scanf("%d",&customerNumsPerTurn[i]);
    int customerCount=0;
    for(int i=0;i<customerAddTurn;i++){
        for(int j=0;j<customerNumsPerTurn[i];j++){
            scanf("%d",&customers[customerCount].costTurn);
            customers[customerCount].startTurn = i;
            customers[customerCount].customerIndex = customerIndexCount++;
            customers[customerCount].waitTurn = -1;
            customerCount++;
        }
    }
    int turnCount = 0;
    int customerAddPos = 0;
    int serveStartPos = 0;
    int serveEndPos = 0;
    int openWindow = 3;
    int waitNums = 0;
    int decreaseFlag = 0;
    while(1){
    //  Turn start
        if(turnCount<customerAddTurn){
            for(int i=0;i<customerNumsPerTurn[turnCount];i++){
                customerQueue[serveEndPos++] = customers[customerAddPos++];
                waitNums++;
            }
            // judge if add more window
            if(openWindow<MAXWINDOW&&waitNums>=openWindow*LIM){
                openWindow++;
                if(openWindow<MAXWINDOW&&waitNums>=openWindow*LIM)
                    openWindow++;
                // add 2 maximum
            }
        }
    // Turn Process
        // Business
        int workWindow = 0; // workWindow may not equal to openWindow
        for(workWindow = 0;workWindow <openWindow;workWindow++){
            int currentPos = serveStartPos+workWindow;
            if(currentPos==serveEndPos)
                break;
            customerQueue[currentPos].costTurn--;
            if(customerQueue[currentPos].waitTurn == -1){
                customerQueue[currentPos].waitTurn = turnCount - customerQueue[currentPos].startTurn;
                waitNums--;
            }
        }
        // Window Decrease Judge
        if(openWindow>MINWINDOW && waitNums<LIM*openWindow)
            decreaseFlag=1;// no possibility to decrease twice
        // Update Queue
        int outNums = 0;
        for(int i=0;i<workWindow;i++){
            int currentPos = serveStartPos+i;
            if(customerQueue[currentPos].costTurn == 0){
                if(decreaseFlag==1){
                    openWindow--;
                    decreaseFlag=0;
                }
                customers[customerQueue[currentPos].customerIndex-1].waitTurn \
                = customerQueue[currentPos].waitTurn;
                // Delete Flag
                customerQueue[currentPos].customerIndex = 0;
                outNums++;
            }
        }
        // an interesting trick,you could take a second to think about this
        qsort(customerQueue+serveStartPos,workWindow,sizeof(customer),cmp);
        serveStartPos += outNums;

    // Turn End
        // End Judge
        if(turnCount>=customerIndexCount && serveEndPos==serveStartPos)
            break;

        turnCount++;
    }
    for(int i=0;i<customerIndexCount-1;i++)
        printf("%d : %d\n",customers[i].customerIndex,customers[i].waitTurn);
    return 0;
}