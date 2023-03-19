""" This file contains all the processing functions needed to prepare
the code dataset. It also contains the file lists. """
import os
import re


# Text processing functions =============================================================
def sanitize(string):
    """ Remove comments and includes/pragmas. """
    return re.sub('//.*?\n|/\*.*?\*/|#.*?\n', '', string, flags=re.S)


def count_labels(file):
    """ Find the last label and keep only its index. """
    with open(file, "r", encoding="utf-8") as f:
        n = f.readlines()[-1].split(',')[0].removeprefix('L')
    return int(n)


def match_brackets(string):
    """ Returns the substring consisting of a whole bracketed 'for' command. """
    [prev, inside] = string.split('{', 1)
    i = prev.index('for')
    prev = prev[i:]
    open = 1
    for index in range(len(inside)):
        if inside[index] in '{}':
            open = (open + 1) if inside[index] == '{' else (open - 1)
        if not open:
            return prev + '{' + inside[:index + 1]


def label_indices(string):
    """ Reads the code and returns the indices of labeled lines. """
    idx = 1
    indices = []
    lines = string.split('\n')
    for i, line in enumerate(lines):
        if line.startswith(f'L{idx}:'):
            indices.append(i)
            idx += 1
    if not indices:
        raise Exception
    return indices
        

def isolate_labels(string, indices):
    lines = string.split('\n')
    new_string = ''
    for idx in indices:
        if 'for' in lines[idx]:
            string = '\n'.join(lines[idx:])
            command = match_brackets(string)
        else:
            command = ' '.join(lines[idx].split()[1:])
        if command not in new_string:
            new_string += command + '\n'
    return new_string


def process_file(file):
    """ Processes content of code files, keeping the labeled parts. """
    foldername = file.split(os.sep)[-2]
    with open(file, "r", encoding="utf-8") as f:
        code = sanitize(f.read())
        try:
            indices = label_indices(code)
            code = isolate_labels(code, indices)
        except Exception:
            print("Warning: Code in", foldername, "left unchanged.")
    return code


def read_file(file):
    """ Reads and cleans the content of code files. """
    foldername = file.split(os.sep)[-2]
    with open(file, "r", encoding="utf-8") as f:
        code = sanitize(f.read())
        code = code.replace('\n', ' ')
        code = ' '.join(code.strip().split())
    return code, foldername
