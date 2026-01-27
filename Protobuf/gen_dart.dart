// ignore_for_file: avoid_print

import "dart:io";

/// The executable to run.
const command = "protoc";

/// The arguments to pass.
const args = ["--dart_out=lib/src/generated", "-I", "Protobuf", "Protobuf/*.proto", "google/protobuf/timestamp.proto"];

void main() async {
	final result = await Process.run(command, args);
	print("Result: ${result.exitCode}");
	print("Output: ${result.stdout}");
	print("Error: ${result.stderr}");
}
