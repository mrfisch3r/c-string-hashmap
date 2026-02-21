# C String Hash Map

A small, educational hash map implementation in C with **string keys** and **generic pointer values** (`void*`).  
Collisions are handled with **separate chaining** (linked lists per bucket). Includes a tiny `demo.c` harness for quick builds.

> This repo contains only code and documentation written by me (no course handouts/prompts or instructor-provided tests).

## What it does
- Create a map with a configurable number of buckets
- Insert or replace a key/value pair
- Lookup a value by key
- Remove a key/value pair (returns the stored value pointer)
- Debug-print bucket contents (`dump()`)

## Project layout
- `src/strmap.c` — implementation
- `src/strmap.h` — public header (opaque `strmap_t`)
- `demo.c` — simple demo program

## Build & run (Windows + GCC / MSYS2)
From the repo root:

```powershell
gcc -Wall -Wextra -std=c11 -Isrc -o demo.exe demo.c src\strmap.c
.\demo.exe