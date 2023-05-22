import subprocess
from subprocess import CompletedProcess
from dataclasses import dataclass
from pathlib import Path
from tempfile import TemporaryDirectory

@dataclass
class SandboxOption:
    # 0: c, 1: cpp, 2: py
    lang_id: int
    compile: bool
    file_stdin: str
    file_stdout: str
    file_stderr: str
    time_limit: int
    memory_limit: int
    large_stack: bool
    output_limit: int
    process_limit: int
    # path to result
    file_result: str

@dataclass
class SandboxResult:
    # e.g. AC, WA
    status: str
    # execution time in ms
    duration: int
    # memory usage in KB
    mem_usage: int
    stdout: str
    stderr: str
    # extra message from sandbox bin
    # e.g. WEXITSTATUS() = ...
    exit_msg: str

def run_sandbox(opt: SandboxOption) -> SandboxResult | CompletedProcess:
    sandbox_path = '../../sandbox'
    args = list(map(str, (
        opt.lang_id,
        int(opt.compile),
        opt.file_stdin,
        opt.file_stdout,
        opt.file_stderr,
        opt.time_limit,
        opt.memory_limit,
        int(opt.lang_id),
        opt.output_limit,
        opt.process_limit,
        opt.file_result,
    )))
    result = subprocess.run(
        ['sudo', sandbox_path, *args], 
        capture_output=True,
    )
    if result.returncode == 0:
        stdout = Path(opt.file_stdout).read_text(errors='ignore')
        stderr = Path(opt.file_stderr).read_text(errors='ignore')
        result_lines = Path(opt.file_result).read_text().splitlines()
        return SandboxResult(
            status=result_lines[0],
            duration=int(result_lines[2]),
            mem_usage=int(result_lines[3]),
            stdout=stdout,
            stderr=stderr,
            exit_msg=result_lines[1],
        )
    return result

@dataclass
class TestData:
    root: Path
    lang_id: int
    expected_status: str

    def src(self) -> Path:
        '''
        Path to source code, e.g. main.py
        '''
        # Currently there should be only one source code file
        return next((self.root / 'src').iterdir())

    def input(self) -> Path:
        '''
        Path to input file
        '''
        return self.root / 'in'

    def opt(self) -> SandboxOption:
        temp_root = Path(TemporaryDirectory(prefix='noj-sandbox-e2e-test').name)
        temp_root.mkdir()
        file_result = (temp_root / 'result').absolute()
        # file_result.write_bytes(b'')
        return SandboxOption(
            lang_id=self.lang_id,
            compile=False,
            file_stdin=str(self.input().absolute()),
            file_stdout=str((temp_root / 'stdout').absolute()),
            file_stderr=str((temp_root / 'stderr').absolute()),
            time_limit=1000,
            memory_limit=512000,
            large_stack=True,
            output_limit=1073741824,
            process_limit=10,
            file_result=str(file_result),
        )
