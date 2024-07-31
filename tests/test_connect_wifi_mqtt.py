import subprocess
import pytest

def test_netfsm_mqtt_error_auth():
    """Test that the Wippersnapper device fails to connect to the MQTT server due to invalid credentials"""
    result = subprocess.run(
        [
            "wokwi-cli",
            "--elf",
            "ws-mqtt-error-auth.elf",
            "--timeout",
            "200000",
            "--scenario",
            "test_netfsm_mqtt_error_auth.scenario.yaml",
        ]
    )
    assert result.returncode == 0

def test_netfsm_mqtt_invalid_server():
    """Test that the Wippersnapper device fails to connect to the MQTT server due to an invalid (or offline) server address provided"""
    result = subprocess.run(
        [
            "wokwi-cli",
            "--elf",
            "ws-mqtt-invalid-server.elf",
            "--timeout",
            "200000",
            "--scenario",
            "test_netfsm_mqtt_invalid_server.scenario.yaml",
        ]
    )
    assert result.returncode == 0

def test_netfsm_establish_wifi():
    """Test that the Wippersnapper device connects to WiFi successfully and the FSM transitions
    to the MQTT state."""
    result = subprocess.run(
        [
            "wokwi-cli",
            "--elf",
            "ws-establish-wifi.elf",
            "--timeout",
            "50000",
            "--scenario",
            "test_netfsm_establish_wifi.scenario.yaml",
        ]
    )
    assert result.returncode == 0

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
            "55000",
            "--scenario",
            "test_netfsm_wifi_invalid_pass.scenario.yaml",
        ]
    )
    assert result.returncode == 0
