import subprocess
import pytest


def test_wifi_ok():
    # Run the Wokwi CLI
    result = subprocess.run(
        [
            "wokwi-cli",
            "--elf",
            "wippersnapper_valid_creds.elf",
            "--timeout",
            "30000",
            "--scenario",
            "test_wifi_ok.scenario.yaml",
        ]
    )
    assert result.returncode == 0

