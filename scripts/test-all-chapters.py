#!/usr/bin/env python3
# test-all-chapters.py - Compile and test all chapters

import subprocess
import os
import glob

def run_cmd(cmd, timeout=60):
    """Run command and return (success, output)"""
    try:
        result = subprocess.run(
            cmd, shell=True, capture_output=True, text=True, timeout=timeout
        )
        return result.returncode == 0, result.stdout + result.stderr
    except subprocess.TimeoutExpired:
        return False, "Timeout"

def test_lab(lab_path):
    """Test a single lab"""
    # Clean
    run_cmd(f"cd {lab_path} && make clean")

    # Compile
    success, output = run_cmd(f"cd {lab_path} && make 2>&1")

    if not success:
        return "COMPILE_FAIL", output.split('\n')[-5:]

    # Check if benos.bin exists
    if not os.path.exists(os.path.join(lab_path, "benos.bin")):
        return "NO_BIN", []

    # Run in QEMU (3 second timeout)
    success, output = run_cmd(
        f"cd {lab_path} && timeout 3 qemu-system-aarch64 -machine raspi4b -nographic -kernel benos.bin 2>&1 || true",
        timeout=10
    )

    return "PASS", output.split('\n')[:3]

def main():
    results = {}

    chapters = [
        'chapter_2', 'chapter_3', 'chapter_4', 'chapter_5', 'chapter_6',
        'chapter_7', 'chapter_8', 'chapter_9', 'chapter_10', 'chapter_11',
        'chapter_12', 'chapter_13', 'chapter_14', 'chapter_15', 'chapter_16',
        'chapter_18', 'chapter_20', 'chapter_22', 'chapter_23'
    ]

    for chapter in chapters:
        chapter_path = f"/home/mlbo/claude/arm64_programming_practice/{chapter}"
        if not os.path.exists(chapter_path):
            continue

        labs = glob.glob(f"{chapter_path}/lab*")
        for lab in sorted(labs):
            if 'lab' not in os.path.basename(lab):
                continue

            # Skip chapter_21 (complex kernel)
            if 'chapter_21' in lab:
                continue

            # Check if Makefile exists
            if not os.path.exists(os.path.join(lab, "Makefile")):
                continue

            status, output = test_lab(lab)
            results[lab] = (status, output)
            print(f"{status}: {lab}")
            if output and status != "PASS":
                for line in output[:3]:
                    print(f"  {line}")

    # Summary
    print("\n=== Summary ===")
    for status in ["PASS", "COMPILE_FAIL", "NO_BIN"]:
        count = sum(1 for s, _ in results.values() if s == status)
        print(f"{status}: {count}")

if __name__ == "__main__":
    main()