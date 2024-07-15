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
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True)
    stdout, stderr = process.communicate(input_data)
    return stdout.strip(), stderr.strip()


def show_result():
    output, error = run_process()
    print(output)


if __name__ == "__main__":
    access_folder()
    while 1:
        show_result()
