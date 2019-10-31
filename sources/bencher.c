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

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "bencher.h"

static const double N_1E9 = 1e9;

struct bencher __bencher_new(const char *const __trace) {
    assert(NULL != __trace);

    struct bencher self = {.trace=__trace, .runs=0};
    timespec_get(&self.timer_start, TIME_UTC);
    self.clock_start = clock();

    return self;
}

void bencher_report(const struct bencher *const self, FILE *const file) {
    assert(NULL != self);
    assert(NULL != file);

    const double cpu_total_time = ((double) (self->clock_end - self->clock_start)) / CLOCKS_PER_SEC;
    const struct timespec elapsed_time = (self->timer_end.tv_nsec - self->timer_start.tv_nsec) < 0 ? (struct timespec) {
            .tv_sec = self->timer_end.tv_sec - self->timer_start.tv_sec - 1,
            .tv_nsec = N_1E9 + self->timer_end.tv_nsec - self->timer_start.tv_nsec
    } : (struct timespec) {
            .tv_sec = self->timer_end.tv_sec - self->timer_start.tv_sec,
            .tv_nsec = self->timer_end.tv_nsec - self->timer_start.tv_nsec
    };
    const double total_time = elapsed_time.tv_sec + elapsed_time.tv_nsec / N_1E9;

    fprintf(file, "[bencher]: %s, iters: %u, total_time: %.8lfs, iter_time: %.8lfs, cpu_total_time: %.8lfs, cpu_iter_time: %.8lfs, cpu_usage: %.2lf%%\r\n",
            self->trace, self->runs, total_time, total_time / self->runs, cpu_total_time, cpu_total_time / self->runs, (100.0 * cpu_total_time) / total_time);
}

void bencher_tick(struct bencher *const self) {
    assert(NULL != self);
    self->clock_end = clock();
    timespec_get(&self->timer_end, TIME_UTC);
    self->runs += 1;
}

struct __bencher_context __bencher_context_new(const struct bencher bencher, const unsigned iters) {
    assert(0 < iters && iters < UINT_MAX);
    return (struct __bencher_context) {.bencher=bencher, .iters=iters};
}

int __bencher_context_check(const struct __bencher_context *const self, FILE *const file) {
    assert(NULL != self);
    assert(NULL != file);

    if (self->iters > self->bencher.runs) {
        return 1;
    } else {
        assert(self->iters == self->bencher.runs);
        bencher_report(&self->bencher, file);
        return 0;
    }
}

void __bencher_context_update(struct __bencher_context *const self) {
    assert(NULL != self);
    bencher_tick(&self->bencher);
}
