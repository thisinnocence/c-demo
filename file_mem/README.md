# 基于Memory Mapping的高性能进程间通信示例

## 1. 概述
使用Memory Mapping实现高性能进程间通信。通过将文件映射到内存的方式，实现了高效的数据共享机制。

## 2. 技术原理

### 2.1 核心机制
| 机制 | 描述 |
|------|------|
| Memory Mapping | 使用`mmap`将文件映射到虚拟地址空间 |
| 页表管理 | 内核维护页表确保多进程间内存同步 |
| Zero-copy | 避免用户态和内核态间的数据拷贝 |
| Page Cache | 利用系统页缓存提升访问性能 |

### 2.2 关键特性对比
| 特性 | Memory Mapping | System V共享内存 | Pipe/消息队列 |
|------|----------------|------------------|----------------|
| API复杂度 | 简单 | 复杂 | 中等 |
| 持久化 | 支持 | 不支持 | 不支持 |
| 权限控制 | 文件系统权限 | IPC权限 | 进程权限 |
| 资源清理 | 自动 | 手动 | 自动 |
| 大小限制 | 文件系统限制 | 固定大小 | 系统限制 |
| 数据传输 | Zero-copy | 需拷贝 | 需拷贝 |
| 实时性 | 高 | 高 | 中等 |
| 调试难度 | 低 | 高 | 中等 |

## 3. 核心API

### 3.1 基础API
```c
/**
 * 将文件映射到内存空间
 * @param addr    建议的映射地址，通常设为NULL让系统自动选择
 * @param length  要映射的字节数，通常是页大小的整数倍
 * @param prot    内存保护标志，如PROT_READ|PROT_WRITE
 * @param flags   映射标志，如MAP_SHARED|MAP_PRIVATE
 * @param fd      要映射的文件描述符
 * @param offset  文件偏移量，通常为0
 * @return        成功返回映射的内存地址，失败返回MAP_FAILED
 * @errno         ENOMEM(内存不足)、EACCES(权限不足)等
 */
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

/**
 * 解除内存映射
 * @param addr    mmap返回的内存地址
 * @param length  映射的字节数，必须与mmap时相同
 * @return        成功返回0，失败返回-1
 * @errno         EINVAL(参数无效)
 */
int munmap(void *addr, size_t length);

/**
 * 同步内存内容到文件
 * @param addr    要同步的内存地址
 * @param length  要同步的字节数
 * @param flags   同步标志(MS_SYNC立即同步/MS_ASYNC异步同步)
 * @return        成功返回0，失败返回-1
 * @errno         EIO(I/O错误)
 */
int msync(void *addr, size_t length, int flags);

/**
 * 锁定内存页，防止被换出
 * @param addr    要锁定的内存地址
 * @param len     要锁定的字节数
 * @return        成功返回0，失败返回-1
 * @errno         ENOMEM(内存不足)
 */
int mlock(const void *addr, size_t len);

/**
 * 修改内存映射的保护属性
 * @param addr    要修改的内存地址
 * @param len     要修改的字节数
 * @param prot    新的保护属性
 * @return        成功返回0，失败返回-1
 * @errno         EACCES(权限不足)
 */
int mprotect(void *addr, size_t len, int prot);
```

### 3.2 内存保护标志
```c
/**
 * 内存保护标志定义
 */
#define PROT_NONE  0x0    /* 页面不可访问 */
#define PROT_READ  0x1    /* 页面可读 */
#define PROT_WRITE 0x2    /* 页面可写 */
#define PROT_EXEC  0x4    /* 页面可执行 */

/**
 * 常见组合:
 * PROT_READ|PROT_WRITE        // 读写权限，最常用
 * PROT_READ|PROT_EXEC         // 代码段权限
 * PROT_NONE                   // 用于隔离内存区域
 */
```

### 3.3 映射标志
```c
/**
 * 映射标志定义
 */
#define MAP_SHARED    0x01     /* 修改对其他进程可见 */
#define MAP_PRIVATE   0x02     /* 私有映射，修改不共享 */
#define MAP_FIXED     0x10     /* 精确指定映射地址 */
#define MAP_ANONYMOUS 0x20     /* 匿名映射，不需要文件 */
#define MAP_LOCKED    0x2000   /* 锁定页面防止换出 */

/**
 * 常见组合示例:
 */

/* 共享内存方案 */
fd = open("file", O_RDWR);
addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_LOCKED, fd, 0);

/* 私有内存分配 */
addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

/* 只读文件映射 */
fd = open("file", O_RDONLY);
addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
```

## 4. 性能优化

### 4.1 优化策略
| 策略 | 实现方式 | 效果 |
|------|----------|------|
| 内存锁定 | MAP_LOCKED | 防止页面被换出 |
| 内存可见性 | volatile | 确保多进程数据同步 |
| 自旋等待 | CPU pause | 降低CPU占用 |
| 页面对齐 | 按页大小对齐 | 提高访问效率 |

### 4.2 常见问题与解决方案
| 问题 | 解决方案 | 实现示例 |
|------|----------|----------|
| 内存泄漏 | 注册清理函数 | `atexit(cleanup_mappings);` |
| 一致性 | 内存屏障 | `__sync_synchronize();` |
| 并发访问 | 原子操作 | `__atomic_store_n(&flag, 1, __ATOMIC_SEQ_CST);` |
| 文件状态 | 错误检查 | `if (fstat(fd, &st) == -1) { /*...*/ }` |

## 5. 应用场景

### 5.1 最佳实践
| 场景 | 推荐方案 | 原因 |
|------|----------|------|
| 大数据共享 | Memory Mapping | Zero-copy优势明显 |
| 小数据传输 | Pipe/消息队列 | 开销更小 |
| 持久化需求 | Memory Mapping | 自带持久化 |
| 临时通信 | Unix Domain Socket | 更灵活 |

### 5.2 应用举例
| 软件 | 应用场景 | 实现方式 |
|------|----------|----------|
| PostgreSQL | 缓冲区管理 | 文件映射 |
| Nginx | 静态文件服务 | sendfile+mmap |
| Chrome | V8内存管理 | 匿名映射 |
| Redis | AOF文件处理 | 文件映射 |

## 6 调试工具
| 工具 | 用途 | 示例命令 |
|------|------|----------|
| pmap | 查看进程映射 | `pmap -x <pid>` |
| strace | 跟踪系统调用 | `strace -e mmap` |
| /proc | 查看内存映射 | `cat /proc/<pid>/maps` |

## 7. 注意事项

### 7.1 系统资源限制
| 资源 | 限制来源 | 查看/设置方法 |
|------|----------|----------------|
| 文件描述符 | ulimit | `ulimit -n` |
| 映射大小 | vm.max_map_count | `sysctl` |
| 锁定内存 | RLIMIT_MEMLOCK | `ulimit -l` |

### 7.2 安全考虑
| 安全项 | 风险 | 防范措施 |
|--------|------|----------|
| 文件权限 | 未授权访问 | 设置适当权限 |
| 内存暴露 | 信息泄露 | 最小权限原则 |
| 资源耗尽 | 拒绝服务 | 设置资源限制 |