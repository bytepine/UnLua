# Changelog — UnLua（bytepine 提取）

所有变更记录遵循 [Keep a Changelog](https://keepachangelog.com/zh-CN/1.0.0/) 格式。
版本号遵循 [语义化版本控制](https://semver.org/lang/zh-CN/)。

本文件**只记录相对 [Tencent/UnLua](https://github.com/Tencent/UnLua) v2.3.6 的本仓改动**。官方历史见 [上游 CHANGELOG](https://github.com/Tencent/UnLua/blob/master/CHANGELOG.md)。写入规范（同类合并 / 内容合并 / Conventional Commits）见 [CONTRIBUTING.md](CONTRIBUTING.md)。日常提交只写入 `[Unreleased]`，不改 `UnLua.uplugin` 的 `VersionName`。

---

## [Unreleased]

### Added

- chore: 从 [Tencent/UnLua](https://github.com/Tencent/UnLua) `Plugins/UnLua` 提取为独立插件仓（基线官方 v2.3.6；不含 UnLuaExtensions / UnLuaTestSuite / TPS 示例工程）；`LICENSE.TXT` 保留腾讯原文与 Lua 第三方声明并标明非官方提取；`README.md`；`CHANGELOG.md`（Keep a Changelog，只记相对官方 v2.3.6 的本仓改动）；`CONTRIBUTING.md`（提交信息与 CHANGELOG 同类/内容合并规范）；`UnLua.uplugin` `CreatedBy` 改为 bytepine（based on Tencent UnLua）
