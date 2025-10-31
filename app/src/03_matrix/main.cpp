#include "matrix.h"
#include "tuple.h"
#include <iostream>

int main() {
  auto identity_matrix = Matrix<4, 4>::identity();
  std::cout << "What happens when you invert the identity matrix?\n"
            << identity_matrix.inverse() << "\n\tWe get inversed matrix.\n";
  auto a =
      Matrix<4, 4>{{6, 4, 4, 4}, {5, 5, 7, 6}, {4, -9, 3, -7}, {9, 1, 7, -6}};

  std::cout << "What do you get when you multiply a matrix by its inverse?\n";
  std::cout << a * a.inverse();
  std::cout << "\n\t We got identity matrix.\n";

  std::cout << "Is there any difference between the inverse of the transpose "
               "of a matrix, and the transpose of the inverse?\n";
  std::cout << "matrix.inverse.transpose =\n"
            << a.inverse().transpose() << "\nmatrix.transpose.inverse:\n"
            << a.transpose().inverse();
  std::cout << "\n\tThere is no difference.\n";

  std::cout
      << "Remember how multiplying the identity matrix by a tuple gives you "
         "the tuple, unchanged? Now, try changing any single element of the "
         "identity matrix to a different number, and then multiplying it by a "
         "tuple. What happens to the tuple?\n";
  Tuple b{1, 2, 3, 4};
  identity_matrix(1, 1) = 2;
  std::cout << identity_matrix << std::endl;
  std::cout << (identity_matrix * b) << std::endl;
  identity_matrix(1, 1) = 1;
  identity_matrix(0, 1) = 2;
  std::cout << identity_matrix << std::endl;
  std::cout << (identity_matrix * b) << std::endl;
  std::cout << "\tIf we change 1 on diagonal, the component will be scaled. If "
               "we change 0 we will mix one coordinate into another.";
}
