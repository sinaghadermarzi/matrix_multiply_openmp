# matrix_multiply_openmp

Integer matrix multiplication parallelised with OpenMP, written for the first assignment of a
parallel-processing course (task sheet: `Assignment 1.pdf`; write-up, in Persian: `Report.pdf` /
`HW_1_report.docx`).

## What is in the repo

- `main_p.cpp` – the OpenMP program. It fills an `n x m` matrix A and an `m x p` matrix B with random
  integers in `[0, 100)`, multiplies them with `thrN` threads (each thread computes an equal block of
  `n / thrN` rows of C), runs the parallel multiplication 3 times and the serial one 3 times, and prints the
  per-run and average times (`omp_get_wtime`) and the speedup. After each phase it asks whether to print
  the matrices (answer `d`). To avoid false sharing at the row boundaries, C is allocated 64-byte aligned
  with a padding of `PAD = 16` ints (one 64-byte cache line) per element.
- `MatrixMultiplication - Sequential  Version - Copy.cpp` – the sequential starter code handed out with
  the assignment (by Ahmad Siavashi), kept for reference.
- `MatrixMult.exe` – a Windows build of `main_p.cpp`.
- `deliverable/` – the submitted copy; `main_p.cpp`, `MatrixMult.exe` and `Report.pdf` there are identical to
  the ones at the top level.

## Running

```
MatrixMult.exe <n> <m> <p> <thrN>
```

If the arguments are omitted the program prompts for them. `n` should be divisible by `thrN`; rows left
over by the integer division are not computed.

## Building

The code targets Windows / Visual C++: it includes `<windows.h>` and uses `_aligned_malloc`,
`_aligned_free`, `scanf_s` and `system("PAUSE")`. With the Visual Studio developer command prompt:

```
cl /openmp /EHsc main_p.cpp /Fe:MatrixMult.exe
```

(or build it in a Visual Studio project with OpenMP support enabled under C/C++ > Language). To build it
elsewhere you would need to replace those Windows-specific calls (e.g. `aligned_alloc`/`free`, `scanf`) and
drop the `<windows.h>` include, then compile with `g++ -fopenmp main_p.cpp`.
