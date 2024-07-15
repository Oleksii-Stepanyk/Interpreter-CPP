import subprocess
import os


def access_folder():
    os.chdir('..')
    os.chdir("Interpreter")
    os.chdir("x64")
    os.chdir("Debug")


def run_process(input_data):
    process = subprocess.Popen(
        ["Interpreter.exe"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True)
    stdout, stderr = process.communicate(input_data)
    return stdout.strip(), stderr.strip()


def test():
    tests = [
        {
            "input": "7 * 2 + 3\n",
            "expected_output": "17"
        },
        {
            "input": "abs(-5) + abs(-2)\n",
            "expected_output": "7"
        },
        {
            "input": "max(4, min(5, 6))\n",
            "expected_output": "5"
        },
        {
            "input": "min(max(2, 3), max(4, 5))\n",
            "expected_output": "3"
        },
        {
            "input": "abs(3 - 6) + abs(2 - 5)\n",
            "expected_output": "6"
        },
        {
            "input": "5 + 3 * 2\n",
            "expected_output": "11"
        },
        {
            "input": "max(5, 2)\n",
            "expected_output": "5"
        },
        {
            "input": "min(3, 4)\n",
            "expected_output": "3"
        },
        {
            "input": "max(min(2 * 3, 2), 2)\n",
            "expected_output": "2"
        },
        {
            "input": "5 - abs(-3)\n",
            "expected_output": "2"
        },
        {
            "input": "5 + abs(-3)\n",
            "expected_output": "8"
        },
        {
            "input": "pow(3,5)\n",
            "expected_output": "243"
        },
        {
            "input": "pow(8, 3)\n",
            "expected_output": "512"
        }
    ]

    for test in tests:
        input_data = test["input"]
        expected_output = test["expected_output"]

        output, error = run_process(input_data)

        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif output.strip() != expected_output:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected_output}")
            print(f"Got:\n{output}")
        else:
            print(f"Test passed for input:\n{input_data}")


if __name__ == "__main__":
    access_folder()
    test()
