# SPDX-FileCopyrightText: 2024 Brent Rubell for Adafruit Industries
#
# SPDX-License-Identifier: MIT
"""Unit tests for WipperSnapper Arduino's network FSM function, runNetFsm."""
import subprocess
import pytest

def run_wokwi(elf_file, scenario_file, timeout=30000):
    """Runs a Wokwi-CLI simulation provided an ELF file, scenario YAML and expected timeout.
    Returns 0 if successful, non-zero otherwise.
    """
    result = subprocess.run(
        [
            "wokwi-cli",
            "--elf",
            elf_file,
            "--timeout",
            str(timeout),
            "--scenario",
            scenario_file,
        ]
    )
    return result.returncode

def test_netfsm_establish_wifi_mqtt():
    """Test that the Wippersnapper device connects to WiFi and MQTT successfully and transitions to the next state."""
    assert run_wokwi(f"elf/ws.elf","test_netfsm_establish_wifi_mqtt.scenario.yaml", 200000) == 0

def test_netfsm_mqtt_error_auth():
    """Test that the Wippersnapper device fails to connect to the MQTT server due to invalid credentials"""
    assert run_wokwi(f"elf/ws-mqtt-error-auth.elf", "test_netfsm_mqtt_error_auth.scenario.yaml", 200000) == 0

def test_netfsm_mqtt_invalid_server():
    """Test that the Wippersnapper device fails to connect to the MQTT server due to an invalid (or offline) server address provided"""
    assert run_wokwi(f"elf/ws-mqtt-invalid-server.elf", "test_netfsm_mqtt_invalid_server.scenario.yaml", 200000) == 0

def test_netfsm_establish_wifi():
    """Test that the Wippersnapper device connects to WiFi successfully and the FSM transitions
    to the MQTT state."""
    assert run_wokwi(f"elf/ws-establish-wifi.elf", "test_netfsm_establish_wifi.scenario.yaml", 50000) == 0

def test_netfsm_wifi_ssid_not_found():
    """Test that the Wippersnapper device fails to connect to WiFi due to an invalid SSID in secrets"""
    assert run_wokwi(f"elf/ws-invalid-net-ssid.elf", "test_netfsm_wifi_ssid_not_found.scenario.yaml", 55000) == 0

def test_netfsm_wifi_invalid_pass():
    """Test that the Wippersnapper device fails to connect to WiFi due to an invalid network password"""
    assert run_wokwi(f"elf/ws-invalid-net-password.elf", "test_netfsm_wifi_invalid_pass.scenario.yaml", 55000) == 0