#ifndef convmpi_H_
#define convmpi_H_

#include<vector>
#include<fstream>
#include<mpi.h>

class ConvolutionMPI
{
	public:
		ConvolutionMPI(std::vector<int> input,std::vector<int> kernel ,  int r , int c, int kr, int kc);
		void Compute();
		void ComputeParallel(std::ofstream &);
		void Reset();
		void Print_to_File(std::ofstream &);
		~ConvolutionMPI();

	private:
		std::vector<int> input;
		std::vector<int> kernel;
		std::vector<int> output;
		int rows;
		int columns;
		int krows;
		int kcolumns;
};

#endif /* convmpi_H_ */
