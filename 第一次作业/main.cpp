#include"p0_starter.h"
using namespace scudb;

int main() {

	// 生成并展示三个矩阵
	RowMatrix<int> M1(3, 4);
	vector<int> M1_data = { 
		3, 2, 5, 4,
		8, 10, 2, 1,
		3, 5, 5, 9,
	};
	M1.FillFrom(M1_data);

	RowMatrix<int> M2(3, 4);
	vector<int> M2_data = {
		7, 8, 5, 6,
		2, 0, 8, 9,
		7, 5, 5, 1,
	};
	M2.FillFrom(M2_data);

	RowMatrix<int> M3(3, 3);
	vector<int> M3_data = {
		4, 1, 7,
		2, 7, 9,
		3, 5, 2,
	};
	M3.FillFrom(M3_data);

	cout << "M1:" << endl;
	RowMatrixOperations<int>::Show(M1);
	cout << "M2:" << endl;
	RowMatrixOperations<int>::Show(M2);
	cout << "M3:" << endl;
	RowMatrixOperations<int>::Show(M3);


	// 矩阵加法
	unique_ptr<RowMatrix<int>> ANS1;
	ANS1 = RowMatrixOperations<int>::Add(&M1, &M2);
	cout << "ANS1 = M1 + M2:" << endl;
	RowMatrixOperations<int>::Show(*ANS1);

	// 矩阵乘法
	unique_ptr<RowMatrix<int>> ANS2;
	ANS2 = RowMatrixOperations<int>::Multiply(&M3, &M2);
	cout << "ANS2 = M3 · M2:" << endl;
	RowMatrixOperations<int>::Show(*ANS2);

	// 通用矩阵乘
	unique_ptr<RowMatrix<int>> ANS3;
	ANS3 = RowMatrixOperations<int>::GEMM(&M3, &M2, &M1);
	cout << "ANS3 = M3 · M2 + M1:" << endl;
	RowMatrixOperations<int>::Show(*ANS3);

	return 0;
}
