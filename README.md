# AMD Radeon GPU Memory Test Suite

这是一个用于在KX7000平台上测试AMD Radeon RX520显卡的显存读写完整性的测试套件。

## 功能特性

- ✅ **Linux Kernel Module驱动**：通过MMIO（内存映射I/O）与GPU硬件直接通信
- ✅ **用户空间应用程序**：C语言实现，简单易用
- ✅ **多种测试模式**：
  - Walking Ones/Zeros（移位模式）
  - Sequential（顺序模式）
  - Random（随机模式）
  - Checkerboard（棋盘模式）
  - All Ones/Zeros（全1/全0模式）
- ✅ **可参数化配置**：测试大小、迭代次数、地址步长等可灵活配置
- ✅ **地址间隔检测**：特别检测15个32bit地址间隔处的不一致
- ✅ **详细的故障报告**：记录每个失败的地址、期望值和实际值

## 硬件要求

- **CPU**：KX7000处理器（兆芯）
- **GPU**：AMD Radeon RX520显卡
- **操作系统**：Kylin Linux
- **内核版本**：5.0+

## 软件要求

- GCC编译器
- Linux内核源代码和开发文件
  ```bash
  sudo apt-get install linux-headers-$(uname -r)
  ```
- make工具

## 安装和编译

### 1. 获取源代码

```bash
git clone https://github.com/zhgdglin/radeon-gpu-test.git
cd radeon-gpu-test
```

### 2. 编译驱动和应用

```bash
make all
```

或分别编译：
```bash
make driver  # 只编译驱动
make app     # 只编译应用
```

### 3. 安装驱动

```bash
sudo make install
```

## 使用方法

### 加载驱动

```bash
sudo insmod radeon_gpu_test. ko
```

或使用：
```bash
sudo make load
```

### 运行基础测试

```bash
./radeon_gpu_test_app
```

### 运行自定义配置测试

```bash
./radeon_gpu_test_app -s 1048576 -p 0 -i 10 -b 15 -v
```

### 命令行参数

| 参数 | 长选项 | 说明 | 默认值 |
|------|--------|------|--------|
| `-s` | `--size` | 测试大小（字节） | 1MB |
| `-p` | `--pattern` | 测试模式（0-6） | 0 |
| `-i` | `--iterations` | 迭代次数 | 1 |
| `-o` | `--offset` | 测试偏移（字节） | 0 |
| `-t` | `--stride` | 地址步长（字节） | 4 |
| `-b` | `--barrier` | 屏障间隔（地址数） | 15 |
| `-r` | `--seed` | 随机种子 | 1 |
| `-v` | `--verbose` | 详细输出 | 否 |
| `-h` | `--help` | 显示帮助 | - |

### 测试模式说明

| 模式 | 名称 | 说明 |
|------|------|------|
| 0 | WALKING_ONES | 移位1：0x1, 0x2, 0x4, 0x8, ...  |
| 1 | WALKING_ZEROS | 移位0：0xFFFFFFFE, 0xFFFFFFFD, ... |
| 2 | SEQUENTIAL | 顺序：0x0, 0x1, 0x2, 0x3, ... |
| 3 | RANDOM | 伪随机数 |
| 4 | CHECKERBOARD | 棋盘：0xAAAAAAAA, 0x55555555, ...  |
| 5 | ALL_ONES | 全1：0xFFFFFFFF |
| 6 | ALL_ZEROS | 全0：0x00000000 |

## 使用示例

### 示例1：基础测试（Walking Ones，10次迭代）

```bash
sudo insmod radeon_gpu_test. ko
./radeon_gpu_test_app -s 1048576 -p 0 -i 10 -b 15 -v
sudo rmmod radeon_gpu_test
```

### 示例2：随机数据测试（100次迭代）

```bash
./radeon_gpu_test_app -p 3 -i 100 -s 2097152 -v
```

### 示例3：检查特定地址间隔

```bash
./radeon_gpu_test_app -s 524288 -p 2 -i 5 -b 15 -o 0x10000 -t 4 -v
```

### 示例4：长时间压力测试

```bash
./radeon_gpu_test_app -s 4194304 -p 4 -i 50 -v
```

## 输出示例

```
Opening device: /dev/radeon_gpu_test
GPU Information:
  Device ID:        0x6907
  Device Name:      Radeon GPU 6907
  VRAM Size:        1024 MB

Test Configuration:
  Pattern:         WALKING_ONES (0)
  Test Size:       1048576 bytes
  Test Offset:     0x0
  Address Stride:  4 bytes
  Barrier Interval:15 addresses
  Iterations:      10
  Random Seed:     1

Initializing GPU test...
Running GPU memory test (this may take a while)... 

========================================
GPU Memory Test Results
========================================
Pattern:               WALKING_ONES
Test Size:             1048576 bytes
Test Offset:           0x0
Address Stride:        4 bytes (32-bit)
Barrier Interval:      15 addresses
Iterations:            10

Results:
Total Writes:          2621440
Total Reads:           2621440
Mismatch Count:        0
Mismatch at Barrier:   0
Test Status:           PASS
Test Duration:         1234 ms

User-space time:       1240 ms
```

## 故障排除

### 问题1：无法打开设备

**错误信息**：
```
open: No such file or directory
Failed to open device.  Make sure the driver is loaded.
```

**解决方案**：
```bash
# 检查驱动是否加载
lsmod | grep radeon_gpu_test

# 如未加载，执行：
sudo insmod radeon_gpu_test. ko

# 检查设备节点
ls -la /dev/radeon_gpu_test
```

### 问题2：编译错误

**错误信息**：找不到Linux内核头文件

**解决方案**：
```bash
# 安装内核开发文件
sudo apt-get install linux-headers-$(uname -r)

# 或指定内核源路径
make KERNEL_SOURCE=/path/to/kernel/source
```

### 问题3：设备未被识别

**症状**：驱动加载成功但没有找到GPU

**排查步骤**：
```bash
# 检查GPU是否被系统识别
lspci | grep -i amd
lspci | grep -i radeon

# 查看内核日志
dmesg | tail -20

# 尝试手动指定设备ID
# 修改驱动中的PCI_DEVICE宏
```

## 测试结果解读

### PASS（通过）
- `Mismatch Count: 0` - 所有读写数据一致
- `Test Status: PASS`

### FAIL（失败）
- `Mismatch Count > 0` - 存在读写不一致
- 检查 `First Mismatch Details` 部分了解详细信息

### 关键指标

- **Mismatch at Barrier**: 在屏障间隔处（15个地址）发生的不一致数
  - 如果此值 > 0，表示可能存在特定地址间隔的问题
  - 这正是您要重点检测的现象

## 内核模块架构

```
┌─────────────────────────────────┐
│   应用程序（用户空间）          │
│   radeon_gpu_test_app            │
└──────────────┬──────────────────┘
               │ ioctl
┌──────────────▼──────────────────┐
│   驱动程序（内核空间）          │
│   radeon_gpu_test. ko             │
│  - PCI设备扫描                  │
│  - MMIO内存映射                 │
│  - 显存读写操作                  │
│  - 数据验证和统计               │
└──────────────┬──────────────────┘
               │
┌──────────────▼──────────────────┐
│   AMD Radeon RX520 GPU          │
│   显存（VRAM）                   │
└─────────────────────────────────┘
```

## 关键代码模块

### 驱动程序核心功能

1. **PCI设备探测** (`radeon_gpu_test_probe`)
   - 扫描AMD GPU设备
   - 映射MMIO内存

2. **显存读写** (`gpu_test_run`)
   - 按配置的模式写入数据
   - 立即读回并验证
   - 检测不一致

3. **地址间隔检测**
   - 每`barrier_interval`个地址检查一次
   - 特别跟踪15个地址间隔处的失败

### 应用程序特性

1. **参数解析** - 灵活的命令行接口
2. **结果分析** - 详细的测试报告
3. **数据导出** - 便于后续分析

## 故障检测机制

驱动程序检测以下类型的故障：

1. **写入/读取不一致**
   - 写入的值与读回的值不相等

2. **地址间隔故障**
   - 在15个地址间隔处出现的不一致

3. **概率性故障**
   - 多次迭代中间歇性出现的不一致

## 性能指标

- **测试吞吐量**：取决于GPU性能和总线速度
- **延迟**：MMIO操作延迟（通常为微秒级）
- **精度**：32bit粒度

## 许可证

GPL v2

## 贡献

欢迎提交bug报告和改进建议！

## 联系方式

如有问题或建议，请联系：zhgdglin@github.com

## 更新日志

### v1.0.0 (2025-12-02)
- 初始版本
- 支持7种测试模式
- 完整的地址间隔检测
- 详细的失败报告

---

**注意**：此工具仅用于GPU显存测试和诊断。请在有充分权限的环境中运行。长时间的压力测试可能会对硬件产生影响，请监控系统温度和稳定性。
