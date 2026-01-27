"""
Generates C++ code from Protobuf files using nanopb

The problem with using nanopb directly is that it tries to use angled brackets in its #include
statement, which fails if you didn't install nanopb yourself. This command uses relative paths 
to a submodule instead, meaning it will "just work" on any machine.
"""

import os
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument("--sketch", "-s", type=str, required=True, help="Sketch name")
parser.add_argument("--proto", "-p", type=str, required=True, help="Proto file name, NOT including .proto extension")
args = parser.parse_args()

# We want the final terminal command to look like this: 
# nanopb_generator ... -L "#include \"utils/%s\"" ...
# - The outer quotes bundle the whole expression as part of the -L flag, including spaces
# - The inner quotes are literal and therefore need to be escaped with a backslash in the terminal
# 	NOTE: In Python, we don't need to escape the double quotes because we're using single quotes.
# 				But in the terminal, there will be two layers of quotes so we need to print them escaped.
# - In order to print the backslash from Python, we use a raw string (r"string")
# - We also need to use single quotes in our Python code to leave the double quotes alone
pb_include_statement = r'#include \"utils/%s\"' if args.sketch != "utils" else r'#include \"%s\"'
proto_include_statement = r'#include \"%s\"'
destination = f"{args.sketch}/src" if args.sketch != "utils" else "."
os.system(f'nanopb_generator -I Protobuf -D {destination} -L "{pb_include_statement}" -Q "{proto_include_statement}" {args.proto}.proto')

