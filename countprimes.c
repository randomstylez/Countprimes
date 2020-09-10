#include "mpi.h"
#include <stdio.h>
#include <unistd.h>


/* Author: Joseph Pietroluongo
// PID: 5901749
// I affirm that I wrote this program myself without any help
// from any other people or sources from the internet
// Summary: This program is designed in C language, for the purpose to
// count the number of prime numbers in the numbers
// from 1 to n inclusive where n is a long integer. 
// The value for n which should be set in the program
// using a constant should be 50,000
*/


// set constant 50,000
#define N 50000

int check_prime(int id, int n)
{

// check if n is 1 or 0.
  if (n == 0 || n == 1)
  {
    return 0;
  }
  int i;

//checks all even numbers
  for (i = 2; i <= n / 2; i++)
  {
    if (n % i == 0)
    {
      return 0;
    }
  }

  printf("%d) %d\n", id, n);
  fflush(stdout);
  return 1;
}


int main(int argc, char *argv[])
{
  int count;
  double timeTaken;
  int totalCount;
  int i;
  int id;
  int p;
  char hostname[1024];

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  hostname[1023] = '\0';
  gethostname(hostname, 1023);
  printf("MPI ranked: %d on host %s\n", id, hostname);

  MPI_Barrier(MPI_COMM_WORLD);
  timeTaken = -MPI_Wtime();

  count = 0;
  for (i = id; i < N; i += p)
    count += check_prime(id, i);

  MPI_Reduce(&count, &totalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  timeTaken += MPI_Wtime();

  if (!id)
  {
    printf("Execution completed in: %8.6f seconds\n", timeTaken);
    fflush(stdout);
  }
  MPI_Finalize();
  if (!id)
  {
    printf("Counted %d different prime numbers.\n", totalCount);
    return 0;
  }
}