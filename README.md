# The Host

Вы — инопланетный паразит, вырвавшийся из лаборатории. Вы крайне уязвимы, но способны мгновенно захватывать тела учёных и солдат, используя их оружие, доступ и здоровье, чтобы прорваться к выходу из подземного бункера.

## Клонирование

```bash
git clone https://github.com/FunCodingMan/HostGame.git
cd HostGame
```

## Требования

- CMake ≥ 3.8
- Компилятор с поддержкой C++17
- SFML 2.6+

### Установка SFML

**Ubuntu / Debian**
```bash
sudo apt install build-essential cmake libsfml-dev
```

**macOS**
```bash
brew install cmake sfml
```

**Windows (vcpkg)**
```powershell
vcpkg install sfml:x64-windows
```

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Windows с vcpkg:

```powershell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

## Запуск

```bash
./build/app
```

Windows (MSVC):

```powershell
.\build\Release\app.exe
```

## Управление

| Клавиша | Действие |
|---|---|
| `A` / `←` | движение влево |
| `D` / `→` | движение вправо |
| `W` / `Space` | прыжок |
| `Shift` + клик мыши | рывок |
