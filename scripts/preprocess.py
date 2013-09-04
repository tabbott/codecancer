#!/usr/bin/env python

import argparse
import itertools
import json
import pygments.lexers.agile
import pygments.token
import re
import sys


LEXER = pygments.lexers.agile.PerlLexer()


def _process_tokens(raw_tokens):
    results = []
    line_number = 1
    for file_position, token_type, text in raw_tokens:
        stripped_value = text.strip()
        if stripped_value and token_type not in pygments.token.Comment:
            results.append(Token(line_number, stripped_value, file_position))
        line_number += text.count('\n')
    return results


def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('--concat-file',
            help='where to output normalized representation')
    parser.add_argument('--index-file', default='positions.json',
            help='where to write position index')

    parser.add_argument('--null-separate', action='store_true',
            help='separate file contents in concat-file using NULL')

    parser.add_argument('file', nargs='*',
            help='source files to normalize')

    return parser.parse_args()


_DROP_WHITESPACE = re.compile(r'\s')
def _simple_filter_text(raw_text):
    return _DROP_WHITESPACE.sub('', raw_text)


_MASKED_TOKEN_TYPES = {
    pygments.token.Token.Name.Variable: lambda t: 'x',
    pygments.token.Token.String: lambda t: hash(t)
}

def filter_text(token_type, raw_text):
    whitespace_removed = _DROP_WHITESPACE.sub('', raw_text)
    if whitespace_removed:
        if token_type in _MASKED_TOKEN_TYPES:
            return str(_MASKED_TOKEN_TYPES[token_type](whitespace_removed))

    return whitespace_removed


def process(raw):
    current_position = 0

    line_positions = [0]
    string_components = []

    for file_position, token_type, text in LEXER.get_tokens_unprocessed(raw):
        filtered_text = filter_text(token_type, text)

        token_line_count = text.count('\n')
        line_positions.extend(itertools.repeat(current_position,
            token_line_count))

        if token_type not in pygments.token.Comment:
            if filtered_text:
                string_components.append(filtered_text)
                current_position += len(filtered_text)


    return line_positions, ''.join(string_components)


def main():
    args = parse_args()

    filenames = []
    file_start_positions = []
    file_sizes = []
    line_start_positions = []

    if args.concat_file:
        concat = open(args.concat_file, 'w')
    else:
        concat = sys.stdout

    current_file_position = 0
    for filename in args.file:
        with open(filename) as f:
            line_positions, string = process(f.read())

        filenames.append(filename)
        file_start_positions.append(current_file_position)
        line_start_positions.append(line_positions)
        file_sizes.append(len(string))

        current_file_position += len(string)
        concat.write(string)

    with open(args.index_file, 'w') as index:
        json.dump({
            'filenames': filenames,
            'file_sizes': file_sizes,
            'file_start_positions': file_start_positions,
            'line_start_positions': line_start_positions,
        }, index)


if __name__ == '__main__':
    main()
