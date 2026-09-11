# 为 UnLua（bytepine 提取）做贡献

本仓是 [Tencent/UnLua](https://github.com/Tencent/UnLua) `Plugins/UnLua` 的非官方提取（基线 v2.3.6）。**不是**腾讯官方仓。上游版权声明见 [LICENSE.TXT](LICENSE.TXT)；改官方源文件须保留腾讯版权头。

## 提交信息

中文 Conventional Commits：`<type>(<scope>): <subject>`

- `type`：`feat` / `fix` / `perf` / `refactor` / `docs` / `chore`
- `scope`：`plugin` / `compat` / `lua` / `docs`，可省略
- **CHANGELOG 与代码改动同一 commit**，不单独提交

## CHANGELOG

有实际功能变更时，追加到 [CHANGELOG.md](CHANGELOG.md) 的 `[Unreleased]`。纯格式 / lint 不写。本文件只记相对官方 v2.3.6 的本仓改动；官方历史见 [上游 CHANGELOG](https://github.com/Tencent/UnLua/blob/master/CHANGELOG.md)。

### 同类合并（硬性）

每个版本段（含 `[Unreleased]`）内，**每种标题只允许出现一次**：`### Added` / `### Changed` / `### Fixed` / `### Deprecated` / `### Removed` / `### Security`。

- 追加条目时：若该标题已存在 → **并入已有小节**（新条目加在该小节列表末尾），禁止再开同名 `###`
- 发现已有重复同名小节 → **先合并再追加**（把后段条目挪到前段同名小节，删掉重复标题）
- 条目按语义归类：`feat`→Added，`fix`→Fixed，`perf`/`refactor`/`chore`/`docs`（行为变更）→Changed；同一条 bullet 只进一个小节

### 内容合并（硬性）

追加前须扫一遍同小节已有条目；**同一主题 / 同一模块 / 同一修复点**不得拆成多条。

| 情形 | 做法 |
|------|------|
| 表述重复或几乎同义 | 删冗余，只留一条最完整表述 |
| 同模块的增量 | **改写进已有 bullet**，用顿号/分号并入，不另起一条 |
| 同根因的多次 fix 描述 | 合并为一条，保留最终行为与关键标识 |
| 仅措辞不同、事实相同 | 视为重复，合并 |

禁止为「本次 commit 又写了一笔」而堆叠同主题 bullet；读者应看到**去重后的能力/修复全集**，而非按提交时间的碎片列表。

## 版本号

日常 commit **只**更新 `CHANGELOG [Unreleased]`，**不**改 `UnLua.uplugin` 的 `VersionName`、**不**打 tag。

发版时：归档 `[Unreleased]` → `[X.Y.Z] - YYYY-MM-DD`，同步 `VersionName`。新功能 → minor；Bug 修复 → patch；破坏性变更 → major。版本号表示**相对官方 v2.3.6 的本仓增量**，不要把上游 2.3.6 之后的官方提交误记成本仓版本。
