# 知识点：CSEL 条件选择指令

## 1. CSEL 指令格式

```asm
CSEL Xd, Xn, Xm, cond
```

- 如果条件 `cond` 为真，`Xd = Xn`
- 否则 `Xd = Xm`

## 2. 常用条件码

| 条件码 | 含义 | 标志位 |
|--------|------|--------|
| eq | Equal | Z=1 |
| ne | Not Equal | Z=0 |
| hi | Unsigned Higher | C=1 && Z=0 |
| lo | Unsigned Lower | C=0 |
| ge | Signed Greater or Equal | N=V |
| lt | Signed Less Than | N!=V |

## 3. 相关条件选择指令

| 指令 | 功能 |
|------|------|
| CSEL | 条件选择 |
| CSINC | 条件选择并自增 |
| CSINV | 条件选择并取反 |
| CSNEG | 条件选择并取负 |

## 4. 示例

```asm
// 返回较大值
cmp x0, x1
csel x0, x0, x1, hi  // x0 = (x0 > x1) ? x0 : x1
```

```asm
// 返回绝对值
cmp x0, #0
cneg x0, x0, lt  // 如果 x0 < 0，x0 = -x0
```