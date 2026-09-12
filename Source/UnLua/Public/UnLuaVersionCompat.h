// Copyright bytepine. All Rights Reserved.

#pragma once

// 跨版本编译兼容宏（对齐 NexusLink NexusVersionCompat.h）
// 将 ENGINE_MAJOR_VERSION / ENGINE_MINOR_VERSION 数值化，使版本比较只需一行。
// 业务代码用法：#if UL_UE_HAS_<语义>（禁止在 UnLuaVersionCompat.h 外写 UL_UE_AT_LEAST）

#ifndef ENGINE_MAJOR_VERSION
#include "Runtime/Launch/Resources/Version.h"
#endif

#include <type_traits>

#define UL_UE_VERSION  (ENGINE_MAJOR_VERSION * 100 + ENGINE_MINOR_VERSION)

#define UL_UE_AT_LEAST(Major, Minor)  (UL_UE_VERSION >= (Major) * 100 + (Minor))

// ── 语义别名：按 API 变更点命名 ──

#define UL_UE_HAS_DEFINE_FUNCTION_MACRO         UL_UE_AT_LEAST(4, 19) // DEFINE_FUNCTION / FNativeFuncPtr
#define UL_UE_HAS_FOBJECT_KEY                   UL_UE_AT_LEAST(4, 27) // FObjectKey + ResolveObjectPtr
#define UL_UE_HAS_INPUT_ACTION_GETTERS          UL_UE_AT_LEAST(4, 20) // FInputActionBinding::GetActionName/IsPaired
#define UL_UE_HAS_INVTEXT_DEFAULT_PARAM         UL_UE_AT_LEAST(4, 21) // FText 默认参数 INVTEXT("...")
#define UL_UE_HAS_TMAP_GETDATA_KEY_INLINE       UL_UE_AT_LEAST(4, 22) // TMap::GetData 已含 Key，不再加 KeyOffset
#define UL_UE_HAS_TMODELS_GET_TYPE_HASHABLE     UL_UE_AT_LEAST(4, 23) // THasGetTypeHash → TModels<CGetTypeHashable>
#define UL_UE_HAS_SPARSE_MULTICAST_DELEGATE     UL_UE_AT_LEAST(4, 23) // FMulticastSparseDelegate / AddDelegate / CPT_MulticastSparseDelegate
#define UL_UE_HAS_STD_IS_SAME_V                 UL_UE_AT_LEAST(5, 2)  // TIsSame → std::is_same_v
#define UL_UE_HAS_USTRUCT_CHILDREN_TOBJECTPTR   UL_UE_AT_LEAST(5, 3)  // UStruct::Children 为 TObjectPtr，需 .Get()
#define UL_UE_HAS_UFUNCTION_SUPER_BIND           UL_UE_AT_LEAST(5, 3)  // UFunction::Bind 走 Super::Bind
#define UL_UE_HAS_TCHOOSE_CLASS                 (!UL_UE_AT_LEAST(5, 4)) // 5.4+ 不再从 CoreMinimal 带入且已 deprecated
#define UL_UE_HAS_TREMOVE_CONST                 (!UL_UE_AT_LEAST(5, 6)) // TRemoveConst 5.5 仍在（deprecated），5.6 删除
#define UL_UE_HAS_TIS_TRIVIALLY_DESTRUCTIBLE     (!UL_UE_AT_LEAST(5, 5)) // 5.5+ 不再默认包含
#define UL_UE_HAS_TSTRING_ALIAS                 UL_UE_AT_LEAST(5, 5)  // ContainersFwd using TString，与 Lua TString 冲突
#define UL_UE_HAS_EINTERNAL_ASYNC_LOADING_MACRO  UL_UE_AT_LEAST(5, 5)  // EInternalObjectFlags::AsyncLoading → EInternalObjectFlags_AsyncLoading
#define UL_UE_HAS_FMETADATA_COPY                 UL_UE_AT_LEAST(5, 6)  // UMetaData::CopyMetadata → FMetaData::CopyMetadata
#define UL_UE_HAS_UCLASS_SET_DEFAULT_OBJECT     UL_UE_AT_LEAST(5, 6)  // ClassDefaultObject 直写 → SetDefaultObject
#define UL_UE_HAS_UFIELD_NEXT_TOBJECTPTR        UL_UE_AT_LEAST(5, 7)  // UField::Next 为 TObjectPtr
#define UL_UE_HAS_CODEGEN_PROPERTY_PARAMS        (UL_UE_AT_LEAST(5, 1) && !UL_UE_AT_LEAST(5, 8)) // F*PropertyParams 含 ObjectFlags
#define UL_UE_HAS_FPROPERTY_SIMPLE_CTOR           UL_UE_AT_LEAST(5, 8)  // FProperty(FFieldVariant, FName) 足够建临时属性
#define UL_UE_HAS_FENUM_PROPERTY_OFFSET_CTOR   (!UL_UE_AT_LEAST(5, 1)) // 6 参 FEnumProperty(..., Offset, Flags, Enum)
#define UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_EARLY  (UL_UE_AT_LEAST(5, 1) && !UL_UE_AT_LEAST(5, 3))
#define UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE   UL_UE_AT_LEAST(5, 3)
#define UL_UE_HAS_METADATA_PARAMS_COUNT_FIRST       UL_UE_AT_LEAST(5, 3)
#define UL_UE_HAS_PROPERTY_ELEMENT_SIZE_ACCESSOR    UL_UE_AT_LEAST(5, 5)  // ElementSize → Get/SetElementSize
#define UL_UE_HAS_REFCOLLECTOR_TOBJECTPTR            UL_UE_AT_LEAST(5, 4)  // AddReferencedObjects 要 TObjectPtr
#define UL_UE_HAS_MULTICAST_PROCESS_DELEGATE        UL_UE_AT_LEAST(5, 8)  // ProcessMulticastDelegate → ProcessDelegate
#define UL_UE_HAS_ASSET_SOFT_OBJECT_PATH            UL_UE_AT_LEAST(5, 1)  // FAssetData::GetSoftObjectPath
#define UL_UE_HAS_FIND_FIRST_OBJECT             UL_UE_AT_LEAST(5, 1)  // 引擎提供 FindFirstObject；此前垫片
#define UL_UE_HAS_CLASS_PATHS                   UL_UE_AT_LEAST(5, 1)  // FARFilter::ClassNames → ClassPaths；FAssetData::AssetClassPath
#define UL_UE_HAS_APP_STYLE                     UL_UE_AT_LEAST(5, 1)  // FEditorStyle → FAppStyle
#define UL_UE_HAS_TOP_LEVEL_ASSET_PATH          UL_UE_AT_LEAST(5, 1)  // ImplementNewInterface/RemoveInterface 走 FTopLevelAssetPath
#define UL_UE_HAS_ASSETREGISTRY_NESTED_HEADER   UL_UE_AT_LEAST(5, 2)  // AssetRegistry/AssetRegistryModule.h（对齐原 UE_VERSION_NEWER_THAN(5,1,0)）
#define UL_UE_HAS_UE_DISABLE_OPTIMIZATION       UL_UE_AT_LEAST(5, 2)  // PRAGMA_DISABLE_OPTIMIZATION → UE_DISABLE_OPTIMIZATION
#define UL_UE_HAS_TOBJECTPTR_FIELD_GET          UL_UE_AT_LEAST(5, 3)  // TObjectPtr MetaClass/PropertyClass 需 .Get()
#define UL_UE_HAS_UPROPERTY_TYPE                    (!UL_UE_AT_LEAST(4, 25)) // 4.25 前 FProperty 仍是 UProperty
#define UL_UE_HAS_FOREACH_OBJECT_WITH_PACKAGE    UL_UE_AT_LEAST(4, 26) // ForEachObjectWithPackage
#define UL_UE_HAS_UFIELD_GET_PACKAGE            UL_UE_AT_LEAST(4, 26) // UField::GetPackage
#define UL_UE_HAS_FSPAWN_OVERRIDE_PACKAGE       UL_UE_AT_LEAST(4, 26) // FActorSpawnParameters OverridePackage 等
#define UL_UE_HAS_EKEYS_MOUSE2D                 UL_UE_AT_LEAST(4, 26) // EKeys::Mouse2D
#define UL_UE_HAS_STATIC_CONSTRUCT_OBJECT_PARAMS UL_UE_AT_LEAST(4, 26) // FStaticConstructObjectParameters
#define UL_UE_HAS_BLUEPRINT_EDITORS_LIST        UL_UE_AT_LEAST(4, 26) // FBlueprintEditorModule::GetBlueprintEditors
#define UL_UE_HAS_LWC                           UL_UE_AT_LEAST(5, 0)  // Large World Coordinates：unluaReal=double；FVector4/FQuat 模板
#define UL_UE_HAS_SCRIPTARRAY_ALIGNMENT         UL_UE_AT_LEAST(5, 0)  // FScriptArray::Empty/Add 对齐参数
#define UL_UE_HAS_OBJECT_IS_VALID               UL_UE_AT_LEAST(5, 0)  // IsValid() 替代 IsPendingKill()
#define UL_UE_HAS_TOOLMENUS_LEVEL_EDITOR_USER   UL_UE_AT_LEAST(5, 0)  // LevelEditor.LevelEditorToolBar.User
#define UL_UE_HAS_GAME_RELOAD_UNLUA_INI         UL_UE_AT_LEAST(5, 0)  // 非 Editor 打包需强制 ReloadConfig UnLua.ini
#define UL_UE_HAS_PACKAGE_SAVE_CONTEXT         UL_UE_AT_LEAST(5, 0)  // PreSavePackageWithContextEvent / ObjectSaveContext
#define UL_UE_HAS_TRY_UPDATE_DEFAULT_CONFIG     UL_UE_AT_LEAST(5, 0)  // UpdateDefaultConfigFile → TryUpdateDefaultConfigFile
#define UL_UE_HAS_COREDELEGATES_GET_POST_ENGINE_INIT UL_UE_AT_LEAST(5, 8) // OnPostEngineInit → GetOnPostEngineInit()
#define UL_UE_HAS_FOREACH_OBJECT_GETOBJECTS_FLAGS UL_UE_AT_LEAST(5, 8) // ForEachObjectWithPackage 第三参 bool → EGetObjectsFlags

#if UL_UE_HAS_PROPERTY_ELEMENT_SIZE_ACCESSOR
#define UL_GET_PROPERTY_ELEMENT_SIZE(P) ((P)->GetElementSize())
#define UL_SET_PROPERTY_ELEMENT_SIZE(P, V) ((P)->SetElementSize(V))
#else
#define UL_GET_PROPERTY_ELEMENT_SIZE(P) ((P)->ElementSize)
#define UL_SET_PROPERTY_ELEMENT_SIZE(P, V) ((P)->ElementSize = (V))
#endif

// ── 类型垫片：引擎已删除 / 不再默认包含的 traits，业务侧仍用原名 ──

#if !UL_UE_HAS_TCHOOSE_CLASS
template <bool Predicate, typename TrueClass, typename FalseClass>
class TChooseClass;

template <typename TrueClass, typename FalseClass>
class TChooseClass<true, TrueClass, FalseClass>
{
public:
	typedef TrueClass Result;
};

template <typename TrueClass, typename FalseClass>
class TChooseClass<false, TrueClass, FalseClass>
{
public:
	typedef FalseClass Result;
};
#endif

#if !UL_UE_HAS_TREMOVE_CONST
template <typename T>
struct TRemoveConst
{
	typedef T Type;
};

template <typename T>
struct TRemoveConst<const T>
{
	typedef T Type;
};
#endif

#if !UL_UE_HAS_TIS_TRIVIALLY_DESTRUCTIBLE
template <typename T>
struct TIsTriviallyDestructible
{
	enum { Value = std::is_trivially_destructible<T>::value };
};
#endif
