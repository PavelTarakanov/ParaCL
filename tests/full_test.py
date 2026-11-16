import subprocess
import os
import sys

# Get the absolute path of the project root directory (parent of 'tests')
CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(CURRENT_DIR)

BUILD_DIR = os.path.join(PROJECT_ROOT, "build")
APP_NAME = "paracl_app.exe" if os.name == "nt" else "paracl_app"
APP_PATH = os.path.join(BUILD_DIR, APP_NAME)

def run_paracl(source_code, user_input=""):
    """Helper function to execute the ParaCL binary with source code and optional stdin input."""
    test_file = "temp_test.pcl"
    with open(test_file, "w", encoding="utf-8") as f:
        f.write(source_code)

    try:
        result = subprocess.run(
            [APP_PATH, test_file],
            input=user_input,
            text=True,
            capture_output=True,
            timeout=5
        )
        return result.returncode, result.stdout, result.stderr
    finally:
        # Cleanup temporary files
        if os.path.exists(test_file):
            os.remove(test_file)
        if os.path.exists("ast.dot"):
            os.remove("ast.dot")
        if os.path.exists("ast.png"):
            os.remove("ast.png")

def test_arithmetic():
    print("Running E2E: Arithmetic evaluation...", end=" ")
    code = "print 2 + 3 * 4 - (10 / 2);"
    code, stdout, stderr = run_paracl(code)

    assert code == 0, f"Process failed with stderr: {stderr}"
    assert "9" in stdout, f"Expected '9' in output, got: {stdout}"
    print("PASSED")

def test_loop_and_input():
    print("Running E2E: While loop & User input...", end=" ")
    code = """
        iters = ?;
        while (iters > 0) {
            print iters;
            iters = iters - 1;
        }
    """
    code, stdout, stderr = run_paracl(code, user_input="3\n")

    assert code == 0, f"Process failed with stderr: {stderr}"
    assert "3" in stdout and "2" in stdout and "1" in stdout, f"Unexpected output: {stdout}"
    print("PASSED")

def test_conditionals():
    print("Running E2E: If-Else branches...", end=" ")
    code = """
        x = 10;
        if (x > 5) {
            print 111;
        } else {
            print 222;
        }
    """
    code, stdout, stderr = run_paracl(code)

    assert code == 0, f"Process failed with stderr: {stderr}"
    assert "111" in stdout, f"Expected '111', got: {stdout}"
    assert "222" not in stdout, f"Did not expect '222', got: {stdout}"
    print("PASSED")

def test_syntax_error():
    print("Running E2E: Syntax error handling...", end=" ")
    code = "x = 5"  # Missing semicolon
    code, stdout, stderr = run_paracl(code)

    assert code != 0, "Expected non-zero exit code for syntax error"
    assert "Error" in stderr or "Error" in stdout, "Expected error message in output"
    print("PASSED")

if __name__ == "__main__":
    if not os.path.exists(APP_PATH):
        print(f"Error: Executable not found at '{APP_PATH}'. Build the project first!")
        sys.exit(1)

    print("=== Starting ParaCL Full E2E Tests ===")
    try:
        test_arithmetic()
        test_loop_and_input()
        test_conditionals()
        test_syntax_error()
        print("\nAll full E2E tests successfully PASSED! 🎉")
    except AssertionError as e:
        print(f"\nTEST FAILED: {e}")
        sys.exit(1)
