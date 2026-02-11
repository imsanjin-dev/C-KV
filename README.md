# C-Super-KV
一个基于 C 的轻量级内存键值数据库：**Hash + LRU + AVL + Min-Heap** 多结构协同，兼顾 **O(1) 存取 / O(1) 淘汰 / O(log N) 有序 / 高效 TTL**。

> 这是一个用于学习与巩固数据结构/内存管理的 Educational Project，重点在底层实现与一致性维护。

---

## 核心卖点

- **O(1) 平均存取**：拉链法 **Hash Table** 作为主索引
- **O(1) 淘汰**：**LRU 双向链表**，容量满时淘汰最久未使用节点
- **O(log N) 有序维护**：**AVL Tree** 自平衡，支持 `KEYS` 有序输出（避免 BST 退化）
- **高效 TTL 管理**：**Min-Heap** 管理过期时间，配合 **Lazy Deletion（懒惰删除）** 安全处理重复 TTL 任务

---

## 架构（多结构一致性）

同一份数据由一个 `KVSNode` 承载（key/value/expire_time），被多个结构同时索引：

```

Hash Table  --->  KVSNode  <---  AVL Tree (按 key 有序)
|              |  ^
|              v  |
+----> LRU Doubly List (head=最近使用, tail=淘汰点)

Min-Heap: 存 (expire_time, keyCopy) 用于 TTL（懒惰删除）

````

### 一致性为什么关键？
任何一次 **PUT/DEL/淘汰/过期** 都必须同步更新结构，否则会出现：
- AVL/LRU 引用已释放节点（悬空指针）
- LRU 断链
- Hash 与有序视图不一致（`KEYS` 输出错误）

---

## 功能列表

- 基本操作：`PUT` / `GET` / `DEL`
- LRU 淘汰：容量满自动淘汰（`O(1)`）
- TTL：支持带过期时间的写入；Min-Heap + 懒惰删除清理过期
- 有序输出：`KEYS` 按 key 排序输出（AVL 中序遍历）
- 持久化：`SAVE/LOAD`（**仅保存永久键**；TTL 键不落盘，符合项目规则）

---

## 快速开始

### 依赖
- Linux / WSL
- GCC（建议 C99）
- CMake（推荐）
- Make / Ninja

### 编译
```bash
mkdir -p build
cd build
cmake ..
cmake --build .
````

### 运行

```bash
./c-kv
```

---

## 命令说明

| 命令           | 用法                               | 说明                     |
| ------------ | -------------------------------- | ---------------------- |
| `put`        | `put <key> <value>`              | 写入/更新键值（永久）            |
| `put_expire` | `put_expire <key> <value> <ttl>` | 写入/更新并设置 TTL（秒）        |
| `get`        | `get <key>`                      | 查询并刷新 LRU              |
| `del`        | `del <key>`                      | 删除键（同步更新 Hash/LRU/AVL） |
| `keys`       | `keys`                           | 按 key 有序输出             |
| `save`       | `save [file]`                    | 保存永久键到文件               |
| `load`       | `load [file]`                    | 从文件加载                  |
| `help`       | `help`                           | 显示帮助                   |
| `exit`       | `exit`                           | 退出                     |

示例：

```bash
put name "hello"
get name
put_expire token "abc" 5
keys
save dump.ckv
exit
```

---

## 技术要点（简版）

### AVL 旋转（保持平衡）

插入后回溯更新高度与平衡因子，触发四类旋转（LL/RR/LR/RL），保证树高 `O(log N)`，从而 `KEYS` 稳定有序、性能稳定。

### TTL 懒惰删除（避免误删）

堆里可能存在同 key 的旧过期任务。弹出堆顶任务后：

1. 去 Hash 找当前节点
2. **比对 expire_time 是否一致**
3. 一致才删除；不一致说明是旧任务，直接丢弃
   这样可以避免“旧任务误删新值”。

### 持久化格式（CKV1）

文件包含 magic header + 多条记录（长度 + 字节流），便于二进制读写与快速加载。
项目规则：**只持久化永久键**，TTL 键不保存。

---

## 内存管理

* `KVSNode` 统一拥有 `key/value` 内存，删除路径只释放一次，避免 double free。
* Heap 中存放的是 `keyCopy`（深拷贝），不会引用已释放节点，降低悬空指针风险。
* 使用 Valgrind 进行泄漏与非法访问检查（建议运行）：

```bash
valgrind --leak-check=full --show-leak-kinds=all ./c-kv
```

---

## License

No license. For learning and reference only.


