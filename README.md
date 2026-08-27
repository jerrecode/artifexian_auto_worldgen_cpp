# Artifexian Automatic World Generator — C++

Memory-safe, deterministic C++ reimplementation of `jerrecode/artifexian_auto_worldgen`.

This repository is being ported from the current Python implementation as a behavior-oriented numerical reimplementation rather than a mechanical syntax translation. Core design goals are deterministic stage-isolated RNG streams, canonical spherical topology, contiguous typed raster storage, bounded-memory algorithms, reusable drainage graphs, checkpointable pipeline stages, and sanitizer-clean modern C++.

The implementation targets C++20 and is organized as a reusable library plus the `worldgen` CLI. See `PORTING.md` for the subsystem mapping, compatibility policy, and C++-specific optimization strategy.
