### Comments

Комментарии — это коммуникация, а коммуникация жизненно важна. Следующие разделы детализируют некоторые важные вещи, которые нужно помнить о комментариях (из Kernighan & Pike *The Practice of Programming*).

### ### Guidelines

+ Пишите самодокументирующий код. Например:

```cpp
// Bad:
t = s + l - b;

// Good:
TotalLeaves = SmallLeaves + LargeLeaves - SmallAndLargeLeaves;
```

+ Пишите полезные комментарии. Например:

```cpp
// Bad:
// increment Leaves
++Leaves;

// Good:
// we know there is another tea leaf
++Leaves;
```

+ Не перекомментируйте плохой код — перепишите его вместо этого. Например:

```cpp
// Bad:
// total number of leaves is sum of
// small and large leaves less the
// number of leaves that are both
t = s + l - b;

// Good:
TotalLeaves = SmallLeaves + LargeLeaves - SmallAndLargeLeaves;
```

+ Не противоречайте коду. Например:

```cpp
// Bad:
// never increment Leaves!
++Leaves;

// Good:
// we know there is another tea leaf
++Leaves;
```

### Const Correctness

Const — это документация в той же мере, что и директива компилятора. Весь код должен стремиться к const-correctness. Это включает следующие руководства:
- Передавайте аргументы функции по const указателю или ссылке, если эти аргументы не предназначены для модификации функцией.
- Помечайте методы как const, если они не модифицируют объект.
- Используйте const итерацию по контейнерам, если цикл не предназначен для модификации контейнера.

Пример Const:
```cpp
void SomeMutatingOperation(FThing& OutResult, const TArray<Int32>& InArray)
{
    // InArray will not be modified here, but OutResult probably will be
}

void FThing::SomeNonMutatingOperation() const
{
    // This code will not modify the FThing it is invoked on
}

TArray<FString> StringArray;
for (const FString& : StringArray)
{
    // The body of this loop will not modify StringArray
}
```

Const также предпочтителен для параметров функции по значению и локальных переменных. Это говорит читателю, что переменная не будет модифицирована в теле функции, что упрощает понимание. Если вы делаете это, убедитесь, что объявление и определение совпадают, поскольку это может повлиять на процесс JavaDoc.
```cpp
void AddSomeThings(const int32 Count);

void AddSomeThings(const int32 Count)
{
    const int32 CountPlusOne = Count + 1;
    // Neither Count nor CountPlusOne can be changed during the body of the function
}
```

Одно исключение — параметры, передаваемые по значению, которые перемещаются в контейнер. Для получения дополнительной информации см. раздел "Move semantics" на этой странице.

Пример:
```cpp
void FBlah::SetMemberArray(TArray<FString> InNewArray)
{
    MemberArray = MoveTemp(InNewArray);
}
```

Поместите ключевое слово const в конец, когда делаете указатель const (а не то, на что он указывает). Ссылки не могут быть "переприсвоены" anyway, и поэтому не могут быть сделаны const таким же образом.

Пример:
```cpp
// Const pointer to non-const object - pointer cannot be reassigned, but T can still be modified
T* const Ptr = ...;

// Illegal
T& const Ref = ...;
```

Никогда не используйте const для типа возвращаемого значения. Это inhibits move semantics для сложных типов и даст предупреждения компиляции для встроенных типов. Это правило применяется только к самому типу возвращаемого значения, не к целевому типу указателя или ссылки, которая возвращается.

Пример:
```cpp
// Bad - returning a const array
const TArray<FString> GetSomeArray();

// Fine - returning a reference to a const array
const TArray<FString>& GetSomeArray();

// Fine - returning a pointer to a const array
const TArray<FString>* GetSomeArray();

// Bad - returning a const pointer to a const array
const TArray<FString>* const GetSomeArray();
```
