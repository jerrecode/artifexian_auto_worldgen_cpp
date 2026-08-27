#include "test.hpp"
#include "worldgen/topology.hpp"
WG_TEST(pole_reflection_rotates_longitude){auto p=worldgen::map_spherical_index(-1,0,4,8);WG_CHECK(p.y==0);WG_CHECK(p.x==4);auto s=worldgen::map_spherical_index(4,1,4,8);WG_CHECK(s.y==3);WG_CHECK(s.x==5);}
WG_TEST(spherical_components_join_seam){worldgen::SphereGrid g(8,4);worldgen::SphericalRasterOps ops(g);worldgen::RasterBool m(4,8,0);m(2,0)=1;m(2,7)=1;auto [lab,n]=ops.connected_components(m);WG_CHECK(n==1);WG_CHECK(lab(2,0)==lab(2,7));}
WG_TEST(spherical_gradient_constant_zero){worldgen::SphereGrid g(16,8);worldgen::SphericalRasterOps ops(g);worldgen::RasterF a(8,16,3);auto [gy,gx]=ops.metric_gradient(a);for(auto z:gy.span())WG_NEAR(z,0,1e-12);for(auto z:gx.span())WG_NEAR(z,0,1e-12);}
