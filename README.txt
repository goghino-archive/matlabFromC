1. kardos@icsmaster01 (master):~/misc/matlabMEX$ module list
   Currently Loaded Modulefiles:
     1) use.own         2) gcc/6.1.0       3) openmpi/2.0.1   4) matlab/R2016a

    OpenMPI was configured as following: --prefix=${INSTALLDIR} --enable-mpi-fortran=all --with-pmi --disable-dlopen
    The important flag when using MEX is "--disable-dlopen" which specifies that  all plugins will be slurped into Open
    MPI's libraries and it will cause that Open MPI will not look for / open any DSOs at run time
    (https://www.open-mpi.org/faq/?category=building#avoid-dso). To elaborate more on this: Open MPI uses a bunch of
    plugins for its functionality.  When you dlopen libmpi in a private namespace (like Matlab doesi???),
    and then libmpi tries to dlopen its  plugins, the plugins can't find the symbols that they need in the main libmpi
    library (because they're in a private namespace).

2. Program architecture is designed in a following way: the main program is started with as many processes as needed
   from the very beginning using mpirun. The master process launches Matlab via system() command or using Matlab
   Engine API. Matlab executes simple script that calls MexFunction(). Mex function initializes MPI and tries to
   communicate with the rest of the processes that were started in the very beginning. 

   system() command or Matlab Engine are internally using fork() which seems to break MPI communication. To test if
   this is supported by OpenFabrics check for variable btl_openib_have_fork_support
   (https://www.open-mpi.org/faq/?category=openfabrics#ofa-fork).

   Try to enable fork support and fail if it is not available by setting '--mca btl_openib_want_fork_support 1'

3. $ make

4. $ make run
