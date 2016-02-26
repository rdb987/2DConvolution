#include<iostream>
#include<cstdlib>
#include "conv_mpi.h"
int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int rank;
    int pcom;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &pcom);
	
	if (argc != 8)
	{
		if(rank==0){
		std::cerr << "Not valid number of args" << std::endl;
		std::cerr<<("Usage: #Rows #Columns #kernel_rows #Kernel_columns Input_Matrix Kernel Output_Matrix \n");
		}
		MPI_Finalize();
	    return 1;
	}
	
	
	
	int rows=strtol(argv[1],NULL,10);
	int columns=strtol(argv[2],NULL,10);
	int krows=strtol(argv[3],NULL,10);
	int kcolumns=strtol(argv[4],NULL,10);
	char *inputm=argv[5];
	char *kernel=argv[6];
	char *outputm=argv[7];

	
	std::ofstream outputfile(outputm,std::ios::out);
	
	if( krows%2==0 || kcolumns%2==0){
		if(rank==0)
			std::cerr<<"Number of kernels rows and columns has to be odd"<<std::endl;
		MPI_Finalize();
		return 1;
	}
		
	//INPUT MATRIX
	std::ifstream matrixfile(inputm,std::ios::in);
	if(!matrixfile){
		if(rank==0)
			std::cerr<<"Impossible to open input file"<<std::endl;
		MPI_Finalize();
		return 1;
	}
	
	std::vector<int> inputmatrix(rows*columns);

	for(int i = 0; i < rows; ++i){
		for(int j = 0; j < columns; ++j){
			matrixfile>>inputmatrix[i*columns+j];
		}
	}
	matrixfile.close();
	
	//KERNEL
	std::ifstream kernelfile(kernel,std::ios::in);
	if(!kernelfile){
		if(rank==0)
			std::cerr<<"Impossible to read kernel"<<std::endl;
		MPI_Finalize();
		return 1;
	}
	

	std::vector<int> kernelmatrix(krows*kcolumns);
	for(int i = 0; i < krows; ++i){
		for(int j = 0; j < kcolumns; ++j){
			kernelfile>>kernelmatrix[i*kcolumns+j];
		}
	}
	
	kernelfile.close();
	ConvolutionMPI C(inputmatrix,kernelmatrix, rows, columns, krows, kcolumns);
	C.ComputeParallel(outputfile);
	MPI_Finalize();
	return 0;
}



