// Tencent is pleased to support the open source community by making UnLua available.
// 
// Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the MIT License (the "License"); 
// you may not use this file except in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, 
// software distributed under the License is distributed on an "AS IS" BASIS, 
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.

using System;
using System.IO;
// using 必须跟 UBT 预处理器（类型不存在）；语义分叉见 UnLuaVersionCompat
#if UE_5_0_OR_LATER
using EpicGames.Core;
#else
using Tools.DotNETCommon;
#endif
using UnrealBuildTool;

public class UnLua : ModuleRules
{
    public UnLua(ReadOnlyTargetRules Target) : base(Target)
    {
        UnLuaVersionCompat.DisableIwyu(this);
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.AddRange(
            new[]
            {
                "UnLua/Private",
                Path.Combine(ModuleDirectory, "Public"),
            }
        );

        PublicDependencyModuleNames.AddRange(
            new[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "InputCore",
                "Lua"
            }
        );

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        if (Target.bBuildEditor)
        {
            OptimizeCode = CodeOptimization.Never;
            PrivateDependencyModuleNames.Add("UnrealEd");
        }

        var projectDir = Target.ProjectFile.Directory;
        var configFilePath = projectDir + "/Config/DefaultUnLuaEditor.ini";
        var configFileReference = new FileReference(configFilePath); 
        var configFile = FileReference.Exists(configFileReference) ? new ConfigFile(configFileReference) : new ConfigFile();
        var config = new ConfigHierarchy(new[] { configFile });
        const string section = "/Script/UnLuaEditor.UnLuaEditorSettings";

        Action<string, string, bool> loadBoolConfig = (key, macro, defaultValue) =>
        {
            bool flag;
            if (!config.GetBool(section, key, out flag))
                flag = defaultValue;
            PublicDefinitions.Add(string.Format("{0}={1}", macro, (flag ? "1" : "0")));
        };
        
        Action<string, string, string > loadStringConfig = (key, macro, defaultValue) =>
        {
            string value;
            if (!config.GetString(section, key, out value))
                value = defaultValue;
            PublicDefinitions.Add(string.Format("{0}={1}", macro, value));
        };

        loadBoolConfig("bAutoStartup", "AUTO_UNLUA_STARTUP", true);
        loadBoolConfig("bEnableDebug", "UNLUA_ENABLE_DEBUG", false);
        loadBoolConfig("bEnablePersistentParamBuffer", "ENABLE_PERSISTENT_PARAM_BUFFER", true);
        loadBoolConfig("bEnableTypeChecking", "ENABLE_TYPE_CHECK", true);
        loadBoolConfig("bEnableUnrealInsights", "ENABLE_UNREAL_INSIGHTS", false);
        loadBoolConfig("bEnableCallOverriddenFunction", "ENABLE_CALL_OVERRIDDEN_FUNCTION", true);
        loadBoolConfig("bEnableFText", "UNLUA_ENABLE_FTEXT", false);
        loadBoolConfig("bLuaCompileAsCpp", "LUA_COMPILE_AS_CPP", false);
        loadBoolConfig("bWithUE4Namespace", "WITH_UE4_NAMESPACE", true);
        loadBoolConfig("bLegacyReturnOrder", "UNLUA_LEGACY_RETURN_ORDER", false);
        loadBoolConfig("bLegacyBlueprintPath", "UNLUA_LEGACY_BLUEPRINT_PATH", false);
        loadBoolConfig("bLegacyAllowUTF8WithBOM", "UNLUA_LEGACY_ALLOW_BOM", false);
        loadBoolConfig("bLegacyArgsPassing", "UNLUA_LEGACY_ARGS_PASSING", true);
        loadStringConfig("LuaVersion", "UNLUA_LUA_VERSION", "lua-5.4.3");

        string hotReloadMode;
        if (!config.GetString(section, "HotReloadMode", out hotReloadMode))
            hotReloadMode = "Manual";

        var withHotReload = hotReloadMode != "Never";
        PublicDefinitions.Add("UNLUA_WITH_HOT_RELOAD=" + (withHotReload ? "1" : "0"));

        if (IsPluginEnabled("LuaCompat"))
            PublicIncludePaths.Add(Path.Combine(PluginDirectory, "Source/ThirdParty/Lua/lua-compat-5.3/c-api"));
    }

    private bool IsPluginEnabled(string name)
    {
        var engineDir = DirectoryReference.FromString(EngineDirectory);
        var projectDir = Target.ProjectFile.Directory;
        var projectDesc = ProjectDescriptor.FromFile(Target.ProjectFile);
        
        foreach (var plugin in Plugins.ReadAvailablePlugins(engineDir, projectDir, null))
        {
            if (plugin.Name != name)
                continue;
            return Plugins.IsPluginEnabledForTarget(plugin, projectDesc, Target.Platform, Target.Configuration, Target.Type);
        }

        return false;
    }
}

/// <summary>
/// C# 跨版本入口（对齐 UnLuaVersionCompat.h）。版本号 / UE_*_OR_LATER 只允许出现在本类型内。
/// UBT 编译 Build.cs 时 using 必须跟预处理器（类型不存在），见各 Build.cs 文件头。
/// </summary>
public static class UnLuaVersionCompat
{
    public static void DisableIwyu(ModuleRules Module)
    {
#if UE_5_2_OR_LATER
        Module.IWYUSupport = IWYUSupport.None;
#else
        Module.bEnforceIWYU = false;
#endif
    }

    public static void ApplyExternalModuleCompileWarnings(ModuleRules Module)
    {
#if UE_5_6_OR_LATER
        Module.CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Off;
        Module.CppCompileWarningSettings.ShadowVariableWarningLevel = WarningLevel.Off;
#elif UE_5_5_OR_LATER
        Module.UndefinedIdentifierWarningLevel = WarningLevel.Off;
        Module.ShadowVariableWarningLevel = WarningLevel.Off;
#else
        Module.bEnableUndefinedIdentifierWarnings = false;
        Module.ShadowVariableWarningLevel = WarningLevel.Off;
#endif
    }

    public static bool HasDeveloperToolSettings
    {
        get
        {
#if UE_5_0_OR_LATER
            return true;
#else
            return false;
#endif
        }
    }

    public static IAndroidToolChain CreateAndroidToolChain(ReadOnlyTargetRules Target)
    {
#if UE_5_2_OR_LATER
        var ueBuildPlatformType = System.Reflection.Assembly.GetAssembly(typeof(IAndroidToolChain)).GetType("UnrealBuildTool.UEBuildPlatform");
        var getBuildPlatformMethod = ueBuildPlatformType.GetMethod("GetBuildPlatform", System.Reflection.BindingFlags.Static | System.Reflection.BindingFlags.Public);
        var androidBuildPlatform = getBuildPlatformMethod.Invoke(null, new object[] { UnrealTargetPlatform.Android });
        var createTempToolChainForProjectMethod = androidBuildPlatform.GetType().GetMethod("CreateTempToolChainForProject");
        return (IAndroidToolChain)createTempToolChainForProjectMethod.Invoke(androidBuildPlatform, new object[] { Target.ProjectFile });
#else
        return AndroidExports.CreateToolChain(Target.ProjectFile);
#endif
    }

    public static string GetArchitectureString(ReadOnlyTargetRules Target)
    {
#if UE_5_2_OR_LATER
        return Target.Architecture.ToString();
#else
        return Target.Architecture;
#endif
    }

    public static string GetWindowsCMakeGenerator(ReadOnlyTargetRules Target)
    {
#if !UE_5_4_OR_LATER
        if (Target.WindowsPlatform.Compiler == WindowsCompiler.VisualStudio2019)
            return "Visual Studio 16 2019";
#endif
#if UE_4_27_OR_LATER
        if (Target.WindowsPlatform.Compiler == WindowsCompiler.VisualStudio2022)
            return "Visual Studio 17 2022";
#endif
#if UE_5_8_OR_LATER
        if (Target.WindowsPlatform.Compiler == WindowsCompiler.VisualStudio2026)
            return "Visual Studio 18 2026";
#endif
        return null;
    }
}