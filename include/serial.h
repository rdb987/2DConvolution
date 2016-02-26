#ifndef serial_H_
#define serial_H_

#include<vector>
#include<fstream>

class Convolution
{
	public:
		Convolution(std::vector<int> input,std::vector<int> kernel ,  int r, int c, int kr, int kc);
		void Compute();
		void Reset();
		void Print_to_File(std::ofstream &);
		~Convolution();

	private:
		std::vector<int> input;
		std::vector<int> kernel;
		std::vector<int> output;
		int rows;
		int columns;
		int krows;
		int kcolumns;
};

#endif /* serial_H_ */
