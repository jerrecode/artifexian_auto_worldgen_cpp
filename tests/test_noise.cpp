#include "test.hpp"
#include "worldgen/noise.hpp"
WG_TEST(noise_deterministic_and_standardized){worldgen::SphereGrid g(64,32);worldgen::NoiseConfig c;c.octaves=3;worldgen::Rng a(123),b(123);auto x=worldgen::hybrid_multifractal(g,a,c),y=worldgen::hybrid_multifractal(g,b,c);double mean=0;for(std::size_t i=0;i<x.size();++i){WG_CHECK(x.span()[i]==y.span()[i]);mean+=x.span()[i];}mean/=x.size();WG_NEAR(mean,0,1e-4);}
