#include "mpi.h"
#include <cstdio>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <math.h>
using namespace std;
#define POINTS 1000000

int main (int argc,  char *argv[]) 
{

   int myid, numprocs;
   int namelen;
   double z;
   double ratio;
   double pi;
   double delta;
   int count = 0;
   double x;
   double y;
   int i;
   char processor_name[MPI_MAX_PROCESSOR_NAME];

   MPI_Init(&argc, &argv); 
   MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Get_processor_name(processor_name, &namelen);
 
   //Random Number Generation Seed
   srand((unsigned)(myid)); 

   printf("Process %d on %s\n", myid, processor_name);


   //Taking the ratio of the POINTS divided by the number of processor
   int s = (int)floor(POINTS/numprocs);
   //Modding the POINTS and processor to add the remainder to catch any extra values
   int s0 = s + POINTS%numprocs;

   //Setting the start index to be where s0 is + the current processor ID
   int startIndex = s0 + (myid-1)*s;
   //Setting the end index to be the start index + s (the points divided by processors)
   int endIndex = startIndex + s;

   double startwtime;
   if (myid == 0) 
   {
      startwtime = MPI_Wtime();
   }
   
   if (myid == 0) 
   {
      // master worker - comput the master's numbers
      for (int i=0; i<s0; i++)
      {
         // calculating whether or not we count the product
         x = ((double)rand())/((double)RAND_MAX);
         y = ((double)rand())/((double)RAND_MAX);
         z = sqrt((x*x)+(y*y));
         if (z<=1)
         {
            ++count;
         }
      }
         printf("Process %d - startIndex 0 endIndex %d; points in the circle %i\n",
               myid, s0-1, count);
   } 
   else 
   {
      //slave's work
      for (i= startIndex; i<endIndex; i++)
      {
         //Generating random numbers for x and y coords
         x = ((double)rand())/((double)RAND_MAX);
         y = ((double)rand())/((double)RAND_MAX);
         z = sqrt((x*x)+(y*y));
         if (z<=1)
         {
            ++count;
         }
      }
         printf ("Process %d - startIndex %d endIndex %d; points in the circle %i\n",
               myid, startIndex, endIndex-1, count);
   }

   int sum;
   MPI_Reduce(&count, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

   if (myid == 0) 
   {
      double runTime = MPI_Wtime() - startwtime;
      printf("Execution time (sec) = %f sum = %i \n",
             runTime, sum);
      ratio = (double)sum/(double)POINTS;
      pi = 4*ratio;
      delta = M_PI - pi;
      printf("Ratio = %f Pi (theoretical) = %f Delta = %f \n",
             ratio, pi, delta); 
   }

   MPI_Finalize();

}

