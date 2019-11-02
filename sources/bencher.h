/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Davide Di Carlo
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <trace/trace.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <time.h>

#if !(defined(__GNUC__) || defined(__clang__))
__attribute__(...)
#endif

#define BENCHER_VERSION_MAJOR       1
#define BENCHER_VERSION_MINOR       0
#define BENCHER_VERSION_PATCH       0
#define BENCHER_VERSION_SUFFIX      ""
#define BENCHER_VERSION_IS_RELEASE  0
#define BENCHER_VERSION_HEX         0x010000

struct Bencher {
    struct timespec timer_start;
    struct timespec timer_end;
    const char *trace;
    clock_t clock_start;
    clock_t clock_end;
    unsigned runs;
};

// private
extern struct Bencher __Bencher_new(const char *__trace)
__attribute__((__warn_unused_result__, __nonnull__(1)));

#define Bencher_new() \
    __Bencher_new(__TRACE__)

extern void Bencher_report(const struct Bencher *self, FILE *file)
__attribute__((__nonnull__(1, 2)));

extern void Bencher_tick(struct Bencher *self)
__attribute__((__nonnull__(1)));

// <----- private begin
struct __BencherContext {
    struct Bencher bencher;
    const unsigned iters;
};

extern struct __BencherContext __BencherContext_new(struct Bencher bencher, unsigned iters)
__attribute__((__warn_unused_result__));

extern int __BencherContext_check(const struct __BencherContext *self, FILE *file)
__attribute__((__warn_unused_result__, __nonnull__(1, 2)));

extern void __BencherContext_update(struct __BencherContext *self)
__attribute__((__nonnull__(1)));
// <----- private end

#define bench_iters_on(n, f) \
  for (struct __BencherContext context = __BencherContext_new(Bencher_new(), (n)); __BencherContext_check(&context, (f)); __BencherContext_update(&context))

#define bench_iters(n) \
    bench_iters_on(n, stderr)

#define bench_on(f) \
  bench_iters_on(1, f)

#define bench \
  bench_on(stderr)

#ifdef __cplusplus
}
#endif
