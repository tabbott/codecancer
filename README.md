# codecancer

## History

One week in late 2013, a code-deduplication hackathon took place in
the [genome](https://github.com/genome/genome) codebase at
[McDonnell Genome Institute](http://genome.wustl.edu). Instead of
just participating directly, some
[colleagues](https://github.com/tabbott/codecancer/graphs/contributors)
and I thought it would be a neat idea to apply some well known techniques
for duplicate sequence detection from the realm of bioinformatics to our
code base. (note: my email has since changed so I don't appear in the list of
contributors. Notice my contributions via name in the list of
[commits](https://github.com/tabbott/codecancer/commits/master))

This is the result of our efforts.

## Methodology

Super-maximal repeats are detected as described
[here](http://www.zbh.uni-hamburg.de/pubs/pdf/AboKurOhl2002.pdf).
The pygments python library is used as a frontend to tokenize and
pre-process source code in various languages.

## Future work

The obvious next step is to support the clang front-end for building
ASTs and deduplicating based on that data.

### Python Dependencies

- pygments
- yaml

on debianish systems:

```
sudo apt-get install python-pygments python-yaml
```
