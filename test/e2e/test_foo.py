import subprocess
from pathlib import Path
from run import TestData, run_sandbox, SandboxResult

def test_usage_message():
    sandbox_path = '../../sandbox'
    result = subprocess.run(sandbox_path, capture_output=True)
    assert result.returncode != 0
    assert result.stderr.decode().startswith('Error: need 11 arguments')

def test_python_c_lib_call():
    test_data = TestData(
        root=Path('testdata') / 'python-c-lib-call',
        lang_id=2,
        expected_status='RE',
    )
    result = run_sandbox(test_data.opt())
    assert isinstance(result, SandboxResult)
    assert result.status == test_data.expected_status

