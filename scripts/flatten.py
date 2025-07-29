import os

# Directory to search
SRC_DIR = "src/"
# Output file
OUTPUT_FILE = "scripts/Zenoa_flattened.txt"

def collect_cpp_and_headers(src_dir):
    cpp_and_headers = []
    for root, _, files in os.walk(src_dir):
        for file in files:
            if file.endswith(".cpp") or file.endswith(".h"):
                full_path = os.path.join(root, file)
                cpp_and_headers.append(full_path)
    return cpp_and_headers

def dump_to_file(file_list, output_file):
    with open(output_file, "w", encoding="utf-8") as out:
        for file_path in file_list:
            out.write(f"\n// FILE: {file_path}\n")
            try:
                with open(file_path, "r", encoding="utf-8") as f:
                    out.write(f.read())
            except Exception as e:
                out.write(f"// ERROR READING FILE: {e}\n")

if __name__ == "__main__":
    files = collect_cpp_and_headers(SRC_DIR)
    print(f"Found {len(files)} files. Dumping to {OUTPUT_FILE}...")
    dump_to_file(files, OUTPUT_FILE)
    print("Done.")
