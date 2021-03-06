# MBC - Mattyw & MeBeiM symmetric encryption algorithm #

[![license][license-img]][license-link]
[![travis][travis-img]][travis-link]
[![appveyor][appveyor-img]][appveyor-link]
[![codecov][codecov-img]][codecov-link]
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fmttbernardini%2Fmbc.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2Fmttbernardini%2Fmbc?ref=badge_shield)

This repository provides a C library for the *MBC symmetric encryption algorithm*, plus a parametric CLI program.

This is a remake of the [mbencrypter][1] project, however the new algorithm is not compatible.


## Disclaimer ##

This is a **work in progress**, backwards compatibility is not guaranteed (have a look at `developing` branch).

**The *MBC symmetric encryption algorithm* shall not be used in any production environment, nor for any kind of cryptographic purposes**, since it's NOT a secure encryption algorithm.

This software is developed just for fun and educational purposes and is therefore provided "as is", without warranty of any kind.


## Download ##

Clone this repo:

	$ git clone https://github.com/mttbernardini/mbc.git
	$ cd mbc

Or just download one of the available [releases][2].


## Building and installation ##

Create a "build" directory and run `cmake` from there:

	$ mkdir build
	$ cd build
	$ cmake ..

### Unix systems (Linux, MacOS, Cygwin, etc...) ###

Build & install using `make`:

	$ sudo make install

### Windows (MinGW) ###

Build & install using `mingw32-make` from an elevated command prompt:

	> mingw32-make install


## Testing ##

All tests can be run using `make` (or `mingw32-make` on Windows):

	$ make mbc_test

Or selective tests can be run:

	$ make unit_tests
	$ ./mbc_unit_tests

	$ make integration_tests
	$ ./mbc_integration_tests

Random test (only for Unix, requires `bash`):

	$ make random_test
	$ ./mbc_random_test <data-size> <key-size>

*NB: `<data-size>` and `<key-size>` are mandatory and allow suffixes like `k`, `M`, `G` and so on (refer to `dd` documentation).*


## Basic algorithm description ##

1. Bit-Swap encoding, by MeBeiM:
   - The key is converted into a swap map (see [`/doc/notes.md`][3] for detailed information).
   - The swap map is reduced to generate the `swap_key`, each byte indicating the position of two bits to swap in each byte of data.
   - Each byte of data is processed applying the swaps indicated by the `swap_key`.

2. Per-Byte XOR encoding:
   - If the key is shorter than data, it will be cycled until the end of data.
   - If data is shorter than the key, it will be cycled until the end of the key.
   - Each byte of data is XORed with each byte of the key.


## Documentation and more ##

All source files provide descriptive comments above functions prototypes and definitions. A complete documentation will be available in the `/doc` folder along with additional information.

---
*Copyright © 2017 Matteo Bernardini & Marco Bonelli.*

[1]: https://github.com/mttbernardini/mbencrypter
[2]: https://github.com/mttbernardini/mbc/releases
[3]: https://github.com/mttbernardini/mbc/blob/master/doc/notes.md#make_swap_key-rationale

[license-img]:   https://img.shields.io/github/license/mttbernardini/mbc.svg
[license-link]:  https://github.com/mttbernardini/mbc/blob/master/LICENSE
[travis-img]:    https://img.shields.io/travis/mttbernardini/mbc/master.svg
[travis-link]:   https://travis-ci.org/mttbernardini/mbc
[appveyor-img]:  https://img.shields.io/appveyor/ci/mttbernardini/mbc/master.svg
[appveyor-link]: https://ci.appveyor.com/project/mttbernardini/mbc/branch/master
[codecov-img]:   https://img.shields.io/codecov/c/github/mttbernardini/mbc/master.svg
[codecov-link]:  https://codecov.io/gh/mttbernardini/mbc/branch/master


## License
[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2Fmttbernardini%2Fmbc.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2Fmttbernardini%2Fmbc?ref=badge_large)