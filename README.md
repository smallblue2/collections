# Allocators

![CI Badge](https://github.com/smallblue2/allocators/actions/workflows/ci.yml/badge.svg)

A C library containing various allocator alternatives to `malloc` and `free`.

Mostly for my own personal use to avoid re-writing various allocators, I will add more allocators as I come across them.

## Current Allocators

 - Arena (Linear Allocator)

## Install

Prerequisites:

 - `meson`

To install:

1. `git clone https://github.com/smallblue2/allocators`
2. `cd allocators`
3. `meson setup build`
4. `cd build`
5. `meson install`

This will install:

 - Static library - `liballocators.a`
 - Shared library - `liballocators.so`
 - Headers - `include/allocators/*.h`

Everything is installed to `/usr/local/`.
