#pragma once
#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
struct TestCase{std::string name;std::function<void()>fn;};inline std::vector<TestCase>&tests(){static std::vector<TestCase>v;return v;}struct Reg{Reg(const char*n,std::function<void()>f){tests().push_back({n,std::move(f)});}};
#define WG_TEST(name) static void name();static Reg reg_##name(#name,name);static void name()
#define WG_CHECK(x) do{if(!(x))throw std::runtime_error(std::string("check failed: ")+#x);}while(0)
#define WG_NEAR(a,b,e) do{if(std::abs((a)-(b))>(e))throw std::runtime_error("near check failed");}while(0)
