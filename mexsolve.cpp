/*
    Send some info to child processes which are regular C code
*/

#include <mpi.h>
#include <cassert>
#include <mex.h>   

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    mexPrintf("========================\n");
    mexPrintf("Before calling init in MEX\n");
    mexPrintf("========================\n");
    
    MPI_Init(NULL, NULL);

    mexPrintf("========================\n");
    mexPrintf("After calling init in MEX\n");
    mexPrintf("========================\n");
    // check number of arguments
    // if (nrhs != 3)
    // {
    //      mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nrhs",
    //              "Three inputs required.");
    //      return;
    // }

    // if (nlhs != 1) {
    //      mexErrMsgIdAndTxt("MyToolbox:arrayProduct:nlhs",
    //              "One output required.");
    //      return;
    // }

    int mpi_rank, mpi_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    int something = 1989;

    // send something to childs
    for (int i=1; i<mpi_size; i++)
    {
        MPI_Send(&something, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    
    //associate outputs
    // mxArray *c_out_m = plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    // double *c = mxGetPr(c_out_m);
    // c[0] = (double)something;
    
    MPI_Finalize();
}
