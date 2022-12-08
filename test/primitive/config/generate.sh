#!/usr/bin/env bash

for CONFIG_PATH in *.json; do
    CONFIG_NAME="${CONFIG_PATH%.*}"
    CONFIG_EXT="${CONFIG_PATH##*.}"

    milo-codegen \
        --config-path "${CONFIG_NAME}.${CONFIG_EXT}" \
        --output-path "../${CONFIG_NAME}.cpp"
done
