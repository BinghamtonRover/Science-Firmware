from argparse import ArgumentParser
import os

parser = ArgumentParser()
parser.add_argument("--proto", "-p", type=str, required=True, help="Proto file name, NOT including .proto extension")
args = parser.parse_args()

os.system(f"protoc --python_out=src/generated -I Protobuf Protobuf/{args.proto}.proto")