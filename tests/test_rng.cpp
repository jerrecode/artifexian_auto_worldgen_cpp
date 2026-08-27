#include "test.hpp"
#include "worldgen/rng.hpp"
WG_TEST(rng_reproducible_and_stage_isolated){worldgen::RngPool p(42);auto a=p("climate"),b=p("climate"),c=p("terrain");for(int i=0;i<20;++i)WG_CHECK(a.u64()==b.u64());WG_CHECK(p("climate").u64()!=c.u64());}
