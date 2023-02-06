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

Environment:

- `gcc version 12.2.1`
- `nasm version 2.16.01`
- `Linux 5.19.11-zen1-1-zen`
- `Ryzen Threadripper PRO 3975WX`


- `hash-sha-1-160`

```shell
./milo benchmark --cpu-clock=3500 primitive "hash-sha-1-160" --message-size=16384
```

```json
{
    "benchmark": {
        "primitive": {
            "hash-sha-1-160": {
                "metrics": {
                    "cpu": {
                        "cycles_per_byte": 1.904761,
                        "cycles_per_call": 31207.603027
                    },
                    "throughput": {
                        "megabytes_per_second": 1837.500943,
                        "gigabytes_per_second": 1.837501
                    },
                    "duration": {
                        "nanoseconds_per_call": 8916.458008
                    }
                },
                "config": {
                    "input": {
                        "message-size": 16384
                    },
                    "benchmark": {
                        "cpu-clock": 3500000000,
                        "repeats-time": 1024,
                        "repeats-warm": 128
                    }
                }
            }
        }
    }
}
```

- `hash-sha-2-256`

```shell
./milo benchmark --cpu-clock=3500 primitive "hash-sha-2-256" --message-size=16384
```

```json
{
    "benchmark": {
        "primitive": {
            "hash-sha-2-256": {
                "metrics": {
                    "cpu": {
                        "cycles_per_byte": 2.033197,
                        "cycles_per_call": 33311.892578
                    },
                    "throughput": {
                        "megabytes_per_second": 1721.427261,
                        "gigabytes_per_second": 1.721427
                    },
                    "duration": {
                        "nanoseconds_per_call": 9517.683594
                    }
                },
                "config": {
                    "input": {
                        "message-size": 16384
                    },
                    "benchmark": {
                        "cpu-clock": 3500000000,
                        "repeats-time": 1024,
                        "repeats-warm": 128
                    }
                }
            }
        }
    }
}
```

- `mac-poly-1305`

```shell
./milo benchmark --cpu-clock=3500 primitive "mac-poly-1305" --key-size=32 --message-size=16384

```

```json
{
    "benchmark": {
        "primitive": {
            "mac-poly-1305": {
                "metrics": {
                    "cpu": {
                        "cycles_per_byte": 2.422251,
                        "cycles_per_call": 39686.158203
                    },
                    "throughput": {
                        "megabytes_per_second": 1444.937041,
                        "gigabytes_per_second": 1.444937
                    },
                    "duration": {
                        "nanoseconds_per_call": 11338.902344
                    }
                },
                "config": {
                    "input": {
                        "message-size": 16384,
                        "key-size": 32
                    },
                    "benchmark": {
                        "cpu-clock": 3500000000,
                        "repeats-time": 1024,
                        "repeats-warm": 128
                    }
                }
            }
        }
    }
}
```

- `cipher-chacha-20`

```shell
./milo benchmark --cpu-clock=3500 primitive "cipher-chacha-20-*" --aad-size=1024 --bytes-size=16384
```

```json
{
    "benchmark": {
        "primitive": {
            "cipher-chacha-20-decrypt": {
                "metrics": {
                    "cpu": {
                        "cycles_per_byte": 2.064460,
                        "cycles_per_call": 33824.112793
                    },
                    "throughput": {
                        "megabytes_per_second": 1695.358585,
                        "gigabytes_per_second": 1.695359
                    },
                    "duration": {
                        "nanoseconds_per_call": 9664.032227
                    }
                },
                "config": {
                    "input": {
                        "bytes-size": 16384
                    },
                    "benchmark": {
                        "cpu-clock": 3500000000,
                        "repeats-time": 1024,
                        "repeats-warm": 128
                    }
                }
            },
            "cipher-chacha-20-encrypt": {
                "metrics": {
                    "cpu": {
                        "cycles_per_byte": 2.067819,
                        "cycles_per_call": 33879.145508
                    },
                    "throughput": {
                        "megabytes_per_second": 1692.604673,
                        "gigabytes_per_second": 1.692605
                    },
                    "duration": {
                        "nanoseconds_per_call": 9679.755859
                    }
                },
                "config": {
                    "input": {
                        "bytes-size": 16384
                    },
                    "benchmark": {
                        "cpu-clock": 3500000000,
                        "repeats-time": 1024,
                        "repeats-warm": 128
                    }
                }
            }
        }
    }
}
```

- `aead-chacha-20-poly-1305`

```shell
./milo benchmark --cpu-clock=3500 primitive "aead-chacha-20-poly-1305-*" --aad-size=1024 --bytes-size=8192
```

```json
{
    "benchmark": {
        "primitive": {
            "aead-chacha-20-poly-1305-decrypt": {
                "metrics": {
                    "cpu": {
                        "cycles_per_byte": 4.356709,
                        "cycles_per_call": 40151.429199
                    },
                    "throughput": {
                        "megabytes_per_second": 803.358701,
                        "gigabytes_per_second": 0.803359
                    },
                    "duration": {
                        "nanoseconds_per_call": 11471.836914
                    }
                },
                "config": {
                    "input": {
                        "bytes-size": 8192,
                        "aad-size": 1024
                    },
                    "benchmark": {
                        "cpu-clock": 3500000000,
                        "repeats-time": 1024,
                        "repeats-warm": 128
                    }
                }
            },
            "aead-chacha-20-poly-1305-encrypt": {
                "metrics": {
                    "cpu": {
                        "cycles_per_byte": 4.353707,
                        "cycles_per_call": 40123.760742
                    },
                    "throughput": {
                        "megabytes_per_second": 803.912679,
                        "gigabytes_per_second": 0.803913
                    },
                    "duration": {
                        "nanoseconds_per_call": 11463.931641
                    }
                },
                "config": {
                    "input": {
                        "bytes-size": 8192,
                        "aad-size": 1024
                    },
                    "benchmark": {
                        "cpu-clock": 3500000000,
                        "repeats-time": 1024,
                        "repeats-warm": 128
                    }
                }
            }
        }
    }
}
```
