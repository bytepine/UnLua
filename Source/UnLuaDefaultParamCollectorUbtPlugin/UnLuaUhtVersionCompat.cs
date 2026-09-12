using System;
using EpicGames.Core;
using EpicGames.UHT.Tables;
using EpicGames.UHT.Types;
using EpicGames.UHT.Utils;

namespace UnLuaDefaultParamCollectorUbtPlugin
{
    /// <summary>
    /// UHT 插件跨版本入口（对齐 UnLuaVersionCompat.h）。版本号只允许出现在本类型内。
    /// </summary>
    internal static class UnLuaUhtVersionCompat
    {
        internal static void ForEachRuntimePackage(
            IUhtExportFactory factory,
            Action<string, UHTModuleType, string> parseModule,
            Action<UhtPackage, UhtPackage> queueClassExports)
        {
            var session = factory.Session;
#if UE_5_5_OR_LATER
            foreach (UhtModule uhtModule in session.Modules)
            {
                var manifest = uhtModule.Module;
                var moduleType = manifest.ModuleType;
                parseModule(manifest.Name, moduleType, manifest.OutputDirectory);
                if (moduleType != UHTModuleType.EngineRuntime && moduleType != UHTModuleType.GameRuntime)
                    continue;
                foreach (UhtPackage package in uhtModule.Packages)
                    queueClassExports(package, package);
            }
#else
            foreach (UhtPackage package in session.Packages)
            {
                var moduleType = package.Module.ModuleType;
                parseModule(package.Module.Name, moduleType, package.Module.OutputDirectory);
                if (moduleType != UHTModuleType.EngineRuntime && moduleType != UHTModuleType.GameRuntime)
                    continue;
                queueClassExports(package, package);
            }
#endif
        }
    }
}
