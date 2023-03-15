import os
import re
import torch
from glob import glob
from os.path import join as pjoin

# Config
path = os.path.dirname(os.path.realpath(__file__))
device = "cuda" if torch.cuda.is_available() else "cpu"


# Processing functions ==================================================================
def sanitize(text):
    """Remove comments and includes/pragmas."""
    return re.sub('//.*?\n|/\*.*?\*/|#.*?\n', '', text, flags=re.S)


def findBrackets( aString ):
    if '{' in aString:
        match = aString.split('{',1)[1]
        open = 1
        for index in range(len(match)):
            if match[index] in '{}':
                open = (open + 1) if match[index] == '{' else (open - 1)
            if not open:
                return match[:index]


def read_file(path):
    """Based on read_examples."""
    foldername = path.split(os.sep)[-2]
    with open(path, "r", encoding="utf-8") as f:
        # code = parse_labels(f.read())
        code = sanitize(f.read())
        code = code.replace('\n', ' ')
        code = ' '.join(code.strip().split())
    return code, foldername


datafiles = [
    file for file in glob(pjoin(path, 'dataset', 'machsuite*', '*.c*'))
    if not file.endswith(('harness.c', 'support.c'))
]

# Read files
code_dataset = [read_file(file) for file in datafiles]

