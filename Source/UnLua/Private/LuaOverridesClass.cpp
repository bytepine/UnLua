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

#include "LuaOverridesClass.h"
#include "LuaFunction.h"
#include "UnLuaBase.h"
#include "UnLuaVersionCompat.h"

namespace
{
    UField* GetNextField(const UField* Field)
    {
#if UL_UE_HAS_UFIELD_NEXT_TOBJECTPTR
        return Field->Next.Get();
#else
        return Field->Next;
#endif
    }
}

ULuaOverridesClass* ULuaOverridesClass::Create(UClass* Class)
{
    auto ClassNameString = FString::Printf(TEXT("LUA_OVERRIDES_%s"), *Class->GetName());
    auto ClassName = MakeUniqueObjectName(GetTransientPackage(), Class, FName(*ClassNameString));
    auto Ret = NewObject<ULuaOverridesClass>(GetTransientPackage(), ClassName, RF_Public | RF_Transient);
    Ret->ClassFlags |= CLASS_NewerVersionExists; // bypass FBlueprintActionDatabase::RefreshClassActions
#if UL_UE_HAS_UCLASS_SET_DEFAULT_OBJECT
    Ret->SetDefaultObject(StaticClass()->GetDefaultObject());
#else
    Ret->ClassDefaultObject = StaticClass()->GetDefaultObject();
#endif
    Ret->SetSuperStruct(StaticClass());
    Ret->Bind();
    Ret->Owner = Class;
    Ret->AddToOwner();
    return Ret;
}

void ULuaOverridesClass::Restore()
{
    SetActive(false);
    RemoveFromOwner();
}

void ULuaOverridesClass::SetActive(const bool bActive)
{
    const auto Class = Owner.Get();
    if (!Class)
        return;

    for (TFieldIterator<ULuaFunction> It(this, EFieldIteratorFlags::ExcludeSuper); It; ++It)
    {
        const auto LuaFunction = *It;
        LuaFunction->SetActive(bActive);
    }

    Class->ClearFunctionMapsCaches();
    if (bActive)
        AddToOwner();
    else
        RemoveFromOwner();
}

void ULuaOverridesClass::BeginDestroy()
{
    Restore();
    UClass::BeginDestroy();
}

void ULuaOverridesClass::AddToOwner()
{
    const auto Class = Owner.Get();
    if (!Class)
        return;

#if UL_UE_HAS_USTRUCT_CHILDREN_TOBJECTPTR
    // TObjectPtr 不能取址当链表头：.Get() 得到的是局部副本，写入会丢失
    UField* Head = Class->Children.Get();
    if (!Head)
    {
        UE_LOG(LogUnLua, Log, TEXT("ULuaOverridesClass::AddToOwner: %s Children 为空，直接写链首"), *GetNameSafe(Class));
        Class->Children = this;
    }
    else if (Head != this)
    {
        UField* Node = Head;
        while (true)
        {
            UField* NextField = GetNextField(Node);
            if (NextField == this)
                break;
            if (!NextField)
            {
                Node->Next = this;
                break;
            }
            Node = NextField;
        }
    }
#else
    auto Field = &(Class->Children);
    while (*Field)
    {
        if (*Field == this)
        {
            Field = nullptr;
            break;
        }
        Field = &(*Field)->Next;
    }

    if (Field)
        *Field = this;
#endif

    if (Class->IsRooted() || GUObjectArray.IsDisregardForGC(Class))
        AddToRoot();
}

void ULuaOverridesClass::RemoveFromOwner()
{
    const auto Class = Owner.Get();
    if (!Class)
        return;

#if UL_UE_HAS_USTRUCT_CHILDREN_TOBJECTPTR
    UField* Head = Class->Children.Get();
    if (Head == this)
    {
        Class->Children = nullptr;
    }
    else
    {
        for (UField* Node = Head; Node; Node = GetNextField(Node))
        {
            if (GetNextField(Node) == this)
            {
                Node->Next = nullptr;
                break;
            }
        }
    }
#else
    auto Field = &Class->Children;
    while (*Field)
    {
        if (*Field == this)
        {
            *Field = nullptr;
            break;
        }
        Field = &(*Field)->Next;
    }
#endif

    if (!Class->IsRooted() && !GUObjectArray.IsDisregardForGC(Class))
        RemoveFromRoot();
}
