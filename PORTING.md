# Python → C++ porting architecture

## Compatibility target

The C++ implementation treats the current `jerrecode/artifexian_auto_worldgen` `main` branch as the behavioral specification. It ports stage contracts and numerical intent, not Python syntax. The physical model remains a reduced-order procedural Earth-system/worldbuilding model; moving to C++ does not justify silently changing scientific assumptions.

Determinism has two levels. **Stage isolation** is mandatory: changing one stage must not consume random numbers from unrelated stages. The native C++ stream derivation is versioned as `v1-xoshiro256ss` and fully specified independent of the standard library. Exact byte-for-byte NumPy `SeedSequence + PCG64DXSM` output is a separate future compatibility backend; native mode does not falsely claim cross-language RNG identity.

## Subsystem mapping

| Python subsystem | C++ subsystem / strategy |
|---|---|
| dataclass configuration + YAML | strongly typed config structs, strict validation, dependency-free YAML mapping parser, generic `--set` overrides |
| NumPy rasters | contiguous `Raster<T>` / `Tensor3<T>` value types with overflow-checked allocation and span access |
| `SphereGrid` / topology | one canonical `SphereGrid` and `SphericalRasterOps`; longitude wraps, poles reflect with 180° longitude rotation |
| SciPy/NumPy filtering | separable spherical kernels with explicit pole topology; reusable scratch storage as stages mature |
| procedural noise | deterministic hybrid value/ridge/billow/wave multifractal and spherical domain warp |
| astronomy | scalar/SoA C++ physics and deterministic system/neighborhood generation |
| tectonics | compact vector structs + contiguous ownership rasters; chunked nearest-seed solves and explicit stress/history state |
| terrain/ocean/climate | typed stage results; scratch-buffer reuse; monthly fields stored plane-major for cache-friendly loops |
| Priority-Flood/hydrology | binary heap flood + compact receiver array + reusable Kahn topological order; no unsafe parallelization of order-dependent routing |
| geology/resources/weather/society | typed classification/intensity rasters plus compact vector records for sparse features/events |
| checkpoints/storage | transactional metadata, content-addressed payloads, fsync/atomic replacement, recoverable manifests; optional mmap backend |
| rendering/output | library-owned output serializers; visualization kept downstream of physical arrays so render resolution remains independent |
| runtime/tiling | deterministic 2-D tile scheduler with memory budget; parallelize only dependency-local kernels |

## C++-specific advantages used intentionally

Ownership is explicit and RAII-only. The core has no owning raw pointers, no manual `new/delete`, no VLAs, and no `vector<bool>`. Allocation sizes are overflow checked. Bounds-checked APIs are available at external boundaries; hot loops use asserted indexing only after shape validation.

Raster data is structure-of-arrays and contiguous. This removes Python object overhead and most temporary-dispatch cost, enables auto-vectorization/SIMD, improves cache locality, and allows explicit scratch-buffer lifetimes. C++ parallelism is reserved for independent tiles, samples, months, and feature generation; global drainage and convergence dependencies remain ordered.

The project does not use implementation-defined standard-library random distributions for reproducible science/procedural state. Integer-to-float mappings and normal generation are specified in-project.

Builds use aggressive warnings. CI runs ordinary tests plus AddressSanitizer/UndefinedBehaviorSanitizer. `-march=native` is opt-in for local benchmarks and never a portability requirement.

## Numerical policy

`double` is used for coordinate geometry, reductions, physical constants, thresholds, and topology metrics. Large state rasters default to `float` where the Python implementation already stores `float32`. Integer labels use the narrowest safe type only when bounds prove it; otherwise `int32_t`/`uint32_t` are used. Sensitive reductions use deterministic traversal and compensated accumulation where appropriate.

Globally coupled algorithms are never tiled merely because tiling is faster. A kernel may be parallel/tiled only when its declared halo contains all dependencies. This mirrors the Python project's separation of numerical fidelity, simulation resolution, rendering resolution, and backend acceleration.

## Port status

The repository is intentionally landed in coherent commits. The core numerical substrate is implemented first, followed by physical stages, hydrology and higher-level layers, then storage/output/runtime parity. Tests are added with each stage rather than postponed until the end.
