## Algorithms

This file describes benchmarking framework of the library.

### Index

- [Index](#index)
- [Building](#building)
- [Methodology](#methodology)
- [Metrics](#metrics)
- [Preparation](#preparation)
- [Parametrization](#preparation)
- [Benchmarking](#benchmarking)

### Building

The benchmarking framework is provided via the `milo` command line application.
Application is not build by default, although [integration installation guide](integration.md#installation) shell snippets
explicitly pass `-DMILO_APP=ON` option which enables application compilation.

After successful compilation, the application will be placed in the `app` directory:

```shell
cd milo-build/app
```

To verify that everything works, list all possible benchmarks with the command:

```shell
./milo benchmark primitive list
```

Possible output:

```
codec-base-16-encode
codec-base-16-decode
codec-base-64-encode
codec-base-64-decode
hash-sha-1-160
hash-sha-2-224
hash-sha-2-256
hash-sha-2-384
hash-sha-2-512
hash-sha-2-512-224
hash-sha-2-512-256
...
```

How to invoke benchmarks will be described later.

### Methodology

Assuming that the algorithm accepts a specific set of arguments, execution time is measured with such parameterization.
For example hash algorithms accept `--message-size=<int>` parameter,
mac algorithms accept `--key-size=<int>` and `--message-size=<int>` and so on.

Most benchmarks react to a common set of basic parameters that control behaviors such as:

- inputs sizes
- timed iterations
- warmup iterations

Parametrization is described later in the document.

To measure execution time `std::chrono::steady_clock` is used.
The reasoning behind using it instead of `rdtsc` or `rdpmc` are few:

- It's portable.
- The logic being measured is complex, mentioned clock sources are better suited for micro-benchmarking.
- Calculations are heavily data-dependent, and high-quality samples come from wider time frames.
- The increase in precision in the range of several dozen cycles will not be noticeable, even noised by one context switch.

Benchmarking with other clock source backends may be implemented in the future.

### Metrics

Metrics collected by benchmarks:

- `duration`
    - `nanoseconds_per_call` - average floating point number
- `throughput`
    - `gigabytes_per_second` - average floating point number
    - `megabytes_per_second` - average floating point number
- `cpu`
    - `cycles_per_call` - average floating point number
    - `cycles_per_byte` - average floating point number

The names and how to interpret them should be pretty self-explanatory .

### Preparation

Before benchmarking, execution environment should be tuned to reduce potential bias.

#### Amd processors

- Disable frequency scaling.

```shell
sudo sh -c 'echo 0 > /sys/devices/system/cpu/cpufreq/boost'
```

- Disable Simultaneous Multithreading.

#### Intel processors

- Disable Turbo Boost.

```shell
sudo sh -c 'echo 1 > /sys/devices/system/cpu/intel_pstate/no_turbo'
```

- Disable Hyper-Threading.

#### General

- Disable Virtualization.
- Disable P-States and C-States.
- Limit number of running services.

### Parametrization

The application parameterization tree describes what parameters or options are accepted by the application and subcommands.
The command line argument list must respect the tree structure.
Parameters and options specified too early or too late will be ignored by the application.
Unrecognized options and parameters are ignored.
When parameterizing a subcommand, the order of parameters and options is irrelevant.

Parameterization tree:

- `milo` - application, root of tree
    - `options`
        - `--advanced` - advanced features switch, e.g. algorithm backend benchmarks
        - `--verbose` - verbose output
    - `subcommands`
        - `benchmark`
            - `parameters`
                - `--repeats-time=<int>` - measured repeats
                - `--repeats-warm=<int>` - unmeasured repeats, can be used for caches warmup, branch predictor training
                - `--cpu-clock=<int>`    - clock speed in `mhz` units, must be set to estimate cycles per call/byte
            - `subcommands`
                - `primitive`
                    - `subcommands`
                        - `list` - list all possible benchmarks
                        - `<expression>` - pattern expression that matches one or more items returned by the list subcommand
                            - `codec-*`
                                - `parameters`
                                    - `--bytes-size=<int>` - bytes size
                            - `hash-*`
                                - `parameters`
                                    - `--message-size=<int>` - message size
                            - `mac-*`
                                - `parameters`
                                    - `--key-size=<int>` - key size
                                    - `--message-size=<int>` - message size
                                    -
                            - `kdf-hkdf-*`
                                - `parameters`
                                    - `--ikm-size=<int>` - ikm size
                                    - `--salt-size=<int>` - salt size
                                    - `--info-size=<int>` - info size
                                    - `--key-size=<int>` - key size
                            - `kdf-pbkdf-2-*`
                                - `parameters`
                                    - `--ikm-size=<int>` - ikm size
                                    - `--salt-size=<int>` - salt size
                                    - `--iterations=<int>` - iterations
                                    - `--key-size=<int>` - key size
                            - `cipher-*`
                                - `parameters`
                                    - `--bytes-size=<int>` - bytes size
                            - `aead-*`
                                - `parameters`
                                    - `--aad-size=<int>` - aad size
                                    - `--bytes-size=<int>` - bytes size

When any parameter that does not have a default fallback value is omitted, an error occurs.

Missing `--message-size=<int>` parameter with no default value:

```shell
milo benchmark primitive hash-*
```

Output:

```shell
Error. Missing --message-size parameter of hash-* command.
```

### Benchmarking

The following examples should give a pretty good understanding of how to compose
command line arguments to benchmark algorithms in the library.

#### Benchmark hash-sha-2-256

```
./milo benchmark --cpu-clock=3500 primitive "hash-sha-2-256" --message-size=16384
```

#### Benchmark hash-sha-2-256 x86 extensions backend

```
./milo --advanced benchmark --cpu-clock=3500 primitive "hash-sha-2-256-hw*" --message-size=16384
```

#### Benchmark hash-sha-2-256 software backend

```
./milo --advanced benchmark --cpu-clock=3500 primitive "hash-sha-2-256-sw" --message-size=16384
```

#### Benchmark all hash functions

```
./milo benchmark --cpu-clock=3500 primitive "hash-*" --message-size=16384
```

#### Benchmark aead-chacha-20-poly-1305

```
./milo benchmark --cpu-clock=3500 primitive "aead-chacha-20-poly-1305" --aad-size=1024 --bytes-size=8192
```

TODO Example outputs and json description.
