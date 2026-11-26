# 第十八届全国大学生智能汽车竞赛 · 智能视觉组（主控工程）

NXP RT1064 平台上的智能视觉组主控工程，含整车控制逻辑、视觉图像处理、外设驱动与逐飞（SeekFree）库。

- **赛事**：第十八届全国大学生智能汽车竞赛 · 智能视觉组（华东赛区一等奖）
- **主控**：NXP i.MX RT1064（Cortex-M7，600 MHz）
- **开发环境**：Keil MDK 5（`project/mdk/rt1064.uvprojx`）
- **语言**：C / C++ 混合

---

## 目录结构

```
├── project/                 主控工程
│   ├── code/                业务代码（核心）
│   │   ├── components/      组件：图像处理、AprilTag、通信等
│   │   ├── driver/          板级外设驱动
│   │   ├── thread/          任务/状态机
│   │   ├── utils/           工具函数
│   │   ├── config.hpp       参数配置
│   │   └── user_code*.h     用户入口
│   ├── user/                启动与中断入口
│   ├── mdk/                 Keil 工程文件
│   └── iar/                 IAR 工程文件
├── libraries/               逐飞库 + NXP SDK + Eigen
│   ├── sdk/                 RT1064 SDK / CMSIS
│   ├── zf_common|zf_driver|zf_device   逐飞通用库与器件驱动
│   ├── components/          fatfs 等第三方组件
│   └── eigen/               Eigen（矩阵运算）
├── logic/                   控制逻辑流程图（drawio）
├── chatgpt/                 视觉算法笔记（canny 等）
└── 附录/                    设计文档：任务流程、迷宫法则、通信协议等
```

## 编译

1. 用 Keil MDK 5 打开 `project/mdk/rt1064.uvprojx`
2. 目标选择 `rt1064_nor_sdram_zf_dtcm`
3. 直接 Build，或配合 J-Link 下载

> `project/mdk/Objects`、`Listings` 等编译中间文件未纳入版本库，首次编译会自动生成。

## 实现要点

- **图像处理**：在 MCU 上完成赛道/图卡识别，含 AprilTag 检测与透视校正（`mapx/mapy` 预计算映射表）
- **运动控制**：串级 PID（位置/航向/速度）、麦克纳姆轮逆解、积分分离与误差死区、分段限幅
- **定位**：四路编码器 + IMU 航向积分融合，零漂标定与坐标系对齐
- **任务调度**：定时中断驱动的前后台状态机，底盘运动与机械臂拾取并行
- **调试**：VOFA+ 波形、Flash 参数保存、TFT 多页菜单在线调参

## 说明

- 本仓库为参赛工程存档，`libraries/` 内为第三方库（逐飞、NXP SDK、Eigen），版权归各自作者所有。

## License

自有代码部分采用 MIT License；`libraries/` 下第三方代码遵循其原有许可。