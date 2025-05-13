#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "vector.h"

TEST_CASE("MyVector push_back ir operator[] veikia", "[MyVector]") {
    MyVector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
    REQUIRE(v[2] == 30);
    REQUIRE(v.size() == 3);
}

TEST_CASE("MyVector at() suveikia su validžiu ir out-of-range indeksu", "[MyVector]") {
    MyVector<int> v(3, 7);
    REQUIRE(v.at(1) == 7);
    REQUIRE_THROWS_AS(v.at(5), std::out_of_range);
}

TEST_CASE("MyVector resize() padidina ir sumažina", "[MyVector]") {
    MyVector<int> v(2, 1);
    v.resize(5, 9);
    REQUIRE(v.size() == 5);
    REQUIRE(v[4] == 9);
    v.resize(2);
    REQUIRE(v.size() == 2);
}

TEST_CASE("MyVector operator== ir !=", "[MyVector]") {
    MyVector<int> v1(3, 5);
    MyVector<int> v2(3, 5);
    MyVector<int> v3(2, 5);

    REQUIRE(v1 == v2);
    REQUIRE(v1 != v3);
}

TEST_CASE("MyVector clear() isvalo viska", "[MyVector]") {
    MyVector<int> v(4, 8);
    REQUIRE(v.size() == 4);
    v.clear();
    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());
}
