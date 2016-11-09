// ################################
//  A lottery implementation
//  Date: 2012
//  Author: Selim
// ################################

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
  int tcId;
  int numberOfColumns;
  int coupon[20][6];
  float spMoney;
  float acMoney;
  int threeTrueInColumn;
  int fourTrueInColumn;
  int fiveTrueInColumn;
  int sixTrueInColumn;
} USER;

typedef struct{
  char lotteryName[30];
  int oneColumnCost;
  int numberOfPlayers;
  int totalOfColumnsPlayed;
  int theNumbers[6];
  long totalIncome;
  float X;
  float SIX;
  float FIVE;
  float FOUR;
  float THREE;
  long transferredMoney;
  int numberOfThreeColumns;
  int numberOfFourColumns;
  int numberOfFiveColumns;
  int numberOfSixColumns;
} LOTTERY;

void printMenu(int *op);

void printMenuAfterDraw(int *op);

USER* addUser( USER *prePointer, int size );

void playColumns(USER *player, int numberOfColumns);

void printUserInfo(USER *players, int start, int finish);

void showInfoAboutLottery(LOTTERY lot);

float calcSpendMoney(float oneColumnCost, int numberOfColumns);

int calcTotalOfColumnsPlayed(USER *players, int numberOfPlayers);

long calcTotalIncome(USER *players, int numberOfPlayers);

void calcTrueNumberForEachUser(int *theNumbers, USER *players, int numberOfUsers);

void calcAboutMoney(LOTTERY *lot, USER *players, int numberOfUsers);

void draw(int *theNumbers);

void listParticipantByAcMoney(USER *players, int numberOfUsers);

void cmpColumnToTheNumbers(int *column, int *theNumbers);

int getUserID(int N);

int main(){
  USER *players;
  LOTTERY lot;
  int M;
  int N;
  int i,j,k;
  int operation;
  int tcId;
  int numberOfColumns;
  int col;
  int newGame=1;

  while( newGame == 1 ){
    srand(time(NULL));

    // get the info about the lottery
    printf("The name of the loto > ");
    scanf("%s", lot.lotteryName);
    printf("The price of one column > ");
    scanf("%d", &lot.oneColumnCost);
    printf("In the beginning how many user would be added > ");
    scanf("%d", &N);
    printf("How many columns would each of these user play > ");
    scanf("%d", &M);
    printf("the percent of the money would be paid to the participants > ");
    scanf("%3f", &lot.X);
    printf("the percent of the given money the participants with 3 true numbers would win>");
    scanf("%f", &lot.THREE);
    printf("the percent of the given money the participants with 4 true numbers would win>");
    scanf("%f", &lot.FOUR);
    printf("the percent of the given money the participants with 5 true numbers would win>");
    scanf("%f", &lot.FIVE);
    printf("the percent of the given money the participants with 6 true numbers would win>");
    scanf("%f", &lot.SIX);

    // Create the memory for players array
    players = (USER*)malloc( N * sizeof(USER) );
    if( players == NULL ){ printf("Couldnt allocate the memory!"); return 0; }


    // N users play M columns
    for( i=0; i<N; i++ ){
      players[i].tcId = i;
      players[i].numberOfColumns = M;
      for( j=0; j<M; j++ ){
        for( k=0; k<6; k++ ){
          players[i].coupon[j][k] = rand() % 48 + 1; // 1..49
        }
      }
      // calc price of the columns played
      players[i].spMoney = calcSpendMoney(lot.oneColumnCost, M);
    }

    lot.numberOfPlayers = N;
    lot.totalOfColumnsPlayed = calcTotalOfColumnsPlayed(players, N);
    lot.totalIncome = calcTotalIncome(players, N);

    do{
      // print Menu and get the operation
      printMenu(&operation);

      switch(operation){
        case 1: // add new user
          N++; // inc number of the user
          players = addUser(players, N); // returns pointer; reallocating memory
          players[N-1].tcId = N-1;

          printf("\nHow many columns would this user play > ");
          scanf("%d", &numberOfColumns);

          players[N-1].numberOfColumns = numberOfColumns;
          playColumns(&players[N-1], numberOfColumns);
          players[N-1].spMoney = calcSpendMoney(lot.oneColumnCost, numberOfColumns);

          break;
        case 2: // play new column
          // get the user's tcId
          tcId = getUserID(N);

          printf("How many columns would this user play > ");
          scanf("%d", &numberOfColumns);
          players[tcId].spMoney = calcSpendMoney(lot.oneColumnCost, numberOfColumns);
          break;
        case 3: // show the info of lottery
          showInfoAboutLottery(lot);
          break;
        case 4: // look at the ınfo of one user
          tcId = getUserID(N);
          // Display info
          printUserInfo(players, tcId, tcId+1); // display info for just one user.
          break;
        case 5: // draw
          draw(lot.theNumbers);
          break;
          // default is unneccesary because check is done when getting input.
      } // end of switch

      // Update the ınfo of lottery
      lot.numberOfPlayers = N;
      lot.totalOfColumnsPlayed = calcTotalOfColumnsPlayed(players, N);
      lot.totalIncome = calcTotalIncome(players, N);
      printf("\nOperation is successful!\n");
    }while( operation != 5 );

    // After the draw, make calculations
    calcTrueNumberForEachUser(lot.theNumbers, players, N);
    calcAboutMoney(&lot, players, N);

    printf("\n ###### The list of the participants by acquired money : \n");
    listParticipantByAcMoney(players, N);
    printf("\n\n The transferred Money : %ld", lot.transferredMoney);
    printf("\n Tne number of the colums with three true numbers : %d", lot.numberOfThreeColumns);
    printf("\n Tne number of the colums with four true numbers : %d", lot.numberOfFourColumns);
    printf("\n Tne number of the colums with five true numbers : %d", lot.numberOfFiveColumns);
    printf("\n Tne number of the colums with six true numbers : %d", lot.numberOfSixColumns);

    do{
      printMenuAfterDraw(&operation);

      switch(operation){
        case 1: // look at a user by id
          tcId = getUserID(N); // send number of the users.
          // Display info
          printUserInfo(players, tcId, tcId+1);
          break;
        case 2: // check a column
          tcId = getUserID(N);

          do{
            printf("Which columns of the user > ");
            scanf("%d", &col);
            if( players[tcId].numberOfColumns < col ){
              printf("Error : This user did not play that many columns!\n");
            }
          }while( players[tcId].numberOfColumns < col );

          cmpColumnToTheNumbers(players[tcId].coupon[col], lot.theNumbers);
          break;
        case 3:
          printf("\nStarting a new loto...");
          break;
        case 4:
          printf("Exiting...");
          newGame = 0;
          break;
      }

    }while( operation != 4 );

  } // newgame while
  return 0;
}

float calcSpendMoney(float oneColumnCost, int numberOfColumns){
  int spMoney;
  spMoney = oneColumnCost * numberOfColumns;
  return spMoney;
}

void printMenu(int *op){
  printf("\n 1 - Add new user");
  printf("\n 2 - Play new column(s)");
  printf("\n 3 - Show the information about the lottery");
  printf("\n 4 - Look at the info of one user by tcId");
  printf("\n 5 - Draw");

  do{
    printf("\n\nWhich operation > ");
    scanf("%d", op);
  }while( *op<=0 || *op>5 );
}

USER* addUser( USER *prePointer, int size ){
  USER *newPointer;

  newPointer = (USER*)realloc( prePointer, size * sizeof(USER) );
  if( newPointer == NULL ){ printf("Couldnt allocate the memory!"); exit(0); }

  return newPointer;
}

void playColumns(USER *player, int numberOfColumns){
  int i,k;

  for( i=player->numberOfColumns; i<numberOfColumns; i++ ){
    for( k=0; k<6; k++ ){
      player->coupon[i][k] = rand() % 48 + 1;
    }
  }
}

void printUserInfo(USER *players, int start, int finish){ // all user start=0; finish=N, for 1 user start=tcID finish=tcId+1
  int i;

  printf("\n");
  printf("tcID numberOfColumns SpendMoney AcMoney        3True 4True 5True 6True");
  for( i=start; i<finish; i++ ){
    printf("\n");
    printf("%4d  ", players[i].tcId);
    printf("%12d  ", players[i].numberOfColumns);
    printf("%10f  ", players[i].spMoney);
    printf("%10f  ", players[i].acMoney);
    printf("%5d  ", players[i].threeTrueInColumn);
    printf("%5d  ", players[i].fourTrueInColumn);
    printf("%5d  ", players[i].fiveTrueInColumn);
    printf("%5d  ", players[i].sixTrueInColumn);
  }
}

void showInfoAboutLottery(LOTTERY lot){
  printf("\n");
  printf("####### The Information About the LOTTERY : \n");
  printf("# The name of the lottery : %s\n", lot.lotteryName);
  printf("# The price of one column : %d\n", lot.oneColumnCost);
  printf("# Total income of the lottey (for now) : %ld", lot.totalIncome);
  printf("# The number of participants (for now) : %d\n", lot.numberOfPlayers);
  printf("# The total number of columns played (for now) : %d\n", lot.totalOfColumnsPlayed);
  printf("# %%%f of the income would be paid to the participants\n", lot.X);
  printf("# Participants with 6 true numbers would win %%%f\n", lot.SIX);
  printf("# 5 true numbers would win %%%f\n", lot.FIVE);
  printf("# 4 true numbers would win %%%f\n", lot.FOUR);
  printf("# 3 true numbers would win %%%f of the delivered money.\n", lot.THREE);
}

int calcTotalOfColumnsPlayed(USER *players, int numberOfPlayers){
  int total=0;
  int i;

  for( i=0; i<numberOfPlayers; i++){
    total = total + players[i].numberOfColumns;
  }

  return total;
}

long calcTotalIncome(USER *players, int numberOfPlayers){
  long total=0;
  int i;

  for( i=0; i<numberOfPlayers; i++){
    total = total + players[i].spMoney;
  }

  return total;
}

void draw(int *theNumbers){
  int i;
  printf("\n");
  printf("~~~~~~~~~~~~~~~~~~~~~\n");
  printf("The Numbers : ");
  for( i=0; i<6; i++ ){
    theNumbers[i] = rand() % 48 + 1;
    printf("  %d", theNumbers[i]);
  }
  printf("\n~~~~~~~~~~~~~~~~~~~~~");
  printf("\n");
}

void calcTrueNumberForEachUser(int *theNumbers, USER *players, int numberOfUsers){
  int i,j,k,l;
  int numberOfTrue;

  for( i=0; i<numberOfUsers; i++ ){
    for( j=0; j<players[i].numberOfColumns; j++ ){
      numberOfTrue = 0;
      for( k=0; k<6; k++ ){
        for( l=0; l<6; l++ ){
          if( theNumbers[k] == players[i].coupon[j][l] ){
            numberOfTrue++;
          }
        }
      }
      switch(numberOfTrue){
        case 6:
          players[i].sixTrueInColumn++;
          break;
        case 5:
          players[i].fiveTrueInColumn++;
          break;
        case 4:
          players[i].fourTrueInColumn++;
          break;
        case 3:
          players[i].threeTrueInColumn++;
          break;
      }
    }
  }
}

void calcAboutMoney(LOTTERY *lot, USER *players, int numberOfUsers){
  float deliveredMoney;
  int numberOfThreeColumns=0;
  int numberOfFourColumns=0;
  int numberOfFiveColumns=0;
  int numberOfSixColumns=0;
  int i;

  lot->transferredMoney = lot->totalIncome;
  // calc the money will be delivered
  deliveredMoney = (lot->totalIncome * 0.01) * lot->X;

  // calc number of columns with 3,4,5,6 true
  for( i=0; i<numberOfUsers; i++ ){
    numberOfThreeColumns += players[i].threeTrueInColumn;
    numberOfFourColumns += players[i].fourTrueInColumn;
    numberOfFiveColumns += players[i].fiveTrueInColumn;
    numberOfSixColumns += players[i].sixTrueInColumn;
  }
  lot->numberOfThreeColumns = numberOfThreeColumns;
  lot->numberOfFourColumns = numberOfFourColumns;
  lot->numberOfFiveColumns = numberOfFiveColumns;
  lot->numberOfSixColumns = numberOfSixColumns;

  // calc acquired money for each user
  for( i=0; i<numberOfUsers; i++ ){
    // cannot be divided by zero
    if( lot->numberOfThreeColumns != 0 ){
      players[i].acMoney += players[i].threeTrueInColumn * ((deliveredMoney / 100) * lot->THREE) / lot->numberOfThreeColumns;
    }
    if( lot->numberOfFourColumns != 0 ){
      players[i].acMoney += players[i].fourTrueInColumn * ((deliveredMoney / 100) * lot->FOUR) / lot->numberOfFourColumns;
    }
    if( lot->numberOfFiveColumns != 0 ){
      players[i].acMoney += players[i].fiveTrueInColumn * ((deliveredMoney / 100) * lot->FIVE) / lot->numberOfFiveColumns;
    }
    if( lot->numberOfSixColumns != 0 ){
      players[i].acMoney += players[i].sixTrueInColumn * ((deliveredMoney / 100) * lot->SIX) / lot->numberOfSixColumns;
    }

    // Calc the transferred money
    lot->transferredMoney -= players[i].acMoney;
  }
}

void listParticipantByAcMoney(USER *players, int numberOfUsers){
  int i,j,max;
  USER tmp;

  for(i=0; i<numberOfUsers-1; i++){
    max = i;
    for( j=i+1; j<numberOfUsers; j++ ){
      if( players[max].acMoney < players[j].acMoney ){
        max = j;
      }
    }
    tmp = players[i];
    players[i] = players[max];
    players[max] = tmp;
  }
  printUserInfo(players, 0 , numberOfUsers);
}

void printMenuAfterDraw(int *op){
  printf("\n");
  printf("\n 1 - Look at an user info by tcId");
  printf("\n 2 - Compare a column to the loto numbers");
  printf("\n 3 - Start a new loto");
  printf("\n 4 - Exit");

  do{
    printf("\n\nWhich operation > ");
    scanf("%d", op);
  }while( *op<=0 || *op>4 );
}

void cmpColumnToTheNumbers(int *column, int *theNumbers){
  int i,j,true=0;

  printf("\nGiven Column : ");
  for( i=0; i<6; i++ ){
    printf("  %2d", column[i]);
  }

  printf("\nThe Numbers  : ");
  for( i=0; i<6; i++ ){
    printf("  %2d", theNumbers[i]);
  }

  for( i=0; i<6; i++ ){
    for( j=0; j<6; j++ ){
      if( column[i] == theNumbers[j] ){
        true++;
        printf("\nKnown number : %d ", theNumbers[j]);
      }
    }
  }

  printf("\nTotal of the true numbers : %d", true);
}

int getUserID(int N){
  int tcId;
  do{
    printf("\nEnter the tcId of the user > ");
    scanf("%d", &tcId);
    if( tcId >= N ){
      printf("Error: There is no user for the given tcID!\n");
    }
  }while( tcId >= N );

  return tcId;
}

