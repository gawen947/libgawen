# libGawen
A nice and tight easy peasy come in handy library of reused C code.

---

## Content

  * **htable**: All purpose simple hashtables.
  * **hash**: Hash functions and utils for hashtables.
  * **bst**: Binary search trees.
  * **crc32**: CRC32 variants (optimized with dedicated opcode when available).
  * **crc-ccitt**: CRC-16-CCITT often used in telecommunication.
  * **sm-kr**: Implementation of the Karp-Rabin String Matching algorithm.
  * **iobuf**: Buffered I/O.
  * **string-utils**: String related functions.
  * **time**: Time related functions.
  * **scale**: Human readable representation of numbers.
  * **help**: Display help messages for long and short options.
  * **safe-call**: Automatically check syscall for error codes.
  * **daemon**: Handle PID and drop privileges.
  * **xatoi**: Efficient parsing of unsigned integer (with error reporting).
  * **atoi-gen**: Generic integer parsing.
  * **verbose**: Toggable verbose messages.
  * **log**: Log in both syslog and stderr.

## Version

The version scheme is *MAJOR.MINOR.PATCH* starting at 1.0.0.

  * *MAJOR*: Major change in one or all components that break backward compatibility.
  * *MINOR*: Optimizations, new component or new feature in a component but still backward compatible.
  * *PATCH*: Bug and security fixes.
