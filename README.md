# Collections

![CI Badge](https://github.com/smallblue2/collections/actions/workflows/ci.yml/badge.svg)

A C library containing various collections to assist C development.

This is mostly for my own learning in the underlying implementation of collections, api design and library creation.

## Current Collections

 - Arena (Linear Allocator)

## Install

Prerequisites:

 - `meson`

To install:

1. `git clone https://github.com/smallblue2/collections`
2. `cd collections`
3. `meson setup build`
4. `cd build`
5. `meson install`

This will install:

 - Static library - `libcollections.a`
 - Shared library - `libcollections.so`
 - Headers - `include/collections/*.h`

Everything is installed to `/usr/local/`.
