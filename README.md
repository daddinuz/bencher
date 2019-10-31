# Bencher

A tiny C11 library to bench C code execution time.

```c
#include <unistd.h>
#include <bencher.h>

static inline int fib(const int n) {
    return n <= 1 ? n : fib(n - 1) + fib(n - 2);
}

int main() {
    bench {
        sleep(1);
    }

    bench_on(stdout) {
        sleep(1);
    }

    bench_iters(3) {
        fib(32);
    }

    bench_iters_on(3, stdout) {
        fib(32);
    }

    return 0;
}
```

giving an output like:

```
[bencher]: /bencher/examples/main.c:36, iters: 1, total_time: 1.00011878s, iter_time: 1.00011878s, cpu_total_time: 0.00001500s, cpu_iter_time: 0.00001500s, cpu_usage: 0.00%
[bencher]: /bencher/examples/main.c:40, iters: 1, total_time: 1.00009343s, iter_time: 1.00009343s, cpu_total_time: 0.00001900s, cpu_iter_time: 0.00001900s, cpu_usage: 0.00%
[bencher]: /bencher/examples/main.c:44, iters: 3, total_time: 0.04165385s, iter_time: 0.01388462s, cpu_total_time: 0.04165100s, cpu_iter_time: 0.01388367s, cpu_usage: 99.99%
[bencher]: /bencher/examples/main.c:48, iters: 3, total_time: 0.03962544s, iter_time: 0.01320848s, cpu_total_time: 0.03962300s, cpu_iter_time: 0.01320767s, cpu_usage: 99.99%
```
