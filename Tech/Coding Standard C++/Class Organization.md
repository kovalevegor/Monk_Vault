
[Original Documentation](https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine)

**Классы** должны быть организованы с учетом читателя, а не автора. Поскольку большинство читателей будут использовать публичный интерфейс класса, публичная реализация должна объявляться первой, за ней следует частная реализация класса.

```cpp
UCLASS()
class EXAMPLEPROJECT_API AExampleActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AExampleActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
```

## Copyright Notice

Любой исходный файл (`.h`, `.cpp`, `.xaml`), предоставленный Epic Games для публичного распространения, должен содержать уведомление об авторских правах в качестве первой строки в файле. Формат уведомления должен точно соответствовать показанному ниже:

```cpp
// Copyright Epic Games, Inc. All Rights Reserved.
```

Если эта строка отсутствует или отформатирована неправильно, CIS выдаст ошибку и завершит работу.

