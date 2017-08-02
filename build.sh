#!/bin/bash

set -eo pipefail

platformio lib install 443
platformio lib install 567
platformio run
