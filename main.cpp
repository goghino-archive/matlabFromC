#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <unistd.h>     /* gethostname */
#include "engine.h"

using namespace std;

int main(int argv, char* argc[])
{

    MPI_Init(&argv, &argc);

    /* workaround to attach GDB */
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    printf("Process with PID %d on %s ready to run\n", getpid(), hostname);
    fflush(stdout);

    int mpi_rank, mpi_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    if (mpi_size < 2) {
        printf("Run with minimum of two processes: mpirun -np 2 [...].\n");
        return -3;
    }


    // if rank == MASTER
    if(mpi_rank == 0)
    {

        //call matlab
        // char comm[500] = "matlab -nosplash -nodisplay -nojvm -nodesktop -r \"mexsolve\"";
        // cout << "Calling the command: " << comm  << " at the MASTER node"<< endl;
        // system(comm);

        Engine* ep;
        if (!(ep = engOpen("")))
        {
            fprintf(stderr, "\n *** Can't start MATLAB engine! *** \n");
            return EXIT_FAILURE;
        }

        engEvalString(ep, "mexsolve");
    }
    else
    {
        int something;
        MPI_Recv(&something, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        cout << "[Child" << mpi_rank << "] recieved " << something << endl;
    }


    MPI_Finalize();

    return 0;
}
