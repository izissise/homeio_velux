#!/bin/bash

set -eo pipefail

platformio lib install 567
platformio lib install 1262
platformio run
