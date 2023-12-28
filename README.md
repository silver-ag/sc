# SC

typical attempts at computer magic use the computer as a tool like a deck or candle.
this is an attempt to make the computer do magic itself.
a computer sigil is not an image file depicting a sigil. computers don't know what an image is.
a computer sigil is the only thing a computer understands: a binary string, partitioned into bytes.

## process

SC keeps track of one current working sigil. these sigils are generated from text strings (future versions may allow other sources).
the sigilisation process works by laying out the bytes of the input in a square, then XNORing the rows of the square together (future versions may allow other algorithms).

once you have a sigil, the computer can be made to chant it in a variety of ways. some chants end in a moment of rupture/gnosis when
memory is exhausted, others must be interrupted to stop them.

you can also view the sigil as a set of geomantic signs, two per byte.

## command reference

commands

| --- | --- |
| sigil | report the current sigil |
| sigil <phrase> | sigilise the given string and set the current sigil accordingly |
| chant <type> | chant a sigil in the given manner |
| geomantic | interpret the current sigil as a series of four-bit geomantic symbols |
| end | close sc |
| help | print this text |
| help <command> | give more in-depth help about a specific option |

chants

| heap | the sigil is written to the heap until a moment of rupture/gnosis occurs when the heap memory runs out |
| stack | the sigil is recursively written to the stack until a moment of rupture/gnosis occurs when the stack memory runs out |
| stdout | the sigil is written to stdout until the program is cancelled. this can be piped to aplay for an audio chant too. |
| disk <path> | takes a block device as an argument, to which the sigil is written until space runs out. |
| net <addr> | takes an ip address as an argument, to which the sigil is sent as a stream of udp packets on port 888. |

