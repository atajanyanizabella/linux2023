#include <pthread.h>
#include <iostream>
#include <vector>
#include <fstream>

//check if file opened successfully
void is_open(std::ifstream& input_file, std::string name)
{
    if (!input_file.is_open()) {
        std::cout << name << ": Couldn't open the file" << std::endl;
        exit(0);
    }
}

//read matrix elements from file to structure
void get_matrix(std::vector<std::vector<int>>& matrix, std::ifstream& input_file)
{
	int i = 0;
	int j = 0;
	while (input_file >> matrix[i][j]) {
        	if (++j >= matrix[i].size()) {
            		j = 0;
            		++i;
        	}
    	}
}

//each thread has particular operations to do
struct pairs
{
    std::vector<int> oper_num;
};

//function gets by argument structure type of matrix_t
struct matrix_t
{
	std::vector<std::vector<int>> matrix1;
	std::vector<std::vector<int>> matrix2;
	std::vector<std::vector<int>> result;
    pairs argument;
};

void* mult(void* arg)
{
    //getting row and col by operation number: E.G oper_num = {0, 1, 2} => row = 0 / 4, col = 0 % 4 => [0][0], row = 1 / 4, cols = 1 % 4 => [0][1]
    matrix_t* matrixes = (matrix_t*) arg;
    for (const auto& item : matrixes->argument.oper_num) {
        int sum = 0;
        for (int i = 0; i < matrixes->matrix1.size(); ++i) {
            sum += matrixes->matrix1[item / 4][i] * matrixes->matrix2[i][item % 4];
        }
        matrixes->result[item / 4][item % 4] = sum;
    }
    return NULL;
}

//only for matrix size of 4*4
int main()
{
	std::cout << "Enter matrix size: " << std::endl;
	int size = 0;
	std::cin >> size;

    //open first file
	std::ifstream matrix1_file("matrix1.txt");
    //check if opened successfully
	is_open(matrix1_file, "matrix1.txt");
    //create first matrix
	std::vector<std::vector<int>> matrix1(size, std::vector<int>(size, 0));
    //read from file to matrix
	get_matrix(matrix1, matrix1_file);
    //close the file
	matrix1_file.close();
    //open second file
	std::ifstream matrix2_file("matrix2.txt");
    //check if opened successfully
	is_open(matrix2_file, "matrix2.txt");
    //create second matrix
	std::vector<std::vector<int>> matrix2(size, std::vector<int>(size, 0));
    //read from file to matrix
	get_matrix(matrix2, matrix2_file);
    //close the file
	matrix2_file.close();
    //create result matrix
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));

    //create matrix_t structure
    matrix_t* matrixes = new matrix_t;
    matrixes->matrix1 = matrix1;
    matrixes->matrix2 = matrix2;
    matrixes->result = result;

	std::cout << "Enter thread count: " << std::endl;
    int thread_count = 0;
	std::cin >> thread_count;
    int operation_count = 16;
    int operation_number = 0;
    int per_thread = operation_count / thread_count;
    int oper_remain = operation_count % thread_count;
    //create thread array
	pthread_t* threads = new pthread_t[thread_count];
	for (int i = 0; i < thread_count; ++i) {
        pairs* arg = new pairs;
        for (int j = 0; j < per_thread; ++j) {
            arg->oper_num.push_back(operation_number++);
        }
        if (oper_remain > 0) {
            arg->oper_num.push_back(operation_number++);
            oper_remain--;
        }
        matrixes->argument = *arg;
        pthread_create(&threads[i], NULL, mult, (void*)matrixes);
        delete arg;
	}
    //join threads
    for (int i = 0; i < thread_count; ++i) {
        int result = pthread_join(threads[i], NULL);
        if (result != 0) {
            exit(result);
        }
    }
    //print result
    for (int i = 0; i < matrixes->result.size(); ++i) {
        for (int j = 0; j < matrixes->result[i].size(); ++j) {
            std::cout << matrixes->result[i][j] << " ";
        }
        std::cout << std::endl;
    }
    delete matrixes;
    return 0;
}
