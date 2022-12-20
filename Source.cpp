﻿#include <iostream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include "complex"
#include <vector>
#include <iterator>

using namespace std;

double epsilon = 0.000000001;

bool test_int(char* b)
{
	if (*b == '-') b++;
	if (*b == 0) return false;
	if (*b == '0' && *(b + 1) != 0) return false;
	while (*b)
	{
		if (*b < '0' || *b>'9') return false;
		b++;
	}
	return true;
}

bool test_double(char* b)
{
	if (*b == '-') b++;
	if (*b == 0 || *b == '.') return false;
	if (*b == '0' && (*(b + 1) != 0 && *(b + 1) != '.')) return false;
	while (*b != 0 && *b != '.')
	{
		if (*b < '0' || *b > '9') return false;
		b++;
	}
	if (*b == '.')
	{
		b++;
		if (*b == 0) return false;
		while (*b)
		{
			if (*b < '0' || *b > '9') return false;
			b++;
		}
	}
	return true;
}

bool test_complex(char* b)
{
	if (*b == '-') b++;
	if (*b == 0 || *b == '.' || *b == ' ') return false;
	if (*b == '0' && (*(b + 1) != ' ' && *(b + 1) != '.')) return false;
	while (*b != 0 && *b != '.' && *b != ' ')
	{
		if (*b < '0' || *b > '9') return false;
		b++;
	}
	if (*b == '.')
	{
		b++;
		if (*b == 0) return false;
		while (*b != 0 && *b != ' ')
		{
			if (*b < '0' || *b > '9') return false;
			b++;
		}
	}
	if (*b == 0) return false;
	b++;
	return test_double(b);
}

template <class Type>
Type scan(int n)
{
	bool i = false;
	while (true)
	{
		char* str = new char[256];
		cin.getline(str, 256);
		if (n == 1) i = test_int(str);
		if (n == 2) i = test_double(str);
		if (i)
		{
			Type x = 0;
			if (n == 1) x = atoi(str);
			if (n == 2) x = atof(str);
			delete[] str;
			return x;
		}
		else puts("Wrong data");
		delete[]str;
	}
}
template<>
complex<double> scan(int n)
{
	while (true)
	{
		char* str = new char[256];
		std::cin.getline(str, 256);
		if (test_complex(str))
		{
			char* tmp = str;
			while (*tmp != ' ') tmp++;
			tmp++;
			complex<double> y(atof(str), atof(tmp));
			delete[] str;
			return y;
		}
		else puts("Wrong data");
		delete[]str;
	}
}
template<>
complex<float> scan(int n)
{
	while (true)
	{
		char* str = new char[256];
		std::cin.getline(str, 256);
		if (test_complex(str))
		{
			char* tmp = str;
			while (*tmp != ' ') tmp++;
			tmp++;
			complex<float> y(atof(str), atof(tmp));
			delete[] str;
			return y;
		}
		else puts("Wrong data");
		delete[]str;
	}
}


template <class Type>
class matrix;

template <class Type>
Type scan2()
{
	if (typeid(Type) == typeid(int)) return scan<int>(1);
	if (typeid(Type) == typeid(double)) return scan<double>(2);
	if (typeid(Type) == typeid(float)) return scan<float>(2);
}
template<>
complex<double> scan2()
{
	return scan<complex<double>>(3);
}
template<>
complex<float> scan2()
{
	return scan<complex<float>>(3);
}



template <class Type>
bool operator == (const complex<Type>& v, bool f) {
	if (v.real() == f && v.imag() == f) return true;
	else return false;
	}
template <class Type>
Type determinant(const matrix<Type>&m, int N);


template <class Type>
class matrix
{
private:
	vector<vector<Type>> data;
	int columns, rows;

public:
	matrix()
	{
		columns = 3;
		rows = 3;
		for (int i = 0; i < 3; i++) {
			vector<Type> v(3);
			for (int j = 0; j < 3; j++) {
				v[j] = 0;
			}
			data.push_back(v); // возврат в матрицу
		}
	}

	matrix(int rows_, int columns_)
	{
		if (columns_ < 1 or rows_ < 1) throw "Invalid matrix size";

		columns = columns_;
		rows = rows_;



		cout << "Input your values" << "\n";
		for (int i = 0; i < rows; i++) {
			vector<Type> v(columns);
			for (int j = 0; j < columns; j++)
			{
				Type _temp;
				cout << "Input value of index [" << i + 1 << "][" << j + 1 << "]: ";
				v[j] = scan2<Type>();

				cout << endl;
			}
			data.push_back(v);
		}
	}

	matrix(const matrix&) = default;

	matrix(vector<vector<Type>> temp, int rows_, int columns_) {
		if (rows_ < 1 or columns_ < 1) throw "Invalid matrix size";

		rows = rows_;
		columns = columns_;



		for (int i = 0; i < rows; i++)
		{
			vector<Type> v(columns);
			for (int j = 0; j < columns; j++)
			{
				v[j] = temp[i][j];
			}
			data.push_back(v);
		}
	}

	matrix(const matrix& a, const matrix& b, const matrix& c)
	{
		if ((a.rows != 1 or a.columns != 3) or (b.rows != 1 or b.columns != 3) or (c.rows != 1 or c.columns != 3)) throw "Invalid matrix size";

		rows = columns = 3;

		for (int i = 0; i < 3; i++)
		{
			vector<Type> v(3);
			data.push_back(v);
		}
		for (int i = 0; i < rows; i++)
		{
			int j = 0;

			data[j][i] = a.data[0][i];
			j++;

			data[j][i] = b.data[0][i];
			j++;

			data[j][i] = c.data[0][i];
		}

	}

	~matrix() = default;

	//Getters
	int get_rows() const { return rows; }

	int get_columns() const { return columns; }

	// оператор () для чтения/записи элемента матрицы по указанным индексам;
	Type& operator()(int i, int j)
	{
		if ((i < 0 or i >= rows) or (j < 0 or j >= columns)) throw "Invalid index";
		return data[i][j];
	}

	const Type& operator()(int i, int j) const
	{
		if ((i < 0 or i >= rows) or (j < 0 or j >= columns)) throw "Invalid index";
		return data[i][j];
	}

	//операторы сложения и вычитания матриц;
	matrix& operator+=(const matrix& m)
	{
		if (rows != m.rows or columns != m.columns) throw "Size of matrix doesn't equal";

		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				data[i][j] = data[i][j] + m.data[i][j];
			}
		}
		return *this;
	}

	matrix operator+(const matrix& m) const
	{
		matrix temp(*this);
		temp += m;
		return temp;
	}

	matrix& operator-=(const matrix& m)
	{
		if (rows != m.rows or columns != m.columns) throw "Size of matrix doesn't equal";

		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < columns; ++j)
			{
				data[i][j] = data[i][j] - m.data[i][j];
			}
		}
		return *this;
	}

	matrix operator-(const matrix& m) const
	{
		matrix temp(*this);
		temp -= m;
		return temp;
	}

	//оператор умножения матриц;
	matrix& operator*=(const matrix& m)
	{
		if (columns != m.rows) throw "Size of matrix doesn't equal";

		for (int rows_ = 0; rows_ < rows; rows_++)
		{
			vector<Type> v(m.columns);
			for (int col = 0; col < m.columns; col++)
			{
				v.push_back(0);
				for (int in_column = 0; in_column < columns; in_column++)
				{
					v[col] += data[rows_][in_column] * m.data[in_column][col];
				}
			}
			data[rows_] = v;
		}

		columns = m.columns;
		return *this;
	}

	matrix operator*(const matrix& m) const
	{
		matrix temp(*this);
		temp *= m;
		return temp;
	}

	//оператор умножения матрицы на скаляр(обеспечить коммутативность);

	matrix& operator *= (double n)
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				data[i][j] *= n;
			}
		}
		return *this;
	}
	matrix operator*(double n)
	{
		matrix temp(*this);
		temp *= n;
		return temp;
	}
	//оператор деления матрицы на скаляр;
	matrix& operator/=(double n)
	{
		if (n == 0) throw "Division by zero";
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				data[i][j] /= n;
			}
		}
		return *this;
	}
	matrix operator/(double n)
	{
		matrix temp(*this);
		temp /= n;
		return temp;
	}

	//метод вычисления следа матрицы - сумма членов главной диагонали, при условии, что матрица - квадратичная
	Type trace()
	{
		if (rows != columns) throw "The matrix is not square";
		Type trace = 0;
		for (int i = 0, j = 0; i < rows; i++, j++)
		{
			trace += data[i][j];
		}
		return trace;
	}

	//Сравнение матриц
	bool operator==(const matrix& m)
	{
		if (rows != m.rows or columns != m.columns)
		{
			return false;
		}
		else
		{
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < columns; j++)
				{
					if (abs(data[i][j] - m.data[i][j]) > epsilon) {
						return false;
					}
				}
			}
			return true;
		}
	}
	bool operator!=(const matrix& m) {
		return !(*this == m);
	}

	friend Type determinant<>(const matrix<Type>& m, int N);

	matrix& operator = (const matrix&) = default;

	friend matrix<Type>& operator*= (double n, matrix<Type>& m)
	{
		for (int i = 0; i < m.get_rows(); i++)
		{
			for (int j = 0; j < m.get_columns(); j++)
			{
				m(i, j) *= n;
			}
		}
		return m;
	}

	friend matrix<Type> operator* (double n, matrix<Type>& m)
	{
		matrix temp(m);
		return n *= temp;
	}

	auto begin()
	{
		return data.begin();

	}

	auto end()
	{
		return data.end();
	}

	friend ostream& operator << (ostream& os, const matrix& m)
	{
		for (auto i : m.data)
		{
			for (auto j : i)
			{
				os << left << j << '\t';
			}
			os << endl;
		}
		return os;
	}

	friend bool coplanarns(const matrix& a, const matrix& b, const matrix& c)
	{
		matrix final(a, b, c);
		cout << final;
		Type det = determinant(final, final.get_columns());
		if (det == 0) return true;
		else return false;
	}

};

template <class Type>
Type determinant(const matrix<Type>& m, int N)
{
	if (m.columns != m.rows)
	{
		return 0;
	}
	else if (N == 1) {
		return m.data[0][0];
	}
	else if (N == 2) {
		return m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0];
	}
	else if (N >= 3) {
		Type determ = 0;

		for (int k = 0; k < N; k++) {

			vector<vector<Type>> temp(N - 1, vector<Type>(N - 1));

			for (int i = 0; i < N; i++)
			{
				for (int j = 1; j < N; j++)
				{
					if (i > k)
					{
						temp[i - 1][j - 1] = m.data[i][j];
					}
					if (i < k)
					{
						temp[i][j - 1] = m.data[i][j];
					}
				}
			}

			determ += pow(-1, k + 2) * m.data[k][0] * determinant(matrix<Type>(temp, N - 1, N - 1), N - 1);
		}
		return determ;
	}
}

int main()
{
	std::cout << "1 - Work with int data" << std::endl;
	std::cout << "2 - Work with double data" << std::endl;
	std::cout << "3 - Work with complex<double> data" << std::endl;
	while (true)
	{
		int zl = getch();
		if (zl == '1')
		{
			matrix<int> a;
			matrix<int> b;
			double n = 1;
			while (true)
			{
				system("cls");
				cout << "first matrix\n\n";
				cout << a << endl;
				cout << "second matrix\n\n";
				cout << b << endl;
				cout << "1 - Create matrix" << endl;
				cout << "2 - Enter value by index" << endl;
				cout << "3 - Matrix addition" << endl;
				cout << "4 - Matrix subtraction" << endl;
				cout << "5 - Matrix multiplication" << endl;
				cout << "6 - Multiplication matrix by scalar" << endl;
				cout << "7 - Division matrix by scalar" << endl;
				cout << "8 - Matrix trace calculation" << endl;
				cout << "9 - Task" << endl;
				cout << "0 - Matrix comparison" << endl;
				cout << "esc - Exit" << endl;
				int z = getch();
				int l = '3';
				system("cls");
				if (z == '1' || z == '2' || (z > '5' && z < '9'))
				{
					cout << "1 - Work with first matrix" << endl;
					cout << "2 - Work with second matrix" << endl;
					while (l != '1' && l != '2') l = getch();
					system("cls");
				}
				if (z == '6' || z == '7')
				{
					cout << "Enter scalar value" << endl;
					n = scan<double>(2);
					system("cls");
				}
				if (z == '1')
				{
					try
					{
						int rows, columns;
						cout << "Enter number of rows" << endl;
						rows = scan<int>(1);
						cout << "Enter number of columns" << endl;
						columns = scan<int>(1);
						matrix<int> d(rows, columns);
						if (l == '1') a = d;
						else b = d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (getch()) z = 0;
					}
					system("cls");
				}
				if (z == '2')
				{
					int index_row, index_column;
					int value;
					cout << "Enter index_row" << endl;
					index_row = scan<int>(1);
					cout << "Enter index_column" << endl;
					index_column = scan<int>(1);
					cout << "Enter value" << endl;
					value = scan<int>(1);
					try
					{
						if (l == '1') a(index_row, index_column) = value;
						else b(index_row, index_column) = value;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (getch()) z = 0;
					}
					system("cls");
				}
				if (z == '3')
				{
					try
					{
						matrix<int> d = a + b;
						cout << "Matrix addition" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '4')
				{
					try
					{
						matrix<int> d = a - b;
						cout << "Matrix subtraction" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '5')
				{
					try
					{
						matrix<int> d = a * b;
						cout << "Matrix multiplication" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '6')
				{
					matrix<int> d;
					matrix<int> f;
					cout << "Multiplication matrix by scalar" << endl;
					if (l == '1') d = a * n;
					else d = b * n;
					cout << d;
					cout << "\nMultiplication scalar by matrix" << endl;
					if (l == '1') f = n * a;
					else f = n * b;
					cout << f;
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '7')
				{
					try
					{
						matrix<int> d;
						if (l == '1') d = a / n;
						else d = b / n;
						cout << "Division matrix by scalar" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '8')
				{
					double trace_ = 0;
					try
					{
						if (l == '1') trace_ = a.trace();
						else trace_ = b.trace();
						cout << "Matrix trace" << endl;
						cout << trace_;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '9')
				{
					cout << "Create first vector" << endl;
					matrix<int> a1(1, 3);
					cout << "Create second vector" << endl;
					matrix<int> a2(1, 3);
					cout << "Create third vector" << endl;
					matrix<int> a3(1, 3);
					if (coplanarns(a1, a2, a3)) cout << "Vectors are coplanar";
					else cout << "Vectors are not coplanar";
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '0')
				{
					cout << "Enter epsilon" << endl;
					epsilon = scan<double>(2);
					if (a == b) cout << "Matrices are the same";
					if (a != b) cout << "Matrices are different";
					if (getch()) z = 0;
				}
				if (z == 27)
				{
					return 0;
				}
			}
		}

		if (zl == '2')
		{
			matrix<double> a;
			matrix<double> b;
			double n = 1;
			while (true)
			{
				system("cls");
				cout << "first matrix\n\n";
				cout << a << endl;
				cout << "second matrix\n\n";
				cout << b << endl;
				cout << "1 - Create matrix" << endl;
				cout << "2 - Enter value by index" << endl;
				cout << "3 - Matrix addition" << endl;
				cout << "4 - Matrix subtraction" << endl;
				cout << "5 - Matrix multiplication" << endl;
				cout << "6 - Multiplication matrix by scalar" << endl;
				cout << "7 - Division matrix by scalar" << endl;
				cout << "8 - Matrix trace calculation" << endl;
				cout << "9 - Task" << endl;
				cout << "0 - Matrix comparison" << endl;
				cout << "esc - Exit" << endl;
				int z = getch();
				int l = '3';
				system("cls");
				if (z == '1' || z == '2' || (z > '5' && z < '9'))
				{
					cout << "1 - Work with first matrix" << endl;
					cout << "2 - Work with second matrix" << endl;
					while (l != '1' && l != '2') l = getch();
					system("cls");
				}
				if (z == '6' || z == '7')
				{
					cout << "Enter scalar value" << endl;
					n = scan<double>(2);
					system("cls");
				}
				if (z == '1')
				{
					try
					{
						int rows, columns;
						cout << "Enter number of rows" << endl;
						rows = scan<int>(1);
						cout << "Enter number of columns" << endl;
						columns = scan<int>(1);
						matrix<double> d(rows, columns);
						if (l == '1') a = d;
						else b = d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (getch()) z = 0;
					}
					system("cls");
				}
				if (z == '2')
				{
					int index_row, index_column;
					double value;
					cout << "Enter index_row" << endl;
					index_row = scan<int>(1);
					cout << "Enter index_column" << endl;
					index_column = scan<int>(1);
					cout << "Enter value" << endl;
					value = scan<double>(2);
					try
					{
						if (l == '1') a(index_row, index_column) = value;
						else b(index_row, index_column) = value;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (getch()) z = 0;
					}
					system("cls");
				}
				if (z == '3')
				{
					try
					{
						matrix<double> d = a + b;
						cout << "Matrix addition" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '4')
				{
					try
					{
						matrix<double> d = a - b;
						cout << "Matrix subtraction" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '5')
				{
					try
					{
						matrix<double> d = a * b;
						cout << "Matrix multiplication" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '6')
				{
					matrix<double> d;
					matrix<double> f;
					cout << "Multiplication matrix by scalar" << endl;
					if (l == '1') d = a * n;
					else d = b * n;
					cout << d;
					cout << "\nMultiplication scalar by matrix" << endl;
					if (l == '1') f = n * a;
					else f = n * b;
					cout << f;
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '7')
				{
					try
					{
						matrix<double> d;
						if (l == '1') d = a / n;
						else d = b / n;
						cout << "Division matrix by scalar" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '8')
				{
					double trace_ = 0;
					try
					{
						if (l == '1') trace_ = a.trace();
						else trace_ = b.trace();
						cout << "Matrix trace" << endl;
						cout << trace_;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '9')
				{
					cout << "Create first vector" << endl;
					matrix<double> a1(1, 3);
					cout << "Create second vector" << endl;
					matrix<double> a2(1, 3);
					cout << "Create third vector" << endl;
					matrix<double> a3(1, 3);
					if (coplanarns(a1, a2, a3)) cout << "Vectors are coplanar";
					else cout << "Vectors are not coplanar";
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '0')
				{
					cout << "Enter epsilon" << endl;
					epsilon = scan<double>(2);
					if (a == b) cout << "Matrices are the same";
					if (a != b) cout << "Matrices are different";
					if (getch()) z = 0;
				}
				if (z == 27)
				{
					return 0;
				}
			}
		}

		if (zl == '3')
		{
			matrix<complex<double>> a;
			matrix<complex<double>> b;
			double n = 1;
			while (true)
			{
				system("cls");
				cout << "first matrix\n\n";
				cout << a << endl;
				cout << "second matrix\n\n";
				cout << b << endl;
				cout << "1 - Create matrix" << endl;
				cout << "2 - Enter value by index" << endl;
				cout << "3 - Matrix addition" << endl;
				cout << "4 - Matrix subtraction" << endl;
				cout << "5 - Matrix multiplication" << endl;
				cout << "6 - Multiplication matrix by scalar" << endl;
				cout << "7 - Division matrix by scalar" << endl;
				cout << "8 - Matrix trace calculation" << endl;
				cout << "9 - Task" << endl;
				cout << "0 - Matrix comparison" << endl;
				cout << "esc - Exit" << endl;
				int z = getch();
				int l = '3';
				system("cls");
				if (z == '1' || z == '2' || (z > '5' && z < '9'))
				{
					cout << "1 - Work with first matrix" << endl;
					cout << "2 - Work with second matrix" << endl;
					while (l != '1' && l != '2') l = getch();
					system("cls");
				}
				if (z == '6' || z == '7')
				{
					cout << "Enter scalar value" << endl;
					n = scan<double>(2);
					system("cls");
				}
				if (z == '1')
				{
					try
					{
						int rows, columns;
						cout << "Enter number of rows" << endl;
						rows = scan<int>(1);
						cout << "Enter number of columns" << endl;
						columns = scan<int>(1);
						matrix<complex<double>> d(rows, columns);
						if (l == '1') a = d;
						else b = d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (getch()) z = 0;
					}
					system("cls");
				}
				if (z == '2')
				{
					int index_row, index_column;
					complex<double> value;
					cout << "Enter index_row" << endl;
					index_row = scan<int>(1);
					cout << "Enter index_column" << endl;
					index_column = scan<int>(1);
					cout << "Enter value" << endl;
					value = scan<complex<double>>(3);
					try
					{
						if (l == '1') a(index_row, index_column) = value;
						else b(index_row, index_column) = value;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (getch()) z = 0;
					}
					system("cls");
				}
				if (z == '3')
				{
					try
					{
						matrix<complex<double>> d = a + b;
						cout << "Matrix addition" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '4')
				{
					try
					{
						matrix<complex<double>> d = a - b;
						cout << "Matrix subtraction" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '5')
				{
					try
					{
						matrix<complex<double>> d = a * b;
						cout << "Matrix multiplication" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '6')
				{
					matrix<complex<double>> d;
					matrix<complex<double>> f;
					cout << "Multiplication matrix by scalar" << endl;
					if (l == '1') d = a * n;
					else d = b * n;
					cout << d;
					cout << "\nMultiplication scalar by matrix" << endl;
					if (l == '1') f = n * a;
					else f = n * b;
					cout << f;
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '7')
				{
					try
					{
						matrix<complex<double>> d;
						if (l == '1') d = a / n;
						else d = b / n;
						cout << "Division matrix by scalar" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '8')
				{
					complex<double> trace_ = 0;
					try
					{
						if (l == '1') trace_ = a.trace();
						else trace_ = b.trace();
						cout << "Matrix trace" << endl;
						cout << trace_;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '9')
				{
					cout << "Create first vector" << endl;
					matrix<complex<double>> a1(1, 3);
					cout << "Create second vector" << endl;
					matrix<complex<double>> a2(1, 3);
					cout << "Create third vector" << endl;
					matrix<complex<double>> a3(1, 3);
					if (coplanarns(a1, a2, a3)) cout << "Vectors are coplanar";
					else cout << "Vectors are not coplanar";
					if (getch()) z = 0;
					system("cls");
				}
				if (z == '0')
				{
					cout << "Enter epsilon" << endl;
					epsilon = scan<double>(2);
					if (a == b) cout << "Matrices are the same";
					if (a != b) cout << "Matrices are different";
					if (getch()) z = 0;
				}
				if (z == 27)
				{
					return 0;
				}
			}
		}
	}
}