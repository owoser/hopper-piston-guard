# HopperPistonGuard

禁止漏斗被活塞移动的 LeviLamina 插件（BDS 1.26.20）。

## 原理

引擎在决定“方块能否被活塞推动/拉动”时查询方块的 `minecraft:movable` 组件
（`BlockMovableComponent::getMovementType`）。本插件钩住该查询，当被查询的方块是
**漏斗（hopper）** 时返回 `MovementType::Immovable` —— 与原版黑曜石/基岩同一机制：

- 普通活塞推不动漏斗；
- 粘性活塞同样推不动（也无法把漏斗拉回）；
- 活塞推动一长串方块、而串中（含末端）有漏斗时，整串都推不动（等同遇到黑曜石）。

钩子只影响漏斗方块的“可移动性”查询结果，不影响漏斗本身的功能（吸取/推出/UI）。

## 安装

把 `bin/hopper-piston-guard/` 整个目录放入服务端 `plugins/` 后重启。

## 验证

- 活塞（普通/粘性）面朝漏斗激活 → 活塞不伸出；
- 粘性活塞伸出状态下面前方放漏斗 → 收回时不会把漏斗拉走（漏斗不可移动）；
- 一排方块（如 5 个箱子）末端是漏斗 → 活塞推不动整排；
- 漏斗其它功能正常。

## 构建

```bash
xmake f -y -p windows -a x64 -m release
xmake
# 产物：bin/hopper-piston-guard/
```

注意：本机构建需要把 VS 自带 LLVM（如 `D:\vs\VC\Tools\Llvm\x64\bin`）加入 PATH。
