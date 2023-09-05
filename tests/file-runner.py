import subprocess
import sys

if len(sys.argv) > 1:
    file_path = sys.argv[1]
    subprocess.run([file_path])
else:
    print("Use python runner.py <file>")
    sys.exit(1)
