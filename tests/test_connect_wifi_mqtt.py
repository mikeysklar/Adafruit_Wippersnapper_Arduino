import subprocess
import pytest


def test_netfsm_wifi_ssid_not_found():
    """Test that the Wippersnapper device fails to connect to WiFi due to an invalid SSID in secrets"""
    # This scenario requires a longer timeout
    # since it has to fail to connect multiple
    # times before the test passes
    result = subprocess.run(
        [
            "wokwi-cli",
            "--elf",
            "ws-invalid-net-ssid.elf",
            "--timeout",
            "55000",
            "--scenario",
            "test_netfsm_wifi_ssid_not_found.scenario.yaml",
        ]
    )
    assert result.returncode == 0

def test_netfsm_wifi_invalid_pass():
    """Test that the Wippersnapper device fails to connect to WiFi due to an invalid network password"""
    # Run the Wokwi CLI
    result = subprocess.run(
        [
            "wokwi-cli",
            "--elf",
            "ws-invalid-net-password.elf",
            "--timeout",
            "30000",
            "--scenario",
            "test_netfsm_wifi_invalid_pass.scenario.yaml",
        ]
    )
    assert result.returncode == 0
