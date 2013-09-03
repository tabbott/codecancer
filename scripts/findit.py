#!/usr/bin/env python

import argparse
import bisect
import json


def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('--index-file', default='positions.json',
            help='index to lookup file positions in')

    parser.add_argument('position', nargs='+', type=int,
            help='index of S for which to find the filename and line number')

    return parser.parse_args()


class Index(object):
    def __init__(self, filename):
        blob = json.load(open(filename))
        self.filenames = blob['filenames']
        self.file_start_positions = blob['file_start_positions']
        self.line_start_positions = blob['line_start_positions']

    def find(self, position):
        file_index = bisect.bisect(self.file_start_positions, position) - 1

        filename = str(self.filenames[file_index])
        file_offset = position - self.file_start_positions[file_index]

        line_number = bisect.bisect(self.line_start_positions[file_index],
                file_offset)
        return (filename, line_number)


def main():
    args = parse_args()

    index = Index(args.index_file)

    for position in args.position:
        filename, line_number = index.find(position)
        print '%d\t%s\t%d' % (position, filename, line_number)


if __name__ == '__main__':
    main()
