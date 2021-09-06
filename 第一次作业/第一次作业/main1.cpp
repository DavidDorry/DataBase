# include<iostream>
# include"p0_starter.h"

int main() {
    std::cout << "test matrix generating..." << std::endl;
    std::cout << "------------------------------" << std::endl;
	scudb::RowMatrix<int> matA(2, 3);
    for (int i = 0; i < matA.GetRowCount(); ++i){
        for (int j = 0; j < matA.GetColumnCount(); ++j){
            matA.SetElement(i, j, i * matA.GetColumnCount() + j);
        }
    }
    
    std::vector<int> src1(6);
    for (int i = 0; i < 6; i++) {
        src1[i] = 2;
    }
    scudb::RowMatrix<int> matB(3, 2);
    matB.FillFrom(src1);

    std::vector<int> src2(4);
    for (int i = 0; i < 4; i++) {
        src2[i] = 6-i;
    }
    scudb::RowMatrix<int> matC(2, 2);
    matC.FillFrom(src2);

    std::vector<int> src3(4);
    for (int i = 0; i < 4; i++) {
        src3[i] = i+2;
    }
    scudb::RowMatrix<int> matD(2, 2);
    matD.FillFrom(src3);
    
    std::cout << "matrixA:" << std::endl;
    scudb::RowMatrixOperations<int>::Print(matA);
    std::cout << "matrixB:" << std::endl;
    scudb::RowMatrixOperations<int>::Print(matB);
    std::cout << "matrixC:" << std::endl;
    scudb::RowMatrixOperations<int>::Print(matC);
    std::cout << "matrixD:" << std::endl;
    scudb::RowMatrixOperations<int>::Print(matD);

    std::unique_ptr<scudb::RowMatrix<int>> ret1;
    ret1 = scudb::RowMatrixOperations<int>::Add(&matC, &matD);
    std::cout << "ret1 = matrixC + matrixD:" << std::endl;
    scudb::RowMatrixOperations<int>::Print(*ret1);

    std::unique_ptr<scudb::RowMatrix<int>> ret2;
    ret2 = scudb::RowMatrixOperations<int>::Multiply(&matA, &matB);
    std::cout << "ret2 = matrixA * matrixB:" << std::endl;
    scudb::RowMatrixOperations<int>::Print(*ret2);

    std::unique_ptr<scudb::RowMatrix<int>> ret3;
    ret3 = scudb::RowMatrixOperations<int>::GEMM(&matA, &matB, &matC);
    std::cout << "ret3 = matrixA * matrixB + matrixC:" << std::endl;
    scudb::RowMatrixOperations<int>::Print(*ret3);

	return 0;
}
