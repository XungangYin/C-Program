// File Name: example1.cpp
// Author: YXG
// Created Time: 2019年12月27日 星期五 09时13分14秒

#include <boost/lambda/lambda.hpp>
#include <iterator>
#include <algorithm>
#include <iostream>

int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;
    std::for_each(
        in(std::cin),in(),std::cout<<(_1 * 3)<<" ");
}
