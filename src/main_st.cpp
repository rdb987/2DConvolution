#include<iostream>
#include "serial.h"
int main(int argc, char **argv)
{
	if (argc != 8)
	{
	    std::cerr << "Not valid number of args" << std::endl;
	    std::cerr<<("Usage: #Rows #Columns #kernel_rows #Kernel_columns Input_Matrix Kernel Output_Matrix \n");
	    return 1;
	}
	
	int rows=strtol(argv[1],NULL,10);
	int columns=strtol(argv[2],NULL,10);
	int krows=strtol(argv[3],NULL,10);
	int kcolumns=strtol(argv[4],NULL,10);
	char *inputm=argv[5];
	char *kernel=argv[6];
	char *outputm=argv[7];
	if( krows%2==0 || kcolumns%2==0)
	{
		std::cerr<<"Number of kernel rows and columns must be odd"<<std::endl;
		return 1;
	}

	//INPUT MATRIX
	std::ifstream matrixfile(inputm,std::ios::in);
	if(!matrixfile)
	{
		std::cerr<<"Impossible to read input matrix"<<std::endl;
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
	if(!kernelfile)
	{
		std::cerr<<"Impossible to read kernel"<<std::endl;
		return 1;
	}
	

	std::vector<int> kernelmatrix(krows*kcolumns);
	for(int i = 0; i < krows; ++i){
			for(int j = 0; j < kcolumns; ++j){
				kernelfile>>kernelmatrix[i*kcolumns+j];
			}
	}
	kernelfile.close();
	std::ofstream outputfile(outputm,std::ios::out);
	Convolution C(inputmatrix,kernelmatrix, rows, columns, krows, kcolumns);
	C.Compute();
	C.Print_to_File(outputfile);
	outputfile.close();
	return 0;
}



