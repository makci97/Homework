#!/usr/bin/env python3

import sys
import argparse

def make_range(s):
    r = (start, end, step) = [int(x) for x in s.split(',')]
    return r

def make_log_range(s):
    r = (base, start, end, step) = [int(x) for x in s.split(',')]
    return r

def log_range(base, start, end, step):
    for p in range(start, end, step):
        yield base ** p
    
if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-e', '--elements_range', metavar='base,start,end,step', type=make_log_range, required=True)
    parser.add_argument('-a', '--additions_range', metavar='base,start,end,step', type=make_log_range, required=True)
    args = parser.parse_args()
    test_num = 1
    
    for e in log_range(*args.elements_range):
        for a in log_range(*args.additions_range):
            with open("{test_num:02d}.in".format(**locals()), "w") as f:
                print("{e} {a}".format(**locals()), file=f)
                test_num += 1
