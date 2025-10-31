#include "matrix.h"
#include "tuple.h"
#include <catch2/catch.hpp>

TEST_CASE("matrix - constructor") {
  SECTION("4x4") {
    const auto matrix = Matrix<4, 4>{{1, 2, 3, 4},
                                     {5.5, 6.5, 7.5, 8.5},
                                     {9, 10, 11, 12},
                                     {13.5, 14.5, 15.5, 16.5}};

    REQUIRE(matrix.At<0, 0>() == Approx(1));
    REQUIRE(matrix.At<0, 3>() == Approx(4));
    REQUIRE(matrix.At<1, 0>() == Approx(5.5));
    REQUIRE(matrix.At<1, 2>() == Approx(7.5));
    REQUIRE(matrix.At<2, 2>() == Approx(11));
    REQUIRE(matrix.At<3, 0>() == Approx(13.5));
    REQUIRE(matrix.At<3, 2>() == Approx(15.5));
  }

  SECTION("2x2") {
    const auto matrix = Matrix<2, 2>{{-3, 5}, {1, -2}};
    REQUIRE(matrix.At<0, 0>() == Approx(-3));
    REQUIRE(matrix.At<0, 1>() == Approx(5));
    REQUIRE(matrix.At<1, 0>() == Approx(1));
    REQUIRE(matrix.At<1, 1>() == Approx(-2));
  }

  SECTION("3x3") {
    const auto matrix = Matrix<3, 3>{{-3, 5, 0}, {1, -2, 7}, {0, 1, 1}};
    REQUIRE(matrix.At<0, 0>() == Approx(-3));
    REQUIRE(matrix.At<1, 1>() == Approx(-2));
    REQUIRE(matrix.At<2, 2>() == Approx(1));
  }
}

TEST_CASE("matrix - equality operator") {
  SECTION("with identical matrices") {
    const auto m1 =
        Matrix<4, 4>{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    const auto m2 =
        Matrix<4, 4>{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    REQUIRE(m1 == m2);
  }

  SECTION("with different matrices") {
    const auto m1 =
        Matrix<4, 4>{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    const auto m2 =
        Matrix<4, 4>{{2, 3, 4, 5}, {6, 7, 8, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};
    REQUIRE_FALSE(m1 == m2);
  }
}

TEST_CASE("matrix - unequality operator") {
  SECTION("with identical matrices") {
    const auto m1 =
        Matrix<4, 4>{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    const auto m2 =
        Matrix<4, 4>{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    REQUIRE_FALSE(m1 != m2);
  }

  SECTION("with different matrices") {
    const auto m1 =
        Matrix<4, 4>{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    const auto m2 =
        Matrix<4, 4>{{2, 3, 4, 5}, {6, 7, 8, 9}, {8, 7, 6, 5}, {4, 3, 2, 1}};
    REQUIRE(m1 != m2);
  }
}

TEST_CASE("matrix - multiplication operator") {
  SECTION("matrix<4,4> * matrix<4,4>") {
    const auto expected = Matrix<4, 4>{{20, 22, 50, 48},
                                       {44, 54, 114, 108},
                                       {40, 58, 110, 102},
                                       {16, 26, 46, 42}};

    const auto m1 =
        Matrix<4, 4>{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 8, 7, 6}, {5, 4, 3, 2}};
    const auto m2 =
        Matrix<4, 4>{{-2, 1, 2, 3}, {3, 2, 1, -1}, {4, 3, 6, 5}, {1, 2, 7, 8}};
    REQUIRE(m1 * m2 == expected);
  }

  SECTION("matrix<4,4> * tuple") {
    const auto matrix =
        Matrix<4, 4>{{1, 2, 3, 4}, {2, 4, 4, 2}, {8, 6, 4, 1}, {0, 0, 0, 1}};
    const auto tuple = Tuple{1, 2, 3, 1};
    REQUIRE(matrix * tuple == Tuple{18, 24, 33, 1});

    SECTION("matrix<4,4> * identity matrix") {
      const auto matrix = Matrix<4, 4>{
          {0, 1, 2, 4}, {1, 2, 3, 8}, {2, 4, 8, 16}, {4, 8, 16, 32}};
      REQUIRE(matrix * Matrix<4, 4>::identity() == matrix);
    }

    SECTION("identity matrix * tuple") {
      const auto t = Tuple(1, 2, 3, 4);
      REQUIRE(Matrix<4, 4>::identity() * t == t);
    }
  }
}

TEST_CASE("matrix - transpose") {
  SECTION("standard matrix") {
    const auto expected =
        Matrix<4, 4>{{0, 9, 1, 0}, {9, 8, 8, 0}, {3, 0, 5, 5}, {0, 8, 3, 8}};

    const auto matrix =
        Matrix<4, 4>{{0, 9, 3, 0}, {9, 8, 0, 8}, {1, 8, 5, 3}, {0, 0, 5, 8}};

    REQUIRE(matrix.transpose() == expected);
  }

  SECTION("identity matrix") {
    const auto identity = Matrix<4, 4>::identity();
    REQUIRE(identity.transpose() == identity);
  }
}

TEST_CASE("matrix - determinant") {
  SECTION("matrix<2,2>") {
    const auto matrix = Matrix<2, 2>{{1, 5}, {-3, 2}};
    REQUIRE(matrix.determinant() == Approx(17));
  }

  SECTION("matrix<3,3>") {
    const auto a = Matrix<3, 3>{{1, 2, 6}, {-5, 8, -4}, {2, 6, 4}};
    REQUIRE(a.cofactor(0, 0) == Approx(56));
    REQUIRE(a.cofactor(0, 1) == Approx(12));
    REQUIRE(a.cofactor(0, 2) == Approx(-46));
    REQUIRE(a.determinant() == Approx(-196));
  }

  SECTION("matrix<4,4>") {
    const auto a = Matrix<4, 4>{
        {-2, -8, 3, 5}, {-3, 1, 7, 3}, {1, 2, -9, 6}, {-6, 7, 7, -9}};
    REQUIRE(a.cofactor(0, 0) == Approx(690));
    REQUIRE(a.cofactor(0, 1) == Approx(447));
    REQUIRE(a.cofactor(0, 2) == Approx(210));
    REQUIRE(a.cofactor(0, 3) == Approx(51));
    REQUIRE(a.determinant() == Approx(-4071));
  }
}

TEST_CASE("matrix - submatrix") {
  SECTION("matric<3,3>") {
    const auto expected = Matrix<2, 2>{{-3, 2}, {0, 6}};

    const auto matrix = Matrix<3, 3>{{1, 5, 0}, {-3, 2, 7}, {0, 6, -3}};
    REQUIRE(matrix.submatrix(0, 2) == expected);
  }

  SECTION("matrix<4,4>") {
    const auto expected = Matrix<3, 3>{{-6, 1, 6}, {-8, 8, 6}, {-7, -1, 1}};

    const auto matrix = Matrix<4, 4>{
        {-6, 1, 1, 6}, {-8, 5, 8, 6}, {-1, 0, 8, 2}, {-7, 1, -1, 1}};
    REQUIRE(matrix.submatrix(2, 1) == expected);
  }
}

TEST_CASE("matrix - minor") {
  SECTION("matrix<3,3>") {
    const auto a = Matrix<3, 3>{{3, 5, 0}, {2, -1, -7}, {6, -1, 5}};
    const auto b = a.submatrix(1, 0);
    REQUIRE(b.determinant() == Approx(25));
    REQUIRE(a.minor(1, 0) == Approx(25));
  }
}

TEST_CASE("minor - cofactor") {
  SECTION("matrix<3,3>") {
    const auto a = Matrix<3, 3>{{3, 5, 0}, {2, -1, -7}, {6, -1, 5}};
    REQUIRE(a.minor(0, 0) == Approx(-12));
    REQUIRE(a.cofactor(0, 0) == Approx(-12));
    REQUIRE(a.minor(1, 0) == Approx(25));
    REQUIRE(a.cofactor(1, 0) == Approx(-25));
  }
}

TEST_CASE("matrix - isInvertible") {
  SECTION("invertible") {
    const auto a =
        Matrix<4, 4>{{6, 4, 4, 4}, {5, 5, 7, 6}, {4, -9, 3, -7}, {9, 1, 7, -6}};

    REQUIRE(a.determinant() == Approx(-2120));
    REQUIRE(a.isInvertible());
  }

  SECTION("not invertible") {
    const auto a = Matrix<4, 4>{
        {-4, 2, -2, -3}, {9, 6, 2, 6}, {0, -5, 1, -5}, {0, 0, 0, 0}};

    REQUIRE(a.determinant() == Approx(0));
    REQUIRE_FALSE(a.isInvertible());
  }
}

TEST_CASE("matrix - inverse") {
  SECTION("first") {
    const auto expected = Matrix<4, 4>{{0.21805, 0.45113, 0.24060, -0.04511},
                                       {-0.80827, -1.45677, -0.44361, 0.52068},
                                       {-0.07895, -0.22368, -0.05263, 0.19737},
                                       {-0.52256, -0.81391, -0.30075, 0.30639}};

    const auto a = Matrix<4, 4>{
        {-5, 2, 6, -8}, {1, -5, 1, 8}, {7, 7, -6, -7}, {1, -3, 7, 4}};
    const auto b = a.inverse();

    REQUIRE(a.determinant() == Approx(532));
    REQUIRE(a.cofactor(2, 3) == Approx(-160));
    REQUIRE(b.At<3, 2>() == Approx(-160.f / 532));
    REQUIRE(a.cofactor(3, 2) == Approx(105));
    REQUIRE(b.At<2, 3>() == Approx(105.f / 532));
    REQUIRE(b == expected);
  }

  SECTION("second") {
    const auto expected =
        Matrix<4, 4>{{-0.15385, -0.15385, -0.28205, -0.53846},
                     {-0.07692, 0.12308, 0.02564, 0.03077},
                     {0.35897, 0.35897, 0.43590, 0.92308},
                     {-0.69231, -0.69231, -0.76923, -1.92308}};

    const auto a = Matrix<4, 4>{
        {8, -5, 9, 2}, {7, 5, 6, 1}, {-6, 0, 9, 6}, {-3, 0, -9, -4}};

    REQUIRE(a.inverse() == expected);
  }

  SECTION("third") {
    const auto expected = Matrix<4, 4>{{-0.04074, -0.07778, 0.14444, -0.22222},
                                       {-0.07778, 0.03333, 0.36667, -0.33333},
                                       {-0.02901, -0.14630, -0.10926, 0.12963},
                                       {0.17778, 0.06667, -0.26667, 0.33333}};

    const auto a = Matrix<4, 4>{
        {9, 3, 0, 9}, {-5, -2, -6, -3}, {-4, 9, 6, 4}, {-7, 6, 6, 2}};
    REQUIRE(a.inverse() == expected);
  }

  SECTION("multiplying product by its inverse") {
    const auto a = Matrix<4, 4>{
        {3, -9, 7, 3}, {3, -8, 2, -9}, {-4, 4, 4, 1}, {-6, 5, -1, 1}};
    const auto b =
        Matrix<4, 4>{{8, 2, 2, 2}, {3, -1, 7, 0}, {7, 0, 5, 4}, {6, -2, 0, 5}};
    const auto c = a * b;
    REQUIRE(c * b.inverse() == a);
  }
}
