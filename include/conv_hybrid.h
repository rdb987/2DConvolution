#ifndef convhybrid_H_
#define convhybrid_H_

#include<vector>
#include<fstream>
#include<mpi.h>

class ConvolutionH
{
	public:
		ConvolutionH(std::vector<int> input,std::vector<int> kernel ,  int r , int c, int kr, int kc);
		void Compute();
		void ComputeParallel(std::ofstream &);
		void Reset();
		void Print_to_File(std::ofstream &);
		~ConvolutionH();

	private:
		std::vector<int> input;
		std::vector<int> kernel;
		std::vector<int> output;
		int rows;
		int columns;
		int krows;
		int kcolumns;
		
};

#endif /* convhybrid_H_ */
