#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_sem.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

/**
 * You might find these declarations useful.
 */
enum Endianness {LITTLE = 0, BIG = 1};
const static enum Endianness oppositeEnd [] = {BIG, LITTLE};

struct Well {
  // TODO
  uthread_sem_t mutex;
  uthread_sem_t done;
  uthread_sem_t littleBigCond[2];
  int entered;
  int endianEnter[2];
  int endianWaiting[2];
  int otherEndWaiting;
  enum Endianness endianness;
};

struct Well* createWell() {
  struct Well* Well = malloc (sizeof (struct Well));
  // TODO
  Well->mutex = uthread_sem_create(1);
  Well->done = uthread_sem_create(0);
  Well->littleBigCond[LITTLE] = uthread_sem_create(0);
  Well->littleBigCond[BIG] = uthread_sem_create(0);
  Well->entered = 0;
  Well->endianEnter[LITTLE] = 0;
  Well->endianEnter[BIG] = 0;
  Well->endianWaiting[LITTLE] = 0;
  Well->endianWaiting[BIG] = 0;
  Well->otherEndWaiting = 0;
  Well->endianness = 0;
  return Well;
}

struct Well* Well;

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_sem_t   waitingHistogramMutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void recordEntry (enum Endianness g) {
  if (Well->entered == 0 || Well->endianness == g) {
    if (Well->entered < MAX_OCCUPANCY) {
        Well->entered++;
        Well->endianness = g;
        entryTicker++;
        occupancyHistogram[Well->endianness][Well->entered]++;
    }
  }
}

void enterWell (enum Endianness g) {
  // TODO
  uthread_sem_wait(Well->mutex);
  int canGoIn = (Well->entered == 0 || (Well->entered < MAX_OCCUPANCY && Well->endianness == g 
  && !(Well->endianWaiting[oppositeEnd[g]] > 0 && Well->endianWaiting[oppositeEnd[g]]> FAIR_WAITING_COUNT)));
  if (canGoIn) {
    recordEntry(g);
  } else {
    Well->endianWaiting[g]++;
  }
  uthread_sem_signal(Well->mutex);
  if (!canGoIn) {
    uthread_sem_wait(Well->littleBigCond[g]);
  }
    
}


void leaveWell() {
  // TODO
  uthread_sem_wait(Well->mutex);
  enum Endianness currEnd = Well->endianness;
  enum Endianness otherEnd = oppositeEnd[currEnd];
  Well->entered--;

  if ((!(Well->endianWaiting[currEnd] > 0) || Well->endianWaiting[otherEnd] > FAIR_WAITING_COUNT)
        && Well->endianWaiting[otherEnd] > 0) {
    if (Well->entered == 0) {
      for (int i = 0; i < Well->endianWaiting[otherEnd] && i < MAX_OCCUPANCY; i++) {
        Well->endianWaiting[otherEnd]--;
        recordEntry(otherEnd);
        uthread_sem_signal(Well->littleBigCond[otherEnd]);
      }
    }
  } else if (Well->endianWaiting[currEnd]) {
    Well->endianWaiting [currEnd]--;
    recordEntry(currEnd);
    uthread_sem_signal (Well->littleBigCond[currEnd]);
  }
  uthread_sem_signal (Well->mutex);
}

void recordWaitingTime (int waitingTime) {
  // TODO
  uthread_sem_wait(waitingHistogramMutex);
    if (waitingTime < WAITING_HISTOGRAM_SIZE)
      waitingHistogram[waitingTime]++;
    else
      waitingHistogramOverflow++;
  uthread_sem_signal(waitingHistogramMutex);
}

//
// TODO
// You will probably need to create some additional produres etc.
//

void* gateKeeper() {
  enum Endianness endianness = random() % 2;
  int startingValue;
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    startingValue = entryTicker;
    enterWell(endianness);
    recordWaitingTime(entryTicker - startingValue - 1);
    for (int j = 0; j < NUM_PEOPLE; j++) {
      uthread_yield();
    }
    leaveWell();
    for (int k =  0; k < NUM_PEOPLE; k++) {
      uthread_yield();
    }
  }
  uthread_sem_signal(Well->done);
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (1);
  Well = createWell();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogramMutex = uthread_sem_create(1);

  // TODO
  for (int i = 0; i < NUM_PEOPLE; i++)
    uthread_detach (uthread_create (gateKeeper, Well));
  
  for (int j = 0; j < NUM_PEOPLE; j++)
    uthread_sem_wait(Well->done);

  printf ("Times with 1 little endian %d\n", occupancyHistogram [LITTLE]   [1]);
  printf ("Times with 2 little endian %d\n", occupancyHistogram [LITTLE]   [2]);
  printf ("Times with 3 little endian %d\n", occupancyHistogram [LITTLE]   [3]);
  printf ("Times with 1 big endian    %d\n", occupancyHistogram [BIG] [1]);
  printf ("Times with 2 big endian    %d\n", occupancyHistogram [BIG] [2]);
  printf ("Times with 3 big endian    %d\n", occupancyHistogram [BIG] [3]);
  printf ("Waiting Histogram\n");
  for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
  if (waitingHistogram [i])
  printf ("  Number of times people waited for %d %s to enter: %d\n", i, i==1?"person":"people", waitingHistogram [i]);
  if (waitingHistogramOverflow)
  printf ("  Number of times people waited more than %d entries: %d\n", WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
