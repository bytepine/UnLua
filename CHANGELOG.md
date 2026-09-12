# Changelog — UnLua（bytepine 提取）

所有变更记录遵循 [Keep a Changelog](https://keepachangelog.com/zh-CN/1.0.0/) 格式。
版本号遵循 [语义化版本控制](https://semver.org/lang/zh-CN/)。

本文件**只记录相对 [Tencent/UnLua](https://github.com/Tencent/UnLua) v2.3.6 的本仓改动**。官方历史见 [上游 CHANGELOG](https://github.com/Tencent/UnLua/blob/master/CHANGELOG.md)。写入规范（同类合并 / 内容合并 / Conventional Commits）见 [CONTRIBUTING.md](CONTRIBUTING.md)。日常提交只写入 `[Unreleased]`，不改 `UnLua.uplugin` 的 `VersionName`。

---

## [Unreleased]

### Added

- chore: 从 [Tencent/UnLua](https://github.com/Tencent/UnLua) `Plugins/UnLua` 提取为独立插件仓（基线官方 v2.3.6；不含 UnLuaExtensions / UnLuaTestSuite / TPS 示例工程）；`LICENSE.TXT` 保留腾讯原文与 Lua 第三方声明并标明非官方提取；`README.md`；`CHANGELOG.md`（Keep a Changelog，只记相对官方 v2.3.6 的本仓改动）；`CONTRIBUTING.md`（提交信息与 CHANGELOG 同类/内容合并规范）；`UnLua.uplugin` `CreatedBy` 改为 bytepine（based on Tencent UnLua）

### Changed

- perf: `FLuaEnv::TryBind` 按 UClass 缓存绑定决策（`ImplementsInterface` / `SKEL_`），并给 `TryBind` / `TryReplaceInputs` / `NotifyUObjectDeleted` 加 cycle stat（`stat UnLua`）

### Fixed

- 补回官方 v2.3.6 随插件分发的 Lua 5.4.3 各平台预编译库（`lib-c` / `lib-cpp` 的 `.a` / `.lib` / `.dll` / `.dylib`）；`.gitignore` 不再忽略 `Source/ThirdParty/Lua` 下这些文件；二进制走 Git LFS
- fix(compat): `Lua.Build.cs` 在 UE5.5+ 不再引用已删除的 `WindowsCompiler.VisualStudio2019`；UE5.6+ 改用 `CppCompileWarningSettings` 关闭第三方 Lua 警告；UE5.8 识别 VS2026；`UnLuaSettings` 的 `MetaClass` 用 `/Script/CoreUObject.Object`；C# UHT 插件 5.1–5.4 走 `Session.Packages`、5.5+ 走 `Session.Modules`，csproj 在引擎带 `DotNet/6.0.302` 时默认 `net6.0` 否则 `net8.0`（UBT props 可覆盖）、去掉多余的 `Microsoft.CSharp`；`DefaultParamCollection.inl` 源码桩避免 UHT 未生成时 C1083；`TRemoveConst` 垫片仅 5.6+（5.5 引擎仍有该模板）；`UnLuaEx.inl` 改用 `std::remove_const_t`；`AddReferencedObjects` 5.4+ 走 `TObjectPtr`；跨版本只保留 `UnLuaVersionCompat.h` 的 `UL_UE_HAS_*`（`UnLuaCompatibility.h` 与业务 `.cpp/.h` 不再写版本号；C# 收口到 `UnLuaVersionCompat` / `UnLuaUhtVersionCompat`）；忽略 UBT 生成的 `.ubtplugin.csproj.props`；`TChooseClass`/`TRemoveConst`/`TIsTriviallyDestructible` 垫片；`FMetaData::CopyMetadata`（5.6+）；Lua 内部头用 `UnLuaLuaInternalBegin/End` 避开 UE `TString` 别名；`UField::Next`/`SetDefaultObject`/`EInternalObjectFlags_AsyncLoading` 跟 5.5–5.7；`FProperty` 临时属性 4.26 六参 / 5.1–5.7 CodeGen / 5.8 两参；`FEnumProperty`/`FByteProperty` 按语义宏分叉；`LoadSynchronous` 走 Lua C 封装避开 5.8 `ELoadFlags` 成员指针 C4191；`ElementSize` 走 `UL_GET/SET_PROPERTY_ELEMENT_SIZE`；Editor 跟 `GetOnPostEngineInit` / `EGetObjectsFlags`（5.8）
- `FClassRegistry::Unregister` 摘除 C++ 侧索引，失效 desc 进墓地列表（Lua 闭包 upvalue 仍可能持有指针）；`PushMetatable` 对 `UScriptStruct` 同样反注册失效 metatable；`~FLuaEnv` 先摘 `GUObjectArray` 监听
- `TSmartPtrConstructor` 日志 `%s` 误对 `TType::GetName()`（`const char*`）解引用，改为 `UTF8_TO_TCHAR`
- 就地覆写路径用 `ULuaFunction::Get` 去重，避免多 env 重复登记；UE 5.3+ `ULuaOverridesClass::AddToOwner`/`RemoveFromOwner` 不再把 `Children` 写进 `TObjectPtr` 局部副本
- 覆写 `Script` 魔数头改存 `FObjectKey`，蓝图重编译 / `UnLua.HotReload` 后不再留下野指针
