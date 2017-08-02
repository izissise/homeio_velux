#!/bin/bash

set -eo pipefail

platformio lib install 567
platformio run
