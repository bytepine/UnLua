# Changelog — UnLua (bytepine extraction)

[中文](CHANGELOG.md) | English

All changes follow [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
Version numbers follow [Semantic Versioning](https://semver.org/).

This document records **only this repository's changes relative to [Tencent/UnLua](https://github.com/Tencent/UnLua) v2.3.6**. Official history is in the [upstream CHANGELOG](https://github.com/Tencent/UnLua/blob/master/CHANGELOG.md). See [CONTRIBUTING.en.md](CONTRIBUTING.en.md) for category/content merging and Conventional Commits. Daily commits update only `[Unreleased]`; do not change `UnLua.uplugin`'s `VersionName`.

---

## [Unreleased]

### Added

- chore: Extract [Tencent/UnLua](https://github.com/Tencent/UnLua)'s `Plugins/UnLua` as a standalone plugin repository based on upstream v2.3.6, excluding UnLuaExtensions / UnLuaTestSuite / the TPS sample project. `LICENSE.TXT` retains Tencent's original text and Lua third-party notices and identifies the unofficial extraction. Add `README.md`, `CHANGELOG.md` (Keep a Changelog, limited to changes relative to upstream v2.3.6), and `CONTRIBUTING.md` (commit messages and CHANGELOG category/content merging rules). Change `UnLua.uplugin`'s `CreatedBy` to bytepine (based on Tencent UnLua).

### Changed

- perf: `FLuaEnv::TryBind` caches binding decisions by UClass (`ImplementsInterface` / `SKEL_`); add cycle stats for `TryBind` / `TryReplaceInputs` / `NotifyUObjectDeleted` (`stat UnLua`).

### Fixed

- Restore the platform-specific prebuilt Lua 5.4.3 libraries shipped with upstream v2.3.6 (`lib-c` / `lib-cpp` `.a` / `.lib` / `.dll` / `.dylib`). `.gitignore` no longer excludes these files under `Source/ThirdParty/Lua`; binaries use Git LFS.
- fix(compat): `Lua.Build.cs` no longer references the removed `WindowsCompiler.VisualStudio2019` on UE 5.5+. UE 5.6+ uses `CppCompileWarningSettings` to disable third-party Lua warnings; UE 5.8 recognizes VS2026. `UnLuaSettings.MetaClass` uses `/Script/CoreUObject.Object`. The C# UHT plugin uses `Session.Packages` on 5.1–5.4 and `Session.Modules` on 5.5+; its csproj defaults to `net6.0` when the engine includes `DotNet/6.0.302`, otherwise `net8.0` (overridable through UBT props), and removes redundant `Microsoft.CSharp`. A source stub for `DefaultParamCollection.inl` prevents C1083 before UHT generation. The `TRemoveConst` shim is limited to 5.6+ because 5.5 still provides that template; `UnLuaEx.inl` uses `std::remove_const_t`. `AddReferencedObjects` uses `TObjectPtr` on 5.4+. Cross-version branches are centralized in `UnLuaVersionCompat.h`'s `UL_UE_HAS_*` macros; `UnLuaCompatibility.h` and business `.cpp/.h` files no longer contain version checks, and C# uses `UnLuaVersionCompat` / `UnLuaUhtVersionCompat`. Ignore UBT-generated `.ubtplugin.csproj.props`. Add `TChooseClass` / `TRemoveConst` / `TIsTriviallyDestructible` shims and `FMetaData::CopyMetadata` support (5.6+). Lua internal headers use `UnLuaLuaInternalBegin/End` to avoid UE's `TString` alias. Adapt `UField::Next` / `SetDefaultObject` / `EInternalObjectFlags_AsyncLoading` for 5.5–5.7. Temporary `FProperty` construction uses six arguments on 4.26, CodeGen on 5.1–5.7, and two arguments on 5.8; `FEnumProperty` / `FByteProperty` use semantic feature macros. `LoadSynchronous` uses a Lua C wrapper to avoid C4191 from 5.8 `ELoadFlags` member pointers. Access `ElementSize` through `UL_GET/SET_PROPERTY_ELEMENT_SIZE`; adapt Editor code for `GetOnPostEngineInit` / `EGetObjectsFlags` (5.8).
- `FClassRegistry::Unregister` removes C++ indexes and retains invalid descriptors in a tombstone list, as Lua closure upvalues may still hold their pointers. `PushMetatable` also unregisters invalid metatables for `UScriptStruct`; `~FLuaEnv` detaches the `GUObjectArray` listener first.
- Fix `TSmartPtrConstructor` logging that incorrectly dereferences `TType::GetName()` (`const char*`) for `%s`; convert with `UTF8_TO_TCHAR`.
- In-place override paths deduplicate through `ULuaFunction::Get` to prevent duplicate registration across environments. On UE 5.3+, `ULuaOverridesClass::AddToOwner` / `RemoveFromOwner` no longer write `Children` into a local `TObjectPtr` copy.
- Store `FObjectKey` in the override `Script` magic header, preventing dangling pointers after Blueprint recompilation / `UnLua.HotReload`.
- fix(compat): On Android NDK clang, `FSoftObjectPtr::Set` no longer exports an `operator=` member pointer, whose overload cannot be converted through C on MSVC 4.26–5.5 / NDK clang. Use a Lua C wrapper for assignment instead. `FEnumRegistry` / `FClassRegistry` destructors use references in range-for loops to avoid `-Werror,-Wrange-loop-construct`.
- UE 5.9 debug-value printing uses `%lld` / `%llu` for 64-bit integers; IntelliSense type names no longer reference the deprecated `TLazyObjectPtr`.
