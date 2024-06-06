#!/usr/bin/env bash

# $ arduino-cli board listall
BOARD="arduino:avr:mega"

acdb --verbose --fqbn "$BOARD"
