#include "Misc/EngineVersionComparison.h"
#include "UnLuaVersionCompat.h"
#include "UnLuaCompatibility.h"
#include "PropertyRegistry.h"
#include "Binding.h"
#include "ClassRegistry.h"
#include "EnumRegistry.h"
#include "LowLevel.h"
#include "LuaEnv.h"
#include "ReflectionUtils/PropertyDesc.h"

namespace UnLua
{
    FPropertyRegistry::FPropertyRegistry(FLuaEnv* Env)
        : Env(Env)
    {
        PropertyCollector = FindFirstObject<UScriptStruct>(TEXT("PropertyCollector"));
        check(PropertyCollector);
    }

    void FPropertyRegistry::NotifyUObjectDeleted(UObject* Object)
    {
        FieldProperties.Remove(static_cast<UField*>(Object));
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::CreateTypeInterface(lua_State* L, int32 Index)
    {
        Index = LowLevel::AbsIndex(L, Index);

        TSharedPtr<ITypeInterface> TypeInterface;
        int32 Type = lua_type(L, Index);
        switch (Type)
        {
        case LUA_TBOOLEAN:
            TypeInterface = GetBoolProperty();
            break;
        case LUA_TNUMBER:
            TypeInterface = lua_isinteger(L, Index) > 0 ? GetIntProperty() : GetFloatProperty();
            break;
        case LUA_TSTRING:
            TypeInterface = GetStringProperty();
            break;
        case LUA_TTABLE:
            {
                lua_pushstring(L, "__name");
                Type = lua_rawget(L, Index);
                if (Type == LUA_TSTRING)
                {
                    const char* Name = lua_tostring(L, -1);
                    auto ClassDesc = Env->GetClassRegistry()->Find(Name);
                    if (ClassDesc)
                    {
                        TypeInterface = GetFieldProperty(ClassDesc->AsStruct());
                    }
                    else
                    {
                        auto EnumDesc = Env->GetEnumRegistry()->Find(Name);
                        if (EnumDesc)
                            TypeInterface = GetFieldProperty(EnumDesc->GetEnum());
                        else
                            TypeInterface = FindTypeInterface(lua_tostring(L, -1));
                    }
                }
                lua_pop(L, 1);
            }
            break;
        case LUA_TUSERDATA:
            {
                // mt/nil
                lua_getmetatable(L, Index);
                if (lua_istable(L, -1))
                {
                    // mt,mt.__name/nil
                    lua_getfield(L, -1, "__name");
                    if (lua_isstring(L, -1))
                    {
                        const char* Name = lua_tostring(L, -1);
                        FClassDesc* ClassDesc = Env->GetClassRegistry()->Find(Name);
                        if (ClassDesc)
                            TypeInterface = GetFieldProperty(ClassDesc->AsStruct());
                    }
                    // mt
                    lua_pop(L, 1);
                }
                lua_pop(L, 1);
            }
            break;
        default:
            break;
        }

        return TypeInterface;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetBoolProperty()
    {
        if (!BoolProperty)
        {
#if UL_UE_HAS_FPROPERTY_SIMPLE_CTOR
            const auto Property = new FBoolProperty(PropertyCollector, NAME_None);
#elif UL_UE_HAS_CODEGEN_PROPERTY_PARAMS
            constexpr auto Params = UECodeGen_Private::FBoolPropertyParams
            {
                nullptr,
                nullptr,
                CPF_None,
                UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool,
                RF_Transient,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_EARLY
                1,
#endif
                nullptr,
                nullptr,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                1,
#endif
                sizeof(bool),
                sizeof(FPropertyCollector),
                nullptr,
#if UL_UE_HAS_METADATA_PARAMS_COUNT_FIRST
                METADATA_PARAMS(0, nullptr)
#else
                METADATA_PARAMS(nullptr, 0)
#endif
            };
            const auto Property = new FBoolProperty(PropertyCollector, Params);
#else
            const auto Property = new FBoolProperty(PropertyCollector, NAME_None, RF_Transient, 0, (EPropertyFlags)0, 0xFF, 1, true);
#endif
            BoolProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return BoolProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetIntProperty()
    {
        if (!IntProperty)
        {
#if UL_UE_HAS_FPROPERTY_SIMPLE_CTOR
            const auto Property = new FIntProperty(PropertyCollector, NAME_None);
#elif UL_UE_HAS_CODEGEN_PROPERTY_PARAMS
            constexpr auto Params = UECodeGen_Private::FIntPropertyParams
            {
                nullptr,
                nullptr,
                CPF_HasGetValueTypeHash,
                UECodeGen_Private::EPropertyGenFlags::Int,
                RF_Transient,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_EARLY
                1,
#endif
                nullptr,
                nullptr,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                1,
#endif
                0,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                METADATA_PARAMS(0, nullptr)
#else
                METADATA_PARAMS(nullptr, 0)
#endif
            };
            const auto Property = new FIntProperty(PropertyCollector, Params);
#else
            const auto Property = new FIntProperty(PropertyCollector, NAME_None, RF_Transient, 0, CPF_HasGetValueTypeHash);
#endif
            IntProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return IntProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetFloatProperty()
    {
        if (!FloatProperty)
        {
#if UL_UE_HAS_FPROPERTY_SIMPLE_CTOR
            const auto Property = new FFloatProperty(PropertyCollector, NAME_None);
#elif UL_UE_HAS_CODEGEN_PROPERTY_PARAMS
            constexpr auto Params = UECodeGen_Private::FFloatPropertyParams
            {
                nullptr,
                nullptr,
                CPF_HasGetValueTypeHash,
                UECodeGen_Private::EPropertyGenFlags::Float,
                RF_Transient,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_EARLY
                1,
#endif
                nullptr,
                nullptr,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                1,
#endif
                0,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                METADATA_PARAMS(0, nullptr)
#else
                METADATA_PARAMS(nullptr, 0)
#endif
            };
            const auto Property = new FFloatProperty(PropertyCollector, Params);
#else
            const auto Property = new FFloatProperty(PropertyCollector, NAME_None, RF_Transient, 0, CPF_HasGetValueTypeHash);
#endif
            FloatProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return FloatProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetStringProperty()
    {
        if (!StringProperty)
        {
#if UL_UE_HAS_FPROPERTY_SIMPLE_CTOR
            const auto Property = new FStrProperty(PropertyCollector, NAME_None);
#elif UL_UE_HAS_CODEGEN_PROPERTY_PARAMS
            constexpr auto Params = UECodeGen_Private::FStrPropertyParams
            {
                nullptr,
                nullptr,
                CPF_HasGetValueTypeHash,
                UECodeGen_Private::EPropertyGenFlags::Str,
                RF_Transient,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_EARLY
                1,
#endif
                nullptr,
                nullptr,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                1,
#endif
                0,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                METADATA_PARAMS(0, nullptr)
#else
                METADATA_PARAMS(nullptr, 0)
#endif
            };
            const auto Property = new FStrProperty(PropertyCollector, Params);
#else
            const auto Property = new FStrProperty(PropertyCollector, NAME_None, RF_Transient, 0, CPF_HasGetValueTypeHash);
#endif
            StringProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return StringProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetNameProperty()
    {
        if (!NameProperty)
        {
#if UL_UE_HAS_FPROPERTY_SIMPLE_CTOR
            const auto Property = new FNameProperty(PropertyCollector, NAME_None);
#elif UL_UE_HAS_CODEGEN_PROPERTY_PARAMS
            constexpr auto Params = UECodeGen_Private::FNamePropertyParams
            {
                nullptr,
                nullptr,
                CPF_HasGetValueTypeHash,
                UECodeGen_Private::EPropertyGenFlags::Name,
                RF_Transient,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_EARLY
                1,
#endif
                nullptr,
                nullptr,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                1,
#endif
                0,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                METADATA_PARAMS(0, nullptr)
#else
                METADATA_PARAMS(nullptr, 0)
#endif
            };
            const auto Property = new FNameProperty(PropertyCollector, Params);
#else
            const auto Property = new FNameProperty(PropertyCollector, NAME_None, RF_Transient, 0, CPF_HasGetValueTypeHash);
#endif
            NameProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return NameProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetTextProperty()
    {
        if (!TextProperty)
        {
#if UL_UE_HAS_FPROPERTY_SIMPLE_CTOR
            const auto Property = new FTextProperty(PropertyCollector, NAME_None);
#elif UL_UE_HAS_CODEGEN_PROPERTY_PARAMS
            constexpr auto Params = UECodeGen_Private::FTextPropertyParams
            {
                nullptr,
                nullptr,
                CPF_HasGetValueTypeHash,
                UECodeGen_Private::EPropertyGenFlags::Text,
                RF_Transient,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_EARLY
                1,
#endif
                nullptr,
                nullptr,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                1,
#endif
                0,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                METADATA_PARAMS(0, nullptr)
#else
                METADATA_PARAMS(nullptr, 0)
#endif
            };
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
            const auto Property = new FTextProperty(PropertyCollector, "", RF_Transient);
#else
            const auto Property = new FTextProperty(PropertyCollector, Params);
#endif
#else
            const auto Property = new FTextProperty(PropertyCollector, NAME_None, RF_Transient, 0, CPF_HasGetValueTypeHash);
#endif
            TextProperty = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        }
        return TextProperty;
    }

    TSharedPtr<ITypeInterface> FPropertyRegistry::GetFieldProperty(UField* Field)
    {
        if (const auto Exists = FieldProperties.Find(Field))
            return *Exists;

        FProperty* Property;
        if (const auto Class = Cast<UClass>(Field))
        {
#if UL_UE_HAS_FPROPERTY_SIMPLE_CTOR
            auto* ObjectProperty = new FObjectProperty(PropertyCollector, NAME_None);
            ObjectProperty->SetPropertyClass(Class);
            Property = ObjectProperty;
#elif UL_UE_HAS_CODEGEN_PROPERTY_PARAMS
            constexpr auto Params = UECodeGen_Private::FObjectPropertyParams
            {
                nullptr,
                nullptr,
                CPF_HasGetValueTypeHash,
                UECodeGen_Private::EPropertyGenFlags::Object,
                RF_Transient,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_EARLY
                1,
#endif
                nullptr,
                nullptr,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                1,
#endif
                0,
                nullptr,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                METADATA_PARAMS(0, nullptr)
#else
                METADATA_PARAMS(nullptr, 0)
#endif
            };
            const auto ObjectProperty = new FObjectProperty(PropertyCollector, Params);
            ObjectProperty->PropertyClass = Class;
            Property = ObjectProperty;
#else
            Property = new FObjectProperty(PropertyCollector, NAME_None, RF_Transient, 0, CPF_HasGetValueTypeHash, Class);
#endif
        }
        else if (const auto ScriptStruct = Cast<UScriptStruct>(Field))
        {
#if UL_UE_HAS_FPROPERTY_SIMPLE_CTOR
            auto* StructProperty = new FStructProperty(PropertyCollector, NAME_None);
            StructProperty->Struct = ScriptStruct;
            UL_SET_PROPERTY_ELEMENT_SIZE(StructProperty, ScriptStruct->PropertiesSize);
            Property = StructProperty;
#elif UL_UE_HAS_CODEGEN_PROPERTY_PARAMS
            const auto Params = UECodeGen_Private::FStructPropertyParams
            {
                nullptr,
                nullptr,
                ScriptStruct->GetCppStructOps()
                    ? ScriptStruct->GetCppStructOps()->GetComputedPropertyFlags() | CPF_HasGetValueTypeHash
                    : CPF_HasGetValueTypeHash,
                UECodeGen_Private::EPropertyGenFlags::Struct,
                RF_Transient,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_EARLY
                1,
#endif
                nullptr,
                nullptr,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                1,
#endif
                0,
                nullptr,
#if UL_UE_HAS_PROPERTY_PARAMS_ARRAY_DIM_LATE
                METADATA_PARAMS(0, nullptr)
#else
                METADATA_PARAMS(nullptr, 0)
#endif
            };
            const auto StructProperty = new FStructProperty(PropertyCollector, Params);
            StructProperty->Struct = ScriptStruct;
            UL_SET_PROPERTY_ELEMENT_SIZE(StructProperty, ScriptStruct->PropertiesSize);
            Property = StructProperty;
#else
            Property = new FStructProperty(PropertyCollector, NAME_None, RF_Transient, 0, CPF_HasGetValueTypeHash, ScriptStruct);
#endif
        }
        else if (const auto Enum = Cast<UEnum>(Field))
        {
#if UL_UE_HAS_FENUM_PROPERTY_OFFSET_CTOR
            const auto EnumProperty = new FEnumProperty(PropertyCollector, NAME_None, RF_Transient, 0, CPF_HasGetValueTypeHash, Enum);
#elif UL_UE_HAS_FPROPERTY_SIMPLE_CTOR
            const auto EnumProperty = new FEnumProperty(PropertyCollector, NAME_None);
            EnumProperty->SetEnum(Enum);
#else
            const auto EnumProperty = new FEnumProperty(PropertyCollector, NAME_None, RF_Transient);
            EnumProperty->SetEnum(Enum);
#endif
#if UL_UE_HAS_FPROPERTY_SIMPLE_CTOR
            const auto UnderlyingProperty = new FByteProperty(EnumProperty, TEXT("UnderlyingType"));
#else
            const auto UnderlyingProperty = new FByteProperty(EnumProperty, TEXT("UnderlyingType"), RF_Transient);
#endif
            Property = EnumProperty;
            Property->AddCppProperty(UnderlyingProperty);
            UL_SET_PROPERTY_ELEMENT_SIZE(Property, UL_GET_PROPERTY_ELEMENT_SIZE(UnderlyingProperty));
            Property->PropertyFlags |= CPF_IsPlainOldData | CPF_NoDestructor | CPF_ZeroConstructor;
        }
        else
        {
            Property = nullptr;
        }

        const auto Ret = TSharedPtr<ITypeInterface>(FPropertyDesc::Create(Property));
        FieldProperties.Add(Field, Ret);
        return Ret;
    }
}
