# UnLua（非官方插件提取）

从 [Tencent/UnLua](https://github.com/Tencent/UnLua) 的 `Plugins/UnLua` 提取，基线 **v2.3.6**。

**不是**腾讯官方仓库，也未获腾讯背书。后续修改只在本仓进行。

## 安装

本仓根目录就是插件根（含 `UnLua.uplugin`）。放到工程的 `Plugins/UnLua`：

```bash
git submodule add https://github.com/bytepine/UnLua.git Plugins/UnLua
```

或克隆/拷贝到 `YourProject/Plugins/UnLua`，在 `.uproject` 中启用名为 `UnLua` 的插件。

不要按官方 README 去「复制整个 `Plugins` 目录」——那是示例工程布局，不是本仓布局。

## 与官方差异

- 仅含核心插件 UnLua，不含 `UnLuaExtensions`、`UnLuaTestSuite` 和 TPS 示例工程
- 需要 luasocket 等调试扩展时，请自行从 [官方仓](https://github.com/Tencent/UnLua/tree/master/Plugins/UnLuaExtensions) 取 `UnLuaExtensions`

## 文档

编程与设置仍以官方文档为准：

- [编程指南](https://github.com/Tencent/UnLua/blob/master/Docs/CN/UnLua_Programming_Guide.md)
- [设置选项](https://github.com/Tencent/UnLua/blob/master/Docs/CN/Settings.md)
- [FAQ](https://github.com/Tencent/UnLua/blob/master/Docs/CN/FAQ.md)

问题请开 [本仓 Issues](https://github.com/bytepine/UnLua/issues)，不要提到 Tencent/UnLua。

## 许可

见 [LICENSE.TXT](LICENSE.TXT)。上游版权归 Tencent；Lua 归 Lua.org, PUC-Rio。本仓修改同样使用 MIT。

## 变更

本仓相对官方 v2.3.6 的改动见 [CHANGELOG.md](CHANGELOG.md)。
