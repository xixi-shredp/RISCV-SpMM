#!/bin/bash

export GEM5_ROOT=/opt/gem5
export GEM5=$GEM5_ROOT/build/RISCV/gem5.opt
export GEM5_CFG_PY=$(pwd)/../boom-gem5/simulate.py

# 创建输出目录
mkdir -p logs

# 参数组合列表，每组是："M K N density"
option_list=(
  "8192 2048 32 0.5"
  "2048 8192 32 0.5"
  "2048 2048 32 0.5"
  "11008 4096 32 0.5"
  "4096 11008 32 0.5"
  "4096 4096 32 0.5"
  "5632 2048 32 0.5"
  "2048 5632 32 0.5"
  "2048 2048 32 0.5"
)

batch_run_spmm() {
  opts="$1"

  echo "Running with options: $opts"

  # 添加前缀 scg_
  name="scg_$(echo $opts | tr ' ' '_')"

  # 执行 gem5 模拟并将输出保存到对应日志文件中
  $GEM5 $GEM5_CFG_PY \
    --cpu="OoO-4W" \
    run-benchmark \
    -c ./spmm_scg \
    --options="$opts" \
    >logs/stdout_$name.txt 2>&1

  # 保存 stats.txt 为单独文件
  cp m5out/stats.txt logs/stats_$name.txt
  cp m5out/config.ini logs/config_$name.ini
  cp m5out/config.json logs/config_$name.json

  echo "Finished: $opts --> logs/stats_$name.txt"
}

export -f batch_run_spmm

# 串行执行
# for opt in "${option_list[@]}"; do
#   batch_run_spmm "$opt"
# done

# 并行执行
parallel -j3 batch_run_spmm ::: "${option_list[@]}"
