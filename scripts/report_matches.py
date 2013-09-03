#!/usr/bin/env python

import argparse
import bisect
import collections
import json
import sys
import uuid
import yaml


def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('--index-file', default='positions.json',
            help='index to lookup file positions in')

    parser.add_argument('--match-file', default=None,
            help='file to read matches from, defaults to STDIN')

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


class Match(object):
    def __init__(self, length, positions):
        self.length = int(length)
        self.positions = positions

    @property
    def weight(self):
        return self.length * len(self.positions)

    def __cmp__(self, other):
        return cmp(self.weight, other.weight)

    def __repr__(self):
        return 'Match(%d, %r)' % (self.length, self.positions)

    def report_as_dict(self, index):
        result = {
            'score': self.weight,
            'id': uuid.uuid4().hex,
            'files': collections.defaultdict(list),
        }

        for position in self.positions:
            filename, begin = index.find(position)
            end_filename, end = index.find(position + self.length - 1)
            assert filename == end_filename

            result['files'][filename].append({'begin': begin, 'end': end})

        result['files'] = dict(result['files'])

        return result


def main():
    args = parse_args()

    if args.match_file is None:
        match_file = sys.stdin
    else:
        match_file = open(args.match_file)

    index = Index(args.index_file)

    matches = []

    for line in match_file:
        length, positions_string = line.split(':')
        positions = sorted(map(int, positions_string.split(',')))
        matches.append(Match(length=length, positions=positions))

    matches.sort(reverse=True)

    reports = [m.report_as_dict(index) for m in matches]

    print yaml.dump(reports)


if __name__ == '__main__':
    main()
