# UnLua (unofficial plugin extraction)

[中文](README.md) | English

Extracted from [Tencent/UnLua](https://github.com/Tencent/UnLua)'s `Plugins/UnLua`, with **v2.3.6** as the baseline.

This is **not** Tencent's official repository and is not endorsed by Tencent. Subsequent modifications are maintained in this repository.

## Installation

This repository root is the plugin root, containing `UnLua.uplugin`. Place it in your project's `Plugins/UnLua`:

```bash
git submodule add https://github.com/bytepine/UnLua.git Plugins/UnLua
```

Alternatively, clone/copy it into `YourProject/Plugins/UnLua` and enable the plugin named `UnLua` in `.uproject`.

Do not follow the official README's instruction to copy the entire `Plugins` directory: that instruction describes the sample-project layout, not this repository's layout.

## Differences from upstream

- Includes only the core UnLua plugin, without `UnLuaExtensions`, `UnLuaTestSuite`, or the TPS sample project.
- If you need debugging extensions such as luasocket, obtain `UnLuaExtensions` separately from the [official repository](https://github.com/Tencent/UnLua/tree/master/Plugins/UnLuaExtensions).

## Documentation

Use the official documentation for programming and settings:

- [Programming guide](https://github.com/Tencent/UnLua/blob/master/Docs/CN/UnLua_Programming_Guide.md).
- [Settings](https://github.com/Tencent/UnLua/blob/master/Docs/CN/Settings.md).
- [FAQ](https://github.com/Tencent/UnLua/blob/master/Docs/CN/FAQ.md).

Report issues in [this repository](https://github.com/bytepine/UnLua/issues), rather than Tencent/UnLua.

All maintained documentation must provide complete Chinese and English versions. Keep paired files and changelogs synchronized; see [CONTRIBUTING.en.md](CONTRIBUTING.en.md).

## License

See [LICENSE.TXT](LICENSE.TXT). Upstream copyright belongs to Tencent; Lua belongs to Lua.org, PUC-Rio. This repository's changes also use MIT.

## Changes

See [CHANGELOG.en.md](CHANGELOG.en.md) for this repository's changes relative to upstream v2.3.6. Contribution conventions are in [CONTRIBUTING.en.md](CONTRIBUTING.en.md).
