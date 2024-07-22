# v1.5.3

> release-date: 2024-07-22

> 「成功的关键在于我们对失败的反应。」 - ChatGPT
> 
> "The key to success is how we respond to failure." - ChatGPT

该版本主要修复了 cURL 在特定条件下会死循环的问题，并新增了日语版本的 README。

This version mainly fixes the issue of cURL getting stuck in an infinite loop under certain conditions, and adds a Japanese version of the README.

## 🐣 What's New

+ Add FileSystem::scanDirRecursive() (b238947) (@twose)

## ✨ What's Enhanced

+ Japanese README (#252) (@eltociear)

## 🐛 What's Fixed

* Fix cURL dead loop bug (libcat/libcat@0d68189e) (@twose)
* Fix strange hard code in builder (0fcf143) (@twose)

## 👻 What's Removed

- Nothing removed

## 📦 Internal

+ Update swow_fs (44621bb) (@dixyes)

---

# v1.5.2

> release-date: 2024-05-13

> 真正的快乐不是没有痛苦，而是能在痛苦中找到乐趣。 - ChatGPT
>
> True joy is not about being without pain, but about finding pleasure in pain. - ChatGPT

这是一个修复版本。

## 🐣 What's New

+ Nothing new

## ✨ What's Enhanced

+ Nothing enhanced

## 🐛 What's Fixed

* Callable curl options should be nullable (682b4be) (@twose)

## 👻 What's Removed

- Nothing removed

---

# v1.5.1

> release-date: 2024-05-13

> 「没有最终的成功，也没有致命的失败，最可贵的是继续前进的勇气。」 - 温斯顿·丘吉尔
>
> "Success is not final, failure is not fatal. It is the courage to continue that counts." - Winston Churchill

这是一个修复版本。

## 🐣 What's New

+ Nothing new

## ✨ What's Enhanced

+ Nothing enhanced

## 🐛 What's Fixed

* Fix flock on Windows (e1121c0) (@dixyes)
* Fix missing CURLM_RECURSIVE_API_CALL on curl < 7.59.0 (a92d70c) (@dixyes)
* Fix wrong parameter position of recvMessage() call (#240) (@assert6)
* Fix FCC implementation compatibility (46e7d3d) (@twose)
* Fix slow cURL when composer install (libcat/libcat@55959bf) (@twose)
+ Fix incorrect nNumUsed of functions table (07cea2b) (@twose)

## 👻 What's Removed

- Nothing removed

---

# v1.5.0

> release-date: 2024-05-05

> 「优秀的项目获取 star，伟大的项目赢得人心。」 - Swow
>
> "Excellent projects earn stars, great projects win hearts and minds." - Swow

版本亮点：

1、全新的 cURL 协程化支持，底层重构实现，覆盖各种边缘 cases；

2、完整包含 PHP cURL 实现，不再依赖 cURL 扩展，因此可适配所有运行环境；

3、Debugger 支持远程 telnet 调试、Psr7 Server 支持 HTTP chunked response。

## 🐣 What's New

+ Brand new cURL full support!!! (9a44716) (@twose)
+ Debugger support EOF Stream now! (2710c0e) (@twose)
+ Support send HTTP chunked response (b185524) (@twose)
+ Add Context and CoroutineContext (a16429d) (@twose)
+ Support PHP 8.3 and PHP 8.4-dev (@twose)

## ✨ What's Enhanced

+ Add EventDriver example for HTTP Server (8521677) (@twose)
+ Show server url in mixed server example (0520690) (@twose)
+ Introduce polyfill file to fix PHP-8.4-dev deprecations (bf47c99) (@twose)

## 🐛 What's Fixed

* Fix WebSocket parsing bug and add a test for it (bd9b93d) (@twose)
* Fix missing socket crypto options (99c502b) (@twose)
* Fix #235 (SSLv3 compilation error) (9e2c0fb) (@twose)
* Fix builder error when php-config is not configured (93fd794) (@twose)
* Fix example code (41da6e8) (@twose)
+ Use maxHeaderLength for Buffer size and Fix construction order (9ac0e70) (@twose)
* Fix errors in tools (6243bb3) (@twose)

## 👻 What's Removed

- We are no longer rely on cURL extension (e462ce2) (@twose)
- Remove unused clean_module_class_callback (a53184d) (@twose)
- Remove unused maxBufferSize things (d38ddce) (@twose)

## 📦 Internal

+ Some SSL fixes in libcat (@dixyes)
+ Update swow_fs from php (b5c9654) (@dixyes)
+ Update swow_fs rev anchors (49a91cc) (@dixyes)
+ Update artifact actions (de65d57) (@dixyes)
+ Support PHPUnit 11 (@twose)
+ Support hook constants and cleanup modules (34ad07c) (@twose)
+ Make blank-fixer smarter (177cbd0) (@twose)
+ Fix -ssl not found error on macOS CI (57a0baa) (ccd28c2) (@twose)

----

# v1.4.1

> release-date: 2023-12-10

> 「生活并不是等待暴风雨过去，而是学会如何在雨中跳舞。」- 维维安·格林
> 
> "Life isn't about waiting for the storm to pass. It's about learning how to dance in the rain." - Vivian Greene

## 🐛 What's Fixed

* Fix SSL context not work (fb15f29) (libcat/libcat@bc6ec0a8) (@twose)
* Initialize in_autoload in runtime init (8fcdbd0) (@dixyes)
* Fix missing stream error info (232b509) (@twose)

---

# v1.4.0

> release-date: 2023-12-03

> 「不要盯着时钟看，要做时钟所做的事情，继续前进。」 - 萨姆·莱文森
>
> "Don't watch the clock; do what it does. Keep going." - Sam Levenson

## 🐣 What's New

+ Add EventDriver for Psr7 Server (54cd3ab) (@twose)
+ New API: Socket->open() (6bbcd89) (@twose)
+ New API: stream_select_unlimited (231f580) (@twose)
+ Support SSL security_level and alpn_protocols (8fdda59) (@twose)
+ Support serialization for Buffer (206fb1b) (@twose)
+ Support coroutine switch in autoload (f49445b) (@twose)
+ Add lo_lseek64 weak symbol (f7cefee) (@dixyes)
+ Add --enable-debug-log to the compilation options (ce6fb89) (@twose)
+ Support disable Swow by `swow.enable=0` (7f46b5e) (@twose)

## ✨ What's Enhanced

+ Enhance showExecutedSourceLines() (addd0de) (@twose)
+ Show SSL info in stream metadata (c12d1c9) (@twose)
+ Improve `php --ri swow` output (393304f) (@twose)
+ Make parsedBody nullable (#202) (@limingxinleo)
+ Support v2.0 for `psr/http-message` (#199) (@limingxinleo)
+ Handle SIGHUP signal in dontdie (#225) (@AuroraYolo)
+ Add return type to recvMessage method (#224) (@AuroraYolo)
+ Update namespace in stream tests (#226) (@AuroraYolo)
+ Make php-wrapper smarter (5e8fc64) (@twose)
+ Use GPT to generate CHANGELOG (4f250e7) (@twose)

## 🐛 What's Fixed

* Workaround for mysql_handle_closer() (56d6ffe) (@twose)
* Fix socket_export_stream() hook (f6e6b28) (f94d381) (@twose)
* Fix cURL on PHP-8.3 (76a5bcc) (@twose)
* Fix #208 (ipv6_v6only compatibility) (d2059ad) (@twose)
* Fix return value of stream_socket_sendto() (e1f6a7f) (@twose)
* Fix weak dep symbols (b97bd1e) (@dixyes)

## 👻 What's Removed

- Remove interface dependencies from psr7-plus (a990bf2) (@twose)

---

# v1.3.1

> release-date: 2023-06-15

> 「成就伟大的唯一途径是热爱自己的工作。」 - 史蒂夫·乔布斯
>
> "The only way to do great work is to love what you do." - Steve Jobs

## 🐛 What's Fixed

* Fix memory error due to call destructor in scheduler (#198) (@twose)

---

# v1.3.0

> release-date: 2023-06-09

> 差距并不可怕，可怕的是因为差距而放弃。
>
> The gap is not terrible. What's terrible is giving up because of it.

## 🐣 What's New

+ Support PDO PostgreSQL (#137) (@huanghantao)
+ Weak dependency for PDO (#171) (@dixyes)
+ Support ServerConnection->sendHttpFile() (#168) (@PandaLIU-1111)
+ Support remove temp file automatically (#172) (@PandaLIU-1111)
+ Support simple HTTP recvMessage timeout (dd615e9) (@twose)
+ Server config connection recvMessage timeout (#194) (@xuanyanwow)
+ Provides psr7-plus in an independent composer package (6142114)  (2667931) (@twose)
+ Introduce dontdie as an independent composer package (6ce662e) (@twose)

## ✨ What's Enhanced

+ Use psr/http-message v1.1 (e6743e5) (@twose)
+ Rename swow.async_stdio to swow.async_tty (7283708) (@twose)
+ Remove extra export ignores (2e5abe9) (@dixyes)
+ Support resolve path list of autoload in composer.json (90e0d39) (@twose)
+ Support maxExecutionTime for dontDie() (ddfe1ad) (@twose)
+ Support nickname option for dontdie (8f347af) (@twose)
+ Add a test for empty Content-Type (e6a7ce1) (@twose)
+ Added ServerConnection SendHttpFile test case (#170) (@PandaLIU-1111)
+ Add comments for some Buffer methods (57b2121) (@twose)
+ Fix "Doc tag without variable name" (ea620d9) (@twose)

## 🐛 What's Fixed

* Fix Psr7 BufferStream implementation and add tests for it (#192) (@twose)
* Fix setParsedBody params type (#174) (@duxphp)
* Fix PHP stream create TLS server error (#187) (f758a40) (@twose)
* Fix incorrect parsedOffset (94b689c) (@twose)
* Fix preserveBodyData not work (4fb20dd) (@twose)
* Fix incorrect error code of try APIs (572278b) (@twose)
* Fix require package name (c9a5dea) (@twose)
* Fix pdo header check (255ac36) (@dixyes)
* Fix ulimit typo (a628513) (@dixyes)
* Fix MODULES_CHECK_PRE memory error (90bb701) (@twose)
* Fix exdev copy logic (70cb109) (@dixyes)
* Fix Socket->sendFile() behaviour with length 0 (aad9911) (@twose)
* Always backup native ops/wrapper (efc2f80) (@twose)
* Defensive programming for ENOTCONN when call getpeername() on accepted socket (32c1fa5) (@twose)
* Defensive programming for preserve body data case (0e2cb0f) (@twose)
* Workaround for test-extension unexpected exit (70c1744) (@twose)

## 👻 What's Removed

- Nothing removed

---

# v1.2.0

> release-date: 2023-02-26

> 成功的秘诀是开始行动。
>
> The secret of getting ahead is getting started.

## 🐣 What's New

- New API: Socket->sendFile() (da2397f)
- New APIs: `Coroutine->getStartTime()`, `Coroutine->getEndTime()` (64f516e)
- New Constant: WebSocket::DEFAULT_MASKING_KEY (72f1920)
- New Config: Introduce autoUnmask config for specific requirements (56088cc)
- Support use PHP ini to configure async stdio/file hook and async threads (swow.async_threads=4, swow.async_file=1, swow.async_stdio=1 by default) (0d94d1a)

## ✨ What's Enhanced

- Support get php binary files via php-config (#158) (@sy-records)
- Run update docs ci under swow organization only (#159) (@sy-records)
- Handle empty maskingKey and empty data for WebSocket::unmask() (3d07a94)
- Skip NULL values when pack headers (977c54c)
- Add HTTP event-stream case for mixed server example (923e69e)

## 🐛 What's Fixed

- Fix stream_select() hangs on server for all platforms (#162)
- Fix missing SSL module init (90ea96c)
- Fix test-extension.php can not run without composer (329d49d)
- Fix callable memory leak when coroutine allocation failed (25be041)
- Some fixes for Debugger: Fix `p` and `vars` does not work; Fix missing step_in command (63a9c84)
- Fix PHP rename() did not work for cross devices (9903049)
- Fix WebSocket payloadData buffer size (ee8bc7c)
- Fix Message->addHeader() and add a test for it (2e51848)

## 👻 What's Removed

- Remove API: Socket->setTcpAcceptBalance() (72e7c44)

---

# v1.1.0

> release-date: 2023-01-18

> 永远不要熄灭心中的火，哪怕别人只看见烟。
> Never extinguish the fire in your heart, even if others can only see smoke.

## 🐣 What's New

- New README (b121caa)
- New PHP API: `Coroutine->getSwitches()`, `Coroutine::getGlobalSwitches()` (2ef2ac3)
- Add `HttpParser` errno (3365aaa) (0a8c96b)
- New C APIs: version getters (f0ce087)

## ✨ What's Enhanced

- Do not always close all connections when closing Server (graceful shutdown)] (dd00372)
- Optimizations for swow-builder (2ed5f60)

## 🐛 What's Fixed

- Fix bad arg name of Signal methods (6041422)

## 🐞 What's Fixed in Dependencies

- Fix va_args memory error (libcat/libcat@6d08909)
- Fix bad usage of buffer value (libcat/libcat@6c20f9e)
- Fix curl easy handle exec dead loop with SSL connection (libcat/libcat@6f5865d)
- Full cURL workaround solution for SSL connection (libcat/libcat@1e8d176)

## 👻 What's Removed

- Remove `Coroutine->getRound()`, `Coroutine::getCurrentRound()` (2ef2ac3)

---

# v1.0.0

> release-date: 2023-01-03

**Swow 是一个专注于并发 I/O 的跨平台协程引擎**，它致力于使用最小 C 核心及多数 PHP 代码以支持 PHP 高性能网络编程，具有极佳的扩展性与强大的调试能力，最大化开发者的编程效率。

Swow is a high-performance pure coroutine network communication engine, which is based on PHP and C.
It is committed to using the smallest C core and most of the PHP code to support PHP high-performance network programming.
Furthermore, it has excellent scalability and powerful debugging capabilities to maximize the programming efficiency of developers.

[Blog Post about v1.0.0](https://docs.toast.run/swow-blog/chs/init.html)
