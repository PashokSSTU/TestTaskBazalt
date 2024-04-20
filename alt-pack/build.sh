#!/bin/bash

cmake  -B ./build -G Ninja .
cd build
ninja
ninja install