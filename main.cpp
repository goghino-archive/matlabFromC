#include <mpi.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <unistd.h>     /* gethostname */
#include "engine.h"

using namespace std;

int main(int argv, char* argc[])
{
    cout << "========================" << endl;
    cout << "I am about to call MPI-INIT" << endl;
    cout << "========================" << endl;
    MPI_Init(NULL, NULL);

    /* workaround to attach GDB */
    char hostname[2048];
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
        cout << "========================" << endl;
        cout << "I am about to call MATLAB" << endl;
        cout << "========================" << endl;

        //call matlab
        const int SYSTEM = 1;
        if(SYSTEM)
        {
            // use system call
            char comm[500] = "matlab -nosplash -nodisplay -nojvm -nodesktop -r \"interface\"";
            cout << "Calling the command: " << comm  << " at the MASTER node"<< endl;
            system(comm);
        }
        else
        {
            // use Matlab engine
            Engine* ep;
            if (!(ep = engOpen("")))
            {
                fprintf(stderr, "\n *** Can't start MATLAB engine! *** \n");
                return EXIT_FAILURE;
            }

            cout << "Calling the Matlab Engine with command 'interface' at the MASTER node"<< endl;
            engEvalString(ep, "interface");
        }

        cout << "========================" << endl;
        cout << "MATLAB call returned" << endl;
        cout << "========================" << endl;
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
