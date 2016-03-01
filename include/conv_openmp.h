#ifndef convmp_H_
#define convmp_H_

#include<vector>
#include<fstream>

class Convolutionmp
{
	public:
		Convolutionmp(std::vector<int> input,std::vector<int> kernel ,  int r , int c, int kr, int kc);
		void Compute();
		void Reset();
		void Print_to_File(std::ofstream &);
		~Convolutionmp();

	private:
		std::vector<int> input;
		std::vector<int> kernel;
		std::vector<int> output;
		int rows;
		int columns;
		int krows;
		int kcolumns;
};

#endif /* convmp_H_ */
